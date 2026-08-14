# SeamOS Hackathon 2026: The Master of Plowing — Participant Guide

> This document is the hands-on guide for SeamOS Hackathon 2026 participants.
> For the competition background and scoring criteria refer to the separate design document; this guide covers the end-to-end procedure from **repository setup → writing the RDDF → applying it to the app**.

---

## 1. Competition Overview

- **Mission**: Control a virtual tractor to plow the **"Alpha"** field with the **shortest time + maximum coverage**
- **Hard constraint**: Maximum driving speed **7 km/h** (an RDDF value above 7 km/h is rejected, never silently clamped)
- **Penalties (3 types)**
  - **Reverse in the plowed area** — the plowed state at that location is reset (unplowed)
  - **Leaving the field** — the timer runs **10× faster** while outside the boundary
  - **Cumulative path error at finish** — the cumulative Cross-Track Error (RMS) is added to the final time
- **Evaluation**: Both **elapsed time** and **coverage rate (minimizing unplowed area)** contribute to the score.
  - There is **no hard cutoff** such as 95% — you may submit a result at any coverage rate
  - However, a lower coverage rate penalizes your score proportionally

> **Key point**: What participants write and submit is the **RDDF (Route/Road Definition Data File)**.
> The tractor control algorithm itself is handled by the provided skeleton; participants express the **optimal route for plowing the field** as an RDDF and submit it.

---

## 2. Prerequisites

| Item | Notes |
|------|-------|
| **SeamOS IDE (FeatureDesigner)** | Use the version distributed by the organizers |
| **Claude Code** | Install the latest version from the official site |
| **SeamOS Everywhere** | Use the version distributed by the organizers |


---

## 3. Repository Structure

```
master_of_plow/
├── com.bosch.fsp.master_of_plow/   # FSP (Feature Spec Project)
├── master_of_plow_app/             # App body (C++ code, config)
│   └── src-gen/AppMain/tracking/   # Path-tracking skeleton — read-only reference (§10)
├── master_of_plow_CPP_SDK/         # SeamOS C++ SDK (provided)
├── customui-src/                   # Dashboard source (React) — builds into master_of_plow_app/ui/
├── rddf/                           # RDDF area for participants to write/validate
├── docs/                           # Run transaction and FIF validation notes
└── HACKATHON_GUIDE.md              # This document
```

### 3.1 Opening in SeamOS IDE

1. Launch SeamOS IDE
2. **File → Open Project...**
3. Specify the cloned `master_of_plow/` path and open

### 3.2 Build Verification

Use SeamOS IDE to build the project and verify your environment is working correctly by running the app and tests.

#### 1. Perform the build

Click the **Build** button in the top toolbar and click **Build all** to build the entire project.
- SDK (SeamOS C++ SDK)
- App (app body)
- Test (local test module)

All three modules must build successfully.

#### 2. Package, install, and verify

Run the local tests, then use the organizer-provided SDK/Feature Designer (or
its CI pipeline) to produce the FIF. Install that FIF through the emulator's
normal install workflow. The user selects and starts the installed feature.
Copying a loose executable into the emulator is not a supported build or test
flow. Record the FIF and source hashes; see
`docs/official-fif-validation-workflow.md`.

---

## 4. What Is an RDDF?

**RDDF (Road Definition Data File)** is a **waypoint sequence** that the tractor follows.
It is the essential deliverable that participants submit in the competition, and it defines:

- Where to go (latitude/longitude)
- How fast to travel (speed)
- Forward/reverse (sign of speed)
- Whether to **engage or disengage** the implement at each point (implement flag)

### 4.1 File Format

**9 columns** separated by tabs (`\t`), one waypoint per line:

```
lineNo  index  lat         lon          res1  res2  res3  speed  implementFlag
1       1      35.8000317  126.8807033  0.0   0.0   0.0   3.00   1
2       2      35.8000317  126.8807141  0.0   0.0   0.0   3.00   1
...
22      22     35.8001131  126.8808124  0.0   0.0   0.0   -1.00  0
23      23     35.8001040  126.8808124  0.0   0.0   0.0   -1.00  0
```

| Column | Meaning |
|--------|---------|
| `lineNo` | Line number in the file (starts at 1, increments by 1) |
| `index` | Waypoint index (usually the same as lineNo) |
| `lat` | Latitude (decimal degrees, WGS84) |
| `lon` | Longitude (decimal degrees, WGS84) |
| `res1`, `res2`, `res3` | Reserved fields — always `0.0` |
| `speed` | Target speed (km/h). **Negative = reverse** |
| `implementFlag` | `1` = implement down (plowing on), `0` = implement up (turn/idle) |

