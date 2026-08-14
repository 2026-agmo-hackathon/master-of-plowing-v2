import { useCallback, useEffect, useMemo, useRef, useState } from 'react';
import FieldCard from './FieldCard';
import MissionBriefingPanel from './MissionBriefingPanel';
import { useSessionStore } from '../state/sessionStore';
import { getSessionClient } from '../protocol/SessionClient';

// ── Inline CSS ────────────────────────────────────────────────────────────────
const FIELD_STYLES = `
.mop-field-card {
  transition: border-color 120ms ease, box-shadow 120ms ease;
}
.mop-field-card:hover {
  border-color: #00DF8F !important;
  box-shadow: 0 0 0 1px #00DF8F, 0 0 16px rgba(0,223,143,0.10) !important;
}
.mop-field-card:focus-visible {
  outline: 2px solid #00DF8F;
  outline-offset: 2px;
}
.mop-launch-btn {
  transition: transform 100ms ease, box-shadow 100ms ease;
}
.mop-launch-btn:hover:not(:disabled) {
  transform: translateY(-1px);
  box-shadow: 0 8px 20px rgba(0, 223, 143, 0.25);
}
.mop-launch-btn:active:not(:disabled) {
  transform: translateY(0);
}
.mop-back-btn-v2 {
  background: transparent;
  border: 1px solid #2b3340;
  color: #7a8494;
  padding: 4px 8px;
  border-radius: 2px;
  cursor: pointer;
  font-family: 'Geist', sans-serif;
  font-size: 10px;
  letter-spacing: 1.5px;
  transition: border-color 120ms, color 120ms;
}
.mop-back-btn-v2:hover {
  border-color: #00DF8F;
  color: #00DF8F;
}
@keyframes spin {
  to { transform: rotate(360deg); }
}
.mop-retry-btn {
  background: transparent;
  border: 1px solid #ef4444;
  color: #ef4444;
  padding: 8px 24px;
  border-radius: 2px;
  cursor: pointer;
  font-family: 'JetBrains Mono', ui-monospace, monospace;
  font-size: 11px;
  letter-spacing: 1px;
  transition: background 120ms;
}
.mop-retry-btn:hover {
  background: rgba(239, 68, 68, 0.1);
}
@media (prefers-reduced-motion: reduce) {
  .mop-field-card, .mop-launch-btn, .mop-back-btn-v2 {
    transition: none !important;
  }
  @keyframes spin {
    to { transform: rotate(0deg); }
  }
}
`;

const MONO = "'JetBrains Mono', ui-monospace, monospace";
const SANS = "'Geist', system-ui, sans-serif";

// Connection status pill
function ConnectionPill({ connected }: { connected: boolean }) {
  return (
    <div
      role="status"
      aria-live="polite"
      style={{
        padding: '5px 12px',
        border: `1px solid ${connected ? '#00DF8F' : '#ef4444'}`,
        borderRadius: 4,
        background: connected ? 'rgba(0,223,143,0.08)' : 'rgba(239,68,68,0.08)',
        color: connected ? '#00DF8F' : '#ef4444',
        fontSize: 11,
        fontFamily: "'JetBrains Mono', ui-monospace, monospace",
        fontWeight: 600,
        letterSpacing: '2px',
        whiteSpace: 'nowrap',
      }}
    >
      {connected ? 'ONLINE' : 'OFFLINE'}
    </div>
  );
}

// ── Props ─────────────────────────────────────────────────────────────────────
interface Props {
  onBack: () => void;
  onLaunch: (mapId: number) => void;
}

// ── Constants ─────────────────────────────────────────────────────────────────
const MAP_LIST_TIMEOUT_MS = 20_000;

