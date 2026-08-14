import type { QualityPresetName } from '../../3d/QualityPreset'
import { useSessionStore } from '../../state/sessionStore'
import { getSessionClient } from '../../protocol/SessionClient'

const MONO = "'JetBrains Mono', ui-monospace, monospace";
const SANS = "'Geist', system-ui, sans-serif";

export interface TopBarProps {
  qualityPreset: QualityPresetName
  onQualityChange: (name: QualityPresetName) => void
  onReset: () => void
}

const SPEED_VALUES: (1 | 2 | 4 | 8)[] = [1, 2, 4, 8]

const QUALITY_OPTIONS: { label: string; value: QualityPresetName }[] = [
  { label: 'S-Low',  value: 'super-low' },
  { label: 'Low',    value: 'low'       },
  { label: 'Medium', value: 'medium'    },
  { label: 'High',   value: 'high'      },
]

export default function TopBar(props: TopBarProps) {
  const { qualityPreset, onQualityChange, onReset } = props

  const runNumber       = useSessionStore(s => s.runNumber)
  const started         = useSessionStore(s => s.started)
  const speedMultiplier = useSessionStore(s => s.speedMultiplier)
  const mapList         = useSessionStore(s => s.mapList)
  const selectedMapId   = useSessionStore(s => s.selectedMapId)

  const mapLabel = selectedMapId != null
    ? `#${selectedMapId} ${mapList.find(m => m.id === selectedMapId)?.label ?? ''}`.trim()
    : '—'

  return (
    <div
      style={{
        flexShrink: 0,
        display: 'flex',
        alignItems: 'center',
        gap: 16,
        rowGap: 10,
        flexWrap: 'wrap',
        padding: '14px 24px',
        borderBottom: '1px solid #1d2a24',
        background: 'rgba(6,10,8,0.85)',
        backdropFilter: 'blur(8px)',
        WebkitBackdropFilter: 'blur(8px)',
        zIndex: 10,
      }}
    >
      {/* Logo image */}
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
      {/* Fallback text logo (hidden by default, shown on image error) */}
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
          minWidth: 0,
          overflow: 'hidden',
          whiteSpace: 'nowrap',
        }}
      >
        <span style={{ color: '#7a8494', fontWeight: 600, fontSize: 15 }}>Master of Plowing</span>
        <span style={{ color: '#4a525c', fontSize: 13 }}>|</span>
        <span style={{ color: '#7a8494' }}>simulator</span>
        <span style={{ color: '#4a525c', fontSize: 12 }}>{'>'}</span>
        <span style={{ color: '#7a8494' }}>{mapLabel}</span>
        <span style={{ color: '#4a525c', fontSize: 12 }}>{'>'}</span>
        <span style={{ color: '#e6edf3' }}>run {runNumber.toString().padStart(2, '0')}</span>
      </div>

      {/* Spacer */}
      <div style={{ flex: 1, minWidth: 0 }} />

      {/* Speed multiplier label + toggle group */}
      <span
        style={{
          fontFamily: SANS,
          fontSize: 12,
          fontWeight: 700,
          color: '#cbd5e1',
        }}
      >
        Speed
      </span>
      <div
        role="group"
        aria-label="Speed multiplier"
        style={{
          display: 'flex',
          gap: 0,
          border: '1px solid #1d2a24',
          borderRadius: 4,
          overflow: 'hidden',
        }}
      >
        {SPEED_VALUES.map(v => {
          const active = v === speedMultiplier
          return (
            <button
              key={v}
              aria-pressed={active}
              aria-label={`Speed ${v}x`}
              onClick={() => getSessionClient().sendSetSpeedMultiplier(v)}
              style={{
                padding: '5px 10px',
                fontFamily: MONO,
                fontSize: 11,
                fontWeight: 600,
                letterSpacing: '1px',
                height: 'auto',
                border: 'none',
                cursor: 'pointer',
                background: active ? '#00DF8F' : 'transparent',
                color: active ? '#04140d' : '#7a8494',
              }}
              onMouseEnter={e => {
                if (!active) {
                  (e.currentTarget as HTMLButtonElement).style.background = 'rgba(255,255,255,0.04)'
                  ;(e.currentTarget as HTMLButtonElement).style.color = '#e6edf3'
                }
              }}
              onMouseLeave={e => {
                if (!active) {
                  (e.currentTarget as HTMLButtonElement).style.background = 'transparent'
                  ;(e.currentTarget as HTMLButtonElement).style.color = '#7a8494'
                }
              }}
            >
              {v}x
            </button>
          )
        })}
      </div>

      {/* Quality preset label + toggle group */}
      <span
        style={{
          fontFamily: SANS,
          fontSize: 12,
          fontWeight: 700,
          color: '#cbd5e1',
        }}
      >
        Quality
      </span>
      <div
        role="group"
        aria-label="Render quality"
        style={{
          display: 'flex',
          gap: 0,
          border: '1px solid #1d2a24',
          borderRadius: 4,
          overflow: 'hidden',
        }}
      >
        {QUALITY_OPTIONS.map(({ label, value }) => {
          const active = value === qualityPreset
          return (
            <button
              key={value}
              aria-pressed={active}
              aria-label={`Quality ${label}`}
              onClick={() => onQualityChange(value)}
              style={{
                padding: '5px 10px',
                fontFamily: MONO,
                fontSize: 11,
                fontWeight: 600,
                letterSpacing: '1px',
                height: 'auto',
                border: 'none',
                cursor: 'pointer',
                background: active ? '#00DF8F' : 'transparent',
                color: active ? '#04140d' : '#7a8494',
              }}
              onMouseEnter={e => {
                if (!active) {
                  (e.currentTarget as HTMLButtonElement).style.background = 'rgba(255,255,255,0.04)'
                  ;(e.currentTarget as HTMLButtonElement).style.color = '#e6edf3'
                }
              }}
              onMouseLeave={e => {
                if (!active) {
                  (e.currentTarget as HTMLButtonElement).style.background = 'transparent'
                  ;(e.currentTarget as HTMLButtonElement).style.color = '#7a8494'
                }
              }}
            >
              {label}
            </button>
          )
        })}
      </div>

      {/* Reset button */}
      <button
        onClick={onReset}
        aria-label="Reset session"
        style={{
          padding: '5px 12px',
          fontFamily: MONO,
          fontSize: 11,
          fontWeight: 600,
          letterSpacing: '0.5px',
          border: '1px solid #ef4444',
          borderRadius: 4,
          background: 'rgba(239,68,68,0.08)',
          color: '#ef4444',
          cursor: 'pointer',
          whiteSpace: 'nowrap',
        }}
        onMouseEnter={e => {
          (e.currentTarget as HTMLButtonElement).style.color = '#fca5a5'
          ;(e.currentTarget as HTMLButtonElement).style.borderColor = '#f87171'
          ;(e.currentTarget as HTMLButtonElement).style.background = 'rgba(239,68,68,0.16)'
        }}
        onMouseLeave={e => {
          (e.currentTarget as HTMLButtonElement).style.color = '#ef4444'
          ;(e.currentTarget as HTMLButtonElement).style.borderColor = '#ef4444'
          ;(e.currentTarget as HTMLButtonElement).style.background = 'rgba(239,68,68,0.08)'
        }}
      >
        ↺ Reset
      </button>

      {/* Running / Idle status pill */}
      <div
        role="status"
        aria-live="polite"
        aria-label={started ? 'Running' : 'Idle'}
        style={{
          padding: '4px 10px',
          border: `1px solid ${started ? '#00DF8F' : '#4a525c'}`,
          borderRadius: 999,
          background: started ? 'rgba(0,223,143,0.08)' : 'rgba(74,82,92,0.08)',
          color: started ? '#00DF8F' : '#7a8494',
          fontSize: 12,
          fontFamily: SANS,
          fontWeight: 600,
          whiteSpace: 'nowrap',
          display: 'flex',
          alignItems: 'center',
          gap: 6,
        }}
      >
        <span
          style={{
            display: 'inline-block',
            borderRadius: '50%',
            background: started ? '#00DF8F' : '#7a8494',
            width: 7,
            height: 7,
            marginRight: 1,
            verticalAlign: 'middle',
          }}
        />
        {started ? 'Running' : 'Idle'}
      </div>
    </div>
  )
}
