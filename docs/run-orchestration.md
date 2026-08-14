# Backend-owned run orchestration (v1.1.35)

The dashboard uses the feature's existing `/socket` WebSocket for setup,
control, telemetry, and recording download. It does not call simulator REST
endpoints from the browser and does not use the legacy game `SessionClient`.

The backend discovers the simulator API through Linux's default gateway.
While stopped, each typed `set_sim_setup` change immediately validates, applies,
and confirms map/tractor/implement selection. Start performs no selection
writes: it requires desired setup, confirmed setup, and the fresh live simulator
selection to match exactly, freezes the confirmed tractor geometry for the run,
reports the first failed preflight predicate rather than collapsing live,
freshness, stopped state, IDs, catalog geometry, and controller readiness into
one error, and rechecks a fresh exact physical-stop sample after recorder
confirmation immediately before engine-on,
requires recorder reset to return HTTP 200 (all other statuses, including 409,
fail closed), starts the recorder and engine, confirms fresh `reactRunning`
state and advancing simulation time, and only then enters `Running`. Failure at
any stage immediately returns the controller to safe
`Idle` and switches the engine off. Recorder cleanup and archival then follow
the generation ownership rules below.

The selected tractor must supply root `wheelbaseM` and `maxForwardGear` plus
nested `steering.maxRateDegPerSec`, `steering.tauSec`, `steering.ratio`, and
`steering.maxFrontWheelAngleDeg`. Degrees are converted to radians once at this
catalog boundary. Missing or invalid selected-tractor geometry is rejected;
defaults exist only for local unit tests and non-orchestrated development. The
validated copy installed for an orchestrated run is immutable.

Finish is the only exposed stop action. Manual Finish and the first RDDF
completion both immediately release authority and enter safe `Idle`, then run
the same asynchronous finalization. Its per-run state is `idle`, `in_progress`,
`completed`, or `retryable_error`: only a successful archive is latched. A
physical-stop, recorder, CSV fetch, or disk failure leaves the same run
generation retryable; the existing Finish action resumes finalization without
repeating a controller run transition or creating a second archive. Concurrent
automatic/manual requests cannot create a second archive. Finalization commands
engine off and then keeps the recorder running
while the vehicle coasts. It requires three consecutive Issue #9 selection
snapshots, each live, no older than 1 second, matching the immutable run setup,
and reporting both `stopped=true` and `reactRunning=false`. A snapshot with
`reactRunning=true` causes another idempotent engine-off request. The selection
contract's `stopped` field is the authoritative physical-stop predicate; the
app does not invent a second speed threshold or combine the divergent status
endpoint with it. Only after stable physical stop does finalization issue the
idempotent recorder stop, confirm through live/fresh `GET /api/simulator/rec` recorder
headers, exact `text/csv` fetch with freshness/header/size checks, and atomic
persistence under the feature's persistent disk. CSV bytes are never rewritten.
Physical-stop timeout leaves the controller in safe `Idle`, preserves the same
generation and recorder ownership, and publishes no finalization event; Finish
may retry it. The controller publishes zero/default commands and releases
authority before engine-off or selection polling. The generated SDK setters do
not provide an actuator/CAN acknowledgement, so this is documented as commanded
safe output rather than fabricated physical acknowledgement.
The team name affects only the sanitized server filename; the original team
text and SHA-256 are stored in a JSON sidecar. The orchestration snapshot retains
the recording ID and digest, so reconnecting dashboards can download the exact
CSV in checksummed WebSocket chunks.

CSV and sidecar are one failure-atomic archive transaction keyed by a stable run
generation. Both exclusive temporary files are synced before rename; the
sidecar is the commit marker. Failures after either rename or directory fsync
remove all incomplete visible files, and a retry reuses the same ID rather than
creating a duplicate.

Feature shutdown first rejects new orchestration admission, signals active
polling to cancel, drains queued work, and joins the managed worker before API,
store, controller-adapter, or orchestrator members can be destroyed.

The dashboard deliberately reuses the established ADS `Button`, `Paragraph`,
`Popup`, dark telemetry palette, status-chip, and persistent status-bar visual
language rather than introducing a second presentation system. Its run fields
use an adaptive grid; the two actions stack at 320 px and share a row at 768 px
and 1280 px, which is verified in a real Chromium viewport test.

