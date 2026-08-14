/**
 * Browser-side bridge that responds to REST API requests
 * relayed through Vite's custom HMR messages.
 */
import type { VehicleState } from './VehicleSystem'

interface BridgeHandlers {
  getState: () => { running: boolean; state: VehicleState }
  setEngine: (on: boolean) => void
  setControl: (throttle: number | undefined, steerAngle: number | undefined) => void
}

let handlers: BridgeHandlers | null = null

export function registerApiBridge(h: BridgeHandlers): () => void {
  handlers = h
  return () => { handlers = null }
}

if (import.meta.hot) {
  import.meta.hot.on('simulator:request', (data: { id: string; type: string; payload: Record<string, unknown> }) => {
    const { id, type, payload } = data
    let response: unknown = { ok: true }

    if (!handlers) {
      import.meta.hot!.send('simulator:response', { id, payload: { error: 'simulator not initialized' } })
      return
    }

    switch (type) {
      case 'status': {
        const { running, state } = handlers.getState()
        response = {
          running,
          latitude: state.latitude,
          longitude: state.longitude,
          speed_kmh: state.speed * 3.6,
          heading_deg: state.heading,
          throttle: state.throttle,
          steer_angle: state.steerAngle,
          rpm: state.rpm,
        }
        break
      }
      case 'engine': {
        const on = (payload as { state: string }).state === 'on'
        handlers.setEngine(on)
        response = { ok: true, running: on }
        break
      }
      case 'control': {
        const p = payload as { throttle?: number; steer_angle?: number }
        handlers.setControl(p.throttle, p.steer_angle)
        response = { ok: true, throttle: p.throttle, steer_angle: p.steer_angle }
        break
      }
    }

    import.meta.hot!.send('simulator:response', { id, payload: response })
  })
}
