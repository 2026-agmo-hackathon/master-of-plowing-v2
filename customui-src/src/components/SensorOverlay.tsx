import type { VehicleState } from '../simulation/VehicleSystem'
import type { SoilGrid } from '../simulation/SoilGrid'

interface Props {
  vehicleState: VehicleState | null
  soilGrid: SoilGrid | null
  visible: boolean
}

// Wheel labels in [FL, FR, RL, RR] order matching VehicleState.slip / tireLoad
const WHEEL_LABELS = ['FL', 'FR', 'RL', 'RR'] as const

export default function SensorOverlay({ vehicleState, soilGrid, visible }: Props) {
  if (!visible) return null

  const slip = vehicleState?.slip
  const tireLoad = vehicleState?.tireLoad
  const soilType = vehicleState?.soilType ?? '-'
  // Prefer precomputed moisture from VehicleState; soilGrid kept for future use / API stability
  void soilGrid
  const soilMoisture = vehicleState?.soilMoisture

  const roll = vehicleState?.roll ?? 0
  const pitch = vehicleState?.pitch ?? 0
  const yaw = vehicleState?.yaw ?? 0

  const containerStyle: React.CSSProperties = {
    position: 'absolute',
    top: 80,
    right: 16,
    width: 260,
    background: 'rgba(0,0,0,0.7)',
    color: '#fff',
    fontFamily: '"JetBrains Mono", monospace',
    fontSize: 12,
    padding: 10,
    borderRadius: 6,
    pointerEvents: 'none',
    zIndex: 25,
    boxSizing: 'border-box',
  }

  const sectionTitle: React.CSSProperties = {
    opacity: 0.6,
    fontSize: 10,
    marginBottom: 4,
    letterSpacing: 1,
  }

  const gridStyle: React.CSSProperties = {
    display: 'grid',
    gridTemplateColumns: '1fr 1fr',
    gap: 4,
    marginBottom: 8,
  }

  const cellStyle: React.CSSProperties = {
    background: 'rgba(255,255,255,0.05)',
    padding: '3px 6px',
    borderRadius: 3,
    display: 'flex',
    justifyContent: 'space-between',
  }

  const fmtSlipPct = (v: number | undefined) =>
    v === undefined ? '-' : `${(v * 100).toFixed(1)}%`
  const fmtLoadN = (v: number | undefined) =>
    v === undefined ? '-' : `${Math.round(v)}N`
  const fmtMoisturePct = (v: number | undefined) =>
    v === undefined ? '-' : `${Math.round(v * 100)}%`

  // IMU indicator — 50x50 SVG
  // roll: red line rotating around center (clockwise = positive roll)
  // pitch: green line rotating vertically (up = positive pitch)
  // yaw: blue arrow pointing in heading direction (0 = north / up)
  const rollRad = roll * Math.PI / 180
  const pitchRad = pitch * Math.PI / 180
  const yawRad = yaw * Math.PI / 180
  const cx = 25, cy = 25, r = 20
  const rollX2 = cx + r * Math.cos(rollRad)
  const rollY2 = cy + r * Math.sin(rollRad)
  const pitchX2 = cx + r * Math.sin(pitchRad)
  const pitchY2 = cy - r * Math.cos(pitchRad)
  // Yaw arrow: tip at radius r, 0 rad = up (north)
  const yawTipX = cx + r * Math.sin(yawRad)
  const yawTipY = cy - r * Math.cos(yawRad)

  return (
    <div style={containerStyle}>
      <div style={{ fontSize: 11, fontWeight: 'bold', marginBottom: 6, letterSpacing: 1 }}>
        SENSOR OVERLAY [H]
      </div>

      <div style={sectionTitle}>SLIP (%)</div>
      <div style={gridStyle}>
        {WHEEL_LABELS.map((label, i) => (
          <div key={label} style={cellStyle}>
            <span style={{ opacity: 0.7 }}>{label}</span>
            <span>{fmtSlipPct(slip?.[i])}</span>
          </div>
        ))}
      </div>

      <div style={sectionTitle}>TIRE LOAD (N)</div>
      <div style={gridStyle}>
        {WHEEL_LABELS.map((label, i) => (
          <div key={label} style={cellStyle}>
            <span style={{ opacity: 0.7 }}>{label}</span>
            <span>{fmtLoadN(tireLoad?.[i])}</span>
          </div>
        ))}
      </div>

      <div style={sectionTitle}>SOIL</div>
      <div style={{ marginBottom: 8, padding: '3px 6px', background: 'rgba(255,255,255,0.05)', borderRadius: 3 }}>
        {soilType} / moisture {fmtMoisturePct(soilMoisture)}
      </div>

      <div style={sectionTitle}>IMU</div>
      <div style={{ display: 'flex', alignItems: 'center', gap: 10 }}>
        <svg width={50} height={50} viewBox="0 0 50 50" style={{ background: 'rgba(255,255,255,0.05)', borderRadius: 3 }}>
          {/* center dot */}
          <circle cx={cx} cy={cy} r={2} fill="#888" />
          {/* roll indicator (red) */}
          <line x1={cx} y1={cy} x2={rollX2} y2={rollY2} stroke="#f44336" strokeWidth={2} strokeLinecap="round" />
          {/* pitch indicator (green) */}
          <line x1={cx} y1={cy} x2={pitchX2} y2={pitchY2} stroke="#4caf50" strokeWidth={2} strokeLinecap="round" />
          {/* yaw arrow (blue) */}
          <line x1={cx} y1={cy} x2={yawTipX} y2={yawTipY} stroke="#2196f3" strokeWidth={2} strokeLinecap="round" />
          <circle cx={yawTipX} cy={yawTipY} r={2.5} fill="#2196f3" />
        </svg>
        <div style={{ fontSize: 10, lineHeight: 1.5 }}>
          <div><span style={{ color: '#f44336' }}>R</span> {roll.toFixed(1)}°</div>
          <div><span style={{ color: '#4caf50' }}>P</span> {pitch.toFixed(1)}°</div>
          <div><span style={{ color: '#2196f3' }}>Y</span> {yaw.toFixed(1)}°</div>
        </div>
      </div>
    </div>
  )
}
