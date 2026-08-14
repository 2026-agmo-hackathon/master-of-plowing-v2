import {describe,expect,it} from 'vitest'
import {appendRecording,beginRecording,completeRecording,MAX_RECORDING_BYTES} from './recordingTransfer'

describe('recording transfer trust boundary',()=>{
  it('accepts an exact sequenced CSV and rejects duplicate/out-of-order chunks',()=>{
    const transfer=beginRecording('run.csv',3,'abc')!
    expect(appendRecording(transfer,0,btoa('abc'))).toBeNull()
    expect(completeRecording(transfer,1)).toBe(true)
    expect(appendRecording(transfer,0,btoa('x'))).toBe('Recording chunk sequence mismatch')
  })

  it('enforces id, total, chunk encoding, chunk count and declared bytes without crypto',()=>{
    expect(beginRecording('run.csv.csv',0)).toBeNull()
    expect(beginRecording('run.csv',MAX_RECORDING_BYTES+1)).toBeNull()
    const malformed=beginRecording('run.csv',1)!
    expect(appendRecording(malformed,0,'%%%')).toBe('Invalid recording chunk encoding')
    const short=beginRecording('run.csv',2)!
    expect(appendRecording(short,0,btoa('x'))).toBeNull()
    expect(completeRecording(short,1)).toBe(false)
    expect(appendRecording(short,1,btoa('x'))).toBe('Recording chunk sequence mismatch')
  })
})