### 4.2 Speed Rules

- `0 < speed ≤ 7.0` — forward; values below `2.05 km/h` load with a sustainable-speed warning
- `speed = 0` — an explicit stop waypoint
- `-7.0 ≤ speed < 0` — reverse (incurs the competition penalty below); magnitudes below `2.05 km/h` warn
- `speed < 0` — reverse (incurs competition penalty: plowed state reset at that location)
- When the speed sign changes between adjacent waypoints, this is a **gear change point** — waypoints must be arranged so that the heading (direction of travel) reverses

### 4.3 implementFlag

- While plowing on a lane: `1`
- While turning on the headland (field edge): `0`
- **All reverse segments must be `0`** — do not plow while reversing.

### 4.4 Automatic Validation

RDDF is automatically validated as soon as it is received from the cloud. Format errors and safety-ceiling violations are **rejected and not saved**; physical tracking limitations load unchanged with warnings. The validation logic is defined in `RddfValidator.cpp`.

| Rule | Description | On violation |
|------|-------------|--------------|
| **(1) Empty file** | Zero waypoints | Reject |
| **(2) Speed envelope** | `\|speed\| ≤ 7.0 km/h`; a nonzero magnitude below `2.05 km/h` may be unsustainable | Reject above the ceiling; load low-speed input unchanged and warn |
| **(3) Waypoint spacing** | Consecutive points satisfy the documented minimum/maximum spacing | Reject with the offending waypoint pair |
| **(4) Physical curvature** | Compare local geometry with the measured steering limit | Load unchanged and warn about the tracking limit |

#### Behavior on rejection

- An **"RDDF Validation Error" dialog** is immediately shown in the simulator UI, displaying the rejection reason verbatim.
- Rejected RDDFs do not enter the pending list and are automatically deleted — **fix and re-upload**.
- If no dialog appears and the file enters the pending list normally, you can consider validation passed.

#### Start pose is a session safety gate

Static file validation does not invent a StartPoint or compare waypoint 0 with
it. The loader has no authoritative live map spawn. Before motion authority is
granted, the simulator/session must instead compare its actual fresh vehicle
pose with waypoint 0. Keep the map spawn and the first waypoint aligned.

> Note: The loader preserves every submitted speed exactly and rejects only
> magnitudes above `7.0 km/h`. Nonzero values below `2.05 km/h` are loaded; if
> the runtime controller applies its creep floor, that is reported in telemetry.

### 4.5 Map Information (Field Maps)

The competition uses **3 public maps (M1, M2, M3)**. Each map's polygon (field boundary) is defined in WGS84 `lat, lon` coordinates, and participants' RDDFs should cover as much of the polygon interior as possible.

| Map | Name | Origin (lat, lon) | Area | Vertices |
|-----|------|-------------------|------|----------|
| **M1** | Open Fields | 35.8000, 126.8800 | 1,500 m² | 4 |
| **M2** | Sloped Acres | 34.6800, 126.9100 | 1,948 m² | 37 |
| **M3** | Patchwork Plots | 35.4200, 127.3900 | 1,355 m² | 54 |

> Origin is the GPS reference point corresponding to local (0, 0) of the polygon (for reference only); area is the m² value calculated using the shoelace formula.

#### M1 · Open Fields (1,500 m², 4 vertices)

![M1 Open Fields](docs/maps/map1_polygon.png)

The simplest rectangular field. Ideal for quickly validating a boustrophedon algorithm.

```
polygon = [
  (35.8002897, 126.8806012), (35.8004622, 126.8802035),
  (35.8001628, 126.8800000), (35.8000000, 126.8803803),
]
```

#### M2 · Sloped Acres (1,948 m², 37 vertices)

![M2 Sloped Acres](docs/maps/map2_polygon.png)

A complex polygon simulating a sloped field. The headland area is long, making U-turn handling tricky.

