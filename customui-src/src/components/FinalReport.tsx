import { useCallback, useEffect, useState } from 'react'
import type { JSX } from 'react'
import { useSessionStore } from '../state/sessionStore'

// Font stacks matching TopBar.tsx
const MONO = "'JetBrains Mono', ui-monospace, monospace"
const SANS = "'Geist', system-ui, sans-serif"

export interface FinalReportMetrics {
  finalCoverage: number        // 0..1
  elapsedS: number             // total elapsed sim seconds
  elapsedTo95S: number | null  // sim seconds when 95% was reached, null if not
  plowedM2: number
  totalM2: number
  distance?: number            // cumulative meters traveled
  avgSpeed?: number            // km/h (optional, derived or tracked)
  avgDraft?: number            // kN
  lanes?: number
  turns?: number
  reversals?: number
  lostAreaM2?: number
  deviationM?: number
  penaltyS?: number            // deviation penalty in seconds
  finalTimeS?: number          // final time = elapsedS + penaltyS
  headlandS?: number           // headland time in seconds
  runNumber?: number
  fieldName?: string
  planName?: string
  bestLane?: { index: number; ratio: number }
  trajectory?: Array<{ t: number; x: number; y: number; heading: number; engaged: boolean }>
  fieldBboxM?: { minX: number; maxX: number; minY: number; maxY: number }
  coverageSeries?: Array<{ t: number; coverage: number }>
}

interface FinalReportProps {
  metrics: FinalReportMetrics
  onNewRun: () => void
}

// --- Helpers ---

function fmtMMSS(s: number): string {
  const m = Math.floor(s / 60)
  const sec = Math.floor(s % 60)
  return `${String(m).padStart(2, '0')}:${String(sec).padStart(2, '0')}`
}

// Split centisecond string into main "MM:SS" + suffix ".cs"
function splitMMSScs(s: number): { main: string; suffix: string } {
  const m = Math.floor(s / 60)
  const sec = s % 60
  const secInt = Math.floor(sec)
  const cs = Math.floor((sec - secInt) * 100)
  return {
    main: `${String(m).padStart(2, '0')}:${String(secInt).padStart(2, '0')}`,
    suffix: `.${String(cs).padStart(2, '0')}`,
  }
}


function fmtVal(v: number | undefined | null, decimals = 0): string {
  if (v === undefined || v === null || isNaN(v as number)) return '—'
  return (v as number).toFixed(decimals)
}

// Cardinal spline: generate smooth SVG path from points array
function cardinalSplinePath(pts: [number, number][], tension = 0.5): string {
  if (pts.length < 2) return ''
  if (pts.length === 2) return `M ${pts[0][0].toFixed(1)} ${pts[0][1].toFixed(1)} L ${pts[1][0].toFixed(1)} ${pts[1][1].toFixed(1)}`

  let d = `M ${pts[0][0].toFixed(1)} ${pts[0][1].toFixed(1)}`
  for (let i = 0; i < pts.length - 1; i++) {
    const p0 = pts[Math.max(0, i - 1)]
    const p1 = pts[i]
    const p2 = pts[i + 1]
    const p3 = pts[Math.min(pts.length - 1, i + 2)]
    const cp1x = p1[0] + (p2[0] - p0[0]) * tension / 3
    const cp1y = p1[1] + (p2[1] - p0[1]) * tension / 3
    const cp2x = p2[0] - (p3[0] - p1[0]) * tension / 3
    const cp2y = p2[1] - (p3[1] - p1[1]) * tension / 3
    d += ` C ${cp1x.toFixed(1)} ${cp1y.toFixed(1)}, ${cp2x.toFixed(1)} ${cp2y.toFixed(1)}, ${p2[0].toFixed(1)} ${p2[1].toFixed(1)}`
  }
  return d
}

// --- Colors ---
const C = {
  bg: '#080a0b',
  bgDark: '#0a0d0e',
  card: '#11181c',
  border: '#1e2832',
  border2: '#2d3748',
  text: '#e6edf3',
  muted: '#7a8494',
  muted2: '#4a525c',
  green: '#4ade80',
  amber: '#fbbf24',
  red: '#f87171',
  fieldFill: '#7a5c3a',
  fieldStroke: '#a07248',
  fieldLine: '#5a4530',
}

