import {describe,expect,it,vi} from 'vitest'
import {BrowserLeaderboardService,shouldRetireStale404,type DrainCompletion,type DrainRequest,type OutboxRecord,type OutboxStore,validateEnvelope} from './browserLeaderboard'
import type { LeaderboardState, OrchestrationState } from './types'

const envelope={format:'agmo-rec-enc',version:2,createdAt:'2026-08-13T00:00:00Z',originalFilename:'rec.csv',keyAlg:'RSA-OAEP-SHA256',cipherAlg:'AES-256-GCM',ivB64:'AAAA',encryptedKeyB64:'BBBB',ciphertextB64:'CCCC',sigAlg:'Ed25519',signerKeyId:'0123456789abcdef',signatureB64:'DDDD'}
class MemoryStore implements OutboxStore{
  values=new Map<string,OutboxRecord>();writes:OutboxRecord[]=[]
  tabs=new Map<string,number>();drain?:DrainRequest;completion?:DrainCompletion;acks=new Set<string>()
  async get(id:string){return this.values.get(id)}async put(v:OutboxRecord){const current=this.values.get(v.runId),c=structuredClone({...v,leaseOwner:current?.leaseOwner,leaseUntilMs:current?.leaseUntilMs});this.values.set(v.runId,c);this.writes.push(c)}async all(){return [...this.values.values()]}
  async putIfOwned(v:OutboxRecord,opToken:string){const current=this.values.get(v.runId);if(current?.leaseOwner!==opToken)return false;const c=structuredClone({...v,leaseOwner:opToken,leaseUntilMs:current.leaseUntilMs});this.values.set(v.runId,c);this.writes.push(c);return true}
  async discard(id:string){const r=this.values.get(id);if(!r)return;const c:OutboxRecord={...r,status:'failed_terminal',retryAllowed:false,error:'discarded by run reset',leaseOwner:undefined,leaseUntilMs:undefined};this.values.set(id,c);this.writes.push(c);return c}
  async discardUnsubmitted(){const retired:OutboxRecord[]=[];for(const r of this.values.values())if(r.status!=='submitted'&&r.status!=='invalid'){const c:OutboxRecord={...r,status:'failed_terminal',retryAllowed:false,error:'discarded by run reset',leaseOwner:undefined,leaseUntilMs:undefined};this.values.set(r.runId,c);this.writes.push(c);retired.push(c)}return retired}
  async recoverLegacyCapture(id:string){const r=this.values.get(id);if(r?.status!=='failed_terminal'||r.retryAllowed||r.envelope||r.error!=='sealed recording content type is invalid')return;const c:OutboxRecord={...r,status:'capture_pending',retryAllowed:true,error:undefined};this.values.set(id,c);this.writes.push(c);return c}
  async retireStale404(id:string,nowMs:number,canCommit:()=>boolean){const r=this.values.get(id),leaseActive=Boolean(r?.leaseOwner)&&(r?.leaseUntilMs??0)>nowMs;if(!canCommit()||r?.status!=='capture_error'||!r.retryAllowed||r.envelope||leaseActive||!/^sealed recording HTTP 404 \([^)]*\)$/.test(r.error??''))return;const c:OutboxRecord={...r,status:'failed_terminal',retryAllowed:false,error:`retired stale capture: ${r.error}`,leaseOwner:undefined,leaseUntilMs:undefined};this.values.set(id,c);this.writes.push(c);return c}
  async claim(id:string,opToken:string,nowMs:number,leaseMs:number){const r=this.values.get(id);if(!r||(r.leaseUntilMs??0)>nowMs)return;const c={...r,leaseOwner:opToken,leaseUntilMs:nowMs+leaseMs};this.values.set(id,c);return c}
  async renew(id:string,opToken:string,nowMs:number,leaseMs:number){const r=this.values.get(id);if(r?.leaseOwner!==opToken)return false;this.values.set(id,{...r,leaseUntilMs:nowMs+leaseMs});return true}
  async release(id:string,opToken:string){const r=this.values.get(id);if(r?.leaseOwner===opToken)this.values.set(id,{...r,leaseOwner:undefined,leaseUntilMs:undefined})}
  async heartbeat(tabId:string,nowMs:number){this.tabs.set(tabId,nowMs)}
  async liveTabs(nowMs:number,maxAgeMs:number){return [...this.tabs].filter(([,seen])=>nowMs-seen<=maxAgeMs).map(([id])=>id)}
  async beginDrain(request:DrainRequest){this.drain=request}
  async currentDrain(){return this.drain}
  async acknowledgeDrain(requestId:string,tabId:string){this.acks.add(`${requestId}:${tabId}`)}
  async drainAcknowledged(requestId:string,tabId:string){return this.acks.has(`${requestId}:${tabId}`)}
  async completeDrain(request:DrainRequest,completedAt:number){if(this.drain?.requestId===request.requestId)this.drain=undefined;this.completion={...request,completedAt};for(const ack of [...this.acks])if(ack.startsWith(`${request.requestId}:`))this.acks.delete(ack)}
  async completedDrain(){return this.completion}
}
const sealed=()=>new Response(JSON.stringify(envelope),{status:200,headers:{'Content-Type':'application/octet-stream'}})
const scored=()=>new Response(JSON.stringify({verdict:'SCORED',mapId:1,score:91,kept:true,applied:true,verified:true}),{status:200,headers:{'Content-Type':'application/json'}})
const settle=()=>new Promise(r=>setTimeout(r,20))
const activate=(service:BrowserLeaderboardService,currentRunId?:string,postRun:OrchestrationState['postRun']='capture_in_progress')=>service.reconcileAuthority(currentRunId?{kind:'orchestration_state',phase:'idle',busy:false,finalization:'completed',recordingId:currentRunId,postRun,maps:[],tractors:[],implements:[],selection:{stopped:true,live:true,snapshotAgeMs:0}}:undefined)

