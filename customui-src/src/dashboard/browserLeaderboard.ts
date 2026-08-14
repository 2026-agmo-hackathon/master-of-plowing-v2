import type { LeaderboardState, OrchestrationState } from './types'

const MAX_BYTES=10*1024*1024
const PRODUCTION_ENDPOINT='https://leaderboard-api.seamos.io/api/rec-submission'
const RETRY_DELAYS=[1000,2000,4000,8000]
const SEALED_RECORDING_URL='/ws/rest/api/simulator/rec.enc'
const LEGACY_CONTENT_TYPE_ERROR='sealed recording content type is invalid'
const STALE_404=/^sealed recording HTTP 404 \([^)]*\)$/

export type OutboxRecord={runId:string;teamName:string;createdAt?:string;status:LeaderboardState['status'];attempt:number;retryAllowed:boolean;envelope?:string;error?:string;result?:Partial<LeaderboardState>;leaseOwner?:string;leaseUntilMs?:number}
export type DrainRequest={requestId:string;resetGeneration:number;createdAt:number}
export type DrainCompletion=DrainRequest&{completedAt:number}
export interface OutboxStore { get(id:string):Promise<OutboxRecord|undefined>; put(value:OutboxRecord):Promise<void>; putIfOwned(value:OutboxRecord,opToken:string):Promise<boolean>; discard(id:string):Promise<OutboxRecord|undefined>; discardUnsubmitted():Promise<OutboxRecord[]>; recoverLegacyCapture(id:string):Promise<OutboxRecord|undefined>; retireStale404(id:string,nowMs:number,canCommit:()=>boolean):Promise<OutboxRecord|undefined>; all():Promise<OutboxRecord[]>; claim(id:string,opToken:string,nowMs:number,leaseMs:number):Promise<OutboxRecord|undefined>; renew(id:string,opToken:string,nowMs:number,leaseMs:number):Promise<boolean>; release(id:string,opToken:string):Promise<void>; heartbeat(tabId:string,nowMs:number):Promise<void>; liveTabs(nowMs:number,maxAgeMs:number):Promise<string[]>; beginDrain(request:DrainRequest):Promise<void>; currentDrain():Promise<DrainRequest|undefined>; acknowledgeDrain(requestId:string,tabId:string):Promise<void>; drainAcknowledged(requestId:string,tabId:string):Promise<boolean>; completeDrain(request:DrainRequest,completedAt:number):Promise<void>; completedDrain():Promise<DrainCompletion|undefined> }
export type BrowserLeaderboardDeps={store:OutboxStore;fetch:typeof fetch;sleep?:(ms:number)=>Promise<void>;simulatorUrl?:string;leaderboardUrl?:string;publish?:(s:LeaderboardState)=>void;captured?:(runId:string)=>void;captureFailed?:(runId:string)=>void;submitted?:(runId:string,verdict:string)=>void;fetchTimeoutMs?:number;retireRetryMs?:number;drainTimeoutMs?:number;tabExpiryMs?:number}

function state(r:OutboxRecord):LeaderboardState{return{kind:'leaderboard_state',status:r.status,attempt:r.attempt,runId:r.runId,error:r.error,retryAllowed:r.retryAllowed,...r.result}}
function requiredString(v:Record<string,unknown>,k:string){return typeof v[k]==='string'&&(v[k] as string).length>0}
export function validateEnvelope(value:unknown):value is Record<string,unknown>{
  if(!value||typeof value!=='object'||Array.isArray(value))return false
  const v=value as Record<string,unknown>
  return v.format==='agmo-rec-enc'&&v.version===2&&v.keyAlg==='RSA-OAEP-SHA256'&&v.cipherAlg==='AES-256-GCM'&&v.sigAlg==='Ed25519'
    &&requiredString(v,'createdAt')&&requiredString(v,'originalFilename')&&requiredString(v,'ivB64')&&requiredString(v,'encryptedKeyB64')
    &&requiredString(v,'ciphertextB64')&&requiredString(v,'signatureB64')&&typeof v.signerKeyId==='string'&&/^[0-9a-f]{16}$/.test(v.signerKeyId)
}

export function shouldRetireStale404(orchestration:OrchestrationState|null,leaderboard:LeaderboardState|null):boolean{
  if(!orchestration||leaderboard?.status!=='capture_error'||!leaderboard.runId
    ||!STALE_404.test(leaderboard.error??''))return false
  const currentCompletedId=orchestration.finalization==='completed'?orchestration.recordingId:undefined
  return leaderboard.runId!==currentCompletedId
}