The UI enables selection only while the single app WebSocket is connected and
the live simulator snapshot is fresh and stopped. It presents exactly two run
actions, **Start** and **Finish**; pause/resume is deliberately not part of the
v1.1.35 UI contract. Team-name editing is app-local metadata: each input and
IME composition update is retained in local storage and never sends a simulator
setup intent. Start carries the latest trimmed team name.

Recorder state is split into two generation-bound obligations. A recorder Start
creates a cleanup obligation immediately before the recorder Start POST. Every
POST is ambiguous: the obligation remains whether the response is HTTP 200,
idempotent 409, an explicit rejection, or a timeout, because the server may
apply it late. Finish after that point sends one Stop and performs bounded
fresh/live stopped reconciliation. If any poll observes `recording=true`, Stop
is reissued idempotently and polling continues with bounded backoff. Completion
requires three consecutive fresh stopped samples; the loop is capped at 30
attempts. Deadline failure remains retryable for the same generation. Cleanup
still does not fetch or archive the buffer. A
fresh/live `recording=true` confirmation in the normal Start poll promotes it
to archive ownership; only then may Finish fetch and persist CSV.
An HTTP 404 `no recording yet` immediately after recorder Start is treated only
as a pending sample within the same bounded confirmation loop. It is never
accepted as recording state; persistent 404 times out and invokes stop-only
cleanup, while other API errors remain actionable failures.

Finish before recorder Start enters safe Idle and turns the engine off without
recorder stop, fetch, or archive. A confirmation error or timeout after Start
still performs the stop-and-confirm cleanup, while leaving ambiguous data
unarchived. Once archive ownership is confirmed, cancellation uses the same
exact-once finalization and archive transaction as a normal Finish.

Shutdown does not bypass generation-owner cleanup. The owner worker performs
bounded safe Idle, engine-off, and the same recorder reconciliation before the
controller destructor joins it. Pre-Start shutdown does not touch the recorder;
post-Start/pre-confirmation shutdown stops without fetching; confirmed ownership
uses the normal exact-once stop/fetch/archive policy.

Simulator setup changes use typed `set_sim_setup` intents over the same app
WebSocket. The backend records desired state immediately, serializes REST work,
and commits confirmed state only for the latest intent generation. While idle
and freshly stopped, changed fields are applied map, tractor, then implement,
with exact fresh selection confirmation after each request. Team-only changes
do not call the simulator. Setup never resets/starts/stops the recorder, starts
the engine, or changes autonomy state. Start is rejected while setup is pending
and never reapplies selections: its fresh gate requires desired, confirmed, and
live simulator selection to match exactly.

The app-facing implement catalog always contains exactly one leading
`none`/`None` item with width 0, because the live catalog omits this selection
while `/selection` legitimately reports it. Selecting `none` sends the
simulator's exact `{"id":null}` contract; real implement IDs remain strings.

## Sealed leaderboard submission

The C++ feature owns safe recorder finalization and its exact plain-CSV archive,
but never connects to the external leaderboard. It publishes the stable run ID
and team through the existing app WebSocket. The browser fetches the sealed v2
envelope through the HTTPS CustomUI's same-origin nginx bridge,
`/ws/rest/api/simulator/rec.enc`, validates its basic v2 shape and strict
`application/octet-stream` success type, and stores
the exact envelope, team, run ID, and state in IndexedDB **before** any external
request. It then submits `{"teamName": ..., "envelope": ...}` over HTTPS.
`mapId` and `dryRun` are omitted, and the browser never decrypts, rewrites, or
signs the envelope.

Successful Finish leaves the backend in `postRun=awaiting_action` and blocks the
next Start until an explicit Reset succeeds. Finish and reconnect never fetch
`rec.enc` and never enqueue a leaderboard submission. The UI instead presents
two post-run actions. **Submit** first sends `begin_sealed_capture` for the exact
current completed recording ID. Only the backend grant
`postRun=capture_in_progress` permits the browser to fetch and durably journal
that envelope; `sealed_capture_durable` then moves the backend to
`capture_durable`, after which external submission retries are independent of
the simulator recorder. A capture failure releases the grant back to
`awaiting_action` so the operator can retry or reset.