```
polygon = [
  (34.6805868, 126.9102540), (34.6804636, 126.9100401),
  (34.6804514, 126.9100264), (34.6804341, 126.9100117),
  (34.6804101, 126.9100000), (34.6803773, 126.9100074),
  (34.6803447, 126.9100296), (34.6803099, 126.9100506),
  (34.6802487, 126.9100895), (34.6801798, 126.9101240),
  (34.6800194, 126.9101844), (34.6800025, 126.9101929),
  (34.6800000, 126.9102047), (34.6800011, 126.9102220),
  (34.6800076, 126.9102429), (34.6800240, 126.9102651),
  (34.6800455, 126.9102861), (34.6800750, 126.9103133),
  (34.6801236, 126.9103631), (34.6801328, 126.9103724),
  (34.6801435, 126.9103860), (34.6801583, 126.9104057),
  (34.6801838, 126.9104396), (34.6802037, 126.9104747),
  (34.6802139, 126.9104994), (34.6802247, 126.9105198),
  (34.6802354, 126.9105480), (34.6802425, 126.9105771),
  (34.6802543, 126.9106086), (34.6802578, 126.9106314),
  (34.6802619, 126.9106436), (34.6802696, 126.9106499),
  (34.6802767, 126.9106461), (34.6802850, 126.9106363),
  (34.6803441, 126.9105654), (34.6803610, 126.9105499),
  (34.6803819, 126.9105290),
]
```

#### M3 · Patchwork Plots (1,355 m², 54 vertices)

![M3 Patchwork Plots](docs/maps/map3_polygon.png)

An irregular field with a mix of concave and convex sections. A simple boustrophedon leaves significant unplowed areas, so polygon clipping and per-sub-region processing are required.

```
polygon = [
  (35.4205363, 127.3902241), (35.4205148, 127.3901085),
  (35.4205020, 127.3900000), (35.4202816, 127.3900305),
  (35.4202305, 127.3900267), (35.4201433, 127.3900066),
  (35.4201374, 127.3900667), (35.4201845, 127.3900979),
  (35.4202004, 127.3901073), (35.4202145, 127.3901207),
  (35.4202350, 127.3901547), (35.4202510, 127.3901941),
  (35.4202633, 127.3902458), (35.4202652, 127.3902776),
  (35.4202611, 127.3903620), (35.4201999, 127.3903605),
  (35.4201579, 127.3903998), (35.4201428, 127.3904155),
  (35.4201145, 127.3904243), (35.4200712, 127.3904327),
  (35.4200707, 127.3904499), (35.4200667, 127.3904538),
  (35.4200640, 127.3904632), (35.4200598, 127.3904727),
  (35.4200525, 127.3904733), (35.4200484, 127.3904905),
  (35.4200105, 127.3904933), (35.4200014, 127.3905578),
  (35.4200000, 127.3905707), (35.4200178, 127.3905879),
  (35.4200319, 127.3906068), (35.4200430, 127.3906307),
  (35.4200766, 127.3906202), (35.4201037, 127.3906140),
  (35.4201269, 127.3906073), (35.4201511, 127.3905483),
  (35.4201598, 127.3905333), (35.4201770, 127.3905294),
  (35.4202127, 127.3905333), (35.4202578, 127.3905433),
  (35.4202830, 127.3904800), (35.4202971, 127.3904454),
  (35.4203168, 127.3904127), (35.4203341, 127.3903826),
  (35.4203501, 127.3903605), (35.4203743, 127.3903364),
  (35.4203994, 127.3903132), (35.4204189, 127.3902909),
  (35.4204427, 127.3902630), (35.4204637, 127.3902486),
  (35.4204810, 127.3902407), (35.4205012, 127.3902347),
  (35.4205213, 127.3902308), (35.4205394, 127.3902358),
]
```

---

## 5. RDDF Authoring Workflow

### 5.1 Directory Structure

```
master_of_plow/rddf/
├── 1.rddf                  # Route for field 1
├── 2.rddf                  # Route for field 2
├── 3.rddf                  # Route for field 3
├── upload_rddf.sh          # Cloud upload script
└── how-to-upload-rddf.md   # Upload command reference
```

### 5.2 Writing an RDDF by Hand

For the simplest approach, open a text editor and write tab-delimited data directly.

```
1	1	35.8000317	126.8807033	0.0	0.0	0.0	3.00	1
2	2	35.8000317	126.8807141	0.0	0.0	0.0	3.00	1
3	3	35.8000317	126.8807249	0.0	0.0	0.0	3.00	1
```

> **Warning**: The delimiter must be a **tab (`\t`)** character. Mixing in spaces will cause the parser to reject the file.

### 5.3 Generating an RDDF with Code

Writing dozens to hundreds of waypoints by hand is impractical. The typical approach is to write a script that takes the **field geometry + algorithm parameters** (lane spacing, turn radius, etc.) as input and generates the RDDF. Any language works — this guide provides Python examples.

#### Example: Boustrophedon (back-and-forth) + semicircular U-turn

Divide the field into parallel lanes spaced by SWATH (implement width), traverse alternating even/odd lanes, and at each end enter the next lane via a **forward semicircular U-turn**. Because no reversing is used, the **unplowed-reset penalty is avoided**.