// --- IsometricFieldView ---
function isoTransform(
  wx: number, wy: number,
  bboxMinX: number, bboxMinY: number, bboxW: number, bboxH: number,
  svgW: number, svgH: number,
): { sx: number; sy: number } {
  const nx = bboxW > 0 ? (wx - bboxMinX) / bboxW : 0.5
  const ny = bboxH > 0 ? (wy - bboxMinY) / bboxH : 0.5
  const isoX = (nx - ny) * 0.5 + 0.5
  const isoY = (nx + ny) * 0.35 + 0.15
  return { sx: isoX * svgW, sy: isoY * svgH }
}

interface IsometricFieldViewProps {
  fieldName: string
  trajectory?: Array<{ t: number; x: number; y: number; heading: number; engaged: boolean }>
  fieldBboxM?: { minX: number; maxX: number; minY: number; maxY: number }
}

function IsometricFieldView({ fieldName, trajectory, fieldBboxM }: IsometricFieldViewProps): JSX.Element {
  const SVG_W = 600
  const SVG_H = 340

  const bboxMinXVal = fieldBboxM?.minX ?? -50
  const bboxMinYVal = fieldBboxM?.minY ?? -50
  const bboxMaxXVal = fieldBboxM?.maxX ?? 50
  const bboxMaxYVal = fieldBboxM?.maxY ?? 50

  const corners = [
    { nx: 0, ny: 0 },
    { nx: 1, ny: 0 },
    { nx: 1, ny: 1 },
    { nx: 0, ny: 1 },
  ]

  const bboxMinX = bboxMinXVal
  const bboxMinY = bboxMinYVal
  const bboxW = bboxMaxXVal - bboxMinXVal
  const bboxH = bboxMaxYVal - bboxMinYVal

  const projCorners = corners.map(({ nx, ny }) => {
    const wx = bboxMinX + nx * bboxW
    const wy = bboxMinY + ny * bboxH
    return isoTransform(wx, wy, bboxMinX, bboxMinY, bboxW, bboxH, SVG_W, SVG_H)
  })

  const polyPoints = projCorners.map(p => `${p.sx.toFixed(1)},${p.sy.toFixed(1)}`).join(' ')

  // Lane lines — parallel furrows across the field
  const laneLines: JSX.Element[] = []
  const laneCount = 16
  for (let i = 1; i < laneCount; i++) {
    const t = i / laneCount
    const leftWorld = { wx: bboxMinX, wy: bboxMinY + t * bboxH }
    const rightWorld = { wx: bboxMinX + bboxW, wy: bboxMinY + t * bboxH }
    const lp = isoTransform(leftWorld.wx, leftWorld.wy, bboxMinX, bboxMinY, bboxW, bboxH, SVG_W, SVG_H)
    const rp = isoTransform(rightWorld.wx, rightWorld.wy, bboxMinX, bboxMinY, bboxW, bboxH, SVG_W, SVG_H)
    laneLines.push(
      <line
        key={`lane-h-${i}`}
        x1={lp.sx.toFixed(1)} y1={lp.sy.toFixed(1)}
        x2={rp.sx.toFixed(1)} y2={rp.sy.toFixed(1)}
        stroke={C.fieldLine} strokeWidth="0.7"
      />
    )
  }
  for (let i = 1; i < laneCount; i++) {
    const t = i / laneCount
    const topWorld = { wx: bboxMinX + t * bboxW, wy: bboxMinY }
    const botWorld = { wx: bboxMinX + t * bboxW, wy: bboxMinY + bboxH }
    const tp = isoTransform(topWorld.wx, topWorld.wy, bboxMinX, bboxMinY, bboxW, bboxH, SVG_W, SVG_H)
    const bp = isoTransform(botWorld.wx, botWorld.wy, bboxMinX, bboxMinY, bboxW, bboxH, SVG_W, SVG_H)
    laneLines.push(
      <line
        key={`lane-v-${i}`}
        x1={tp.sx.toFixed(1)} y1={tp.sy.toFixed(1)}
        x2={bp.sx.toFixed(1)} y2={bp.sy.toFixed(1)}
        stroke={C.fieldLine} strokeWidth="0.7"
      />
    )
  }

  // Trajectory polyline
  let trajPolyline: JSX.Element | null = null
  if (trajectory && trajectory.length >= 2) {
    const pts = trajectory
      .map(pt => {
        const p = isoTransform(pt.x, pt.y, bboxMinX, bboxMinY, bboxW, bboxH, SVG_W, SVG_H)
        return `${p.sx.toFixed(1)},${p.sy.toFixed(1)}`
      })
      .join(' ')
    trajPolyline = (
      <polyline
        points={pts}
        stroke={C.green}
        strokeWidth="1.2"
        fill="none"
        opacity="0.9"
      />
    )
  }

  // Current/last position marker
  let currentDot: JSX.Element | null = null
  if (trajectory && trajectory.length > 0) {
    const last = trajectory[trajectory.length - 1]
    const p = isoTransform(last.x, last.y, bboxMinX, bboxMinY, bboxW, bboxH, SVG_W, SVG_H)
    currentDot = (
      <g>
        {/* Outer ring */}
        <circle cx={p.sx} cy={p.sy} r={8} fill="none" stroke={C.green} strokeWidth="1" opacity="0.3" />
        {/* Inner filled */}
        <circle cx={p.sx} cy={p.sy} r={4} fill={C.green} />
        {/* Crosshair lines */}
        <line x1={p.sx - 12} y1={p.sy} x2={p.sx - 6} y2={p.sy} stroke={C.green} strokeWidth="1" opacity="0.6" />
        <line x1={p.sx + 6} y1={p.sy} x2={p.sx + 12} y2={p.sy} stroke={C.green} strokeWidth="1" opacity="0.6" />
        <line x1={p.sx} y1={p.sy - 12} x2={p.sx} y2={p.sy - 6} stroke={C.green} strokeWidth="1" opacity="0.6" />
        <line x1={p.sx} y1={p.sy + 6} x2={p.sx} y2={p.sy + 12} stroke={C.green} strokeWidth="1" opacity="0.6" />
      </g>
    )
  }

  return (
    <svg
      width="100%"
      viewBox={`0 0 ${SVG_W} ${SVG_H}`}
      style={{ display: 'block', background: C.bgDark, width: '100%', height: '100%' }}
    >
      {/* Field gradient def */}
      <defs>
        <linearGradient id="fieldGrad" x1="0%" y1="0%" x2="0%" y2="100%">
          <stop offset="0%" stopColor="#8a6a44" />
          <stop offset="100%" stopColor="#6b502f" />
        </linearGradient>
      </defs>
      {/* Field parallelogram */}
      <polygon
        points={polyPoints}
        fill="url(#fieldGrad)"
        stroke={C.fieldStroke}
        strokeWidth="1"
      />
      {/* Lane furrow lines */}
      {laneLines}
      {/* Trajectory */}
      {trajPolyline}
      {/* Current position marker */}
      {currentDot}
      {/* Field label bottom-right */}
      <text
        x={SVG_W - 10}
        y={SVG_H - 10}
        fontSize="11"
        fill={C.muted}
        textAnchor="end"
        fontFamily={MONO}
        letterSpacing="1"
      >
        FIELD {fieldName}
      </text>
    </svg>
  )
}

