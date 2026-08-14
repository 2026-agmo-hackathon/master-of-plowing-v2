# Master of Plowing — participant skeleton

A working SeamOS feature skeleton for the 2026 AGMO hackathon. Clone it, open it
in the SeamOS IDE, and start writing your path-tracking code — the run
transaction (setup, start, finish, sealed-recording submission, reset) is
already implemented and verified end to end against the official emulator.

The full participant instructions are in **[HACKATHON_GUIDE.md](HACKATHON_GUIDE.md)**
(also available in [English](HACKATHON_GUIDE.en.md), [Deutsch](HACKATHON_GUIDE.de.md),
and [ไทย](HACKATHON_GUIDE.th.md)).

## Quick start

```bash
git clone https://github.com/2026-agmo-hackathon/master-of-plowing-v2.git
cd master-of-plowing-v2
```

Then in the SeamOS IDE: **File → Open Project…** and pick the cloned directory.

The IDE recognises the project from `com.bosch.fsp.master_of_plow/FDProject.props`,
which points at `master_of_plow_app` and `master_of_plow_CPP_SDK` by relative
path, so no per-machine setup is needed. `.mcp.json`, `.seamos-context.json`, and
`.seamos-workspace.json` are machine-local and deliberately not committed; the
IDE writes its own.

Build all three modules (SDK, App, Test) from the toolbar, then package a FIF and
install it through the emulator's normal install flow. See
[docs/official-fif-validation-workflow.md](docs/official-fif-validation-workflow.md).
Copying an executable into an installed feature is not a supported build or
validation path.

## Layout

| Path | What it is |
|---|---|
| `com.bosch.fsp.master_of_plow/` | FSP — the authoritative feature model (`.ecore`, `.fgd`) |
| `com.bosch.fsp.master_of_plow.gen{,.tests}/` | Generated Java codegen templates |
| `master_of_plow_app/` | The C++ application and its unit tests |
| `master_of_plow_app/src-gen/AppMain/tracking/` | Path-tracking skeleton — **this is what you write** |
| `master_of_plow_CPP_SDK/` | SeamOS C++ SDK, bundled so a clone builds anywhere |
| `customui-src/` | Dashboard **source** (React + Vite) |
| `master_of_plow_app/ui/` | Dashboard **build output** — what ships in the FIF's `static/` |
| `rddf/` | Your RDDF inputs, plus the upload helper and format docs |
| `seamos-assets/` | Marketplace listing images, and `builds/` where packaged FIFs land |
| `docs/` | Run orchestration, FIF validation, and local-dev notes |

`master_of_plow_app/ui/` is the dashboard build output, and it **is** committed
so that a fresh clone packages a working FIF immediately — including with
`build-fif.sh` / `nvx-fif-gen`, which copy the app directory as-is and never run
npm. (The IDE's own FIF build regenerates it before packaging either way.)

Rebuild it whenever you change the dashboard, and commit the result together
with your source change:

```bash
cd customui-src
npm ci
npx vite build --outDir ../master_of_plow_app/ui --emptyOutDir
```

If you forget, your FIF ships the previous dashboard — the C++ side will look
correct while the screen does not match your code.

`seamos-assets/` holds the marketplace listing images in the layout the
`upload-app` flow expects — `iconImage.png`, `mainImage.png`, and
`screenshots/screenshot*.png` — plus `builds/`, where `build-fif` writes the
packaged `.fif`. The `.fif` files are build output and are not committed.

Dashboard unit tests, from `customui-src/`:

```bash
npm run test
```

C++ unit tests build with the App/Test modules from the IDE.

`customui-src/` holds only what the dashboard ships: `src/main.tsx`,
`src/dashboard/**`, and the eight static assets `index.html` references (four
favicons and four fonts). Its runtime dependencies are React, `@seamos/ads`, and
`@seamos/connect` — there is no 3D engine, map library, or asset pipeline in the
way. Add whatever your dashboard needs.

## What the app already does

The dashboard shows the live map / tractor / implement catalogs and your team
name, and drives one safe, exactly-once run transaction through the app backend:

1. **Start** is granted only after the backend confirms fresh selections,
   recorder state, engine state, and the selected vehicle geometry.
2. **Finish** (and automatic RDDF completion) share the same finalization: the
   app commands a stop while holding autonomous authority, confirms the vehicle
   is at rest, confirms the recorder stopped, archives the CSV, and only then
   cuts the engine. That order matters — see
   [docs/run-orchestration.md](docs/run-orchestration.md).
3. **Submit** fetches the unchanged sealed recording (`.csv.enc`) through the
   same-origin simulator REST bridge, persists it in IndexedDB, and submits it
   to the leaderboard from the browser. The C++ feature never calls the external
   leaderboard itself.
4. **Reset** clears the completed run so the next one can start.

Submissions are durable: if the leaderboard is unreachable the envelope is kept
and retried with the documented 1/2/4/8 s backoff, and a manual **Retry
leaderboard** button appears. Nothing is lost across a browser restart.

> **Known external issue.** The leaderboard currently answers every submission
> with `503 {"error":"rec private key … is not a PKCS#8 PEM"}`. That is a
> server-side key configuration problem, not something to work around in your
> app — your run is stored and will submit once it is fixed. Tracked in
> AGMO-Inc/seamos-simulator#13.

## RDDF inputs

Participant RDDFs are pass-through: the app preserves the submitted speed and
rejects malformed values, unsafe spacing, flags outside the documented 0/1
implement domain, and speeds above the 7.0 km/h safety ceiling. A nonzero speed
below the sustainable 2.05 km/h creep floor, or geometry tighter than the
measured vehicle limit, is loaded unchanged and reported as a warning — those
are tracking limitations, not file-format errors. Accepted waypoints are tracked
without smoothing, speed changes, or flag relabeling.

Static RDDF validation deliberately does not compare waypoint 0 against a
fabricated start position. The loader has no live map spawn; the session start
gate compares the actual fresh pose with waypoint 0 before granting motion
authority.

The shipped RDDFs are the three participant inputs (`1.rddf`, `2.rddf`,
`3.rddf`) plus documentation and the upload helper. No route generator or
transformed validation fixture is included.

## Distribution boundary

`distribution/participant-deliverable-v1.json` records exactly what belongs in
this skeleton and what is deliberately excluded (validation artifacts, internal
tooling, emulator operations notes, build outputs, caches).
