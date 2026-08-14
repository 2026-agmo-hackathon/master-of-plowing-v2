import { create } from 'zustand';
import { resetOrigin } from '../simulation/GpsOrigin';
import { getSessionClient } from '../protocol/SessionClient';

export interface MapListEntry {
  id: number;
  label: string;
  description?: string;
  area_m2?: number;
  origin?: { lat: number; lon: number };
  start_lat?: number;
  start_lon?: number;
  start_heading_deg?: number;
  has_start?: boolean;
  polygonLL?: [number, number][];   // raw [lat, lon] pairs (single ring per map)
  rddfAvailable?: boolean;
}

export interface RddfPendingItem {
  mapId: number;
  receivedAt: string;
}

// Raw lat/lon form of an RDDF waypoint. ENU conversion is deferred to render
// time (RddfPathDriver) and keyed on originVersion so re-projection happens
// automatically when the GPS origin changes.
export interface RddfWaypointLL {
  lat: number;
  lon: number;
  speed: number;
  implementFlag: number;
}

export interface ApiErrorState {
  source: 'map_list' | 'leaderboard' | 'start_session' | 'rddf_validation'
  reason: string
  code?: 'timeout' | 'http_error' | 'parse_error' | 'no_rddf' | 'invalid_rddf' | 'too_few_points'
  status?: number      // HTTP status (only for leaderboard or http_error)
  op?: 'start' | 'finished' | 'dnf'  // leaderboard only
  runId?: string                       // leaderboard only
}

export interface SessionState {
  // Mirror state (C++ is source of truth. UI is cache)
  tractorPreset: string;     // '50hp' | '120hp' | '200hp' | ''
  implementPreset: string;   // 'plow_2.3m' | ... | ''
  selectedMapId: number | null;
  started: boolean;
  missionName: string;
  runNumber: number;
  startedAt: string;
  coverageGoal: number;
  speedMultiplier: 1 | 2 | 4 | 8;

  // UI auxiliary state
  wsConnected: boolean;
  mapList: MapListEntry[];
  error: string | null;
  apiError: ApiErrorState | null;
  restoring: boolean;        // loading indicator during reconnect restore
  isLaunching: boolean;      // loading indicator while sendStartSession is in flight
  isAutonomous: boolean;

  // Map geometry (populated from map_list entry polygonLL)
  mapSize: { width: number; height: number } | null;
  mapPolygonsLL: [number, number][][];
  // Raw [lon, lat] polygons kept for re-projection when RDDF origin arrives
  rawPolygonsLonLat: [number, number][][];
  // Incremented each time GpsOrigin changes so components can re-derive ENU
  originVersion: number;
  rddfWaypointsLL: RddfWaypointLL[];
  // RDDF ENU origin (lat/lon of first waypoint)
  rddfOrigin: { lat: number; lon: number } | null;
  // RDDF pending queue (staging before user-confirmed replacement)
  rddfPendingQueue: RddfPendingItem[];

  // actions (all setters — mirror C++ responses)
  setTractorPreset: (v: string) => void;
  setImplementPreset: (v: string) => void;
  setSelectedMap: (v: number | null) => void;
  setStarted: (b: boolean) => void;
  setWsConnected: (b: boolean) => void;
  setMapList: (list: MapListEntry[]) => void;
  setError: (msg: string | null) => void;
  setApiError: (e: ApiErrorState | null) => void;
  dismissApiError: () => void;
  setRestoring: (b: boolean) => void;
  setLaunching: (b: boolean) => void;
  setIsAutonomous: (b: boolean) => void;
  setMapSize: (size: { width: number; height: number } | null) => void;
  setMapPolygonsLL: (polys: [number, number][][]) => void;
  setRawPolygonsLonLat: (polys: [number, number][][]) => void;
  bumpOriginVersion: () => void;
  setRddfWaypointsLL: (wps: RddfWaypointLL[]) => void;
  setRddfOrigin: (origin: { lat: number; lon: number } | null) => void;
  setSpeedMultiplier: (v: 1 | 2 | 4 | 8) => void;
  teamName: string;
  setTeamName: (v: string) => void;
  // bulk apply session_state response
  applySessionState: (s: { tractorPreset: string; implementPreset: string; selectedMapId: number; started: boolean; missionName?: string; runNumber?: number; startedAt?: string; coverageGoal?: number; speedMultiplier?: 1 | 2 | 4 | 8 }) => void;
  // RDDF pending queue actions
  enqueueRddfPending: (item: RddfPendingItem) => void;
  dequeueRddfPending: (mapId: number) => void;
  coldStartReset: () => void;
}