// --- CoverageChart ---
interface CoverageChartProps {
  elapsedS: number
  elapsedTo95S: number | null
  finalCoverage: number
  coverageSeries?: Array<{ t: number; coverage: number }>
}

function CoverageChart({ elapsedS, elapsedTo95S, finalCoverage, coverageSeries }: CoverageChartProps): JSX.Element {
  const W = 340
  const H = 160
  const PAD_L = 34
  const PAD_R = 14
  const PAD_T = 14
  const PAD_B = 28

  const chartW = W - PAD_L - PAD_R
  const chartH = H - PAD_T - PAD_B

  const toX = (t: number) => PAD_L + (elapsedS > 0 ? (t / elapsedS) * chartW : 0)
  const toY = (pct: number) => PAD_T + chartH - (pct / 100) * chartH

  // Build points from real series or approximation
  let points: [number, number][]
  if (coverageSeries && coverageSeries.length >= 2) {
    points = coverageSeries.map(({ t, coverage }) => [t, coverage * 100] as [number, number])
    if (points[points.length - 1][0] < elapsedS) {
      points.push([elapsedS, finalCoverage * 100])
    }
  } else {
    points = [[0, 0]]
    if (elapsedTo95S && elapsedTo95S < elapsedS) {
      points.push([elapsedTo95S * 0.3, 20])
      points.push([elapsedTo95S * 0.7, 70])
      points.push([elapsedTo95S, 95])
    }
    points.push([elapsedS, finalCoverage * 100])
  }

  // Map to SVG coords for cardinal spline
  const svgPoints: [number, number][] = points.map(([t, pct]) => [toX(t), toY(pct)])
  const smoothPath = cardinalSplinePath(svgPoints, 0.5)

  const y95 = toY(95)
  const lastPt = points[points.length - 1]
  const lastX = toX(lastPt[0])
  const lastY = toY(lastPt[1])

  // X-axis: just start and end
  const endLabel = fmtMMSS(elapsedS)

  return (
    <svg width="100%" viewBox={`0 0 ${W} ${H}`} style={{ display: 'block' }}>
      {/* Background grid lines at 0/25/50/75/100% */}
      {[0, 25, 50, 75, 100].map(pct => (
        <line
          key={pct}
          x1={PAD_L} y1={toY(pct).toFixed(1)}
          x2={PAD_L + chartW} y2={toY(pct).toFixed(1)}
          stroke={C.border} strokeWidth="1"
        />
      ))}

      {/* 95% dashed reference line */}
      <line
        x1={PAD_L} y1={y95.toFixed(1)}
        x2={PAD_L + chartW} y2={y95.toFixed(1)}
        stroke={C.green} strokeWidth="1" strokeDasharray="4 4" opacity="0.4"
      />

      {/* Coverage smooth curve */}
      <path d={smoothPath} stroke={C.green} strokeWidth="2" fill="none" />

      {/* NOW marker */}
      <circle cx={lastX} cy={lastY} r={8} fill="none" stroke={C.green} strokeWidth="1" opacity="0.3" />
      <circle cx={lastX} cy={lastY} r={4} fill={C.green} />

      {/* Axes */}
      <line x1={PAD_L} y1={PAD_T} x2={PAD_L} y2={PAD_T + chartH} stroke={C.border2} strokeWidth="1" />
      <line x1={PAD_L} y1={PAD_T + chartH} x2={PAD_L + chartW} y2={PAD_T + chartH} stroke={C.border2} strokeWidth="1" />

      {/* X axis labels: start + end only */}
      <text x={toX(0).toFixed(1)} y={PAD_T + chartH + 12} fontSize="8" fill={C.muted} textAnchor="middle" fontFamily={MONO}>
        00:00
      </text>
      <text x={(PAD_L + chartW).toFixed(1)} y={PAD_T + chartH + 12} fontSize="8" fill={C.muted} textAnchor="middle" fontFamily={MONO}>
        {endLabel}
      </text>

      {/* Y axis labels */}
      {[0, 50, 100].map(pct => (
        <text
          key={pct}
          x={PAD_L - 4}
          y={(toY(pct) + 3).toFixed(1)}
          fontSize="8"
          fill={C.muted}
          textAnchor="end"
          fontFamily={MONO}
        >
          {pct}%
        </text>
      ))}
    </svg>
  )
}

