import {
  IncomingMessage,
  OutgoingMessage,
  SessionStateMsg,
  MapListMsg,
  MapListErrorMsg,
  SessionStateChangedMsg,
  SimulationSnapshotPatchedMsg,
  RddfPathMsg,
  RunSummaryMessage,
  RddfPendingMsg,
  RddfAppliedMsg,
  RddfCancelledMsg,
  RddfErrorMsg,
  TransformPayload,
  ControlPayload,
  StatsPayload,
  TillageCellPatch,
  LeaderboardErrorMsg,
  StartSessionErrorMsg,
} from './SessionProtocol';
import { useSessionStore } from '../state/sessionStore';
import { initPorts, createWebSocketClient, sendJson, closeWebSocketSafe } from '@seamos/connect';
import type { WebSocketClientResult } from '@seamos/connect';

const PATH = '/socket';

type SnapshotHandler = (msg: SimulationSnapshotPatchedMsg) => void;
type RunSummaryHandler = (msg: RunSummaryMessage) => void;

export class SessionClient {
  private client: WebSocketClientResult | null = null;

  // One-shot response callbacks (simpler than Promise-based)
  private snapshotHandlers = new Set<SnapshotHandler>();
  private runSummaryHandlers = new Set<RunSummaryHandler>();

  // External raw message router handlers
  private rawHandlers = new Set<(data: unknown) => void>();

  // Track previous selectedMapId to detect changes
  private prevSelectedMapId: number | null = null;

  // host/port params are kept for API compatibility but ignored; initPorts() determines the URL.
  constructor(_host?: string, _port?: number) {
  }

  start(): void {
    void this.connectAsync();
  }

  stop(): void {
    closeWebSocketSafe(this.client?.socket ?? null, 1000, 'stopped');
    if (this.client) {
      this.client.close(1000, 'stopped');
      this.client = null;
    }
  }

  private async connectAsync(): Promise<void> {
    await initPorts();
    this.client = createWebSocketClient(PATH, {
      autoReconnect: true,
      reconnectInterval: 3000,
      events: {
        // createWebSocketClient returns synchronously; the open event fires on the next
        // microtask, so `this.client` is guaranteed to be assigned at this point.
        // send() 내부 try-catch가 안전 처리하므로 여기서의 send 호출도 보호됨.
        open: () => {
          useSessionStore.getState().setWsConnected(true);
          this.sendGetSessionState();
          this.sendGetRddfPath();
        },
        message: (ev: MessageEvent) => {
          try {
            const msg = JSON.parse(typeof ev.data === 'string' ? ev.data : '');
            this.handleIncoming(msg as IncomingMessage);
          } catch (_e) {}
        },
        close: () => {
          useSessionStore.getState().setWsConnected(false);
        },
        error: () => {},
      },
    });
  }