```
↑ Lane 0       U-turn ↓ Lane 1       U-turn ↑ Lane 2  ...
│             ╲          │            ╱          │
│              ╲         │           ╱           │
│       semicircle U-turn │   semicircle U-turn   │
```

#### Minimal working example (`gen_rddf.py`)

```python
"""
gen_rddf.py — Example boustrophedon pattern RDDF generator
Participants should use this script as a starting point and evolve it with their own algorithm.
"""

import math

# --- Field origin (bottom-left) and lat/lon conversion ---
LAT0, LON0 = 35.8001, 126.8807
M_PER_DEG_LAT = 110540.0
M_PER_DEG_LON = 111320.0 * math.cos(math.radians(LAT0))

def to_latlon(x_m, y_m):
    return (LAT0 + y_m / M_PER_DEG_LAT,
            LON0 + x_m / M_PER_DEG_LON)

# --- Parameters ---
FIELD_W = 30.0     # Field width E-W (m)
FIELD_H = 50.0     # Field height N-S (m)
SWATH   = 4.0      # Lane spacing = implement width (m)
STEP    = 1.0      # Waypoint interval (m)
V_LANE  = 3.0      # Lane speed (km/h)
V_TURN  = 1.5      # Turn speed (km/h)
N_ARC   = 16       # Number of segments for U-turn semicircle

waypoints = []  # (x, y, speed, implementFlag)
r = SWATH / 2.0
n_lanes = int((FIELD_W - SWATH) // SWATH) + 1

for k in range(n_lanes):
    x = SWATH / 2.0 + k * SWATH
    going_up = (k % 2 == 0)

    # (1) Straight lane — implement ON
    y0, y1 = (r, FIELD_H - r) if going_up else (FIELD_H - r, r)
    n_steps = max(2, int(abs(y1 - y0) / STEP) + 1)
    for i in range(n_steps):
        t = i / (n_steps - 1)
        waypoints.append((x, y0 + (y1 - y0) * t, V_LANE, 1))

    # (2) Semicircular U-turn to next lane — implement OFF
    if k == n_lanes - 1:
        break
    cx, cy = x + r, y1
    th0 = math.pi
    th1 = 0.0 if going_up else 2.0 * math.pi   # upper ↗ / lower ↘ semicircle
    for j in range(1, N_ARC + 1):
        theta = th0 + (th1 - th0) * j / N_ARC
        waypoints.append((cx + r * math.cos(theta),
                          cy + r * math.sin(theta),
                          V_TURN, 0))

# --- Write RDDF file (tab-delimited, 9 columns) ---
with open("alpha.rddf", "w") as f:
    for idx, (x, y, v, flag) in enumerate(waypoints, 1):
        lat, lon = to_latlon(x, y)
        f.write(f"{idx}\t{idx}\t{lat:.7f}\t{lon:.7f}\t0.0\t0.0\t0.0\t{v:.2f}\t{flag}\n")

print(f"{len(waypoints)} waypoints → alpha.rddf")
```

#### Areas to evolve with your own algorithm

The script above handles only a **rectangular field with uniform lanes**. To compete on score you will need to replace the following with your own algorithm:

- **Non-rectangular field boundary handling** — clip lane lengths to fit non-rectangular field shapes
- **Separate headland processing** — if the U-turn semicircle exits the field boundary, the **10× timer acceleration** penalty applies
- **Lane spacing / turn radius optimization** — the tractor's minimum turning radius is approximately 3.28 m; SWATH must be at least that large for smooth U-turns
- **Start/end point** — design the approach path from point A to the first lane
- **Minimizing unplowed areas** — separately handle small unplowed regions near corners and edges

### 5.4 Validation Checklist

Before uploading an RDDF, verify the following.

- [ ] Every line has exactly 9 columns with **tab delimiters**
- [ ] `lineNo` starts at 1 and increments without gaps
- [ ] Map/session start pose is aligned with waypoint 0 before motion authority (§4.4)
- [ ] Every `speed` has absolute value **at most 7.0 km/h** (below `2.05 km/h` warns)
- [ ] Waypoint spacing is appropriate — **two points (start + end) are sufficient for straight segments**; **curve segments should be densely spaced**
- [ ] No segments with `speed < 0` over plowed area (avoid reverse penalty)
- [ ] When transitioning from reverse to forward, **heading reverses** (in-place rotation is not possible)
- [ ] All reverse segments have `implementFlag` = `0`
- [ ] U-turn path does not stray significantly outside the field boundary (10× acceleration penalty)

---

## 6. Applying the RDDF to the App

### 6.1 Cloud Upload

