# Participant RDDF files

`1.rddf`, `2.rddf`, and `3.rddf` are the participant route inputs retained by
this source skeleton. Upload instructions are in `how-to-upload-rddf.md`.

The application parses and validates an RDDF exactly as received. It does not
smooth geometry, relabel `implementFlag`, or change a submitted speed.
`implementFlag` controls only implement up/down and has no effect on lateral
steering.

Validation accepts zero as an explicit stop waypoint. Every nonzero speed must
have magnitude 2.05..7.0 km/h; unsupported values are rejected with a clear
error rather than clamped. Invalid indices, fields, spacing, flags, non-finite
data, and geometry tighter than the vehicle's physical limit are also rejected.

Start-position proximity is not intrinsic to the file and is not checked
against a fabricated position. Before granting motion authority, the official
simulator/session must compare its actual fresh pose with waypoint 0.

Earlier straight, HL24, original-prefix, and R10 routes and generator scripts
were validation-only diagnostics for a replaced simulator. They were removed
from the participant deliverable. Their non-official evidence remains under
`artifacts/validation-runs/` and is described in
`docs/emulator-simulator-defects.md`.
