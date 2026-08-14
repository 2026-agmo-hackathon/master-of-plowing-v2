import { chromium } from 'playwright'
import { writeFileSync } from 'node:fs'

const SIZE = 1024
const html = `<!doctype html><html><body style="margin:0">
<canvas id="c" width="${SIZE}" height="${SIZE}"></canvas>
<script>
const c = document.getElementById('c')
const ctx = c.getContext('2d')
const S = ${SIZE}

// Deterministic PRNG (mulberry32)
function rng(seed){ return ()=>{ seed=(seed+0x6D2B79F5)>>>0; let t=seed; t=Math.imul(t^(t>>>15),t|1); t^=t+Math.imul(t^(t>>>7),t|61); return ((t^(t>>>14))>>>0)/4294967296 }}
const rand = rng(20260512)

// Step 1: base dry dirt color, lightly varying
const baseImg = ctx.createImageData(S, S)
function mix(a,b,t){ return a*(1-t)+b*t }
function noise2(seed){
  // simple value-noise lookup via random grid
  const N = 256
  const grid = new Float32Array(N*N)
  const r = rng(seed)
  for (let i=0;i<grid.length;i++) grid[i] = r()
  return (x,y)=>{
    const fx = (x*0.1) % N, fy = (y*0.1) % N
    const xi = Math.floor(fx), yi = Math.floor(fy)
    const ax = fx-xi, ay = fy-yi
    const xi1 = (xi+1)%N, yi1 = (yi+1)%N
    const v00 = grid[yi*N+xi], v10 = grid[yi*N+xi1]
    const v01 = grid[yi1*N+xi], v11 = grid[yi1*N+xi1]
    const tx = ax*ax*(3-2*ax), ty = ay*ay*(3-2*ay)
    return mix(mix(v00,v10,tx), mix(v01,v11,tx), ty)
  }
}
const nA = noise2(1), nB = noise2(2), nC = noise2(3)
for (let y=0;y<S;y++){
  for (let x=0;x<S;x++){
    const i = (y*S+x)*4
    // Multi-octave: coarse + fine
    const v = 0.55*nA(x*0.3, y*0.3) + 0.30*nB(x*1.2, y*1.2) + 0.15*nC(x*4, y*4)
    // Map to tan/brown range
    const r = Math.round(mix(135, 195, v))
    const g = Math.round(mix(108, 168, v))
    const b = Math.round(mix( 78, 128, v))
    baseImg.data[i]   = r
    baseImg.data[i+1] = g
    baseImg.data[i+2] = b
    baseImg.data[i+3] = 255
  }
}
ctx.putImageData(baseImg, 0, 0)

// Step 2: add fine granular speckle (pebbles / sand grains)
for (let i=0;i<6500;i++){
  const x=rand()*S, y=rand()*S
  const r=0.4+rand()*1.4
  const shade = rand()<0.5 ? 60 : 230
  ctx.fillStyle = 'rgba('+shade+','+(shade-10)+','+(shade-25)+','+(0.07+rand()*0.18)+')'
  ctx.beginPath(); ctx.arc(x,y,r,0,6.283); ctx.fill()
}

// Step 3: scattered darker clumps (soil aggregates)
for (let i=0;i<260;i++){
  const x=rand()*S, y=rand()*S
  const r=4+rand()*8
  ctx.fillStyle = 'rgba(82,65,42,'+(0.18+rand()*0.18)+')'
  ctx.beginPath(); ctx.arc(x,y,r,0,6.283); ctx.fill()
}
// lighter dry spots
for (let i=0;i<160;i++){
  const x=rand()*S, y=rand()*S
  const r=6+rand()*10
  ctx.fillStyle = 'rgba(215,190,148,'+(0.10+rand()*0.18)+')'
  ctx.beginPath(); ctx.arc(x,y,r,0,6.283); ctx.fill()
}

// Step 4: WEEDS — clumps of small green strokes
function drawWeedClump(cx, cy, count){
  for (let i=0;i<count;i++){
    const ang = (rand()-0.5)*1.6
    const len = 14+rand()*28
    const x1 = cx + (rand()-0.5)*16
    const y1 = cy + (rand()-0.5)*16
    const x2 = x1 + Math.sin(ang)*len
    const y2 = y1 - Math.cos(ang)*len  // upward strokes
    const greenMix = rand()
    const r = Math.round(mix(40, 95, greenMix))
    const g = Math.round(mix(120, 190, greenMix))
    const b = Math.round(mix(28, 55, greenMix))
    ctx.strokeStyle = 'rgba('+r+','+g+','+b+','+(0.6+rand()*0.35)+')'
    ctx.lineWidth = 1.8+rand()*1.6
    ctx.beginPath()
    ctx.moveTo(x1,y1)
    ctx.lineTo(x2,y2)
    ctx.stroke()
    // tip dot
    ctx.fillStyle = 'rgba('+(r+10)+','+(g+15)+','+(b-5)+',0.85)'
    ctx.beginPath(); ctx.arc(x2,y2,1.6,0,6.283); ctx.fill()
  }
}

// Place ~240 weed clumps with varied density across the texture
for (let i=0;i<240;i++){
  const cx = rand()*S, cy = rand()*S
  const count = 8 + Math.floor(rand()*22)  // 8..29 strokes per clump
  drawWeedClump(cx, cy, count)
}
// A few denser tufts
for (let i=0;i<55;i++){
  const cx = rand()*S, cy = rand()*S
  drawWeedClump(cx, cy, 38 + Math.floor(rand()*30))
}

// Step 5: final subtle darkening at random spots for variation
for (let i=0;i<80;i++){
  const x=rand()*S, y=rand()*S
  const r=14+rand()*22
  const grad = ctx.createRadialGradient(x,y,1,x,y,r)
  grad.addColorStop(0,'rgba(60,48,30,0.12)')
  grad.addColorStop(1,'rgba(60,48,30,0)')
  ctx.fillStyle = grad
  ctx.beginPath(); ctx.arc(x,y,r,0,6.283); ctx.fill()
}

window._done = c.toDataURL('image/jpeg', 0.92)
</script></body></html>`

const browser = await chromium.launch()
const page = await browser.newPage({ viewport:{ width:SIZE, height:SIZE }})
await page.setContent(html)
await page.waitForFunction(()=> typeof window._done === 'string', { timeout: 20000 })
const dataUrl = await page.evaluate(()=> window._done)
await browser.close()
const b64 = dataUrl.replace(/^data:image\/jpeg;base64,/, '')
writeFileSync('public/textures/soil/albedo.jpg', Buffer.from(b64, 'base64'))
console.log('wrote public/textures/soil/albedo.jpg', Math.round(Buffer.from(b64,'base64').length/1024), 'KB')