Competition evaluation is based on the **RDDF uploaded to the cloud**. Use the `FEU_ID` and `FEATURE_ID` issued by the organizers to upload.

```bash
cd master_of_plow/rddf

# Grant execute permission (once only)
chmod +x ./upload_rddf.sh

# Upload
./upload_rddf.sh \
  -f ./1.rddf \
  --feu-id <YOUR_FEU_ID> \
  --feature-id <YOUR_FEATURE_ID>
```

Example of successful output:

```
[1/2] Requesting CP token from https://... ...
  provider_id (sub) = ...
[2/2] Uploading './1.rddf'
       -> .../api/v1/features/<FEATURE_ID>/feu/<FEU_ID>/files
  Status: 200
Done.
```

#### Parameter format

```
FEU_ID example      : abcdefgh-abcd-abcd-1234-abcd1234abcd
FEATURE_ID example  : 10234dev
```

#### Common errors

| Symptom | Cause / Fix |
|---------|-------------|
| `jq is required` | `sudo apt install jq` or `brew install jq` |
| `Status: 401` | FEU_ID / FEATURE_ID typo or expired — contact organizers |
| `Status: 4xx` (file rejected) | RDDF format error — re-check §5.4 checklist |
| `Status: 5xx` | Temporary server issue — retry after a moment; if it persists contact organizers |

### 6.2 Post-upload Verification

After uploading, the app receives the RDDF via `CloudDownloadListener` and parses it with `RddfParser`. Verify in the simulator that the tractor follows the intended route.

---

## 7. Competition Rules Summary

### 7.1 Mission Conditions

| Item | Value |
|------|-------|
| Start point | **First waypoint location in the RDDF** (tractor aligns to that location on map switch) |
| Target fields | **M1 · Open Fields**, **M2 · Sloped Acres**, **M3 · Patchwork Plots** — 3 public maps (see §4.5) |
| Target achievement | **No hard cutoff** — coverage rate is reflected proportionally in the score |
| Maximum speed | **7 km/h** |
| Overlap | Allowed (no deduction) |

### 7.2 Penalties

There are three penalties that affect competition scoring.

#### (1) Reverse in plowed area — unplowed reset

Plowing at the reversed location is invalidated and reset to unplowed.
→ Design efficient forward-only turn patterns (e.g., U-turn, Omega-turn).

#### (2) Leaving the field — timer 10× acceleration

While the tractor is **outside the field (Alpha boundary)**, the simulation timer runs **10× faster**.
From the participant's perspective, every second spent outside the field adds 10 seconds to the recorded time.

→ When designing headland turns that cross the field boundary, there is a large time penalty. Turn patterns that complete within the field boundary are advantageous.

#### (3) Cumulative path error at finish — final time penalty

A penalty in seconds proportional to the **Cross-Track Error (CTE) RMS** accumulated during the run is added to the final time.

Code reference (`MainControllerImpl.cpp`, `track_complete` handler):

```cpp
RunSummary& rs = getRunSummaryMut();
rs.elapsedS   = elapsedSAtComplete_;
rs.penaltyS   = rs.deviationM * 1000.0;   // RMS CTE(m) × 1000
rs.finalTimeS = rs.elapsedS + rs.penaltyS;
```

- `deviationM` — Root-mean-square (RMS) of CTE accumulated each tick, in meters
- `penaltyS` — `deviationM × 1000` seconds
- `finalTimeS` — `elapsedS + penaltyS`, **the final leaderboard time**

→ An average CTE of **0.1 m** adds **100 seconds**; **0.5 m** adds **500 seconds**.
→ Sparse curve waypoints or abrupt heading changes cause Pure Pursuit tracking error to spike, dramatically inflating the penalty.

##### Tips for reducing average error

- **Straight segments** only need **2 points** (start and end) — the tracker follows between those two points, so intermediate points are unnecessary.
- **Curve segments** should be densely spaced — the higher the curvature, the finer the subdivision.
- Keep heading changes between adjacent waypoints small (avoid large single-step angle changes, e.g., keep below 15°)
- It is recommended to add a heading-jump validation step in your own generator

### 7.3 Winning Criteria

- Both **final time (`finalTimeS = elapsedS + penaltyS`)** and **coverage rate** contribute to the score.
- There is **no minimum coverage rate threshold** (e.g., 95%) that must be met.
- However, a lower coverage rate is disadvantageous in scoring, so you must **jointly optimize** time reduction, coverage rate, and average error minimization.
- `finalTimeS` includes both the 10× acceleration due to leaving the field and the cumulative error addition.

### 7.4 Evaluation Method