**Reset** is backend authoritative and serialized against capture. It is
rejected during `capture_in_progress`. The backend first keeps the controller in
safe Idle and confirms three fresh exact-selection samples with
`stopped=true` and `reactRunning=false`, then calls simulator refresh and an
additional three-sample post-refresh readiness gate before an exact-HTTP-200
bodyless recorder reset. Transport failures and HTTP 503 are retried only within
one bounded monotonic deadline; other 4xx responses fail immediately with a
sanitized status diagnostic. It does not clear the completed-run gate until a
subsequent recorder GET returns authoritative 404/no-recording while selection
remains safely stopped. A 200 response containing old CSV is not reset proof.
Failures preserve the run ID and expose `reset_retryable`. Reset requires
explicit destructive confirmation when the current run has not reached a
terminal accepted leaderboard verdict. The backend keeps an atomic submitted
ledger for SCORED and INVALID results. The browser replays a durable terminal
acknowledgement after reconnect until the backend confirms
`postRun=submitted`, so a lost WebSocket acknowledgement is harmless. Before
simulator reset, the backend enters `reset_cleanup_preparing`; the browser
broadcasts cross-tab cancellation, aborts and drains active capture/submission
requests, and atomically tombstones every non-submitted IndexedDB row and
lease. Only `reset_cleanup_prepared` permits simulator refresh/reset. After
reset proof the backend purges plain CSV/sidecar pairs not present in the
submitted ledger, then waits in `reset_cleanup_pending`. Only
`reset_cleanup_durable` clears the
completed-run Start gate. Pending/retryable external submissions are therefore
discarded by Reset; submitted audit records and archives are preserved.

The backend assigns a monotonic `resetGeneration` when cleanup begins, and the
same generation is carried by every preparation, ready, durable, completion,
and orchestration-snapshot message. Reset drain requests, tab heartbeats, and
per-tab acknowledgements are durable in IndexedDB. Completion atomically
replaces the current drain with a durable generation-scoped marker and wakes
peer tabs through `BroadcastChannel`; the broadcast alone is never trusted to
reopen work. Each existing or late tab clears its drain block only after its own
authoritative snapshot reports `postRun=none` for that same generation. Failed
or timed-out reset remains blocked in `reset_retryable`; a later reset receives
a new generation and can complete without leaving long-lived tabs wedged.
The **모두 제거** action has no browser-only cleanup shortcut: it first
sends typed `prepare_purge_all`, receives a positive backend cleanup generation,
and follows the same full simulator refresh/recorder-reset, tab-drain, archive
purge, durable completion, and authoritative-snapshot handshake even when no
current recording ID exists.

The responsive **미제출 기록** dialog requests the authoritative backend list,
shows team/run/time/size/error and available CSV downloads, and presents the
union of backend archives and browser-only IndexedDB records, merged by run ID.
**모두 제거** uses the same
coordinated purge boundary; it never deletes submitted-ledger records.

Construction alone never scans or resumes retryable capture work. Authoritative
snapshots reconcile exact run ownership before any explicitly initiated work
can resume. The stable run-ID key deduplicates reconnects against the explicit
Submit grant.
Cross-tab processing uses a per-run Web Lock and re-reads the durable record
only after acquiring that lock, so a waiter cannot act on pre-lock state. Where
Web Locks are unavailable, an atomic IndexedDB read/write lease provides the
same single-owner rule. Every processing invocation has a unique lease token;
only that exact token may renew or release the lease. A heartbeat keeps a live
operation's lease ahead of the bounded 30-second fetch timeout, while an
expired lease from a crashed tab can be taken over. The deadline covers both
response headers and complete capped body consumption/JSON parsing; timeout or
lease loss aborts the request and cancels an open response reader.
Every lease-owned durable transition is an IndexedDB compare-token write. A
takeover between POST and response persistence therefore cannot be overwritten
or published by the former owner. Envelope-present submission work may resume
after authority reconciliation. `capture_pending` and `capture_error` never
perform another sealed GET unless an explicit Submit makes the backend grant
that exact run as `capture_in_progress`; a failed old capture therefore cannot
consume a future recorder buffer. Cross-tab purge also uses BroadcastChannel
notification, while conditional lease writes reject late responses.
`capture_pending`, `capture_error`, `pending`, and `retry_wait` are recoverable;
terminal states remain retained and are not replayed.