  private handleIncoming(msg: IncomingMessage): void {
    const store = useSessionStore.getState();
    switch (msg.kind) {
      case 'session_state': {
        const m = msg as SessionStateMsg;
        store.applySessionState({
          tractorPreset: m.tractorPreset,
          implementPreset: m.implementPreset,
          selectedMapId: m.selectedMapId,
          started: m.started,
          missionName: m.mission_name ?? 'Master of Plowing',
          runNumber: m.run_number ?? 1,
          startedAt: m.started_at ?? '',
          coverageGoal: m.coverage_goal ?? 95,
          speedMultiplier: (m.speed_multiplier as 1 | 2 | 4 | 8) ?? 1,
        });
        // Initialize prevSelectedMapId without triggering sendGetRddfPath
        this.prevSelectedMapId = m.selectedMapId ?? null;
        break;
      }
      case 'session_state_changed': {
        const m = msg as SessionStateChangedMsg;
        store.applySessionState({
          tractorPreset: m.tractorPreset,
          implementPreset: m.implementPreset,
          selectedMapId: m.selectedMapId,
          started: m.started,
          missionName: m.mission_name ?? 'Master of Plowing',
          runNumber: m.run_number ?? 1,
          startedAt: m.started_at ?? '',
          coverageGoal: m.coverage_goal ?? 95,
          speedMultiplier: (m.speed_multiplier as 1 | 2 | 4 | 8) ?? 1,
        });
        // Auto-trigger sendGetRddfPath if selectedMapId changed
        if ((m.selectedMapId ?? null) !== this.prevSelectedMapId) {
          this.prevSelectedMapId = m.selectedMapId ?? null;
          window.setTimeout(() => {
            this.sendGetRddfPath();
          }, 300);
        }
        break;
      }
      case 'map_list': {
        const m = msg as MapListMsg;
        store.setMapList(m.maps || []);
        // Collect each map's polygonLL ring into the store.
        // entry.polygonLL is a single ring ([lat,lon][]);
        // wrap it so mapPolygonsLL is an array of rings.
        const allPolygonsLL: [number, number][][] = [];
        for (const entry of m.maps || []) {
          if (entry.polygonLL && entry.polygonLL.length >= 3) {
            allPolygonsLL.push(entry.polygonLL as [number, number][]);
          }
        }
        store.setMapPolygonsLL(allPolygonsLL);
        break;
      }
      case 'simulation_snapshot_patched': {
        const m = msg as SimulationSnapshotPatchedMsg;
        for (const h of this.snapshotHandlers) h(m);
        break;
      }
      case 'rddf_path': {
        const m = msg as RddfPathMsg;
        const waypointsLL = (m.waypoints ?? []).map(w => ({
          lat: w.lat,
          lon: w.lon,
          speed: w.speed,
          implementFlag: w.implementFlag,
        }))
        console.log('[SessionClient] rddf_path received', {
          count: waypointsLL.length,
          originLat: m.originLat,
          originLon: m.originLon,
          first: waypointsLL[0],
        })
        store.setRddfWaypointsLL(waypointsLL)
        break;
      }
      case 'run_summary': {
        const m = msg as RunSummaryMessage;
        for (const h of this.runSummaryHandlers) {
          try { h(m); } catch (_) {}
        }
        break;
      }
      case 'rddf_pending': {
        const m = msg as RddfPendingMsg;
        store.enqueueRddfPending({ mapId: m.mapId, receivedAt: m.receivedAt });
        break;
      }
      case 'rddf_applied': {
        const m = msg as RddfAppliedMsg;
        store.dequeueRddfPending(m.mapId);
        // Auto-refresh map list so the just-applied RDDF flips its card from disabled to active.
        this.sendRequestMapList();
        break;
      }
      case 'rddf_cancelled': {
        const m = msg as RddfCancelledMsg;
        store.dequeueRddfPending(m.mapId);
        break;
      }
      case 'rddf_error': {
        const m = msg as RddfErrorMsg;
        if (m.mapId !== undefined) store.dequeueRddfPending(m.mapId);
        store.setApiError({
          source: 'rddf_validation',
          reason: m.reason,
        });
        break;
      }
      case 'map_list_error': {
        const m = msg as MapListErrorMsg;
        store.setError(m.reason);  // legacy compat
        store.setApiError({
          source: 'map_list',
          reason: m.reason,
          code: m.code,
        });
        break;
      }
      case 'leaderboard_error': {
        const m = msg as LeaderboardErrorMsg;
        useSessionStore.getState().setApiError({
          source: 'leaderboard',
          reason: m.reason,
          code: m.code,
          status: m.status,
          op: m.op,
          runId: m.runId,
        });
        break;
      }
      case 'start_session_error': {
        const m = msg as StartSessionErrorMsg;
        store.setApiError({
          source: 'start_session',
          reason: m.reason,
          code: m.code,
        });
        // legacy compat
        store.setError(m.reason);
        break;
      }
      default: {
        // Forward unrecognized messages to external raw handlers
        for (const h of this.rawHandlers) {
          try { h(msg); } catch (_) {}
        }
        break;
      }
    }
  }