- All evaluation is based on **data recorded in the server leaderboard**.
- When a simulation is run with a participant's uploaded RDDF, the results (`finalTimeS`, coverage rate, etc.) are automatically aggregated onto the leaderboard.
- Locally measured time/coverage rate is for reference only; **the official record is the leaderboard value**.
- There is no separate result submission process — your team's best score recorded on the leaderboard at the deadline is automatically evaluated.

### 7.5 Run / Connection / Team Name Rules (strictly enforced)

> Violating the rules below may result in that run being invalidated, and repeated violations may constitute grounds for disqualification.

- **You cannot perform two or more runs simultaneously within a single app (simulator instance).**
  - The previous run must be completely finished before starting the next run.
  - Do not upload a new RDDF or restart the simulator before the previous run has ended.
- **Parallel runs across multiple apps (instances) are allowed, but simultaneous runs with the "same team name + same map" combination are prohibited.**
  - Running different maps on separate apps simultaneously is OK (e.g., App A on Map 1, App B on Map 2).
  - **Running the same map with the same team name on two instances simultaneously is prohibited** — leaderboard aggregation breaks and the records will be invalidated.
- **Multiple browser connections are prohibited.**
  - Do not connect to the same app instance from **multiple browser tabs or windows simultaneously**.
  - Multiple connections can cause leaderboard aggregation errors, coordinate/timer mismatches, and result loss.
  - If you want to share the screen, use screen mirroring / screen sharing (no separate browser session).
- **Use the exact same team name for all map runs.**
  - Per-map scores are **summed using the team name as the key**, so a single-character difference will be treated as a separate team with separate scores.
  - Example: `Team-A`, `team-A`, and `Team A` (with a space) are all treated as different teams.
  - Finalize your team name before the first run and enter **exactly the same string** for all maps and all runs.

---

## 8. Recommended Workflow

```
[1] Clone repo & IDE Import & confirm build passes
        ↓
[2] Upload rddf/1.rddf sample and observe tractor behavior in the simulator
        ↓
[3] Algorithm design — decide lane width, headland handling, turn pattern
        ↓
[4] Write RDDF generator (e.g., gen_rddf.py) — implement your coverage algorithm
        ↓
[5] Validate generated RDDF against §5.4 checklist
        ↓
   ┌─→ [6] Upload to cloud via upload_rddf.sh
   │        ↓
   │   [7] Run driving session in simulator → measure finalTime / coverage rate
   │        ↓
   │   [8] Confirm leaderboard update — your team's best record updates automatically
   │        ↓
   └── [9] Tune algorithm / parameters → return to 4 or 6

   * Repeat the [6]–[9] loop until the deadline, updating the leaderboard each attempt.
   * There is no separate "final submission" step — your team's best record
     (summed across maps) on the leaderboard at the deadline is automatically evaluated.
   * If you get stuck between [2] and [3], read §10. Knowing how the tractor
     reacts to a path makes the algorithm design in [3] much easier.
```

---

## 9. FAQ

Questions are grouped by topic. `(§N)` at the end of each answer refers to the corresponding section in the body of this guide.