// --- Main component ---
export default function FinalReport({ metrics, onNewRun }: FinalReportProps): JSX.Element {

  const [isNarrow, setIsNarrow] = useState(() =>
    typeof window !== 'undefined' && window.matchMedia('(max-width: 768px)').matches
  )
  useEffect(() => {
    const mq = window.matchMedia('(max-width: 768px)')
    const handler = (e: MediaQueryListEvent) => setIsNarrow(e.matches)
    mq.addEventListener('change', handler)
    return () => mq.removeEventListener('change', handler)
  }, [])

  const teamName = useSessionStore(s => s.teamName)
  const mapList = useSessionStore(s => s.mapList)

  const mapId = metrics.fieldName != null ? Number(metrics.fieldName) : null
  const mapEntry = mapList.find(m => m.id === mapId)
  const displayLabel = mapId != null
    ? `#${mapId}${mapEntry?.label ? ' ' + mapEntry.label : ''}`
    : null

  const coverageRatio = metrics.finalCoverage ?? 0
  const coveragePct = coverageRatio * 100
  const met95 = coverageRatio >= 0.95
  const met80 = coverageRatio >= 0.80
  const fieldName = metrics.fieldName ?? 'ALPHA'
  const planName = metrics.planName ?? 'plan_serpentine_e5'
  const runNumber = metrics.runNumber ?? 1

  const derivedAvgSpeed = metrics.avgSpeed != null
    ? metrics.avgSpeed
    : (metrics.distance != null && metrics.elapsedS > 0)
      ? (metrics.distance / metrics.elapsedS) * 3.6
      : undefined

  const elapsedParts = splitMMSScs(metrics.elapsedS)
  const penaltyParts = splitMMSScs(metrics.penaltyS ?? 0)
  const finalParts = splitMMSScs(metrics.finalTimeS ?? metrics.elapsedS)

  const handleNewRun = useCallback(() => { onNewRun() }, [onNewRun])

  // Hero quote by coverage bracket
  let heroQuote: string
  if (met95) {
    heroQuote = 'Steady serpentine, light on the brakes.'
  } else if (met80) {
    heroQuote = 'Solid coverage — trim the gaps next time.'
  } else {
    heroQuote = "Coverage short — let’s tighten the lanes."
  }


  // Key metrics grid rows
  const kmRow1: Array<{ label: string; value: string; unit: string }> = [
    { label: 'DISTANCE', value: fmtVal(metrics.distance, 1), unit: 'm' },
    { label: 'AVG SPEED', value: fmtVal(derivedAvgSpeed, 2), unit: 'km/h' },
    { label: 'AVG DRAFT', value: fmtVal(metrics.avgDraft, 2), unit: 'kN' },
  ]
  const kmRow2Green: Array<{ label: string; value: string; unit: string }> = [
    { label: 'LANES', value: fmtVal(metrics.lanes), unit: '' },
    { label: 'TURNS', value: fmtVal(metrics.turns), unit: '' },
    { label: 'HEADLAND', value: metrics.headlandS != null ? fmtMMSS(metrics.headlandS) : '—', unit: '' },
  ]
  const kmRow3: Array<{ label: string; value: string; unit: string }> = [
    { label: 'REVERSALS', value: fmtVal(metrics.reversals), unit: '' },
    { label: 'LOST AREA', value: fmtVal(metrics.lostAreaM2, 1), unit: 'm²' },
    { label: 'DEVIATION', value: fmtVal(metrics.deviationM, 2), unit: 'm' },
  ]

  return (
    <div style={{
      position: 'fixed',
      inset: 0,
      zIndex: 1000,
      background: C.bg,
      color: C.text,
      fontFamily: SANS,
      display: 'flex',
      flexDirection: 'column',
      overflow: 'hidden',
    }}>

      {/* ── HEADER — matches TopBar.tsx pattern exactly ── */}
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
        {/* Logo image */}
        <img
          src="./title-card/logo.png"
          alt="SeamOS"
          style={{ height: 28, width: 'auto', flexShrink: 0 }}
          onError={(e) => {
            const target = e.currentTarget as HTMLImageElement
            target.style.display = 'none'
            const next = target.nextSibling as HTMLElement | null
            if (next) next.style.display = 'flex'
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
          <span style={{ color: '#7a8494' }}>summary</span>
          <span style={{ color: '#4a525c', fontSize: 12 }}>{'>'}</span>
          <span style={{ color: '#7a8494' }}>field {fieldName.toLowerCase()}</span>
          {displayLabel && (
            <>
              <span style={{ color: '#4a525c', fontSize: 12 }}>{'>'}</span>
              <span style={{ color: '#7a8494', fontFamily: MONO, fontSize: 13 }}>{displayLabel}</span>
            </>
          )}
          <span style={{ color: '#4a525c', fontSize: 12 }}>{'>'}</span>
          <span style={{ color: '#e6edf3' }}>run {String(runNumber).padStart(2, '0')}</span>
        </div>

        {/* Spacer */}
        <div style={{ flex: 1, minWidth: 0 }} />

        {/* Team name chip */}
        <span style={{
          fontSize: 12,
          fontWeight: 500,
          color: C.muted,
          background: C.card,
          border: `1px solid ${C.border}`,
          borderRadius: 4,
          padding: '4px 10px',
          fontFamily: SANS,
          whiteSpace: 'nowrap',
          flexShrink: 0,
        }}>
          Team: {teamName || '—'}
        </span>
      </div>

      {/* ── SCROLLABLE BODY ── */}
      <div style={{ flex: 1, overflowY: 'auto', padding: '24px 32px', display: 'flex', flexDirection: 'column', gap: 20 }}>

        {/* ── HERO ── */}
        <div style={{
          display: 'flex',
          flexDirection: isNarrow ? 'column' : 'row',
          justifyContent: 'space-between',
          alignItems: 'flex-start',
          gap: 24,
        }}>
          {/* Left: chip + headline + subtitle */}
          <div style={{ flex: 1, minWidth: 0 }}>

            {/* Mission chip — icon + status only */}
            <div style={{
              display: 'inline-flex',
              alignItems: 'center',
              gap: 6,
              background: met95 ? 'rgba(74,222,128,0.08)' : 'rgba(248,113,113,0.08)',
              border: `1px solid ${met95 ? 'rgba(74,222,128,0.3)' : 'rgba(248,113,113,0.3)'}`,
              borderRadius: 2,
              padding: '4px 10px',
              fontSize: 10,
              fontWeight: 600,
              letterSpacing: '0.1em',
              color: met95 ? C.green : C.red,
              marginBottom: 12,
              fontFamily: SANS,
            }}>
              {/* Check / X icon */}
              {met95 ? (
                <svg width="10" height="10" viewBox="0 0 10 10" fill="none" style={{ flexShrink: 0 }}>
                  <polyline points="1.5,5.5 4,8 8.5,2" stroke={C.green} strokeWidth="1.5" strokeLinecap="round" strokeLinejoin="round" />
                </svg>
              ) : (
                <svg width="10" height="10" viewBox="0 0 10 10" fill="none" style={{ flexShrink: 0 }}>
                  <line x1="2" y1="2" x2="8" y2="8" stroke={C.red} strokeWidth="1.5" strokeLinecap="round" />
                  <line x1="8" y1="2" x2="2" y2="8" stroke={C.red} strokeWidth="1.5" strokeLinecap="round" />
                </svg>
              )}
              <span>MISSION {met95 ? 'COMPLETE' : 'INCOMPLETE'}</span>
            </div>

            {/* Hero quote */}
            <div style={{
              fontSize: isNarrow ? 28 : 38,
              fontWeight: 200,
              color: C.text,
              lineHeight: 1.15,
              marginBottom: 8,
              fontFamily: SANS,
            }}>
              &ldquo;{heroQuote}&rdquo;
            </div>

            {/* Subtitle */}
            <div style={{ fontSize: 12, color: C.muted, fontFamily: SANS }}>
              Run #{runNumber} &middot; {planName} &middot; Field {fieldName.toUpperCase()}
            </div>
          </div>

          {/* Right: 2 inline KPI columns — no card backgrounds */}
          <div style={{
            display: 'flex',
            gap: isNarrow ? 24 : 52,
            flexShrink: 0,
            alignItems: 'flex-start',
            paddingTop: 4,
            fontVariantNumeric: 'tabular-nums',
          }}>
            {/* FINAL COVERAGE */}
            <div>
              <div style={{
                fontSize: 10,
                letterSpacing: '0.14em',
                color: '#e6edf3',
                textTransform: 'uppercase',
                marginBottom: 4,
                fontFamily: SANS,
              }}>
                FINAL COVERAGE
              </div>
              <div style={{ lineHeight: 1, display: 'flex', alignItems: 'baseline' }}>
                <span style={{
                  fontSize: isNarrow ? 38 : 56,
                  fontWeight: 400,
                  color: met95 ? C.green : (met80 ? C.amber : C.red),
                  fontFamily: SANS,
                  letterSpacing: '-0.02em',
                  fontVariantNumeric: 'tabular-nums',
                }}>
                  {coveragePct.toFixed(1)}
                </span>
                <span style={{
                  fontSize: 26,
                  fontWeight: 400,
                  color: met95 ? C.green : (met80 ? C.amber : C.red),
                  marginLeft: 4,
                  fontFamily: SANS,
                }}>%</span>
              </div>
            </div>

            {/* ELAPSED — 3-row: RAW / PENALTY / FINAL */}
            <div style={{ display: 'flex', flexDirection: 'column', gap: 6 }}>

              {/* Row 1: RAW ELAPSED */}
              <div>
                <div style={{
                  fontSize: 10,
                  letterSpacing: '0.14em',
                  color: C.muted,
                  textTransform: 'uppercase',
                  marginBottom: 2,
                  fontFamily: SANS,
                }}>
                  RAW ELAPSED
                </div>
                <div style={{ lineHeight: 1, display: 'flex', alignItems: 'baseline' }}>
                  <span style={{
                    fontSize: 28,
                    fontWeight: 400,
                    color: C.text,
                    fontFamily: SANS,
                    letterSpacing: '-0.02em',
                    fontVariantNumeric: 'tabular-nums',
                  }}>
                    {elapsedParts.main}
                  </span>
                  <span style={{
                    fontSize: 16,
                    fontWeight: 400,
                    color: C.muted,
                    marginLeft: 2,
                    fontFamily: SANS,
                  }}>
                    {elapsedParts.suffix}
                  </span>
                </div>
              </div>

              {/* Row 2: DEVIATION PENALTY */}
              <div>
                <div style={{
                  fontSize: 10,
                  letterSpacing: '0.14em',
                  color: '#f85149',
                  textTransform: 'uppercase',
                  marginBottom: 2,
                  fontFamily: SANS,
                }}>
                  DEVIATION PENALTY
                </div>
                <div style={{ lineHeight: 1, display: 'flex', alignItems: 'baseline' }}>
                  <span style={{
                    fontSize: 28,
                    fontWeight: 400,
                    color: '#f85149',
                    fontFamily: SANS,
                    letterSpacing: '-0.02em',
                    fontVariantNumeric: 'tabular-nums',
                  }}>
                    +{penaltyParts.main}
                  </span>
                  <span style={{
                    fontSize: 16,
                    fontWeight: 400,
                    color: '#f85149',
                    marginLeft: 2,
                    fontFamily: SANS,
                  }}>
                    {penaltyParts.suffix}
                  </span>
                </div>
              </div>

              {/* Row 3: FINAL TIME */}
              <div>
                <div style={{
                  fontSize: 10,
                  letterSpacing: '0.14em',
                  color: '#e6edf3',
                  textTransform: 'uppercase',
                  marginBottom: 2,
                  fontFamily: SANS,
                }}>
                  FINAL TIME
                </div>
                <div style={{ lineHeight: 1, display: 'flex', alignItems: 'baseline' }}>
                  <span style={{
                    fontSize: isNarrow ? 38 : 56,
                    fontWeight: 400,
                    color: C.text,
                    fontFamily: SANS,
                    letterSpacing: '-0.02em',
                    fontVariantNumeric: 'tabular-nums',
                  }}>
                    {finalParts.main}
                  </span>
                  <span style={{
                    fontSize: 26,
                    fontWeight: 400,
                    color: C.muted,
                    marginLeft: 2,
                    fontFamily: SANS,
                  }}>
                    {finalParts.suffix}
                  </span>
                </div>
              </div>

            </div>
          </div>
        </div>

        {/* ── 2-COLUMN MAIN ── */}
        <div style={{ display: 'grid', gridTemplateColumns: isNarrow ? '1fr' : '3fr 2fr', gap: 14 }}>

          {/* ── LEFT: REPLAY card ── */}
          <div style={{
            background: C.card,
            borderRadius: 8,
            border: `1px solid ${C.border}`,
            padding: '14px',
            display: 'flex',
            flexDirection: 'column',
            gap: 10,
            height: '100%',
            boxSizing: 'border-box',
          }}>
            {/* Header row */}
            <div style={{ display: 'flex', alignItems: 'center' }}>
              <span style={{ fontSize: 14, fontWeight: 700, letterSpacing: '0.14em', color: '#e6edf3', textTransform: 'uppercase', fontFamily: SANS }}>
                REPLAY
              </span>
            </div>

            {/* Viewport */}
            <div style={{
              background: C.bgDark,
              borderRadius: 4,
              overflow: 'hidden',
              position: 'relative',
              flex: 1,
              minHeight: 0,
            }}>
              <IsometricFieldView
                fieldName={fieldName.toUpperCase()}
                trajectory={metrics.trajectory}
                fieldBboxM={metrics.fieldBboxM}
              />
            </div>

          </div>

          {/* ── RIGHT column ── */}
          <div style={{ display: 'flex', flexDirection: 'column', gap: 14, height: '100%' }}>

            {/* KEY METRICS card */}
            <div style={{
              background: C.card,
              borderRadius: 8,
              border: `1px solid ${C.border}`,
              padding: '22px',
              flex: '0 0 auto',
            }}>
              <div style={{
                fontSize: 14,
                fontWeight: 700,
                letterSpacing: '0.14em',
                color: '#e6edf3',
                marginBottom: 14,
                textTransform: 'uppercase',
                fontFamily: SANS,
              }}>
                KEY METRICS
              </div>
              {/* 3x3 grid */}
              <div style={{ display: 'grid', gridTemplateColumns: isNarrow ? 'repeat(2, 1fr)' : 'repeat(3, 1fr)', gap: 22 }}>
                {/* Row 1: white values */}
                {kmRow1.map(({ label, value, unit }) => (
                  <div key={label} style={{ padding: 0 }}>
                    <div style={{ fontSize: 10, letterSpacing: '0.1em', color: '#e6edf3', marginBottom: 3, textTransform: 'uppercase', fontFamily: SANS }}>
                      {label}
                    </div>
                    <div style={{ fontSize: 28, fontWeight: 300, color: value === '—' ? C.muted2 : C.text, lineHeight: 1, fontVariantNumeric: 'tabular-nums', fontFamily: SANS }}>
                      {value}
                      {unit && value !== '—' && (
                        <span style={{ fontSize: 11, color: C.muted, marginLeft: 4 }}>{unit}</span>
                      )}
                    </div>
                  </div>
                ))}
                {/* Row 2: green values */}
                {kmRow2Green.map(({ label, value, unit }) => (
                  <div key={label} style={{ padding: 0 }}>
                    <div style={{ fontSize: 10, letterSpacing: '0.1em', color: '#e6edf3', marginBottom: 3, textTransform: 'uppercase', fontFamily: SANS }}>
                      {label}
                    </div>
                    <div style={{ fontSize: 28, fontWeight: 300, color: value === '—' ? C.muted2 : C.green, lineHeight: 1, fontVariantNumeric: 'tabular-nums', fontFamily: SANS }}>
                      {value}
                      {unit && value !== '—' && (
                        <span style={{ fontSize: 11, color: C.muted, marginLeft: 4 }}>{unit}</span>
                      )}
                    </div>
                  </div>
                ))}
                {/* Row 3: white values */}
                {kmRow3.map(({ label, value, unit }) => (
                  <div key={label} style={{ padding: 0 }}>
                    <div style={{ fontSize: 10, letterSpacing: '0.1em', color: '#e6edf3', marginBottom: 3, textTransform: 'uppercase', fontFamily: SANS }}>
                      {label}
                    </div>
                    <div style={{ fontSize: 28, fontWeight: 300, color: value === '—' ? C.muted2 : C.text, lineHeight: 1, fontVariantNumeric: 'tabular-nums', fontFamily: SANS }}>
                      {value}
                      {unit && value !== '—' && (
                        <span style={{ fontSize: 11, color: C.muted, marginLeft: 4 }}>{unit}</span>
                      )}
                    </div>
                  </div>
                ))}
              </div>
            </div>

            {/* COVERAGE vs TIME card */}
            <div style={{
              background: C.card,
              borderRadius: 8,
              border: `1px solid ${C.border}`,
              padding: '22px',
              flex: 1,
              display: 'flex',
              flexDirection: 'column',
              minHeight: 0,
              boxSizing: 'border-box',
            }}>
              <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: 10 }}>
                <span style={{ fontSize: 14, fontWeight: 700, letterSpacing: '0.14em', color: '#e6edf3', textTransform: 'uppercase', fontFamily: SANS }}>
                  COVERAGE vs TIME
                </span>
                <span style={{ fontSize: 10, color: C.muted2, fontFamily: SANS }}>(95% line)</span>
              </div>
              <div style={{ flex: 1, minHeight: 0, display: 'flex', flexDirection: 'column' }}>
              <CoverageChart
                elapsedS={metrics.elapsedS}
                elapsedTo95S={metrics.elapsedTo95S}
                finalCoverage={coverageRatio}
                coverageSeries={metrics.coverageSeries}
              />
              {/* Legend */}
              <div style={{ display: 'flex', gap: 16, marginTop: 8 }}>
                <div style={{ display: 'flex', alignItems: 'center', gap: 5 }}>
                  <svg width="12" height="8" style={{ display: 'block', flexShrink: 0 }}>
                    <circle cx="4" cy="4" r="4" fill={C.green} />
                  </svg>
                  <span style={{ fontSize: 10, color: C.muted, fontFamily: SANS }}>coverage</span>
                </div>
                <div style={{ display: 'flex', alignItems: 'center', gap: 5 }}>
                  <svg width="14" height="4" style={{ display: 'block', flexShrink: 0 }}>
                    <line x1="0" y1="2" x2="14" y2="2" stroke={C.green} strokeWidth="1.5" strokeDasharray="3 2" opacity="0.5" />
                  </svg>
                  <span style={{ fontSize: 10, color: C.muted, fontFamily: SANS }}>95% target</span>
                </div>
              </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      {/* ── FOOTER ── */}
      <div style={{
        minHeight: 56,
        borderTop: `1px solid ${C.border}`,
        display: 'flex',
        alignItems: 'center',
        padding: '0 24px',
        justifyContent: 'flex-end',
        gap: 16,
        flexShrink: 0,
        background: C.bg,
      }}>
        {/* Score status */}
        <span style={{
          fontSize: 12,
          color: C.muted,
          fontFamily: SANS,
          flex: 1,
        }}>
          Score submitted via live API
        </span>

        {/* Right: action buttons */}
        <div style={{ display: 'flex', gap: 10, flexShrink: 0 }}>
          <button
            onClick={handleNewRun}
            style={{
              padding: '12px 26px',
              fontSize: 15,
              fontWeight: 500,
              background: 'transparent',
              border: `1px solid ${C.border2}`,
              borderRadius: 6,
              color: C.text,
              cursor: 'pointer',
              whiteSpace: 'nowrap',
              fontFamily: SANS,
            }}
          >
            New run
          </button>
        </div>
      </div>
    </div>
  )
}
