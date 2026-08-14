# Field Mode E2E Checklist

Manual verification for `?mode=field` regression. Run after every PR that touches field-mode code.

## Pre-flight
- Dev server running at http://localhost:3002
- signal-controller available at ws://localhost:8765 (optional but required for items 7-8)
- MQTT broker available at ws://localhost:8121 (optional but required for item 7)

## Items
- [ ] 1. Open `http://localhost:3002/?mode=field` -> EngineOffOverlay is visible (background blurred, "Press to Start" centered, key hint at bottom). HUD start button is clickable through the overlay.
- [ ] 2. Click HUD start button -> overlay fades out within ~200ms; tractor and field become fully visible.
- [ ] 3. Keyboard inputs work: W/S throttle, A/D steer, up/down hand throttle, Space brake, P plow toggle, R rotary toggle, H sensors overlay toggle, C camera mode toggle. Engine ON/OFF is only via HUD button (no keyboard shortcut).
- [ ] 4. Enable plow (P), drive forward -> soil color darkens behind the tractor (tilled track), RPM drops, top speed reduced compared to plow OFF run (draft force feedback).
- [ ] 5. Press C -> free camera (OrbitControls) responds to mouse drag/scroll. Press C again -> follow camera resumes without jumping to (0,0,0).
- [ ] 6. Open `http://localhost:3002/` (no params) -> legacy SimulatorView still renders. Open `?mode=field` again in a new tab -> both modes coexist without errors.
- [ ] 7. With MQTT broker running, observe SignalPublisher topics fire normally (use mqtt-cli or browser devtools network tab). No regression vs `/` mode.
- [ ] 8. With signal-controller running, ws://localhost:8765 receives `manual_inject` payloads from FieldSimulatorView. State broadcast back is consumed without errors.