async function cappedText(response:Response,cap=MAX_BYTES,signal?:AbortSignal):Promise<string>{
  const declared=Number(response.headers.get('content-length')||0);if(declared>cap)throw new Error('response exceeds size cap')
  if(!response.body){const b=await response.arrayBuffer();if(b.byteLength>cap)throw new Error('response exceeds size cap');return new TextDecoder().decode(b)}
  const reader=response.body.getReader();const chunks:Uint8Array[]=[];let total=0
  const abort=()=>{void reader.cancel(signal?.reason)};signal?.addEventListener('abort',abort,{once:true})
  try{for(;;){if(signal?.aborted)throw signal.reason;const {done,value}=await reader.read();if(done)break;total+=value.byteLength;if(total>cap){await reader.cancel();throw new Error('response exceeds size cap')}chunks.push(value)}}finally{signal?.removeEventListener('abort',abort);reader.releaseLock()}
  if(signal?.aborted)throw signal.reason
  const bytes=new Uint8Array(total);let at=0;for(const c of chunks){bytes.set(c,at);at+=c.byteLength}return new TextDecoder().decode(bytes)
}

export class BrowserLeaderboardService{
  private active=new Set<string>();private activeAbort=new Map<string,AbortController>();private activeTasks=new Map<string,Promise<void>>();private authorityGeneration=0;private authorityReady=false;private authorityRunId?:string;private captureGrantedRunId?:string;private authorityPostRun?:OrchestrationState['postRun'];private authorityResetGeneration=0;private authorityAbort=new AbortController();private stopped=false
  private purgeChannel?:BroadcastChannel
  private tabId=`tab-${Date.now()}-${Math.random()}`;private heartbeatTimer?:ReturnType<typeof setInterval>;private drainBlocked=false
  constructor(private d:BrowserLeaderboardDeps){if(typeof BroadcastChannel!=='undefined'){this.purgeChannel=new BroadcastChannel('master-of-plow-leaderboard-purge');this.purgeChannel.onmessage=event=>{const message=event.data as ({kind?:'drain';ack?:string}|{kind:'complete'})&DrainRequest;if(!message?.requestId)return;if(message.kind==='complete')void this.observeCompletion(message);else if(!message.ack)void this.handleDrain(message)}}void this.initializeTab()}
  private async initializeTab(){await this.d.store.heartbeat(this.tabId,Date.now());const current=await this.d.store.currentDrain();if(current)await this.handleDrain(current);this.heartbeatTimer=setInterval(()=>{void this.d.store.heartbeat(this.tabId,Date.now())},1000)}
  async reconcileAuthority(orchestration?:OrchestrationState){
    this.authorityAbort.abort();this.authorityAbort=new AbortController();const signal=this.authorityAbort.signal,generation=++this.authorityGeneration;this.authorityReady=false
    this.authorityRunId=orchestration?.finalization==='completed'?orchestration.recordingId:undefined
    this.authorityPostRun=orchestration?.postRun
    this.authorityResetGeneration=orchestration?.resetGeneration??0
    this.captureGrantedRunId=orchestration?.postRun==='capture_in_progress'?orchestration.recordingId:undefined
    if(orchestration?.postRun==='none')await this.confirmAuthoritativeDrain(this.authorityResetGeneration)
    // 완료 확인 경로는 세대가 맞을 때만 표식을 지운다. 앱이 재시작하면 백엔드의
    // resetGeneration 은 0 으로 돌아가므로 어떤 세대와도 맞지 않아, 남은 표식이
    // 그대로 살아남아 이후 모든 제출을 막았다.
    // The confirming path only clears a marker whose generation matches. After an
    // app restart the backend's resetGeneration is back to 0 and matches nothing,
    // so a leftover marker survived and blocked every later submission.
    await this.releaseAbandonedDrain(orchestration)
    const rows=await this.d.store.all()
    for(const row of rows)if(this.isStale404(row)&&row.runId!==this.authorityRunId){await this.retireForAuthority(row.runId,generation,signal);if(signal.aborted||generation!==this.authorityGeneration)return}
    if(signal.aborted||generation!==this.authorityGeneration||this.stopped)return
    this.authorityReady=!this.drainBlocked;await this.resume()
  }
  async resume(){if(!this.authorityReady||this.stopped)return;for(const stored of await this.d.store.all()){let r=stored;if(this.isLegacyContentTypeFailure(stored)&&stored.runId===this.captureGrantedRunId)r=await this.d.store.recoverLegacyCapture(stored.runId)??await this.d.store.get(stored.runId)??stored
      if(r.status==='submitted'||r.status==='invalid'){
        if(r.runId===this.authorityRunId&&this.authorityPostRun!=='submitted'){
          if(this.authorityPostRun==='capture_in_progress')this.d.captured?.(r.runId)
          else if(this.authorityPostRun==='capture_durable')this.d.submitted?.(r.runId,r.status==='invalid'?'INVALID':'SCORED')
        }
        continue
      }
      // 봉투를 들고 있는 낡은 기록까지 매 스냅샷마다 캡처 완료를 다시
      // 알리면, 백엔드는 그 주행을 더 이상 기다리지 않으므로 "sealed capture
      // acknowledgement is not authorized" 오류 프레임을 붙어 있는 모든
      // 대시보드로 계속 뿌린다. 지금 백엔드가 기다리는 주행에 대해서만 알린다.
      //
      // Re-announcing capture for every stored record that still holds an
      // envelope made the backend — which no longer waits on those runs —
      // broadcast "sealed capture acknowledgement is not authorized" to every
      // attached dashboard on each snapshot. Only report the run it waits on.
      if(r.envelope&&(r.runId===this.captureGrantedRunId||r.runId===this.authorityRunId))
        this.d.captured?.(r.runId)
      if(this.eligible(r)&&(Boolean(r.envelope)||r.runId===this.captureGrantedRunId))this.schedule(r)}}
  async enqueue(teamName:string,runId:string){
    if(!teamName.trim()||!runId)return
    this.captureGrantedRunId=runId
    const prior=await this.d.store.get(runId);if(prior){if(prior.envelope)this.d.captured?.(runId);if(this.authorityReady&&prior.teamName===teamName&&prior.retryAllowed)this.schedule(prior);return}
    const r:OutboxRecord={runId,teamName:teamName.trim(),createdAt:new Date().toISOString(),status:'capture_pending',attempt:0,retryAllowed:true}
    await this.persist(r);if(this.authorityReady)this.schedule(r)
  }
  async retry(runId:string){if(!this.authorityReady||this.active.has(runId))return;const r=await this.d.store.get(runId);if(!r?.retryAllowed||(!r.envelope&&runId!==this.captureGrantedRunId))return;this.schedule(r,true)}
  async discard(runId:string){const retired=await this.d.store.discard(runId);if(retired)this.d.publish?.(state(retired))}
  private abortActive(){for(const controller of this.activeAbort.values())controller.abort(new Error('discarded by run reset'))}
  private async abortAndDrain(){this.abortActive();await Promise.allSettled([...this.activeTasks.values()])}
  private async handleDrain(request:DrainRequest){this.drainBlocked=true;this.authorityReady=false;await this.abortAndDrain();const retired=await this.d.store.discardUnsubmitted();for(const row of retired)this.d.publish?.(state(row));await this.d.store.acknowledgeDrain(request.requestId,this.tabId);this.purgeChannel?.postMessage({...request,kind:'drain',ack:this.tabId})}
  private async observeCompletion(completion:DrainRequest){
    // A broadcast is only a wake-up hint.  The tab reopens admission solely
    // after its own authoritative backend snapshot confirms the same reset.
    if(this.authorityPostRun==='none'&&this.authorityResetGeneration===completion.resetGeneration)
      await this.confirmAuthoritativeDrain(completion.resetGeneration)
  }
  /** 실패한 리셋이 남긴 드레인 표식을 걷어낸다.
   *
   *  드레인은 리셋이 도는 동안 제출을 막는 장치이고, 다시 열리는 유일한 조건이
   *  "백엔드가 리셋 완료(postRun=none)를 확인해 줌" 이었다. 리셋이 실패하면
   *  postRun 은 reset_retryable 로 끝나므로 표식이 IndexedDB 에 영구히 남고,
   *  이후 모든 페이지 로드가 그 표식을 보고 스스로를 잠갔다 — 제출을 눌러도
   *  capture_pending 에서 영영 움직이지 않는다. 브라우저를 다시 켜도 같다.
   *  백엔드가 리셋 중이 아니라고 말하면 표식도 함께 정리한다.
   *
   *  Clears the drain marker a failed reset leaves behind. The drain exists to
   *  block submissions while a reset runs, and the only way it reopened was the
   *  backend confirming the reset finished (postRun=none). A failed reset ends
   *  at reset_retryable, so the marker stayed in IndexedDB for good and every
   *  later page load read it and locked itself: Submit sat at capture_pending
   *  forever, across browser restarts. If the backend says no reset is running,
   *  the marker goes too. */
  private async releaseAbandonedDrain(orchestration?:OrchestrationState){
    if(!orchestration)return
    if(orchestration.postRun==='reset_cleanup_preparing'
      ||orchestration.postRun==='reset_cleanup_pending')return
    const current=await this.d.store.currentDrain()
    if(!current)return
    await this.d.store.completeDrain(current,Date.now())
    this.purgeChannel?.postMessage({...current,kind:'complete'})
    this.drainBlocked=false
  }
  private async confirmAuthoritativeDrain(resetGeneration:number){
    if(!resetGeneration)return
    const current=await this.d.store.currentDrain()
    if(current?.resetGeneration===resetGeneration){await this.d.store.completeDrain(current,Date.now());this.purgeChannel?.postMessage({...current,kind:'complete'})}
    const completed=await this.d.store.completedDrain()
    if(completed?.resetGeneration===resetGeneration)this.drainBlocked=false
  }
  async discardAllUnsubmitted(resetGeneration:number):Promise<boolean>{
    if(!Number.isSafeInteger(resetGeneration)||resetGeneration<=0)return false
    const request={requestId:`reset-${resetGeneration}-${Date.now()}-${Math.random()}`,resetGeneration,createdAt:Date.now()};await this.d.store.beginDrain(request);this.purgeChannel?.postMessage({...request,kind:'drain'});await this.handleDrain(request)
    const deadline=Date.now()+(this.d.drainTimeoutMs??5000),expiry=this.d.tabExpiryMs??3000
    while(Date.now()<deadline){const live=await this.d.store.liveTabs(Date.now(),expiry),acks=await Promise.all(live.map(tab=>this.d.store.drainAcknowledged(request.requestId,tab)));if(acks.every(Boolean))return true;await new Promise(resolve=>setTimeout(resolve,25))}
    return false
  }
  async outboxStates(){return (await this.d.store.all()).map(record=>({...state(record),teamName:record.teamName,createdAt:record.createdAt}))}
  shutdown(){this.stopped=true;this.authorityReady=false;this.authorityAbort.abort();this.abortActive();if(this.heartbeatTimer)clearInterval(this.heartbeatTimer);this.purgeChannel?.close()}
  private async persist(r:OutboxRecord){await this.d.store.put(r);this.d.publish?.(state(r))}
  private async persistOwned(r:OutboxRecord,opToken:string){const ok=await this.d.store.putIfOwned(r,opToken);if(ok)this.d.publish?.(state(r));return ok}
  private schedule(r:OutboxRecord,manualRetry=false){if(this.activeTasks.has(r.runId))return;const task=this.process(r,manualRetry).finally(()=>this.activeTasks.delete(r.runId));this.activeTasks.set(r.runId,task)}
  private async process(r:OutboxRecord,manualRetry=false){
    const locks=typeof navigator!=='undefined'?navigator.locks:undefined
    const opToken=`${Date.now()}-${Math.random()}-${r.runId}`
    if(locks){await locks.request(`master-of-plow-leaderboard:${r.runId}`,async()=>{const current=await this.d.store.get(r.runId);if(!this.eligible(current))return;await this.runClaimed(r.runId,opToken,manualRetry)});return}
    await this.runClaimed(r.runId,opToken,manualRetry)
  }
  private async runClaimed(runId:string,opToken:string,manualRetry:boolean){
    const claimed=await this.d.store.claim(runId,opToken,Date.now(),120_000)
    if(!this.eligible(claimed))return
    const leaseAbort=new AbortController();this.activeAbort.set(runId,leaseAbort)
    const renew=async()=>{try{const ok=await this.d.store.renew(runId,opToken,Date.now(),120_000);if(!ok)leaseAbort.abort(new Error('leaderboard lease lost'));return ok}catch(e){leaseAbort.abort(e);return false}}
    const heartbeat=setInterval(()=>{void renew()},30_000)
    try{await this.processClaimed(claimed,manualRetry,opToken,leaseAbort.signal,renew)}finally{clearInterval(heartbeat);this.activeAbort.delete(runId);await this.d.store.release(runId,opToken)}
  }
  private isStale404(r:OutboxRecord|undefined){return !!r&&r.status==='capture_error'&&r.retryAllowed&&!r.envelope&&STALE_404.test(r.error??'')}
  private async retireForAuthority(runId:string,generation:number,signal:AbortSignal){
    const valid=()=>!this.stopped&&!signal.aborted&&generation===this.authorityGeneration
    while(valid()){
      const retired=await this.d.store.retireStale404(runId,Date.now(),valid)
      if(retired){if(valid())this.d.publish?.(state(retired));return}
      const current=await this.d.store.get(runId);if(!this.isStale404(current))return
      await new Promise<void>(resolve=>{const timer=setTimeout(resolve,this.d.retireRetryMs??250);signal.addEventListener('abort',()=>{clearTimeout(timer);resolve()},{once:true})})
    }
  }
  private isLegacyContentTypeFailure(r:OutboxRecord){return r.status==='failed_terminal'&&!r.retryAllowed&&!r.envelope&&r.error===LEGACY_CONTENT_TYPE_ERROR}
  private eligible(r:OutboxRecord|undefined):r is OutboxRecord{return !!r&&(r.status==='submitting'||r.retryAllowed)&&r.status!=='submitted'&&r.status!=='invalid'&&r.status!=='failed_terminal'}
  private async processClaimed(r:OutboxRecord,manualRetry:boolean,opToken:string,leaseSignal?:AbortSignal,renew?:()=>Promise<boolean>){if(manualRetry){r.attempt=0;r.status=r.envelope?'pending':'capture_pending';r.error=undefined;r.retryAllowed=true;if(!await this.persistOwned(r,opToken))return}else if(r.status==='submitting')r.retryAllowed=true;await this.processOwned(r,opToken,leaseSignal,renew)}
  private async processOwned(r:OutboxRecord,opToken:string,leaseSignal?:AbortSignal,renew?:()=>Promise<boolean>){
    if(this.active.has(r.runId)||this.stopped)return;this.active.add(r.runId)
    try{
      if(!r.envelope){
        // The dashboard and CustomUI share nginx's /ws/rest/ bridge. Keeping
        // this relative avoids mixed content and never targets the operator laptop.
        const url=this.d.simulatorUrl??SEALED_RECORDING_URL
        let captured:{response:Response;text:string};try{captured=await this.withBoundedResponse(url,undefined,leaseSignal,async(response,signal)=>({response,text:response.ok?await cappedText(response,MAX_BYTES,signal):''}))}catch(e){if(leaseSignal?.aborted)return;return this.captureError(r,String(e),opToken)}
        if(leaseSignal?.aborted||renew&&!(await renew()))return
        const response=captured.response,observedType=(response.headers.get('content-type')||'<missing>').slice(0,128),mediaType=observedType.split(';',1)[0].trim().toLowerCase()
        if(!response.ok){const error=`sealed recording HTTP ${response.status} (${observedType})`;return response.status===404||response.status>=500?this.captureError(r,error,opToken):this.terminal(r,error,opToken)}
        if(mediaType!=='application/octet-stream')return this.terminal(r,`sealed recording content type is invalid (HTTP ${response.status}, observed ${observedType})`,opToken)
        let exact=captured.text,envelope:unknown;try{envelope=JSON.parse(exact)}catch(e){return this.terminal(r,String(e),opToken)}
        if(!validateEnvelope(envelope))return this.terminal(r,'sealed recording envelope v2 is invalid',opToken)
        r.envelope=exact;r.status='pending';r.retryAllowed=true;if(!await this.persistOwned(r,opToken))return;this.d.captured?.(r.runId)
      }
      else this.d.captured?.(r.runId)
      const parsed=JSON.parse(r.envelope);const body=JSON.stringify({teamName:r.teamName,envelope:parsed});if(new TextEncoder().encode(body).byteLength>MAX_BYTES)return this.terminal(r,'leaderboard request exceeds 10 MiB',opToken)
      // 재시도가 소진된 기록은 2.5초마다 스냅샷이 올 때 다시 이 자리에 들어온다.
      // 그때마다 문구를 덮어쓰면 화면에 남는 사유가 실제 원인(HTTP 503 등)이
      // 아니라 "중단된 요청"으로 바뀌어, 리더보드 장애를 우리 쪽 문제로 읽게
      // 된다. 이미 알고 있는 사유가 있으면 그대로 둔다.
      //
      // An exhausted record comes back through here on every 2.5 s snapshot.
      // Overwriting the text each time replaces the real cause (HTTP 503 and
      // friends) with "interrupted request", which reads as our fault rather
      // than a leaderboard outage. Keep the reason already recorded.
      if(r.attempt>=5)return this.retryWait(r,
        r.error??'leaderboard retry attempts exhausted after interrupted request',opToken)
      for(let attempt=Math.max(1,r.attempt+1);attempt<=5&&!this.stopped;attempt++){
        // A persisted `submitting` record is intentionally left retryable by
        // reload if ownership is lost; this invocation must not overwrite the
        // new owner's durable state.
        if(leaseSignal?.aborted||renew&&!(await renew()))return
        r.attempt=attempt;r.status='submitting';r.retryAllowed=false;if(!await this.persistOwned(r,opToken))return
        let resultResponse:{response:Response;text:string}
        try{resultResponse=await this.withBoundedResponse(this.d.leaderboardUrl??PRODUCTION_ENDPOINT,{method:'POST',headers:{'Content-Type':'application/json'},body},leaseSignal,async(response,signal)=>({response,text:await cappedText(response,1024*1024,signal)}))}
        catch(e){if(leaseSignal?.aborted)return;if(attempt===5)return this.retryWait(r,String(e),opToken);if(!await this.retryDelay(r,attempt,String(e),opToken,leaseSignal,renew))return;continue}
        if(leaseSignal?.aborted||renew&&!(await renew()))return
        const response=resultResponse.response
        if(response.status===200){let result:Record<string,unknown>;try{result=JSON.parse(resultResponse.text)}catch{return this.terminal(r,'leaderboard response schema is invalid',opToken)}
          if(result.verdict!=='SCORED'&&result.verdict!=='INVALID')return this.terminal(r,'leaderboard response schema is invalid',opToken)
          r.status=result.verdict==='SCORED'?'submitted':'invalid';r.retryAllowed=false;r.error=undefined;r.result={verdict:String(result.verdict),mapId:String(result.mapId??''),reason:String(result.reason??''),score:typeof result.score==='number'?result.score:undefined,kept:Boolean(result.kept),applied:Boolean(result.applied),verified:Boolean(result.verified)};if(await this.persistOwned(r,opToken))this.d.submitted?.(r.runId,String(result.verdict));return
        }
        if(response.status>=400&&response.status<500)return this.terminal(r,`leaderboard rejected submission: HTTP ${response.status}`,opToken)
        if(attempt===5)return this.retryWait(r,`leaderboard unavailable: HTTP ${response.status}`,opToken)
        if(!await this.retryDelay(r,attempt,`leaderboard unavailable: HTTP ${response.status}`,opToken,leaseSignal,renew))return
      }
    }finally{this.active.delete(r.runId)}
  }
  private async withBoundedResponse<T>(input:RequestInfo|URL,init:RequestInit|undefined,leaseSignal:AbortSignal|undefined,consume:(response:Response,signal:AbortSignal)=>Promise<T>){const controller=new AbortController(),timeout=setTimeout(()=>controller.abort(new Error('HTTP response timeout')),this.d.fetchTimeoutMs??30_000),abort=()=>controller.abort(leaseSignal?.reason);if(leaseSignal?.aborted)abort();else leaseSignal?.addEventListener('abort',abort,{once:true});try{const response=await this.d.fetch(input,{...init,signal:controller.signal});return await consume(response,controller.signal)}finally{clearTimeout(timeout);leaseSignal?.removeEventListener('abort',abort)}}
  private async retryDelay(r:OutboxRecord,attempt:number,error:string,opToken:string,leaseSignal?:AbortSignal,renew?:()=>Promise<boolean>){r.status='retry_wait';r.retryAllowed=true;r.error=error;if(!await this.persistOwned(r,opToken))return false;await(this.d.sleep?.(RETRY_DELAYS[attempt-1])??new Promise(x=>setTimeout(x,RETRY_DELAYS[attempt-1])));return !leaseSignal?.aborted&&(!renew||await renew())}
  private async captureError(r:OutboxRecord,error:string,opToken:string){r.status='capture_error';r.retryAllowed=true;r.error=error;if(await this.persistOwned(r,opToken))this.d.captureFailed?.(r.runId)}
  private async retryWait(r:OutboxRecord,error:string,opToken:string){r.status='retry_wait';r.retryAllowed=true;r.error=error;await this.persistOwned(r,opToken)}
  private async terminal(r:OutboxRecord,error:string,opToken:string){r.status='failed_terminal';r.retryAllowed=false;r.error=error;await this.persistOwned(r,opToken)}
}

