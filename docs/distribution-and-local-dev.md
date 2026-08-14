# Distribution boundary and local developer bridge

The versioned participant-source boundary is
`distribution/participant-deliverable-v1.json`. It includes the application, the
C++ SDK, feature model/generated sources, participant RDDF inputs, the canonical
UI runtime in `customui-src/`, and the guides. It excludes workflow evidence and
machine-local state (`artifacts/`, `provenance/`, `references/`, `.agmo/`,
`.serena/`), internal tooling (`tools/`), caches, dependency and build trees,
logs, validation routes/scripts, and the packaged `master_of_plow_app/ui` output.

The dashboard unit tests under `customui-src/src/dashboard/` are part of the
deliverable — participants run them with `npm run test` — but they are excluded
from the UI runtime hash because they are not FIF runtime content.

Run `python3 tools/check_distribution.py` from the repository root. It prints
the exact sorted file list, a content hash for that list, and the canonical UI
runtime hash. The checker fails if a required-absent validation artifact returns
or if runtime UI bytes differ from the recorded canonical baseline.

## `SEAMOS_DEV_LOCAL_SIM`

`TestSimMqttBridge` in `ApplicationMainImpl.cpp` is pre-existing local developer
support. It forwards local MQTT frames to the development WebSocket only when
the compile-time CMake option `SEAMOS_DEV_LOCAL_SIM` is explicitly enabled.
This bridge is not lateral-control logic and does not change steering math,
sensor fusion, RDDF parsing, or `implementFlag` behavior.

The root CMake project declares `SEAMOS_DEV_LOCAL_SIM` with default `OFF`.
Official FIF builds must leave it off. With the default, the bridge class and
startup path are removed by the preprocessor and the Paho MQTT dependency is not
linked. Enabling it emits a CMake warning and is permitted only for a local
developer build; that build is not an official FIF candidate.