// ── FieldSelectStep ───────────────────────────────────────────────────────────
export default function FieldSelectStep({ onBack, onLaunch }: Props) {
  const mapList             = useSessionStore(s => s.mapList);
  const selectedMapId       = useSessionStore(s => s.selectedMapId);
  const wsConnected         = useSessionStore(s => s.wsConnected);
  const error               = useSessionStore(s => s.error);
  const setError            = useSessionStore(s => s.setError);

  const [isNarrow, setIsNarrow] = useState(() =>
    typeof window !== 'undefined' && window.matchMedia('(max-width: 768px)').matches
  );
  useEffect(() => {
    const mq = window.matchMedia('(max-width: 768px)');
    const handler = (e: MediaQueryListEvent) => setIsNarrow(e.matches);
    mq.addEventListener('change', handler);
    return () => mq.removeEventListener('change', handler);
  }, []);

  const client = getSessionClient();

  const didFetchRef  = useRef(false);
  const timeoutRef   = useRef<ReturnType<typeof setTimeout> | null>(null);

  // Shared map list request: sends the request and arms a 20-second timeout.
  const requestMapList = useCallback(() => {
    if (timeoutRef.current !== null) {
      clearTimeout(timeoutRef.current);
      timeoutRef.current = null;
    }
    client.sendRequestMapList();
    timeoutRef.current = setTimeout(() => {
      timeoutRef.current = null;
      // Guard: do not overwrite a healthy state that arrived late.
      const { mapList: currentList, error: currentError, setError: doSetError } =
        useSessionStore.getState();
      if (currentList.length === 0 && currentError === null) {
        doSetError('Map list request timed out — no response from server after 20 seconds.');
      }
    }, MAP_LIST_TIMEOUT_MS);
  }, [client]);

  // True when map list has arrived (detail is included in list response)
  const mapsReady = useMemo(
    () => mapList.length > 0,
    [mapList]
  );

  // Clear the timeout when mapList arrives or an error is set.
  useEffect(() => {
    if ((mapsReady || error) && timeoutRef.current !== null) {
      clearTimeout(timeoutRef.current);
      timeoutRef.current = null;
    }
  }, [mapsReady, error]);

  // Cleanup on unmount.
  useEffect(() => {
    return () => {
      if (timeoutRef.current !== null) {
        clearTimeout(timeoutRef.current);
        timeoutRef.current = null;
      }
    };
  }, []);

  // Request map list once WS is connected.
  // SessionClient.send silently drops when WS not OPEN, so we must wait.
  useEffect(() => {
    if (!wsConnected || didFetchRef.current) return;
    didFetchRef.current = true;
    requestMapList();
  }, [wsConnected]); // eslint-disable-line react-hooks/exhaustive-deps

  // Count equals total since all data arrives with map_list
  const loadedCount = mapList.length;

  // Helper: select a map
  function selectMap(id: number) {
    client.sendSetSelectedMap(id);
  }

  // 3x2 grid keyboard navigation
  const COLS = 3;
  useEffect(() => {
    const handler = (e: KeyboardEvent) => {
      if (e.key === 'Escape') { onBack(); return; }

      if (['ArrowDown', 'ArrowUp', 'ArrowLeft', 'ArrowRight'].includes(e.key)) {
        e.preventDefault();
        const idx = mapList.findIndex(m => m.id === selectedMapId);
        if (idx < 0 && mapList.length > 0) {
          selectMap(mapList[0].id);
          return;
        }
        let next = idx;
        if (e.key === 'ArrowRight') next = idx + 1;
        if (e.key === 'ArrowLeft')  next = idx - 1;
        if (e.key === 'ArrowDown')  next = idx + COLS;
        if (e.key === 'ArrowUp')    next = idx - COLS;
        // clamp within bounds
        next = Math.max(0, Math.min(mapList.length - 1, next));
        const target = mapList[next];
        if (target) selectMap(target.id);
      }

      if (e.key === 'Enter' && selectedMapId != null) {
        onLaunch(selectedMapId);
      }
    };
    window.addEventListener('keydown', handler);
    return () => window.removeEventListener('keydown', handler);
  }, [mapList, selectedMapId, onBack, onLaunch]); // eslint-disable-line react-hooks/exhaustive-deps

  // Derive selected entry
  const selectedEntry = mapList.find(m => m.id === selectedMapId) ?? null;
  const codeName = selectedEntry ? (selectedEntry.label ?? selectedEntry.id).toUpperCase() : '';
  const subLabel = selectedEntry?.description ?? '';

  return (
    <div
      style={{
        position: 'absolute',
        top: 0,
        left: 0,
        width: '100vw',
        height: '100vh',
        background: '#0a0f12',
        color: '#e6edf3',
        fontFamily: SANS,
        display: 'flex',
        flexDirection: 'column',
        boxSizing: 'border-box',
        overflow: 'hidden',
      }}
    >
      <style>{FIELD_STYLES}</style>

      {/* ── Top bar ── */}
      <div
        style={{
          flexShrink: 0,
          display: 'flex',
          alignItems: 'center',
          gap: 16,
          padding: '14px 24px',
          borderBottom: '1px solid #1d2a24',
          background: 'rgba(6,10,8,0.85)',
          backdropFilter: 'blur(8px)',
          WebkitBackdropFilter: 'blur(8px)',
          zIndex: 10,
        }}
      >
        {/* Logo */}
        <img
          src="./title-card/logo.png"
          alt="SeamOS"
          style={{ height: 28, width: 'auto', flexShrink: 0 }}
          onError={(e) => {
            const target = e.currentTarget as HTMLImageElement;
            target.style.display = 'none';
            const next = target.nextSibling as HTMLElement | null;
            if (next) next.style.display = 'flex';
          }}
        />
        {/* Fallback text logo (hidden by default) */}
        <div
          style={{
            display: 'none',
            alignItems: 'center',
            gap: 6,
            flexShrink: 0,
          }}
        >
          <div
            style={{
              width: 8,
              height: 8,
              borderRadius: '50%',
              background: '#00DF8F',
            }}
          />
          <span
            style={{
              fontFamily: MONO,
              fontSize: 14,
              fontWeight: 700,
              color: '#e6edf3',
              letterSpacing: '1px',
            }}
          >
            SeamOS
          </span>
        </div>

        {/* Breadcrumb */}
        <div
          style={{
            display: 'flex',
            alignItems: 'center',
            gap: 8,
            fontFamily: SANS,
            fontSize: 14,
            fontWeight: 500,
          }}
        >
          <span style={{ color: '#7a8494', fontWeight: 600, fontSize: 15 }}>Master of Plowing</span>
          <span style={{ color: '#4a525c', fontSize: 13 }}>|</span>
          <span style={{ color: '#7a8494' }}>simulator</span>
          <span style={{ color: '#4a525c', fontSize: 12 }}>{'>'}</span>
          <span style={{ color: '#e6edf3' }}>new mission</span>
        </div>

        <div style={{ flex: 1 }} />

        {/* Connection pill */}
        <ConnectionPill connected={wsConnected} />
      </div>

      {/* ── Header block ── */}
      <div
        style={{
          flexShrink: 0,
          padding: '20px 28px 16px',
          borderBottom: '1px solid #1d2a24',
        }}
      >
        <div
          style={{
            fontFamily: MONO,
            fontSize: 11,
            color: '#7a8494',
            letterSpacing: '1px',
            marginBottom: 8,
          }}
        >
          Step 01 / 03 · Select field
        </div>
        <h1
          style={{
            fontFamily: SANS,
            fontSize: 'clamp(24px, 3vw, 36px)',
            fontWeight: 700,
            color: '#e6edf3',
            margin: 0,
            letterSpacing: '-0.02em',
            lineHeight: 1.1,
          }}
        >
          Choose your plowing course
        </h1>
        <p
          style={{
            fontFamily: SANS,
            fontSize: 14,
            color: '#7a8494',
            margin: '8px 0 0',
            lineHeight: 1.5,
          }}
        >
          Each field has its own shape, lane geometry, and target coverage.
        </p>
      </div>

      {/* ── Main 2-column area ── */}
      <div
        style={{
          display: 'flex',
          flexDirection: isNarrow ? 'column' : 'row',
          gap: '24px',
          flex: 1,
          minHeight: 0,
          padding: '24px 28px',
          overflowY: 'auto',
        }}
      >
        {/* Left: 3x2 card grid */}
        <div
          role="radiogroup"
          aria-label="Field selection"
          style={{
            flex: 1,
            display: 'grid',
            gridTemplateColumns: isNarrow ? 'repeat(2, 1fr)' : 'repeat(3, 1fr)',
            gap: '16px',
            alignContent: 'start',
          }}
        >
          {!mapsReady && !error && (
            <div
              role="status"
              aria-label="Loading maps"
              style={{
                gridColumn: '1 / -1',
                minHeight: 320,
                display: 'flex',
                flexDirection: 'column',
                alignItems: 'center',
                justifyContent: 'center',
                gap: 12,
              }}
            >
              {/* Spinner */}
              <div
                style={{
                  width: 32,
                  height: 32,
                  borderRadius: '50%',
                  border: '3px solid rgba(0, 223, 143, 0.15)',
                  borderTopColor: '#00DF8F',
                  animation: 'spin 0.8s linear infinite',
                }}
              />
              {/* Primary label */}
              <span style={{ fontFamily: SANS, fontSize: 14, color: '#e6edf3' }}>
                {mapList.length === 0 ? 'Loading map list…' : 'Loading maps…'}
              </span>
              {/* Progress — only when list is known */}
              {mapList.length > 0 && (
                <span style={{ fontFamily: MONO, fontSize: 12, color: '#7a8494' }}>
                  ({loadedCount} / {mapList.length})
                </span>
              )}
            </div>
          )}
          {error && (
            <div
              role="alert"
              style={{
                gridColumn: '1 / -1',
                minHeight: 320,
                display: 'flex',
                flexDirection: 'column',
                alignItems: 'center',
                justifyContent: 'center',
              }}
            >
              {/* Error card */}
              <div
                style={{
                  background: '#11181d',
                  border: '1px solid rgba(239, 68, 68, 0.3)',
                  borderRadius: 4,
                  padding: '32px 40px',
                  maxWidth: 440,
                  textAlign: 'center',
                }}
              >
                {/* Warning icon */}
                <div style={{ fontSize: 32, color: '#ef4444', marginBottom: 16, lineHeight: 1 }}>
                  ⚠
                </div>
                {/* Title */}
                <p style={{
                  fontFamily: SANS,
                  fontSize: 16,
                  fontWeight: 600,
                  color: '#e6edf3',
                  margin: '0 0 8px',
                }}>
                  Failed to load maps
                </p>
                {/* Error detail */}
                <p style={{
                  fontFamily: SANS,
                  fontSize: 13,
                  color: '#7a8494',
                  lineHeight: 1.5,
                  margin: '0 0 20px',
                }}>
                  {error}
                </p>
                {/* Retry button */}
                <button
                  className="mop-retry-btn"
                  onClick={() => { setError(null); requestMapList(); }}
                >
                  Retry
                </button>
              </div>
            </div>
          )}
          {mapsReady && mapList.map((entry) => {
            const disabled = entry.rddfAvailable === false;
            return (
              <div
                key={entry.id}
                style={{
                  position: 'relative',
                  opacity: disabled ? 0.4 : 1,
                  pointerEvents: disabled ? 'none' : 'auto',
                }}
              >
                {disabled && (
                  <div style={{
                    position: 'absolute',
                    bottom: 8,
                    left: 8,
                    zIndex: 1,
                    background: 'rgba(10,15,18,0.85)',
                    color: '#7a8494',
                    fontFamily: MONO,
                    fontSize: 9,
                    letterSpacing: '1px',
                    padding: '2px 6px',
                    borderRadius: 2,
                  }}>
                    RDDF not uploaded
                  </div>
                )}
                <FieldCard
                  codeName={`#${entry.id} ${(entry.label ?? String(entry.id)).toUpperCase()}`}
                  subLabel={entry.description ?? ''}
                  area_m2={entry.area_m2}
                  selected={selectedMapId === entry.id}
                  onClick={() => {
                    if (entry.rddfAvailable === false) return;
                    selectMap(entry.id);
                  }}
                  polygons={entry.polygonLL ? [entry.polygonLL] : []}
                  mapId={String(entry.id)}
                />
              </div>
            );
          })}
        </div>

        {/* Right: Mission briefing panel */}
        <div
          style={{
            flex: isNarrow ? '0 0 auto' : '0 0 340px',
            width: isNarrow ? '100%' : undefined,
            minWidth: 0,
          }}
        >
          <MissionBriefingPanel
            codeName={codeName}
            subLabel={subLabel}
            description={selectedEntry?.description}
            area_m2={selectedEntry?.area_m2}
            polygons={selectedEntry?.polygonLL ? [selectedEntry.polygonLL] : []}
            mapId={selectedMapId == null ? '' : String(selectedMapId)}
            onLaunch={() => { if (selectedMapId == null) return; onLaunch(selectedMapId); }}
            startLat={selectedEntry?.start_lat}
            startLon={selectedEntry?.start_lon}
          />
        </div>
      </div>
    </div>
  );
}
