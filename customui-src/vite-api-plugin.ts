/**
 * Vite plugin that adds REST API endpoints to the dev server.
 * Communicates with the browser via Vite's custom HMR messages
 * to read/write simulator state.
 */
import type { Plugin } from 'vite'
import type { IncomingMessage, ServerResponse } from 'http'

function readBody(req: IncomingMessage): Promise<string> {
  return new Promise((resolve, reject) => {
    let body = ''
    req.on('data', (chunk: Buffer) => { body += chunk.toString() })
    req.on('end', () => resolve(body))
    req.on('error', reject)
  })
}

function cors(res: ServerResponse): void {
  res.setHeader('Access-Control-Allow-Origin', '*')
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type')
}

function json(res: ServerResponse, status: number, data: unknown): void {
  cors(res)
  res.statusCode = status
  res.setHeader('Content-Type', 'application/json')
  res.end(JSON.stringify(data))
}

export default function simulatorApiPlugin(): Plugin {
  return {
    name: 'simulator-api',
    configureServer(server) {
      const pending = new Map<string, (payload: unknown) => void>()

      server.ws.on('simulator:response', (data: { id: string; payload: unknown }) => {
        const resolve = pending.get(data.id)
        if (resolve) {
          pending.delete(data.id)
          resolve(data.payload)
        }
      })

      function sendToBrowser(type: string, payload: unknown, timeout = 2000): Promise<unknown> {
        return new Promise((resolve, reject) => {
          const id = Math.random().toString(36).slice(2)
          pending.set(id, resolve)
          server.ws.send('simulator:request', { id, type, payload })
          setTimeout(() => {
            if (pending.has(id)) {
              pending.delete(id)
              reject(new Error('timeout'))
            }
          }, timeout)
        })
      }

      // Must be registered before Vite's own middleware (return early)
      server.middlewares.use(async (req: IncomingMessage, res: ServerResponse, next: () => void) => {
        const url = req.url ?? ''

        // CORS preflight
        if (req.method === 'OPTIONS' && url.startsWith('/api/')) {
          cors(res)
          res.statusCode = 204
          res.end()
          return
        }

        // GET /api/status
        if (url === '/api/status' && req.method === 'GET') {
          try {
            const status = await sendToBrowser('status', {})
            json(res, 200, status)
          } catch {
            json(res, 503, { error: 'simulator not connected' })
          }
          return
        }

        // POST /api/engine
        if (url === '/api/engine' && req.method === 'POST') {
          try {
            const body = JSON.parse(await readBody(req))
            if (body.state !== 'on' && body.state !== 'off') {
              json(res, 400, { error: 'state must be "on" or "off"' })
              return
            }
            const result = await sendToBrowser('engine', { state: body.state })
            json(res, 200, result)
          } catch {
            json(res, 503, { error: 'simulator not connected or invalid request' })
          }
          return
        }

        // POST /api/control
        if (url === '/api/control' && req.method === 'POST') {
          try {
            const body = JSON.parse(await readBody(req))
            const throttle = typeof body.throttle === 'number' ? Math.max(0, Math.min(1, body.throttle)) : undefined
            const steerAngle = typeof body.steer_angle === 'number' ? Math.max(-45, Math.min(45, body.steer_angle)) : undefined
            if (throttle === undefined && steerAngle === undefined) {
              json(res, 400, { error: 'provide throttle (0-1) and/or steer_angle (-45 to 45)' })
              return
            }
            const result = await sendToBrowser('control', { throttle, steer_angle: steerAngle })
            json(res, 200, result)
          } catch {
            json(res, 503, { error: 'simulator not connected or invalid request' })
          }
          return
        }

        next()
      })
    },
  }
}
