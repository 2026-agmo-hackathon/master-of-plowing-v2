import { useEffect, useState } from 'react'
import type { VehicleState } from '../simulation/VehicleSystem'

interface Props {
  onAttach: (handler: (state: VehicleState) => void) => () => void
}

export default function MockSink({ onAttach }: Props) {
  const [visible, setVisible] = useState(true)
  const [latest, setLatest] = useState<VehicleState | null>(null)

  useEffect(() => {
    const handler = (s: VehicleState) => {
      setLatest(s)
    }
    const detach = onAttach(handler)
    return detach
  }, [onAttach])

  const fmt = (v: number | undefined, digits = 4): string =>
    v === undefined || !isFinite(v) ? '-' : v.toFixed(digits)

  const speedKmh = latest ? latest.speed * 3.6 : undefined

  // Toggle button — matches MapSelector (terrain) exactly: same padding/border/colors.
  // Parent wraps this inside a flex column to stack with MapSelector.
  const toggleBtnStyle: React.CSSProperties = {
    background: 'rgba(0,0,0,0.75)',
    color: '#ccc',
    border: '1px solid #555',
    borderRadius: 6,
    padding: '6px 10px',
    fontSize: 12,
    cursor: 'pointer',
    outline: 'none',
    textAlign: 'left',
    width: '100%',
    boxSizing: 'border-box',
  }

  const toggleBtn = (
    <button style={toggleBtnStyle} onClick={() => setVisible(v => !v)}>
      {visible ? '▾ values' : '▸ values'}
    </button>
  )

  if (!visible) return toggleBtn

  const containerStyle: React.CSSProperties = {
    marginTop: 2,
    width: 260,
    maxHeight: 'calc(100vh - 100px)',
    overflowY: 'auto',
    background: 'rgba(0,0,0,0.72)',
    color: '#e2e8f0',
    borderRadius: 8,
    padding: 10,
    fontFamily: 'ui-monospace, SFMono-Regular, Menlo, monospace',
    fontSize: 12,
    zIndex: 30,
    boxShadow: '0 4px 14px rgba(0,0,0,0.4)',
    boxSizing: 'border-box',
  }

  const sectionTitle: React.CSSProperties = {
    opacity: 0.6,
    fontSize: 10,
    marginBottom: 4,
    marginTop: 6,
    letterSpacing: 1,
  }

  const rowStyle: React.CSSProperties = {
    display: 'flex',
    justifyContent: 'space-between',
    background: 'rgba(255,255,255,0.05)',
    padding: '3px 6px',
    borderRadius: 3,
    marginBottom: 3,
  }

  const labelStyle: React.CSSProperties = { opacity: 0.7 }

  return (
    <>
      {toggleBtn}
      <div style={containerStyle}>
      <div style={sectionTitle}>POSITION</div>
      <div style={rowStyle}><span style={labelStyle}>latitude</span><span>{fmt(latest?.latitude, 6)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>longitude</span><span>{fmt(latest?.longitude, 6)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>altitude (m)</span><span>{fmt(latest?.altitude, 2)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>numSat</span><span>{latest?.numSat ?? '-'}</span></div>

      <div style={sectionTitle}>MOTION</div>
      <div style={rowStyle}><span style={labelStyle}>speed (m/s)</span><span>{fmt(latest?.speed, 3)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>speed (km/h)</span><span>{fmt(speedKmh, 2)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>heading (°)</span><span>{fmt(latest?.heading, 2)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>wheelSpeed (rad/s)</span><span>{fmt(latest?.wheelSpeed, 3)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>gear</span><span>{latest?.gear ?? '-'}</span></div>
      <div style={rowStyle}><span style={labelStyle}>running</span><span>{latest ? String(latest.running) : '-'}</span></div>

      <div style={sectionTitle}>ENGINE / PEDAL</div>
      <div style={rowStyle}><span style={labelStyle}>rpm</span><span>{fmt(latest?.rpm, 1)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>throttle</span><span>{fmt(latest?.throttle, 3)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>brake</span><span>{fmt(latest?.brake, 3)}</span></div>

      <div style={sectionTitle}>STEERING</div>
      <div style={rowStyle}><span style={labelStyle}>steerAngle (°)</span><span>{fmt(latest?.steerAngle, 2)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>measuredSteer (°)</span><span>{fmt(latest?.measuredSteerAngleDeg, 2)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>frontWheel (°)</span><span>{fmt(latest?.vehicleFrontWheelAngleDeg, 2)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>motorCmdRpm</span><span>{fmt(latest?.motorCmdRpm, 1)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>appSteerCmd</span><span>{fmt(latest?.appSteerCommand, 1)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>feedbackActive</span><span>{latest ? String(latest.steerFeedbackActive) : '-'}</span></div>

      <div style={sectionTitle}>IMU</div>
      <div style={rowStyle}><span style={labelStyle}>accel_x</span><span>{fmt(latest?.accel_x, 3)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>accel_y</span><span>{fmt(latest?.accel_y, 3)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>gyro_z</span><span>{fmt(latest?.gyro_z, 3)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>roll (°)</span><span>{fmt(latest?.roll, 2)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>pitch (°)</span><span>{fmt(latest?.pitch, 2)}</span></div>
      <div style={rowStyle}><span style={labelStyle}>yaw (°)</span><span>{fmt(latest?.yaw, 2)}</span></div>

      <div style={sectionTitle}>TIRE SLIP (%)</div>
      <div style={rowStyle}><span style={labelStyle}>FL</span><span>{latest?.slip ? (latest.slip[0] * 100).toFixed(1) : '-'}</span></div>
      <div style={rowStyle}><span style={labelStyle}>FR</span><span>{latest?.slip ? (latest.slip[1] * 100).toFixed(1) : '-'}</span></div>
      <div style={rowStyle}><span style={labelStyle}>RL</span><span>{latest?.slip ? (latest.slip[2] * 100).toFixed(1) : '-'}</span></div>
      <div style={rowStyle}><span style={labelStyle}>RR</span><span>{latest?.slip ? (latest.slip[3] * 100).toFixed(1) : '-'}</span></div>

      <div style={sectionTitle}>TIRE LOAD (N)</div>
      <div style={rowStyle}><span style={labelStyle}>FL</span><span>{latest?.tireLoad ? Math.round(latest.tireLoad[0]) : '-'}</span></div>
      <div style={rowStyle}><span style={labelStyle}>FR</span><span>{latest?.tireLoad ? Math.round(latest.tireLoad[1]) : '-'}</span></div>
      <div style={rowStyle}><span style={labelStyle}>RL</span><span>{latest?.tireLoad ? Math.round(latest.tireLoad[2]) : '-'}</span></div>
      <div style={rowStyle}><span style={labelStyle}>RR</span><span>{latest?.tireLoad ? Math.round(latest.tireLoad[3]) : '-'}</span></div>

      <div style={sectionTitle}>SOIL</div>
      <div style={rowStyle}><span style={labelStyle}>type</span><span>{latest?.soilType ?? '-'}</span></div>
      <div style={rowStyle}><span style={labelStyle}>moisture (%)</span><span>{latest?.soilMoisture !== undefined ? Math.round(latest.soilMoisture * 100) : '-'}</span></div>
      </div>
    </>
  )
}