export class IndexedDbOutboxStore implements OutboxStore{
  private db:Promise<IDBDatabase>
  constructor(){this.db=new Promise((resolve,reject)=>{const q=indexedDB.open('master-of-plow-leaderboard',2);q.onupgradeneeded=()=>{if(!q.result.objectStoreNames.contains('runs'))q.result.createObjectStore('runs',{keyPath:'runId'});if(!q.result.objectStoreNames.contains('control'))q.result.createObjectStore('control',{keyPath:'key'})};q.onsuccess=()=>resolve(q.result);q.onerror=()=>reject(q.error)})}
  private async request<T>(mode:IDBTransactionMode,op:(s:IDBObjectStore)=>IDBRequest<T>){const db=await this.db;return new Promise<T>((resolve,reject)=>{const tx=db.transaction('runs',mode),q=op(tx.objectStore('runs'));q.onsuccess=()=>resolve(q.result);q.onerror=()=>reject(q.error);tx.onerror=()=>reject(tx.error)})}
  get(id:string){return this.request<OutboxRecord|undefined>('readonly',s=>s.get(id))}
  async put(v:OutboxRecord){const db=await this.db;await new Promise<void>((resolve,reject)=>{const tx=db.transaction('runs','readwrite'),store=tx.objectStore('runs'),get=store.get(v.runId);get.onsuccess=()=>{const current=get.result as OutboxRecord|undefined;store.put({...v,leaseOwner:current?.leaseOwner,leaseUntilMs:current?.leaseUntilMs})};get.onerror=()=>reject(get.error);tx.oncomplete=()=>resolve();tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  async putIfOwned(v:OutboxRecord,opToken:string){const db=await this.db;return new Promise<boolean>((resolve,reject)=>{const tx=db.transaction('runs','readwrite'),store=tx.objectStore('runs'),get=store.get(v.runId);let wrote=false;get.onsuccess=()=>{const current=get.result as OutboxRecord|undefined;if(current?.leaseOwner===opToken){wrote=true;store.put({...v,leaseOwner:opToken,leaseUntilMs:current.leaseUntilMs})}};get.onerror=()=>reject(get.error);tx.oncomplete=()=>resolve(wrote);tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  async discard(id:string){const db=await this.db;return new Promise<OutboxRecord|undefined>((resolve,reject)=>{const tx=db.transaction('runs','readwrite'),store=tx.objectStore('runs'),get=store.get(id);let retired:OutboxRecord|undefined;get.onsuccess=()=>{const r=get.result as OutboxRecord|undefined;if(r){retired={...r,status:'failed_terminal',retryAllowed:false,error:'discarded by run reset',leaseOwner:undefined,leaseUntilMs:undefined};store.put(retired)}};get.onerror=()=>reject(get.error);tx.oncomplete=()=>resolve(retired);tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  async discardUnsubmitted(){const db=await this.db;return new Promise<OutboxRecord[]>((resolve,reject)=>{const tx=db.transaction('runs','readwrite'),store=tx.objectStore('runs'),get=store.getAll(),retired:OutboxRecord[]=[];get.onsuccess=()=>{for(const r of get.result as OutboxRecord[])if(r.status!=='submitted'&&r.status!=='invalid'){const row={...r,status:'failed_terminal' as const,retryAllowed:false,error:'discarded by run reset',leaseOwner:undefined,leaseUntilMs:undefined};retired.push(row);store.put(row)}};get.onerror=()=>reject(get.error);tx.oncomplete=()=>resolve(retired);tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  async recoverLegacyCapture(id:string){const db=await this.db;return new Promise<OutboxRecord|undefined>((resolve,reject)=>{const tx=db.transaction('runs','readwrite'),store=tx.objectStore('runs'),get=store.get(id);let recovered:OutboxRecord|undefined;get.onsuccess=()=>{const r=get.result as OutboxRecord|undefined;if(r?.status==='failed_terminal'&&!r.retryAllowed&&!r.envelope&&r.error===LEGACY_CONTENT_TYPE_ERROR){recovered={...r,status:'capture_pending',retryAllowed:true,error:undefined};store.put(recovered)}};get.onerror=()=>reject(get.error);tx.oncomplete=()=>resolve(recovered);tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  async retireStale404(id:string,nowMs:number,canCommit:()=>boolean){const db=await this.db;return new Promise<OutboxRecord|undefined>((resolve,reject)=>{const tx=db.transaction('runs','readwrite'),store=tx.objectStore('runs'),get=store.get(id);let retired:OutboxRecord|undefined;get.onsuccess=()=>{const r=get.result as OutboxRecord|undefined,leaseActive=Boolean(r?.leaseOwner)&&(r?.leaseUntilMs??0)>nowMs;if(canCommit()&&r?.status==='capture_error'&&r.retryAllowed&&!r.envelope&&!leaseActive&&STALE_404.test(r.error??'')){retired={...r,status:'failed_terminal',retryAllowed:false,error:`retired stale capture: ${r.error}`,leaseOwner:undefined,leaseUntilMs:undefined};store.put(retired)}};get.onerror=()=>reject(get.error);tx.oncomplete=()=>resolve(retired);tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  all(){return this.request<OutboxRecord[]>('readonly',s=>s.getAll())}
  async claim(id:string,opToken:string,nowMs:number,leaseMs:number){const db=await this.db;return new Promise<OutboxRecord|undefined>((resolve,reject)=>{const tx=db.transaction('runs','readwrite'),store=tx.objectStore('runs'),get=store.get(id);let claimed:OutboxRecord|undefined
    get.onsuccess=()=>{const r=get.result as OutboxRecord|undefined;if(!r||(r.leaseUntilMs??0)>nowMs)return;claimed={...r,leaseOwner:opToken,leaseUntilMs:nowMs+leaseMs};store.put(claimed)}
    get.onerror=()=>reject(get.error);tx.oncomplete=()=>resolve(claimed);tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  async renew(id:string,opToken:string,nowMs:number,leaseMs:number){const db=await this.db;return new Promise<boolean>((resolve,reject)=>{const tx=db.transaction('runs','readwrite'),store=tx.objectStore('runs'),get=store.get(id);let renewed=false;get.onsuccess=()=>{const r=get.result as OutboxRecord|undefined;if(r?.leaseOwner===opToken){renewed=true;store.put({...r,leaseUntilMs:nowMs+leaseMs})}};get.onerror=()=>reject(get.error);tx.oncomplete=()=>resolve(renewed);tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  async release(id:string,opToken:string){const db=await this.db;await new Promise<void>((resolve,reject)=>{const tx=db.transaction('runs','readwrite'),store=tx.objectStore('runs'),get=store.get(id);get.onsuccess=()=>{const r=get.result as OutboxRecord|undefined;if(r?.leaseOwner===opToken)store.put({...r,leaseOwner:undefined,leaseUntilMs:undefined})};get.onerror=()=>reject(get.error);tx.oncomplete=()=>resolve();tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  private async controlPut(value:Record<string,unknown>){const db=await this.db;await new Promise<void>((resolve,reject)=>{const tx=db.transaction('control','readwrite');tx.objectStore('control').put(value);tx.oncomplete=()=>resolve();tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  private async controlGet<T>(key:string){const db=await this.db;return new Promise<T|undefined>((resolve,reject)=>{const tx=db.transaction('control','readonly'),q=tx.objectStore('control').get(key);q.onsuccess=()=>resolve(q.result as T|undefined);q.onerror=()=>reject(q.error)})}
  heartbeat(tabId:string,nowMs:number){return this.controlPut({key:`tab:${tabId}`,tabId,nowMs})}
  async liveTabs(nowMs:number,maxAgeMs:number){const db=await this.db;return new Promise<string[]>((resolve,reject)=>{const tx=db.transaction('control','readonly'),q=tx.objectStore('control').getAll();q.onsuccess=()=>resolve((q.result as Array<{key:string;tabId?:string;nowMs?:number}>).filter(v=>v.key.startsWith('tab:')&&typeof v.tabId==='string'&&typeof v.nowMs==='number'&&nowMs-v.nowMs<=maxAgeMs).map(v=>v.tabId!));q.onerror=()=>reject(q.error)})}
  beginDrain(request:DrainRequest){return this.controlPut({key:'drain:current',...request})}
  async currentDrain(){const value=await this.controlGet<DrainRequest&{key:string}>('drain:current');return value&&{requestId:value.requestId,resetGeneration:value.resetGeneration,createdAt:value.createdAt}}
  acknowledgeDrain(requestId:string,tabId:string){return this.controlPut({key:`drain:${requestId}:ack:${tabId}`,requestId,tabId})}
  async drainAcknowledged(requestId:string,tabId:string){return Boolean(await this.controlGet(`drain:${requestId}:ack:${tabId}`))}
  async completeDrain(request:DrainRequest,completedAt:number){const db=await this.db;await new Promise<void>((resolve,reject)=>{const tx=db.transaction('control','readwrite'),store=tx.objectStore('control'),current=store.get('drain:current'),keys=store.getAllKeys();current.onsuccess=()=>{const active=current.result as (DrainRequest&{key:string})|undefined;if(active?.requestId===request.requestId&&active.resetGeneration===request.resetGeneration)store.delete('drain:current')};keys.onsuccess=()=>{for(const raw of keys.result){const key=String(raw);if(key.startsWith(`drain:${request.requestId}:ack:`))store.delete(raw)}store.put({key:'drain:completed',...request,completedAt})};current.onerror=()=>reject(current.error);keys.onerror=()=>reject(keys.error);tx.oncomplete=()=>resolve();tx.onerror=()=>reject(tx.error);tx.onabort=()=>reject(tx.error)})}
  async completedDrain(){const value=await this.controlGet<DrainCompletion&{key:string}>('drain:completed');return value&&{requestId:value.requestId,resetGeneration:value.resetGeneration,createdAt:value.createdAt,completedAt:value.completedAt}}
}