// Fields that must be restored to initial values on cold-start reset.
// Excludes wsConnected, mapList, teamName — those are repopulated by
// reconnect/side-effects, not by store initialisation.
const COLD_START_STATE = {
  tractorPreset: '',
  implementPreset: '',
  selectedMapId: null as number | null,
  started: false,
  missionName: 'Master of Plowing',
  runNumber: 1,
  startedAt: '',
  coverageGoal: 95,
  speedMultiplier: 1 as 1 | 2 | 4 | 8,
  error: null as string | null,
  apiError: null as ApiErrorState | null,
  restoring: false,
  isLaunching: false,
  isAutonomous: false,
  mapSize: null as { width: number; height: number } | null,
  mapPolygonsLL: [] as [number, number][][],
  rawPolygonsLonLat: [] as [number, number][][],
  originVersion: 0,
  rddfWaypointsLL: [] as RddfWaypointLL[],
  rddfOrigin: null as { lat: number; lon: number } | null,
  rddfPendingQueue: [] as RddfPendingItem[],
};

export const useSessionStore = create<SessionState>((set) => ({
  ...COLD_START_STATE,
  wsConnected: false,
  mapList: [],

  setTractorPreset: (v) => set({ tractorPreset: v }),
  setImplementPreset: (v) => set({ implementPreset: v }),
  setSelectedMap: (v) => set({ selectedMapId: v }),
  setStarted: (b) => set(b ? { started: b, isLaunching: false } : { started: b }),
  setWsConnected: (b) => set({ wsConnected: b }),
  setMapList: (list) => set({ mapList: list }),
  setError: (msg) => set({ error: msg }),
  setApiError: (e) => set({ apiError: e }),
  dismissApiError: () => set({ apiError: null }),
  setRestoring: (b) => set({ restoring: b }),
  setLaunching: (b) => set({ isLaunching: b }),
  setIsAutonomous: (b) => set({ isAutonomous: b }),
  setMapSize: (size) => set({ mapSize: size }),
  setMapPolygonsLL: (polys) => set({ mapPolygonsLL: polys }),
  setRawPolygonsLonLat: (polys) => set({ rawPolygonsLonLat: polys }),
  bumpOriginVersion: () => set((state) => ({ originVersion: state.originVersion + 1 })),
  setRddfWaypointsLL: (wps) => set({ rddfWaypointsLL: wps }),
  setRddfOrigin: (origin) => set({ rddfOrigin: origin }),
  setSpeedMultiplier: (v) => set({ speedMultiplier: v }),
  teamName: localStorage.getItem('leaderboard_teamName') ?? '',
  setTeamName: (v) => { localStorage.setItem('leaderboard_teamName', v); set({ teamName: v }); },
  applySessionState: (s) => {
    set({
      tractorPreset: s.tractorPreset,
      implementPreset: s.implementPreset,
      selectedMapId: s.selectedMapId,
      started: s.started,
      missionName: s.missionName ?? 'Master of Plowing',
      runNumber: s.runNumber ?? 1,
      startedAt: s.startedAt ?? '',
      coverageGoal: s.coverageGoal ?? 95,
      speedMultiplier: s.speedMultiplier ?? 1,
    });
  },
  enqueueRddfPending: (item) => set((state) => {
    const existing = state.rddfPendingQueue.find((x) => x.mapId === item.mapId);
    if (existing) {
      // Update receivedAt for existing mapId
      return {
        rddfPendingQueue: state.rddfPendingQueue.map((x) =>
          x.mapId === item.mapId ? { ...x, receivedAt: item.receivedAt } : x
        ),
      };
    } else {
      // Add new item
      return {
        rddfPendingQueue: [...state.rddfPendingQueue, item],
      };
    }
  }),
  dequeueRddfPending: (mapId) => set((state) => ({
    rddfPendingQueue: state.rddfPendingQueue.filter((x) => x.mapId !== mapId),
  })),
  coldStartReset: () => {
    set({ ...COLD_START_STATE });
    resetOrigin();
    getSessionClient().resetCaches();
  },
}));
