import { expect, test } from '@playwright/test'

test('run controls stack at 320px and adapt at tablet/desktop widths', async ({ page }) => {
  for (const width of [320, 768, 1280]) {
    await page.setViewportSize({ width, height: 900 })
    await page.goto('/')
    const actions = page.getByTestId('run-actions')
    await expect(actions).toBeVisible()
    const buttons = actions.locator('button')
    await expect(buttons).toHaveCount(2)
    const first = await buttons.nth(0).boundingBox()
    const second = await buttons.nth(1).boundingBox()
    expect(first).not.toBeNull()
    expect(second).not.toBeNull()
    if (width === 320) {
      expect(Math.abs(first!.x - second!.x)).toBeLessThan(2)
      expect(second!.y).toBeGreaterThan(first!.y + first!.height - 2)
    } else {
      expect(second!.x).toBeGreaterThan(first!.x + first!.width - 2)
      expect(Math.abs(first!.y - second!.y)).toBeLessThan(2)
    }
    const setup = await page.getByTestId('run-fields').boundingBox()
    expect(setup).not.toBeNull()
    expect(setup!.x).toBeGreaterThanOrEqual(0)
    expect(setup!.x + setup!.width).toBeLessThanOrEqual(width)
  }
})

test('team input preserves focus and IME text without simulator setup messages', async ({ page }) => {
  await page.route('**/get_assigned_ports', route => route.fulfill({
    status: 200, contentType: 'application/json', body: '{"app":1456}',
  }))
  await page.addInitScript(() => {
    const sent: string[]=[]
    ;(globalThis as unknown as { __sent: string[] }).__sent=sent
    class FakeWebSocket extends EventTarget {
      static OPEN=1; static CLOSED=3
      readyState=1
      constructor() {
        super()
        setTimeout(() => {
          this.dispatchEvent(new Event('open'))
          this.dispatchEvent(new MessageEvent('message',{data:JSON.stringify({
            kind:'sim_setup_state', phase:'idle', busy:false,
            maps:[{id:'map-a',label:'Map A'}],
            tractors:[{id:'tractor-a',label:'Tractor A'}],
            implements:[{id:'none',label:'None',widthM:0}],
            selection:{mapId:'map-a',tractorId:'tractor-a',implementId:'none',
              stopped:true,live:true,snapshotAgeMs:10},
            confirmedSetup:{mapId:'map-a',tractorId:'tractor-a',implementId:'none'},
          })}))
          this.dispatchEvent(new MessageEvent('message',{data:JSON.stringify({
            kind:'pp_telemetry',runState:'idle',dtMs:100,
          })}))
        },0)
      }
      send(value:string) { sent.push(value) }
      close() { this.readyState=3; this.dispatchEvent(new Event('close')) }
    }
    Object.defineProperty(globalThis,'WebSocket',{value:FakeWebSocket})
  })
  await page.goto('/')
  const input=page.getByLabel('Team name')
  await expect(input).toBeEnabled()
  await input.focus()
  await input.pressSequentially('밭갈이팀')
  await expect(input).toBeFocused()
  await expect(input).toHaveValue('밭갈이팀')
  await input.evaluate((element) => {
    const C=(globalThis as unknown as {CompositionEvent:new(type:string,init:unknown)=>never}).CompositionEvent
    element.dispatchEvent(new C('compositionstart',{data:'한'}))
    element.dispatchEvent(new C('compositionend',{data:'한'}))
  })
  await expect(input).toBeFocused()
  expect(await page.evaluate(() => localStorage.getItem('master_of_plow_teamName')))
    .toBe('밭갈이팀')
  const setupMessages=await page.evaluate(() =>
    (globalThis as unknown as { __sent:string[] }).__sent
      .map((raw)=>JSON.parse(raw) as {kind?:string})
      .filter((message)=>message.kind==='set_sim_setup'))
  expect(setupMessages).toEqual([])
  page.on('dialog', dialog => void dialog.accept())
  await page.getByRole('button',{name:'Start'}).click()
  const start=await page.evaluate(() =>
    (globalThis as unknown as { __sent:string[] }).__sent
      .map((raw)=>JSON.parse(raw) as Record<string,unknown>)
      .find((message)=>message.kind==='start_run'))
  expect(start).toMatchObject({teamName:'밭갈이팀',implementId:'none'})
})
