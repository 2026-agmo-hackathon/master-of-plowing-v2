# Simulator

Tractor field simulator with terrain interaction, ridges, soil deformation, and basic vehicle physics.

## Run

```bash
npm install
npm run dev
```

Open:

```text
http://localhost:3002
```

Build:

```bash
npm run build
```

Run tests:

```bash
npm test
```

## Controls

- `Ignition`: click the ignition switch to start or stop the tractor.
- `↑`: increase throttle.
- `↓`: decrease throttle.
- `←`: steer left.
- `→`: steer right.
- `Space`: brake.
- `P`: toggle plow.
- `R`: toggle rotary.
- `H`: toggle sensor overlay.
- `Refresh`: reset the current map, tractor position, terrain deformation, and tire tracks.

## Backend WebSocket API

The simulator opens a WebSocket connection to `ws://<host>:1456/socket` and uses it as a fully bidirectional channel — telemetry flows out, commands flow in. There is no auth and no handshake; reconnect is automatic (3 s).

The host defaults to `window.location.hostname` (so `localhost` for `npm run dev`). To point at a remote backend, override the URL when constructing `SignalControllerClient`.

### sim → backend (telemetry, ≤ 10 Hz while engine is on)

```json
{
  "kind": "telemetry",
  "ts": 1700000000000,
  "state": {
    "latitude": 37.5665,
    "longitude": 126.978,
    "altitude": 0,
    "heading": 0,
    "speed": 0,
    "rpm": 900,
    "throttle": 0,
    "brake": 0,
    "steerAngle": 0,
    "measuredSteerAngleDeg": 0,
    "vehicleFrontWheelAngleDeg": 0,
    "motorCmdRpm": 0,
    "accelX": 0,
    "accelY": 0,
    "gyroZ": 0,
    "roll": 0,
    "pitch": 0,
    "yaw": 0,
    "gear": 1,
    "running": true
  },
  "implement": "plow"
}
```

| field | unit / range | notes |
|---|---|---|
| `ts` | ms epoch | `Date.now()` at send time |
| `state.latitude` / `state.longitude` | WGS84 degrees | |
| `state.altitude` | m | terrain height under vehicle |
| `state.heading` | deg, `[0, 360)` | 0 = north, CW positive |
| `state.speed` | m/s, signed | negative = reverse |
| `state.rpm` | engine rpm | |
| `state.throttle` / `state.brake` | `[0, 1]` | |
| `state.steerAngle` | deg | legacy alias of `vehicleFrontWheelAngleDeg` |
| `state.measuredSteerAngleDeg` | deg | steering wheel angle |
| `state.vehicleFrontWheelAngleDeg` | deg | front wheel angle |
| `state.motorCmdRpm` | rpm-equivalent, `[-500, 500]` | last steer-motor command applied |
| `state.accelX` / `state.accelY` | m/s² | body frame |
| `state.gyroZ` | rad/s | yaw rate |
| `state.roll` / `state.pitch` / `state.yaw` | deg | |
| `state.gear` | `-1` (R), `0` (N), `1..4` | |
| `state.running` | bool | |
| `implement` | `"plow"` \| `"rotary"` \| `null` | currently attached implement |

### backend → sim (commands, fire-and-forget)

Send any number of these as plain JSON text frames. Unknown `kind` values and malformed frames are silently ignored.

| message | semantics |
|---|---|
| `{"kind":"steer_motor","speedRpm":200}` | Steer-motor speed command, `[-500, 500]`. Internally normalized via `value/500` and routed through the same path as the device CAN protocol (sign is flipped to match vehicle frame). |
| `{"kind":"throttle","value":0.5}` | Throttle, `[0, 1]`. |
| `{"kind":"brake","value":0.3}` | Brake, `[0, 1]`. |
| `{"kind":"engine","on":true}` | Start / stop the engine (same as clicking the ignition). |
| `{"kind":"gear","gear":2}` | Manual gear: `-1` (R), `1..4`, or `null` (auto). |
| `{"kind":"implement","type":"plow"}` | Attach an implement: `"plow"`, `"rotary"`, or `null` to detach. |
| `{"kind":"inject_state","latitude":37.5,"longitude":127.0,"heading":90}` | Teleport the tractor. `altitude` and `heading` optional. |
| `{"kind":"reset"}` | Reset map, tractor position, terrain deformation, and tire tracks. |

Backend is the WS server; the simulator is the client. If no backend is listening on 1456 the simulator just keeps reconnecting in the background and remains fully operable from the keyboard / HUD.

### Standalone vs connected mode

The `VITE_STANDALONE` env var (in `.env`) decides whether the simulator tries to connect:

```bash
VITE_STANDALONE=0   # connect to ws://<host>:1456/socket   ← default for backend dev
VITE_STANDALONE=1   # never open a WebSocket               ← for offline / demo use
```

You can also override at runtime with `?standalone=1` in the URL.

### Browser tab throttling caveat

The simulator runs its physics on `requestAnimationFrame`, which Chrome/Safari pause when the tab is in the background. While the sim tab is hidden, telemetry stops flowing — the dashboard then shows a `stale (sim tab backgrounded?)` banner. To watch the dashboard live, **open the simulator and the dashboard in separate windows** (e.g. drag the dashboard tab out, place the two windows side-by-side); both stay foregrounded and the WS stream keeps ticking.
