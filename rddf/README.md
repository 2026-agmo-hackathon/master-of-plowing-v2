# Participant RDDF files

`1.rddf`, `2.rddf`, and `3.rddf` are the participant route inputs retained by
this source skeleton. Upload instructions are in `how-to-upload-rddf.md`.

The application parses and validates an RDDF exactly as received. It does not
smooth geometry, relabel `implementFlag`, or change a submitted speed.
`implementFlag` controls only implement up/down and has no effect on lateral
steering.

Validation accepts zero as an explicit stop waypoint. A magnitude above
7.0 km/h is rejected with a clear error rather than clamped. A nonzero
magnitude below the machine's continuous floor of 2.05 km/h is loaded
unchanged and reported as a warning, because the speed controller may apply
its creep floor at runtime. Invalid indices, fields, flags, non-finite data,
and spacing outside 0.05..5.0 m are rejected. Geometry tighter than the
vehicle's minimum turn radius (about 4.35 m) is loaded unchanged and reported
as a tracking-limit warning.

Start-position proximity is not intrinsic to the file and is not checked
against a fabricated position. Before granting motion authority, the official
simulator/session must compare its actual fresh pose with waypoint 0.

Earlier straight, HL24, original-prefix, and R10 routes and generator scripts
were validation-only diagnostics for a replaced simulator. They were removed
from the participant deliverable. Their non-official evidence remains under
`artifacts/validation-runs/` and is described in
`docs/emulator-simulator-defects.md`.