HTTP 200 `SCORED` and `INVALID` verdicts are terminal results. All 4xx responses
are terminal operator errors. Network and 5xx failures use five total attempts
with 1, 2, 4, and 8 second delays, preserving the same team and envelope on
every attempt. Retryable state is durable across UI reloads and exposes a
per-run Retry action; terminal 4xx state is never automatically
resubmitted. Sealed-capture or leaderboard failure does not undo safe Finish or
the plain CSV archive/download.

Issue #13 defines idempotency by retransmitting the same signed envelope and
does not define a separate idempotency header. The browser therefore preserves
the exact envelope and team across attempts; the stable run ID prevents local
duplicate workers, while the server's signed-envelope rule handles an ambiguous
response followed by retransmission.

TLS verification is browser-enforced. Endpoint injection is a unit-test
dependency only; production has no endpoint or dry-run override. The UI must be
open when the operator explicitly chooses **Submit**. A later reconnect
reconstructs the post-run actions but never starts sealed capture by itself.
Once the IndexedDB capture is durable, explicit Retry may resume it; clearing
browser site data removes the browser-owned outbox. The simulator bridge and
leaderboard must permit the CustomUI origin through their CORS policies.

This browser-owned boundary supersedes earlier C++ leaderboard workers.

## Finish ordering: what the simulator lets you observe (v1.1.51)

The post-run sequence is constrained by three facts about the simulator that the
REST reference does not spell out. Each one broke Finish or Reset before it was
understood; each is measured in `docs/emulator-simulator-defects.md`.

**1. Cutting the engine stops the physics loop.** `POST /api/simulator/engine
{"on":false}` calls the game loop's `stop()`. The 1 Hz snapshot publisher usually
survives, but a page that is hidden *and* has a stopped loop publishes nothing at
all. So anything that must be observed has to be observed before the engine goes
off. `finishImpl` therefore runs:

    enterSafeIdle -> confirm vehicle at rest -> rec/stop + confirm
      -> persist the CSV -> engine(false) -> confirm engine off

Do not move `engine(false)` earlier. The stop it commands becomes unobservable
the instant it lands, and finalisation can only time out.

**2. Releasing authority does not stop the tractor.** Every command frame is
gated on `AD_Generic_Mode_Cmd == 1`; the receiver drops the frame otherwise. A
zero opening written while authority is released is discarded, so the run's last
opening stays applied and the wheels keep turning. A stop must be *commanded*:
`MainController::beginCommandedStop()` makes `TrackingLoop::holdSafe` hold
authority with zero opening and neutral until standstill, then release. Plain
idle does not hold authority — a human has to be able to drive in the simulator.

**3. Snapshots are 1 Hz, and the observed age sawtooths past one second.**
Measured peaks reach 1508 ms. `MAX_SNAPSHOT_AGE_MS` is 2000 for that reason: a
budget equal to the publish period rejects perfectly normal samples, and every
loop here requires three *consecutive* fresh ones. For the same reason the whole
reset procedure gets a 25 s budget rather than 3.5 s.

Reset additionally has to tolerate what `POST /api/simulator/refresh` changes:
it drops the implement (Issue #9 §2.19). The post-refresh checks compare map and
tractor and accept either the run's implement or `none`.

Two multi-client rules follow from the dashboard being an ordinary web page that
can be open in several tabs:

- A drain failure reported by one client must not cancel a reset another client
  already admitted (`noteResetCleanupPrepared`). One stale tab used to be enough
  to make every later run impossible to start.
- The browser's drain marker must be released as soon as the backend leaves the
  reset, however the reset ended. It used to clear only on a confirmed
  completion, so a failed reset left a marker in IndexedDB that locked the outbox
  permanently — Submit sat at `capture_pending` across browser restarts.
