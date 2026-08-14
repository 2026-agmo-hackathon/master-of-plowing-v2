export const MAX_RECORDING_BYTES=64*1024*1024
export const RECORDING_CHUNK_BYTES=32*1024

export type RecordingTransfer={
  chunks:Uint8Array[]
  sha?:string
  total:number
  received:number
  next:number
  maxChunks:number
}

export function beginRecording(id:unknown,total:unknown,sha?:string):RecordingTransfer|null {
  if(typeof id!=='string' || !id.endsWith('.csv') || id.slice(0,-4).endsWith('.csv')
    || !Number.isSafeInteger(total)
    || (total as number)<1 || (total as number)>MAX_RECORDING_BYTES)return null
  return {chunks:[],sha,total:total as number,received:0,next:0,
    maxChunks:Math.ceil((total as number)/RECORDING_CHUNK_BYTES)}
}

export function appendRecording(transfer:RecordingTransfer,index:unknown,encoded:unknown):string|null {
  if(!Number.isSafeInteger(index) || index!==transfer.next || (index as number)>=transfer.maxChunks
    || typeof encoded!=='string')return 'Recording chunk sequence mismatch'
  let raw=''
  try { raw=atob(encoded) } catch { return 'Invalid recording chunk encoding' }
  if(raw.length<1 || raw.length>RECORDING_CHUNK_BYTES)return 'Invalid recording chunk size'
  transfer.chunks.push(Uint8Array.from(raw,(c)=>c.charCodeAt(0)))
  transfer.next++
  transfer.received+=raw.length
  return transfer.received>transfer.total ? 'Recording exceeds declared size' : null
}

export function completeRecording(transfer:RecordingTransfer,count:unknown):boolean {
  return Number.isSafeInteger(count) && count===transfer.next
    && (count as number)<=transfer.maxChunks && transfer.received===transfer.total
}