describe('browser leaderboard boundary',()=>{
  it('replays a durable terminal verdict after a lost websocket ACK until backend confirms submitted',async()=>{
    const store=new MemoryStore();await store.put({runId:'ack-lost',teamName:'T',status:'submitted',attempt:1,retryAllowed:false,envelope:JSON.stringify(envelope),result:{verdict:'SCORED'}})
    const frames:string[]=[],fetcher=vi.fn(async()=>scored()),service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,captured:id=>frames.push(`captured:${id}`),submitted:(id,verdict)=>frames.push(`submitted:${id}:${verdict}`)})
    await activate(service,'ack-lost','capture_in_progress')
    expect(frames).toEqual(['captured:ack-lost']);expect(fetcher).not.toHaveBeenCalled()
    await activate(service,'ack-lost','capture_durable')
    expect(frames).toEqual(['captured:ack-lost','submitted:ack-lost:SCORED'])
    await activate(service,'ack-lost','submitted');expect(frames).toHaveLength(2)
  })
  it('waits for every live tab durable drain acknowledgement without same-realm shortcuts',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore();await store.put({runId:'peer-post',teamName:'T',status:'pending',attempt:0,retryAllowed:true,envelope:JSON.stringify(envelope)})
      let peerAborted=false
      const peerFetch=vi.fn(async(_url:string|URL|Request,init?:RequestInit)=>new Promise<Response>(resolve=>init?.signal?.addEventListener('abort',()=>{peerAborted=true;resolve(scored())},{once:true})))
      const peer=new BrowserLeaderboardService({store,fetch:peerFetch as typeof fetch,leaderboardUrl:'/submit'}),initiator=new BrowserLeaderboardService({store,fetch:vi.fn() as unknown as typeof fetch,drainTimeoutMs:1000,tabExpiryMs:500})
      await activate(peer);await settle();expect(peerFetch).toHaveBeenCalledTimes(1)
      const drained=await initiator.discardAllUnsubmitted(1)
      expect(drained).toBe(true);expect(peerAborted).toBe(true);expect(store.drain?.resetGeneration).toBe(1);expect(store.values.get('peer-post')?.status).toBe('failed_terminal')
      peer.shutdown();initiator.shutdown()
    }finally{vi.unstubAllGlobals()}
  })
  it('waits out a crashed tab heartbeat and blocks a late tab during the reset generation',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore();store.tabs.set('crashed',Date.now());const initiator=new BrowserLeaderboardService({store,fetch:vi.fn() as unknown as typeof fetch,drainTimeoutMs:500,tabExpiryMs:40})
      const start=Date.now();expect(await initiator.discardAllUnsubmitted(2)).toBe(true);expect(Date.now()-start).toBeGreaterThanOrEqual(35)
      const request={requestId:'late-reset',resetGeneration:9,createdAt:Date.now()};await store.beginDrain(request);const fetcher=vi.fn(async()=>scored()),late=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch});await settle();await late.reconcileAuthority();await settle();expect(fetcher).not.toHaveBeenCalled();expect((await Promise.all([...store.tabs.keys()].filter(id=>id!=='crashed').map(id=>store.drainAcknowledged(request.requestId,id)))).some(Boolean)).toBe(true)
      initiator.shutdown();late.shutdown()
    }finally{vi.unstubAllGlobals()}
  })
  it('keeps every peer blocked until the same authoritative reset generation completes, then reuses the peer',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore(),fetcher=vi.fn(async(url:string|URL|Request)=>String(url)==='/ws/rest/api/simulator/rec.enc'?sealed():scored())
      await store.put({runId:'browser-only-purge',teamName:'Old',status:'capture_error',attempt:0,retryAllowed:true,error:'network'})
      const a=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit',drainTimeoutMs:1000}),b=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit',drainTimeoutMs:1000})
      await settle();expect(await a.discardAllUnsubmitted(11)).toBe(true);expect(store.drain?.resetGeneration).toBe(11);expect(store.completion).toBeUndefined();expect(store.values.get('browser-only-purge')?.status).toBe('failed_terminal')
      const completed:OrchestrationState={kind:'sim_setup_state',phase:'idle',busy:false,finalization:'idle',postRun:'none',resetGeneration:11,maps:[],tractors:[],implements:[],selection:{stopped:true,live:true,snapshotAgeMs:0}}
      await a.reconcileAuthority(completed);await b.reconcileAuthority(completed);expect(store.completion?.resetGeneration).toBe(11)
      await activate(b,'next-run','capture_in_progress');await b.enqueue('Next','next-run');await settle()
      expect(store.values.get('next-run')?.status).toBe('submitted');expect(fetcher).toHaveBeenCalledTimes(2)
      a.shutdown();b.shutdown()
    }finally{vi.unstubAllGlobals()}
  })
  // 드레인 표식은 리셋이 도는 동안만 제출을 막아야 한다. 예전에는 백엔드가
  // postRun=none 으로 리셋 완료를 확인해 줄 때만 풀렸는데, 실패한 리셋은
  // reset_retryable 에서 끝나므로 표식이 IndexedDB 에 영구히 남았다. 그러면
  // 그 뒤로 브라우저를 다시 켜도, 앱을 재설치해도, 전혀 다른 다음 주행의
  // 제출까지 capture_pending 에서 멈춘다 — 실제로 그렇게 막혔다.
  //
  // The drain marker must block submissions only while a reset is running. It
  // used to clear only when the backend confirmed completion with postRun=none,
  // but a failed reset ends at reset_retryable, so the marker stayed in
  // IndexedDB for good — after which even a different, later run could not be
  // submitted, across browser restarts and reinstalls. That is what happened.
  it('releases the drain marker once the backend leaves the reset, however it ended',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const snapshot=(postRun:OrchestrationState['postRun'],resetGeneration:number):OrchestrationState=>
        ({kind:'sim_setup_state',phase:'idle',busy:false,finalization:'completed',postRun,resetGeneration,recordingId:'old',maps:[],tractors:[],implements:[],selection:{stopped:true,live:true,snapshotAgeMs:0}})
      const store=new MemoryStore(),service=new BrowserLeaderboardService({store,fetch:vi.fn() as unknown as typeof fetch,drainTimeoutMs:500})
      await settle();expect(await service.discardAllUnsubmitted(20)).toBe(true)
      // A reset still under way keeps the marker.
      await service.reconcileAuthority(snapshot('reset_cleanup_preparing',20))
      expect(store.drain?.resetGeneration).toBe(20)
      // A failed reset releases it: the run was not discarded after all.
      await service.reconcileAuthority(snapshot('reset_retryable',20))
      expect(store.drain).toBeUndefined();expect(store.completion?.resetGeneration).toBe(20)
      // And a completed reset still releases it through the confirming path.
      expect(await service.discardAllUnsubmitted(21)).toBe(true)
      await service.reconcileAuthority({...snapshot('none',21),finalization:'idle',recordingId:undefined})
      expect(store.drain).toBeUndefined();expect(store.completion?.resetGeneration).toBe(21)
      // 앱이 재시작하면 백엔드 resetGeneration 은 0 이라 어떤 세대와도 맞지
      // 않는다. 그래도 남은 표식은 풀려야 한다.
      // After an app restart the backend's resetGeneration is 0 and matches no
      // marker; a leftover one still has to be released.
      expect(await service.discardAllUnsubmitted(22)).toBe(true)
      await service.reconcileAuthority({...snapshot('none',0),finalization:'idle',recordingId:undefined})
      expect(store.drain).toBeUndefined()
      service.shutdown()
    }finally{vi.unstubAllGlobals()}
  })
  it('resumes capture for a new run after an earlier reset failed and was abandoned',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore()
      const stale=new BrowserLeaderboardService({store,fetch:vi.fn() as unknown as typeof fetch,drainTimeoutMs:500})
      await settle();expect(await stale.discardAllUnsubmitted(7)).toBe(true);stale.shutdown()
      // 앱이 재시작하면 백엔드의 resetGeneration 은 0 으로 돌아간다. 그래도
      // 남은 표식은 풀려야 한다.
      // An app restart puts the backend's resetGeneration back to 0; the
      // leftover marker still has to be released.
      // A fresh tab reads the abandoned marker on startup and used to lock itself.
      const fetcher=vi.fn(async(url:string|URL|Request)=>String(url)==='/ws/rest/api/simulator/rec.enc'?sealed():scored())
      const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'})
      await settle()
      await activate(service,'next-run','capture_in_progress')
      await service.enqueue('T','next-run');await settle()
      expect(store.values.get('next-run')?.status).toBe('submitted')
      service.shutdown()
    }finally{vi.unstubAllGlobals()}
  })
  it('allows a late tab only after it learns completed generation from an authoritative snapshot',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore(),owner=new BrowserLeaderboardService({store,fetch:vi.fn() as unknown as typeof fetch,drainTimeoutMs:500});await settle();expect(await owner.discardAllUnsubmitted(30)).toBe(true)
      const done:OrchestrationState={kind:'sim_setup_state',phase:'idle',busy:false,finalization:'idle',postRun:'none',resetGeneration:30,maps:[],tractors:[],implements:[],selection:{stopped:true,live:true,snapshotAgeMs:0}};await owner.reconcileAuthority(done)
      const fetcher=vi.fn(async(url:string|URL|Request)=>String(url)==='/ws/rest/api/simulator/rec.enc'?sealed():scored()),late=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'});await settle();await late.resume();expect(fetcher).not.toHaveBeenCalled()
      await late.reconcileAuthority(done);await activate(late,'late-next','capture_in_progress');await late.enqueue('Late','late-next');await settle();expect(store.values.get('late-next')?.status).toBe('submitted')
      owner.shutdown();late.shutdown()
    }finally{vi.unstubAllGlobals()}
  })
  it('never retries sealed capture after failure until a fresh explicit grant',async()=>{
    const store=new MemoryStore();await store.put({runId:'failed-capture',teamName:'T',status:'capture_error',attempt:0,retryAllowed:true,error:'sealed recording HTTP 404 (application/json)'})
    const fetcher=vi.fn(async(url:string|URL|Request)=>String(url)==='/ws/rest/api/simulator/rec.enc'?sealed():scored()),service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'})
    await activate(service,'failed-capture','awaiting_action');await settle();expect(fetcher).not.toHaveBeenCalled()
    await activate(service,'failed-capture','capture_in_progress');await service.enqueue('T','failed-capture');await settle()
    expect(fetcher).toHaveBeenCalledTimes(2);expect(store.values.get('failed-capture')?.status).toBe('submitted')
  })
  it('abort-and-drain tombstones an in-flight post so a late response cannot win',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore();await store.put({runId:'abort-post',teamName:'T',status:'pending',attempt:0,retryAllowed:true,envelope:JSON.stringify(envelope)})
      let aborted=false
      const fetcher=vi.fn(async(_url:string|URL|Request,init?:RequestInit)=>new Promise<Response>(resolve=>{init?.signal?.addEventListener('abort',()=>{aborted=true;resolve(scored())},{once:true})}))
      const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'});await activate(service);await settle();const draining=service.discardAllUnsubmitted(3);await draining
      expect(aborted).toBe(true);expect(store.values.get('abort-post')).toMatchObject({status:'failed_terminal',retryAllowed:false,error:'discarded by run reset'});expect(fetcher).toHaveBeenCalledTimes(1)
    }finally{vi.unstubAllGlobals()}
  })
  it('exposes browser-only unsubmitted metadata for authoritative-list union',async()=>{
    const store=new MemoryStore();await store.put({runId:'browser-only',teamName:'Lost Team',createdAt:'2026-08-13T00:00:00Z',status:'capture_error',attempt:0,retryAllowed:true,error:'network'})
    const service=new BrowserLeaderboardService({store,fetch:vi.fn() as unknown as typeof fetch});expect(await service.outboxStates()).toEqual([expect.objectContaining({runId:'browser-only',status:'capture_error',error:'network'})])
  })
  it('validates the basic v2 server envelope contract',()=>{expect(validateEnvelope(envelope)).toBe(true);expect(validateEnvelope({...envelope,version:1})).toBe(false);expect(validateEnvelope({...envelope,signerKeyId:'BAD'})).toBe(false)})
  it('durably records exact envelope and team before external POST',async()=>{
    const store=new MemoryStore();const calls:string[]=[]
    const fetcher=vi.fn(async(url:string|URL|Request,_init?:RequestInit)=>{calls.push(String(url));if(calls.length===2)expect(store.values.get('run-1')?.envelope).toBe(JSON.stringify(envelope));return calls.length===1?sealed():scored()})
    const states:string[]=[];const captured:string[]=[];const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,simulatorUrl:'/sealed',leaderboardUrl:'https://leaderboard.test/submit',publish:s=>states.push(s.status),captured:id=>captured.push(id),sleep:async()=>{}})
    await activate(service);await service.enqueue('Team','run-1');await settle()
    expect(calls).toEqual(['/sealed','https://leaderboard.test/submit']);expect(store.values.get('run-1')?.status).toBe('submitted');expect(states[0]).toBe('capture_pending');expect(captured).toEqual(['run-1'])
    const body=JSON.parse(String(fetcher.mock.calls[1][1]?.body));expect(body).toEqual({teamName:'Team',envelope});expect(body.mapId).toBeUndefined();expect(body.dryRun).toBeUndefined()
  })
  it('reports terminal accepted verdicts and atomically retires every non-submitted row',async()=>{
    const store=new MemoryStore(),reported:string[]=[];const service=new BrowserLeaderboardService({store,fetch:vi.fn(async(url:string|URL|Request)=>String(url)==='/sealed'?sealed():scored()) as typeof fetch,simulatorUrl:'/sealed',leaderboardUrl:'/submit',submitted:(id,v)=>reported.push(`${id}:${v}`)})
    await activate(service,'accepted');await service.enqueue('T','accepted');await settle()
    await store.put({runId:'pending-old',teamName:'Old',status:'capture_error',attempt:0,retryAllowed:true,error:'network'})
    await store.put({runId:'invalid-kept',teamName:'Old',status:'invalid',attempt:1,retryAllowed:false,result:{verdict:'INVALID'}})
    await service.discardAllUnsubmitted(4)
    expect(reported).toEqual(['accepted:SCORED']);expect(store.values.get('accepted')?.status).toBe('submitted')
    expect(store.values.get('invalid-kept')?.status).toBe('invalid')
    expect(store.values.get('pending-old')).toMatchObject({status:'failed_terminal',retryAllowed:false,error:'discarded by run reset',leaseOwner:undefined})
  })
  it('uses the canonical same-origin sealed endpoint and accepts only octet-stream v2',async()=>{
    const store=new MemoryStore();const fetcher=vi.fn(async(url:string|URL|Request)=>String(url)==='/ws/rest/api/simulator/rec.enc'?sealed():scored())
    const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'});await activate(service);await service.enqueue('T','canonical');await settle()
    expect(fetcher.mock.calls.map(c=>String(c[0]))).toEqual(['/ws/rest/api/simulator/rec.enc','/submit']);expect(store.values.get('canonical')?.status).toBe('submitted')
  })
  it('rejects an HTML 200 with actionable diagnostics but keeps a 404 retryable',async()=>{
    const htmlStore=new MemoryStore(),htmlFetch=vi.fn(async()=>new Response('<!doctype html>',{status:200,headers:{'Content-Type':'text/html; charset=utf-8'}}))
    const htmlService=new BrowserLeaderboardService({store:htmlStore,fetch:htmlFetch as typeof fetch});await activate(htmlService);await htmlService.enqueue('T','html');await settle()
    expect(htmlFetch).toHaveBeenCalledTimes(1);expect(htmlStore.values.get('html')).toMatchObject({status:'failed_terminal',retryAllowed:false,error:'sealed recording content type is invalid (HTTP 200, observed text/html; charset=utf-8)'})
    const missingStore=new MemoryStore(),missingService=new BrowserLeaderboardService({store:missingStore,fetch:vi.fn(async()=>new Response('',{status:404,headers:{'Content-Type':'application/json; charset=utf-8'}})) as typeof fetch})
    await activate(missingService);await missingService.enqueue('T','missing');await settle();expect(missingStore.values.get('missing')).toMatchObject({status:'capture_error',retryAllowed:true,error:'sealed recording HTTP 404 (application/json; charset=utf-8)'})
  })
  it('waits for authority, then retires an unmatched stale 404 before any resume fetch',async()=>{
    const orchestration:OrchestrationState={kind:'orchestration_state',phase:'idle',busy:false,finalization:'idle',maps:[],tractors:[],implements:[],selection:{stopped:true,live:true,snapshotAgeMs:0}}
    const stale:LeaderboardState={kind:'leaderboard_state',status:'capture_error',attempt:0,runId:'old-run',retryAllowed:true,error:'sealed recording HTTP 404 (application/json)'}
    const store=new MemoryStore();await store.put({runId:'old-run',teamName:'Old Team',status:'capture_error',attempt:0,retryAllowed:true,error:stale.error})
    const fetcher=vi.fn(async()=>sealed()),published:string[]=[];const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,publish:s=>published.push(s.status)})
    expect(shouldRetireStale404(orchestration,stale)).toBe(true);await service.resume();expect(fetcher).not.toHaveBeenCalled();await activate(service);await settle()
    expect(store.values.get('old-run')).toMatchObject({status:'failed_terminal',retryAllowed:false,error:'retired stale capture: sealed recording HTTP 404 (application/json)'})
    expect(fetcher).not.toHaveBeenCalled();expect(published).toEqual(['failed_terminal'])

    const current={...orchestration,finalization:'completed' as const,recordingId:'old-run'}
    expect(shouldRetireStale404(current,stale)).toBe(false)
    expect(shouldRetireStale404(orchestration,{...stale,error:'network timeout'})).toBe(false)
    expect(shouldRetireStale404(orchestration,{...stale,error:'sealed recording HTTP 404 malformed'})).toBe(false)
  })
  it('keeps fallback reconciliation pending until another owner releases the row',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore(),row:OutboxRecord={runId:'leased-old',teamName:'Old',status:'capture_error',attempt:0,retryAllowed:true,error:'sealed recording HTTP 404 (application/json)'};await store.put(row)
      expect(await store.claim(row.runId,'owner-a',Date.now(),1000)).toBeTruthy()
      const fetcher=vi.fn(async()=>sealed()),published:string[]=[];const b=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,publish:s=>published.push(s.status),retireRetryMs:5})
      const reconciliation=activate(b);await settle();expect(store.values.get(row.runId)?.status).toBe('capture_error');expect(fetcher).not.toHaveBeenCalled()
      await store.release(row.runId,'owner-a');await new Promise(r=>setTimeout(r,25))
      await reconciliation;expect(store.values.get(row.runId)).toMatchObject({status:'failed_terminal',retryAllowed:false});expect(published).toEqual(['failed_terminal']);expect(fetcher).not.toHaveBeenCalled();b.shutdown()
    }finally{vi.unstubAllGlobals()}
  })
  it('retires after a crashed fallback lease expires without fetching the newer envelope',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore(),row:OutboxRecord={runId:'crashed-old',teamName:'Old',status:'capture_error',attempt:0,retryAllowed:true,error:'sealed recording HTTP 404 (<missing>)'};await store.put(row)
      expect(await store.claim(row.runId,'crashed',Date.now(),15)).toBeTruthy()
      const fetcher=vi.fn(async()=>sealed()),b=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,retireRetryMs:5})
      await activate(b);await new Promise(r=>setTimeout(r,20))
      expect(store.values.get(row.runId)?.status).toBe('failed_terminal');expect(fetcher).not.toHaveBeenCalled();b.shutdown()
    }finally{vi.unstubAllGlobals()}
  })
  it('cancels pending retirement when backend authority changes to the matching current run',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      let enter!:()=>void,release!:()=>void
      const entered=new Promise<void>(resolve=>{enter=resolve}),barrier=new Promise<void>(resolve=>{release=resolve})
      class TransactionBarrierStore extends MemoryStore{
        override async retireStale404(id:string,nowMs:number,canCommit:()=>boolean){
          enter();await barrier
          return super.retireStale404(id,nowMs,canCommit)
        }
      }
      const store=new TransactionBarrierStore(),row:OutboxRecord={runId:'becomes-current',teamName:'T',status:'capture_error',attempt:0,retryAllowed:true,error:'sealed recording HTTP 404 (application/json)'};await store.put(row)
      const fetcher=vi.fn(async(url:string|URL|Request)=>String(url)==='/ws/rest/api/simulator/rec.enc'?sealed():scored())
      const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit',retireRetryMs:5})
      const staleReconcile=activate(service);await entered
      const currentReconcile=activate(service,row.runId);release();await Promise.all([staleReconcile,currentReconcile]);await settle()
      expect(store.values.get(row.runId)).toMatchObject({status:'submitted',retryAllowed:false})
      expect(store.writes.some(r=>r.error?.startsWith('retired stale capture:'))).toBe(false)
      expect(fetcher).toHaveBeenCalledTimes(2);service.shutdown()
    }finally{vi.unstubAllGlobals()}
  })
  it('does not fetch before authority and resumes a matching completed run only after reconciliation',async()=>{
    const row:OutboxRecord={runId:'completed-run',teamName:'T',status:'capture_error',attempt:0,retryAllowed:true,error:'sealed recording HTTP 404 (application/json)'}
    const store=new MemoryStore();await store.put(row)
    const fetcher=vi.fn(async(url:string|URL|Request)=>String(url)==='/ws/rest/api/simulator/rec.enc'?sealed():scored())
    const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'})
    await service.resume();await settle();expect(fetcher).not.toHaveBeenCalled()
    await activate(service,row.runId);await settle()
    expect(fetcher).toHaveBeenCalledTimes(2);expect(store.values.get(row.runId)?.status).toBe('submitted')
  })
  it('migrates only the exact legacy MIME terminal and retries through normal ownership',async()=>{
    const store=new MemoryStore();await store.put({runId:'legacy',teamName:'T',status:'failed_terminal',attempt:3,retryAllowed:false,error:'sealed recording content type is invalid'});await store.put({runId:'other',teamName:'T',status:'failed_terminal',attempt:2,retryAllowed:false,error:'sealed recording envelope v2 is invalid'})
    const fetcher=vi.fn(async(url:string|URL|Request)=>String(url)==='/ws/rest/api/simulator/rec.enc'?sealed():scored()),service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'});await activate(service,'legacy');await settle()
    expect(store.values.get('legacy')).toMatchObject({status:'submitted',attempt:4,retryAllowed:false});expect(store.values.get('other')).toMatchObject({status:'failed_terminal',attempt:2,retryAllowed:false,error:'sealed recording envelope v2 is invalid'});expect(fetcher).toHaveBeenCalledTimes(2)
  })
  it('deduplicates a stable run id and resumes persisted pending work',async()=>{
    const store=new MemoryStore();await store.put({runId:'same',teamName:'T',status:'pending',attempt:0,retryAllowed:true,envelope:JSON.stringify(envelope)})
    const fetcher=vi.fn(async()=>scored());const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit',sleep:async()=>{}})
    await activate(service);await service.enqueue('T','same');await settle();expect(fetcher).toHaveBeenCalledTimes(1);expect(store.values.get('same')?.status).toBe('submitted')
  })
  it('re-reads durable state inside the cross-tab lock so a stale waiter never posts',async()=>{
    const store=new MemoryStore();await store.put({runId:'race',teamName:'T',status:'pending',attempt:0,retryAllowed:true,envelope:JSON.stringify(envelope)})
    let tail=Promise.resolve();const locks={request:vi.fn(async(_name:string,callback:()=>Promise<void>)=>{const turn=tail.then(callback);tail=turn.catch(()=>{});return turn})}
    vi.stubGlobal('navigator',{locks})
    try{
      const fetcher=vi.fn(async()=>scored())
      const a=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'})
      const b=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'})
      await Promise.all([activate(a),activate(b)])
      await Promise.all([a.enqueue('T','race'),b.enqueue('T','race')]);await tail
      expect(fetcher).toHaveBeenCalledTimes(1);expect(store.values.get('race')?.status).toBe('submitted')
    }finally{vi.unstubAllGlobals()}
  })
  it('atomically leases and deduplicates concurrent enqueue when Web Locks are unavailable',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore(),fetcher=vi.fn(async(url:string|URL|Request)=>String(url)==='/sealed'?sealed():scored())
      const a=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,simulatorUrl:'/sealed',leaderboardUrl:'/submit'})
      const b=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,simulatorUrl:'/sealed',leaderboardUrl:'/submit'})
      await Promise.all([activate(a),activate(b)])
      await Promise.all([a.enqueue('T','new-race'),b.enqueue('T','new-race')]);await settle()
      expect(fetcher.mock.calls.filter(c=>String(c[0])==='/submit')).toHaveLength(1);expect(store.values.get('new-race')?.status).toBe('submitted')
    }finally{vi.unstubAllGlobals()}
  })
  it('does not let same-tab retry or another tab release an active backoff lease',async()=>{
    vi.stubGlobal('navigator',{})
    let releaseDelay!:()=>void;const delay=new Promise<void>(resolve=>{releaseDelay=resolve})
    try{
      const store=new MemoryStore();await store.put({runId:'backoff',teamName:'T',status:'pending',attempt:0,retryAllowed:true,envelope:JSON.stringify(envelope)})
      let posts=0;const fetcher=vi.fn(async()=>++posts===1?new Response('',{status:503}):scored())
      const a=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit',sleep:async()=>delay})
      const b=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'})
      await activate(a);await activate(b);await settle();expect(posts).toBe(1);const owner=store.values.get('backoff')?.leaseOwner;expect(owner).toBeTruthy()
      await Promise.all([a.retry('backoff'),b.retry('backoff')]);await settle();expect(posts).toBe(1);expect(store.values.get('backoff')?.leaseOwner).toBe(owner);expect(store.values.get('backoff')?.attempt).toBe(1)
      releaseDelay();await settle();expect(posts).toBe(2);expect(store.values.get('backoff')?.status).toBe('submitted')
    }finally{vi.unstubAllGlobals()}
  })
  it('renews only the exact operation token and blocks takeover beyond the old expiry',async()=>{
    const store=new MemoryStore();await store.put({runId:'slow',teamName:'T',status:'pending',attempt:0,retryAllowed:true,envelope:JSON.stringify(envelope)})
    expect(await store.claim('slow','op-a',0,100)).toBeTruthy();expect(await store.renew('slow','wrong',90,100)).toBe(false);expect(await store.renew('slow','op-a',90,100)).toBe(true)
    expect(await store.claim('slow','op-b',101,100)).toBeUndefined();await store.release('slow','wrong');expect(store.values.get('slow')?.leaseOwner).toBe('op-a')
    await store.release('slow','op-a');expect(store.values.get('slow')?.leaseOwner).toBeUndefined()
  })
  it('allows takeover after a crashed operation lease expires',async()=>{
    const store=new MemoryStore();await store.put({runId:'crash-lease',teamName:'T',status:'pending',attempt:0,retryAllowed:true,envelope:JSON.stringify(envelope)})
    expect(await store.claim('crash-lease','crashed',0,100)).toBeTruthy();expect(await store.claim('crash-lease','early',99,100)).toBeUndefined();expect((await store.claim('crash-lease','takeover',101,100))?.leaseOwner).toBe('takeover')
  })
  it('times out and cancels a sealed body that never closes, then releases its lease',async()=>{
    vi.stubGlobal('navigator',{});let cancelled=false
    try{
      const store=new MemoryStore(),body=new ReadableStream<Uint8Array>({pull(){},cancel(){cancelled=true}})
      const fetcher=vi.fn(async()=>new Response(body,{status:200,headers:{'Content-Type':'application/octet-stream'}}))
      const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,fetchTimeoutMs:10});await activate(service);await service.enqueue('T','sealed-hang');await new Promise(r=>setTimeout(r,40))
      expect(cancelled).toBe(true);expect(store.values.get('sealed-hang')?.status).toBe('capture_error');expect(store.values.get('sealed-hang')?.leaseOwner).toBeUndefined()
    }finally{vi.unstubAllGlobals()}
  })
  it('times out leaderboard JSON body consumption and leaves a retryable result',async()=>{
    vi.stubGlobal('navigator',{});let calls=0,cancelled=0
    try{
      const store=new MemoryStore(),fetcher=vi.fn(async()=>{if(++calls===1)return sealed();return new Response(new ReadableStream<Uint8Array>({pull(){},cancel(){cancelled++}}),{status:200,headers:{'Content-Type':'application/json'}})})
      const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,fetchTimeoutMs:5,sleep:async()=>{}});await activate(service);await service.enqueue('T','json-hang');await new Promise(r=>setTimeout(r,80))
      expect(cancelled).toBe(5);expect(store.values.get('json-hang')?.status).toBe('retry_wait');expect(store.values.get('json-hang')?.leaseOwner).toBeUndefined()
    }finally{vi.unstubAllGlobals()}
  })
  it('cancels an oversized streaming sealed response',async()=>{
    vi.stubGlobal('navigator',{});let cancelled=false
    try{
      const store=new MemoryStore(),chunk=new Uint8Array(6*1024*1024),body=new ReadableStream<Uint8Array>({start(c){c.enqueue(chunk);c.enqueue(chunk)},cancel(){cancelled=true}})
      const service=new BrowserLeaderboardService({store,fetch:vi.fn(async()=>new Response(body,{status:200,headers:{'Content-Type':'application/octet-stream'}})) as typeof fetch});await activate(service);await service.enqueue('T','oversize');await settle()
      expect(cancelled).toBe(true);expect(store.values.get('oversize')?.status).toBe('capture_error')
    }finally{vi.unstubAllGlobals()}
  })
  it('handles renewal rejection without unhandled work or a POST',async()=>{
    vi.stubGlobal('navigator',{})
    try{
      class RejectRenewStore extends MemoryStore{override async renew(_id:string,_opToken:string,_nowMs:number,_leaseMs:number):Promise<boolean>{throw new Error('idb unavailable')}}
      const store=new RejectRenewStore();await store.put({runId:'renew-fail',teamName:'T',status:'pending',attempt:0,retryAllowed:true,envelope:JSON.stringify(envelope)})
      const fetcher=vi.fn(async()=>scored()),service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch});await activate(service);await settle()
      expect(fetcher).not.toHaveBeenCalled();expect(store.values.get('renew-fail')?.status).toBe('pending');expect(store.values.get('renew-fail')?.leaseOwner).toBeUndefined()
    }finally{vi.unstubAllGlobals()}
  })
  it.each([['scored',scored()],['terminal',new Response('',{status:422})]])('never overwrites a takeover during POST before %s persistence',async(_label,response)=>{
    vi.stubGlobal('navigator',{})
    try{
      const store=new MemoryStore();await store.put({runId:'takeover',teamName:'T',status:'pending',attempt:0,retryAllowed:true,envelope:JSON.stringify(envelope)})
      const published:string[]=[];let writesAtTakeover=0
      const fetcher=vi.fn(async()=>{const current=store.values.get('takeover')!;store.values.set('takeover',{...current,leaseOwner:'new-owner',leaseUntilMs:Date.now()+120_000,status:'submitted',retryAllowed:false,result:{verdict:'SCORED',score:99}});writesAtTakeover=store.writes.length;return response})
      const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,publish:s=>published.push(s.status)});await activate(service);await settle()
      const final=store.values.get('takeover');expect(final?.leaseOwner).toBe('new-owner');expect(final?.status).toBe('submitted');expect(final?.result?.score).toBe(99);expect(store.writes).toHaveLength(writesAtTakeover);expect(published).toEqual(['submitting'])
    }finally{vi.unstubAllGlobals()}
  })
  it('recovers persisted submitting without resetting the attempt budget',async()=>{
    const store=new MemoryStore();await store.put({runId:'crash',teamName:'T',status:'submitting',attempt:5,retryAllowed:false,envelope:JSON.stringify(envelope)})
    const fetcher=vi.fn(async()=>scored());const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,leaderboardUrl:'/submit'});await activate(service);await settle()
    expect(fetcher).not.toHaveBeenCalled();expect(store.values.get('crash')?.status).toBe('retry_wait');expect(store.values.get('crash')?.attempt).toBe(5)
  })
  it('keeps 404 and 5xx capture failures retryable but 4xx terminal',async()=>{
    for(const [code,wanted,retry] of [[404,'capture_error',true],[500,'capture_error',true],[400,'failed_terminal',false]] as const){
      const store=new MemoryStore();const service=new BrowserLeaderboardService({store,fetch:vi.fn(async()=>new Response('',{status:code})) as typeof fetch,simulatorUrl:'/sealed'});await activate(service);await service.enqueue('T',`r-${code}`);await settle();expect(store.values.get(`r-${code}`)?.status).toBe(wanted);expect(store.values.get(`r-${code}`)?.retryAllowed).toBe(retry)
    }
  })
  it('retries external 5xx exactly five times and never retries 4xx',async()=>{
    const store=new MemoryStore();const delays:number[]=[];let calls=0
    const fetcher=vi.fn(async()=>++calls===1?sealed():new Response('',{status:503}));const service=new BrowserLeaderboardService({store,fetch:fetcher as typeof fetch,simulatorUrl:'/sealed',leaderboardUrl:'/submit',sleep:async n=>{delays.push(n)}});await activate(service);await service.enqueue('T','five');await settle();expect(fetcher).toHaveBeenCalledTimes(6);expect(delays).toEqual([1000,2000,4000,8000]);expect(store.values.get('five')?.status).toBe('retry_wait')
    expect(store.values.get('five')?.error).toBe('leaderboard unavailable: HTTP 503')
    // 2.5초마다 오는 스냅샷이 소진된 기록을 다시 밀어 넣는다. 그때 사유가
    // "중단된 요청"으로 덮여 리더보드 장애가 우리 쪽 문제로 보이면 안 된다.
    // The 2.5 s snapshot pushes an exhausted record back through processing;
    // the reason must not be overwritten with "interrupted request", which
    // would read as our failure rather than a leaderboard outage.
    await activate(service);await settle()
    expect(store.values.get('five')?.error).toBe('leaderboard unavailable: HTTP 503')
    const terminalStore=new MemoryStore();let n=0;const terminalFetch=vi.fn(async()=>++n===1?sealed():new Response('',{status:422}));const terminal=new BrowserLeaderboardService({store:terminalStore,fetch:terminalFetch as typeof fetch,simulatorUrl:'/sealed',leaderboardUrl:'/submit'});await activate(terminal);await terminal.enqueue('T','bad');await settle();expect(terminalFetch).toHaveBeenCalledTimes(2);expect(terminalStore.values.get('bad')?.status).toBe('failed_terminal')
  })
})