- [9.1 Submission & Evaluation](#91-submission--evaluation)
- [9.2 Validation & Errors](#92-validation--errors)
- [9.3 Path Design](#93-path-design)
- [9.4 Operational Rules](#94-operational-rules)
- [9.5 Environment & Tools](#95-environment--tools)

---

### 9.1 Submission & Evaluation

> **Q1.** Do you submit a single RDDF file, or one per map?

**Upload one per map.** The competition uses three public maps M1, M2, and M3; upload an RDDF for each map using the corresponding mapId. Per-map scores are summed using the **team name** as the key. (§4.5, §7.5)

---

> **Q2.** Do you need 100% coverage to win?

**No — there is no minimum coverage rate threshold.** However, coverage rate is reflected proportionally in the score, so at equal time a team with higher coverage has an advantage. The strategic core is deciding how to **balance time vs. coverage rate**.

---

> **Q3.** Is there a separate "final submission" step?

**No.** Your team's **best record** on the leaderboard at the deadline is automatically evaluated. Keep uploading and running until the deadline to keep improving your record. (§7.4)

---

### 9.2 Validation & Errors

> **Q4.** I uploaded an RDDF but nothing happens in the simulator.

Check both possibilities below.

| Cause | Check / Action |
|-------|----------------|
| **Validation failure** | Check whether an "RDDF Validation Error" dialog appeared → fix the RDDF according to the reported reason and re-upload (§4.4) |
| **Previous track still active** | Confirm that the new RDDF has progressed past the pending list and that the **apply** step was not skipped |

---

> **Q5.** Why does the file validator not check the map StartPoint?

The application loader does not receive an authoritative live map spawn, so it
must not validate against an invented position. Start-pose alignment is checked
by the simulator/session safety gate before motion authority. (§4.4)

---

> **Q6.** Can I slightly exceed 7 km/h?

**No.** A value above 7.0 km/h is rejected explicitly; it is never silently
clamped. Nonzero values below 2.05 km/h are accepted with a warning because the
machine may apply its runtime creep floor. (§4.2, §4.4)

---

### 9.3 Path Design

> **Q7.** Does a route with lots of overlap help?

**It hurts.** Overlap itself is not penalized, but it increases total time, which puts you at a disadvantage in time competition. Design as efficient a coverage pattern as possible.

---

> **Q8.** Is reverse strictly forbidden during turns?

**Only reverse within the plowed area (implement down) triggers the unplowed-reset penalty.**

- OK: Reverse in the headland (`implementFlag=0`) area — no unplowed penalty
- NG: Reverse inside the plowed implement-down zone — unplowed-reset penalty applies
- Note: Reverse in any zone still causes time loss and tricky heading-reversal handling, making it generally inefficient

---

> **Q9.** It is convenient to swing the U-turn far outside the field — how bad is the penalty?

**The timer runs 10× faster while outside the field boundary.** For example, turning outside the headland for 3 seconds adds 30 seconds. Complete U-turns inside the field as much as possible. (§7.2)

---

> **Q10.** How significant is the average error penalty?

The formula is `penaltyS = deviationM × 1000` seconds. Concrete examples:

| Average CTE | Penalty |
|------------:|--------:|
| 0.1 m | 100 s |
| 0.5 m | 500 s |
| 1.0 m | 1000 s |

→ Reduce it by increasing **waypoint density on curves** and **minimizing heading jumps**. (§7.2)

---

### 9.4 Operational Rules

> **Q11.** Can I run two maps simultaneously on one device?

**Yes — but the same team name + same map combination is prohibited.**

- OK: App A runs M1 while App B runs M2 simultaneously
- NG: Two instances both run M1 under the same team name simultaneously
- NG: Multiple browser tabs connected to the same app instance

(§7.5)

---

> **Q12.** Can I change the team name mid-competition?

**It is strongly discouraged.** The team name is the key for summing per-map scores, so **even a single character difference** is treated as a separate team with separate scores. Finalize your team name before the first run and use exactly the same string across all maps and all runs. (§7.5)

---

### 9.5 Environment & Tools

> **Q13.** Can I freely build my own RDDF generator tool?

**Yes — no restrictions on language or tool.** Python, MATLAB, a custom algorithm — anything goes. The only requirement is that the final output is an `.rddf` file that satisfies the format described in this document. (§4.1)

---

> **Q14.** Can I modify the code (C++ app, path tracker, etc.)?

**Yes.** Participants may modify autonomy logic and parameters. Official
evaluation uses the participant's approved FIF/application build together with
the submitted RDDF, subject to the organizer's current rules. Package and
install that build only through the official FIF workflow (§3.2); do not assume
that an unapproved local or loose-binary build is eligible.

That said, **reading** the tracking code is encouraged. Knowing which paths the tractor follows well — and which ones make its error grow — directly improves the quality of your RDDF. §10 explains the structure and how to read it.

---

## 10. Advanced: Understanding the Path-Tracking Code (optional)

> The tracker is part of the participant application. Changes affect an official
> evaluation only when they are included in the participant's approved FIF build
> under the organizer's rules (§9.5 Q14). Reading it also helps explain which
> RDDF geometry the vehicle can follow reliably.

### 10.1 Where the Code Lives

Path tracking is collected in one place: `master_of_plow_app/src-gen/AppMain/tracking/`.

```
AppMain/tracking/
├── TrackerTypes.hpp      Frame/sign conventions + the exchanged data types  ← start here
├── IPathTracker.hpp      The contract a tracking algorithm must satisfy
├── PathTrackerBase.hpp    Base class that pre-implements the boilerplate
├── SpeedController.hpp    Gear/throttle/brake (shared by all algorithms)
├── TrackerFactory.*       Name → implementation
├── TrackingLoop.*         Control loop (GPS conversion, command publishing, telemetry)
└── impl/
    ├── PurePursuitTracker.*  Default tracker (the one that actually drives in the contest)
    └── StanleyTracker.*      A second worked example
```

Every comment is written in **both Korean and English**. Start at the top of
`TrackerTypes.hpp` — nothing else reads correctly until you know the conventions.

### 10.2 Frame and Signs — the First Thing to Learn

| Item | Convention |
|------|------------|
| Position | ENU metres `(x, z)`, **+x = East, +z = South**. Origin = waypoint 0 of the RDDF |
| Heading | `atan2(z, x)` radians. 0 = facing East, +π/2 = facing South |
| Steering | Positive = **left turn**. The frame is left-handed, so a left turn *decreases* heading |
| Speed | Waypoint speed is km/h and **signed** (negative = reverse); measured speed is m/s and **unsigned** |

The compass bearing from GPS (0 = North, clockwise) is converted into this frame
by the control loop.

### 10.3 One Control Tick

`TrackingLoop` repeats this every 100 ms (shorter in proportion to the simulation
speed multiplier):

```
isReady()  →  sense()  →  tracker->update()  →  act()  →  report()
  ready?     GPS→ENU      tracking algorithm    publish    telemetry / summary
```

`sense()` and `act()` are the agreement with the simulator and are identical for
every algorithm. The only part that changes is `update()`.

### 10.4 How the Default Tracker Chooses a Steering Angle

Despite the name, it is a **weighted sum of five terms**, and the pure Pure
Pursuit geometry contributes only 45%.

| Term | Role |
|------|------|
| Curvature feedforward | Steering the path's own curvature demands (independent of error) |
| Heading error (HDE) | Aligns the vehicle with the path tangent |
| Lateral deviation (LTD) | Pulls the vehicle back onto the path. On lanes it is measured at the **predicted position 1.5 s ahead**; in turns, at the current position |
| Pure Pursuit | Arc toward the lookahead target — responsible for smoothness (weight 0.45) |
| Yaw-rate damping | Suppresses oscillation when the terms above overreact |

### 10.5 ★ Properties That Directly Affect RDDF Design

This is the most practical part of this section.

| Tracker property | Write your RDDF like this |
|------------------|---------------------------|
| Lookahead distance is **5.5–8 m** (scales with speed) | Curves with a tighter radius get cut on the inside. Ease sharp turns out, or space waypoints more densely |
| Curvature estimated as a weighted average over **5 consecutive points** around the closest one | Sparse waypoints degrade the curvature estimate, leaving arcs under-steered (§4.4 rule 4) |
| The implement is **3 m behind** the vehicle | `implementFlag` transitions fire when the implement passes, so allow ~3 m of margin at lane starts and ends |
| Tracking does not start until the vehicle is **within 3 m of waypoint 0** | Match your first waypoint to the map's point A (§4.4) |
| A forward↔reverse change **brakes to a full stop** | Every direction change costs a full stop plus re-acceleration. Reversing also voids plowed ground (§7.2), so forward-based turns win |
| Completion = **within 1 m** of the last waypoint + a progress gate | Make the last waypoint an unambiguous end point |
| Progress never regresses even if the path crosses itself | Skip-lane patterns are safe |

### 10.6 Observing Behaviour Locally

Running the app locally produces two diagnostic outputs.

- **`/tmp/pp_trace.csv`** — one row per control tick: vehicle pose, steering,
  lateral deviation (LTD), and **each of the five steering terms separately**, so it is
  immediately visible which one is running away. After a run, overlay the path
  and the actual trajectory in Python or a spreadsheet.
- **`PP_LOG ...`** (stderr) — a one-line summary per tick, for watching live in a
  terminal.

If `cte` keeps accumulating to one side through a curve, that curve's curvature
exceeds the tractor's physical limit. Before touching any gain, try **easing the
curvature in the RDDF** first.

### 10.7 Trying a Different Algorithm

A Stanley example implementation ships alongside the default tracker. One
environment variable switches between them:

```bash
PATH_TRACKER=stanley   # default is pure_pursuit
```

Adding your own algorithm takes three steps:

1. Create a class in `tracking/impl/` deriving from `PathTrackerBase` and
   implement `update()` and `isTrackComplete()` (copying `StanleyTracker` is the
   easiest start)
2. Add one line to the registry in `TrackerFactory.cpp`
3. Run with `PATH_TRACKER=<its name>`

`master_of_plow_app/tests/PathTrackerContractTest.cpp` exercises **every
registered tracker** automatically, so finishing step 2 puts your implementation
under test as well. Where googletest is installed, a `master_of_plow_app_tests`
target is built alongside the app.

Changes must pass the local suite and be included in the approved FIF before
they can be evaluated; loose executable replacement is never an official run.

---

## 11. Getting Help

- **Urgent inquiries**: Speak directly to the organizers at the on-site booth

Good luck.