  private send(msg: OutgoingMessage | { kind: string; [k: string]: unknown }): void {
    if (!this.client?.socket || this.client.socket.readyState !== WebSocket.OPEN) return;
    try {
      sendJson(this.client.socket, msg);
    } catch (_e) {
      // sendJson can throw under race conditions (socket transitioning to CLOSING).
      // Silent drop matches legacy behavior; auto-reconnect will recover.
    }
  }

  // ---- Send methods ----
  sendGetSessionState(): void { this.send({ kind: 'get_session_state' }); }
  sendSetTractorPreset(value: string): void {
    this.send({ kind: 'set_tractor_preset', value });
    useSessionStore.getState().setTractorPreset(value); // optimistic update
  }
  sendSetImplementPreset(value: string): void {
    this.send({ kind: 'set_implement_preset', value });
    useSessionStore.getState().setImplementPreset(value);
  }
  sendSetSelectedMap(value: number | null): void {
    this.send({ kind: 'set_selected_map', value } as OutgoingMessage);
    useSessionStore.getState().setSelectedMap(value);
  }
  sendStartSession(teamName: string, implementWidth: number): void {
    this.send({ kind: 'start_session', teamName, implementWidth });
  }
  public resetCaches(): void {
    this.prevSelectedMapId = null;
  }

  sendResetSession(): void {
    this.resetCaches();
    this.send({ kind: 'reset_session' });
  }
  sendUpdateTransform(payload: TransformPayload & ControlPayload & StatsPayload): void {
    this.send({ kind: 'update_transform', ...payload });
  }
  sendUpdateTillage(cells: TillageCellPatch[]): void {
    if (!cells || cells.length === 0) return;
    this.send({ kind: 'update_tillage', cells });
  }
  sendRequestMapList(): void { this.send({ kind: 'request_map_list' }); }
  sendGetRddfPath(): void { this.send({ kind: 'get_rddf_path' }); }
  sendSetSpeedMultiplier(value: 1 | 2 | 4 | 8): void {
    this.send({ kind: 'set_speed_multiplier', value });
  }
  sendProgressMirror(payload: {
    finalCoverage: number;
    plowedM2: number;
    totalM2: number;
    avgDraftKn: number;
    lostAreaM2: number;
    elapsedTo95S: number | null;
    bestLane?: { index: number; ratio: number };
  }): void {
    this.send({ kind: 'progress_mirror', ...payload });
  }
  sendGetRunSummary(runNumber: number): void {
    this.send({ kind: 'get_run_summary', runNumber });
  }
  sendRddfApply(mapId: number): void {
    this.send({ kind: 'rddf_apply', mapId });
  }
  sendRddfCancel(mapId: number): void {
    this.send({ kind: 'rddf_cancel', mapId });
  }

  // ---- Callback registration ----
  onSnapshotPatched(handler: SnapshotHandler): () => void {
    this.snapshotHandlers.add(handler);
    return () => this.snapshotHandlers.delete(handler);
  }
  onRunSummary(handler: RunSummaryHandler): () => void {
    this.runSummaryHandlers.add(handler);
    return () => this.runSummaryHandlers.delete(handler);
  }

  // ---- External raw message router ----
  onRawMessage(handler: (data: unknown) => void): () => void {
    this.rawHandlers.add(handler);
    return () => this.rawHandlers.delete(handler);
  }

  // ---- Arbitrary envelope sender ----
  sendEnvelope(envelope: unknown): boolean {
    if (!this.client?.socket || this.client.socket.readyState !== WebSocket.OPEN) return false;
    try {
      sendJson(this.client.socket, envelope);
      return true;
    } catch (_e) {
      return false;
    }
  }

  // ---- Connection status ----
  isConnected(): boolean {
    return this.client?.socket?.readyState === WebSocket.OPEN;
  }
}

// Module-scoped singleton — shared across App.tsx, FieldSimulatorView, etc.
let _instance: SessionClient | null = null;
export function getSessionClient(): SessionClient {
  if (_instance === null) {
    _instance = new SessionClient();
  }
  return _instance;
}
