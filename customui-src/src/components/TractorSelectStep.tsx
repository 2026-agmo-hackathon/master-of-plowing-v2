import React, { useState, useEffect, useRef } from 'react';
import { useSessionStore } from '../state/sessionStore';

// ── Types ─────────────────────────────────────────────────────────────────────
type TractorKey    = '50hp' | '120hp' | '200hp';
type ImplementKey  = 'plow_2.3m' | 'plow_3.5m' | 'plow_5.5m';

export interface TractorSelectStepProps {
  onBack: () => void;
  onConfirm: (tractorKey: TractorKey, implementKey: ImplementKey) => void;
}

// ── Option data ───────────────────────────────────────────────────────────────
type TractorOption = {
  tractorKey: TractorKey;
  implementKey: ImplementKey;
  label: string;
  hp: 50 | 120 | 200;
  plowWidth: 2.3 | 3.5 | 5.5;
  turnRadius: number;
  mass: number;
  lanesAlpha: number;
  description: string;
  strategyNote: string;
};

const OPTIONS: TractorOption[] = [
  {
    tractorKey: '50hp',
    implementKey: 'plow_2.3m',
    label: 'COMPACT',
    hp: 50,
    plowWidth: 2.3,
    turnRadius: 2.4,
    mass: 2.8,
    lanesAlpha: 12,
    description: 'Nimble and agile. Small turn radius suits fields with tight corners and short headlands.',
    strategyNote: 'Tight corners are easy, but you\'ll plow more passes. Best for irregular boundaries and tight headlands.',
  },
  {
    tractorKey: '120hp',
    implementKey: 'plow_3.5m',
    label: 'MID-SIZE',
    hp: 120,
    plowWidth: 3.5,
    turnRadius: 3.6,
    mass: 5.4,
    lanesAlpha: 7,
    description: 'The all-rounder. Balanced power and a moderate plow width — recommended for the official Alpha course.',
    strategyNote: 'The default pick. Plow width and turn radius are balanced — most baseline serpentine implementations will work without much tuning.',
  },
  {
    tractorKey: '200hp',
    implementKey: 'plow_5.5m',
    label: 'HEAVY',
    hp: 200,
    plowWidth: 5.5,
    turnRadius: 5.4,
    mass: 8.6,
    lanesAlpha: 5,
    description: 'Brute force. Wider plow trims total passes, but wide turns penalize irregular fields.',
    strategyNote: 'Fewer passes, but wide turns. Best on long straight lanes with simple boundary geometry.',
  },
];

const DEFAULT_KEY: TractorKey = '120hp';

// ── Inline styles ─────────────────────────────────────────────────────────────
const STYLES = `
  @keyframes ts-fadeIn {
    from { opacity: 0; transform: translateY(6px); }
    to   { opacity: 1; transform: translateY(0); }
  }

  .ts-card {
    display: flex;
    flex-direction: column;
    background: rgba(7,14,11,0.6);
    border: 1px solid transparent;
    outline: 1px solid #243a32;
    cursor: pointer;
    position: relative;
    transition: outline-color 160ms ease, box-shadow 160ms ease, transform 160ms ease, background 160ms ease;
    overflow: hidden;
    min-height: 0;
    border-radius: 2px;
  }
  .ts-card:hover:not(.ts-card--selected) {
    outline-color: rgba(0,223,143,0.35);
    transform: translateY(-2px);
  }
  .ts-card:focus-visible {
    outline: 2px solid #00DF8F;
    outline-offset: 3px;
  }
  .ts-card--selected {
    outline: 2px solid #00DF8F;
    box-shadow: 0 0 36px rgba(0,223,143,0.28);
    background: rgba(0,223,143,0.04);
  }

  .ts-launch-btn {
    transition: transform 160ms ease, box-shadow 160ms ease;
  }
  .ts-launch-btn:hover:not(:disabled) {
    transform: translateY(-1px);
    box-shadow: 0 12px 32px 0 rgba(0,223,143,0.36);
  }
  .ts-launch-btn:focus-visible {
    outline: 2px solid #00DF8F;
    outline-offset: 4px;
  }
  .ts-launch-btn:disabled {
    opacity: 0.4;
    cursor: not-allowed;
  }

  .ts-spec-cell {
    background: #0b1410;
    border: 1px solid #243a32;
    padding: 10px 14px;
    display: flex;
    flex-direction: column;
    gap: 4px;
  }

  @media (max-width: 1100px) {
    .ts-main-layout {
      flex-direction: column !important;
    }
    .ts-right-panel {
      flex: none !important;
      width: 100% !important;
      max-height: none !important;
      overflow-y: visible !important;
    }
    .ts-cards-area {
      flex-direction: row !important;
      flex-wrap: wrap !important;
      height: auto !important;
    }
    .ts-card {
      flex: 1 1 220px !important;
      min-height: 420px;
    }
  }

  .ts-spec-row-value {
    display: inline-flex;
    align-items: baseline;
    gap: 4px;
  }

  @media (prefers-reduced-motion: reduce) {
    .ts-card, .ts-launch-btn { transition: none !important; }
  }
`;

// ── Top-down solid-filled SVG art for tractor + plow ──────────────────────────
function TractorTopDown({ hp, plowWidth, size = 'full' }: {
  hp: 50 | 120 | 200;
  plowWidth: 2.3 | 3.5 | 5.5;
  size?: 'full' | 'mini';
}) {
  const BODY_FILL  = '#10b876';
  const BODY_STR   = '#1a3d2c';
  const CABIN_FILL = '#0a5a3a';
  const CABIN_STR  = '#10b876';
  const WHEEL_FILL = '#1a2620';
  const WHEEL_STR  = '#2a3d35';
  const HITCH_STR  = '#1a3d2c';
  const ARROW_CLR  = '#8a9a90';
  const BROWN      = '#6b4c2a';

  // Tractor body size — kept small so it occupies ~30-40% of the card height
  const bodyW = hp === 50 ? 32 : hp === 120 ? 42 : 52;
  const bodyH = hp === 50 ? 44 : hp === 120 ? 58 : 70;

  // Rear wheel dimensions (large side wheels)
  const wheelW = hp === 50 ? 9 : hp === 120 ? 12 : 15;
  const wheelH = hp === 50 ? 24 : hp === 120 ? 30 : 36;

  // Front/small wheel dimensions
  const fWheelW = hp === 50 ? 6 : hp === 120 ? 8 : 10;
  const fWheelH = hp === 50 ? 14 : hp === 120 ? 18 : 22;

  // Plow width in SVG units — compact slightly narrower, mid same, heavy wider
  const plowSvgW = plowWidth === 2.3 ? bodyW * 1.6 : plowWidth === 3.5 ? bodyW * 2.2 : bodyW * 3.0;
  const plowH    = size === 'mini' ? 9 : 11;
  const toothCount = plowWidth === 2.3 ? 6 : plowWidth === 3.5 ? 10 : 16;

  // SVG canvas — sized so illustration fills without being too large
  const svgW = size === 'mini' ? 120 : Math.max(180, plowSvgW + 40);
  const svgH = size === 'mini' ? 160 : 220;
  const cx   = svgW / 2;

  // Vertical layout
  const arrowBotY = size === 'mini' ? 14 : 18;
  const bodyTopY  = arrowBotY + (size === 'mini' ? 8 : 10);
  const bodyBotY  = bodyTopY + bodyH;

  // Wheel Y positions
  const rWheelTopY = bodyTopY + bodyH * 0.4;
  const fWheelTopY = bodyTopY + bodyH * 0.05;

  // Plow
  const plowGapY = size === 'mini' ? 5 : 7;
  const plowTopY = bodyBotY + plowGapY;
  const plowBotY = plowTopY + plowH;

  // Width label
  const labelY = plowBotY + (size === 'mini' ? 8 : 11);

  // Cabin window — centered inside body, lower portion
  const cabinInset = 4;
  const cabinTopY  = bodyTopY + bodyH * 0.42;
  const cabinH     = bodyH * 0.48;

  // Plow teeth
  const teeth: React.ReactNode[] = [];
  const toothW = plowSvgW / toothCount;
  const toothH = size === 'mini' ? 5 : 7;
  for (let i = 0; i < toothCount; i++) {
    const tx = cx - plowSvgW / 2 + i * toothW;
    teeth.push(
      <polygon
        key={`t${i}`}
        points={`${tx},${plowBotY} ${tx + toothW * 0.9},${plowBotY} ${tx + toothW * 0.45},${plowBotY + toothH}`}
        fill={BROWN}
      />
    );
  }

  // Measurement line
  const mlY  = labelY - 2;
  const mlX1 = cx - plowSvgW / 2;
  const mlX2 = cx + plowSvgW / 2;

  return (
    <svg
      viewBox={`0 0 ${svgW} ${svgH}`}
      width="100%"
      height="100%"
      preserveAspectRatio="xMidYMid meet"
      xmlns="http://www.w3.org/2000/svg"
      aria-hidden="true"
    >
      {/* Direction arrow — small */}
      <text
        x={cx}
        y={arrowBotY}
        textAnchor="middle"
        fill={ARROW_CLR}
        fontSize={size === 'mini' ? 10 : 12}
        fontFamily="system-ui, sans-serif"
      >
        ↑
      </text>

      {/* Rear wheels — solid dark fill */}
      <rect
        x={cx - bodyW / 2 - wheelW}
        y={rWheelTopY}
        width={wheelW}
        height={wheelH}
        rx={2}
        fill={WHEEL_FILL}
        stroke={WHEEL_STR}
        strokeWidth={1}
      />
      <rect
        x={cx + bodyW / 2}
        y={rWheelTopY}
        width={wheelW}
        height={wheelH}
        rx={2}
        fill={WHEEL_FILL}
        stroke={WHEEL_STR}
        strokeWidth={1}
      />

      {/* Front wheels — solid dark fill */}
      <rect
        x={cx - bodyW / 2 - fWheelW}
        y={fWheelTopY}
        width={fWheelW}
        height={fWheelH}
        rx={1}
        fill={WHEEL_FILL}
        stroke={WHEEL_STR}
        strokeWidth={1}
      />
      <rect
        x={cx + bodyW / 2}
        y={fWheelTopY}
        width={fWheelW}
        height={fWheelH}
        rx={1}
        fill={WHEEL_FILL}
        stroke={WHEEL_STR}
        strokeWidth={1}
      />

      {/* Tractor main body — solid bright green fill */}
      <rect
        x={cx - bodyW / 2}
        y={bodyTopY}
        width={bodyW}
        height={bodyH}
        rx={2}
        fill={BODY_FILL}
        stroke={BODY_STR}
        strokeWidth={1}
      />

      {/* Cabin window — darker green fill inside body */}
      <rect
        x={cx - bodyW / 2 + cabinInset}
        y={cabinTopY}
        width={bodyW - cabinInset * 2}
        height={cabinH}
        rx={1}
        fill={CABIN_FILL}
        stroke={CABIN_STR}
        strokeWidth={1}
      />

      {/* Connector hitch */}
      <line
        x1={cx}
        y1={bodyBotY}
        x2={cx}
        y2={plowTopY}
        stroke={HITCH_STR}
        strokeWidth={2}
      />

      {/* Plow bar */}
      <rect
        x={cx - plowSvgW / 2}
        y={plowTopY}
        width={plowSvgW}
        height={plowH}
        fill={BROWN}
        stroke="#8b5e30"
        strokeWidth={1}
      />

      {/* Plow teeth */}
      {teeth}

      {/* Width measurement line */}
      <line x1={mlX1} y1={mlY} x2={mlX2} y2={mlY} stroke={ARROW_CLR} strokeWidth={0.8} opacity={0.7} />
      <line x1={mlX1} y1={mlY - 3} x2={mlX1} y2={mlY + 3} stroke={ARROW_CLR} strokeWidth={0.8} opacity={0.7} />
      <line x1={mlX2} y1={mlY - 3} x2={mlX2} y2={mlY + 3} stroke={ARROW_CLR} strokeWidth={0.8} opacity={0.7} />

      {/* Width label */}
      <text
        x={cx}
        y={labelY + (size === 'mini' ? 8 : 10)}
        textAnchor="middle"
        fill={ARROW_CLR}
        fontSize={size === 'mini' ? 8 : 10}
        fontFamily="'JetBrains Mono', ui-monospace, monospace"
      >
        {plowWidth.toFixed(1)} m
      </text>
    </svg>
  );
}

// ── Subtle blueprint grid (subtle version for card backgrounds) ───────────────
function SubtleGrid() {
  return (
    <svg
      style={{
        position: 'absolute',
        inset: 0,
        width: '100%',
        height: '100%',
        pointerEvents: 'none',
        opacity: 0.04,
      }}
      xmlns="http://www.w3.org/2000/svg"
      aria-hidden="true"
    >
      <defs>
        <pattern id="ts-smallGrid" width="20" height="20" patternUnits="userSpaceOnUse">
          <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#00DF8F" strokeWidth="0.5" />
        </pattern>
        <pattern id="ts-bigGrid" width="100" height="100" patternUnits="userSpaceOnUse">
          <rect width="100" height="100" fill="url(#ts-smallGrid)" />
          <path d="M 100 0 L 0 0 0 100" fill="none" stroke="#00DF8F" strokeWidth="1" />
        </pattern>
      </defs>
      <rect width="100%" height="100%" fill="url(#ts-bigGrid)" />
    </svg>
  );
}

// ── Connection status pill ────────────────────────────────────────────────────
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

// ── Main component ────────────────────────────────────────────────────────────
export default function TractorSelectStep({ onBack, onConfirm }: TractorSelectStepProps) {
  const tractorPreset  = useSessionStore(s => s.tractorPreset);
  const implementPreset = useSessionStore(s => s.implementPreset);
  const wsConnected    = useSessionStore(s => s.wsConnected);

  // Derive initial selection from store, fallback to MID-SIZE
  const resolveInitial = (): TractorKey => {
    const fromStore = OPTIONS.find(o => o.tractorKey === tractorPreset && o.implementKey === implementPreset);
    return fromStore ? fromStore.tractorKey : DEFAULT_KEY;
  };

  const [selectedKey, setSelectedKey] = useState<TractorKey>(resolveInitial);
  const cardRefs = useRef<Array<HTMLButtonElement | null>>([]);

  const selectedOpt = OPTIONS.find(o => o.tractorKey === selectedKey) ?? OPTIONS[1];
  const selectedIdx = OPTIONS.findIndex(o => o.tractorKey === selectedKey);

  // Keyboard navigation
  useEffect(() => {
    const handler = (e: KeyboardEvent) => {
      if (e.key === 'Escape') {
        e.preventDefault();
        onBack();
        return;
      }
      if (e.key === 'ArrowRight') {
        e.preventDefault();
        const next = (selectedIdx + 1) % OPTIONS.length;
        setSelectedKey(OPTIONS[next].tractorKey);
        cardRefs.current[next]?.focus();
      } else if (e.key === 'ArrowLeft') {
        e.preventDefault();
        const prev = selectedIdx === 0 ? OPTIONS.length - 1 : selectedIdx - 1;
        setSelectedKey(OPTIONS[prev].tractorKey);
        cardRefs.current[prev]?.focus();
      }
    };
    window.addEventListener('keydown', handler);
    return () => window.removeEventListener('keydown', handler);
  }, [selectedIdx, onBack]);

  // Focus first card on mount
  useEffect(() => {
    cardRefs.current[selectedIdx]?.focus();
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  const handleConfirm = () => {
    if (!wsConnected) return;
    onConfirm(selectedOpt.tractorKey, selectedOpt.implementKey);
  };

  const MONO = "'JetBrains Mono', ui-monospace, monospace";
  const SANS = "'Geist', system-ui, sans-serif";

  return (
    <div
      style={{
        position: 'relative',
        width: '100vw',
        height: '100vh',
        background: 'radial-gradient(ellipse at 40% 60%, #0d1117 0%, #060a08 100%)',
        fontFamily: SANS,
        overflowX: 'hidden',
        overflowY: 'auto',
        display: 'flex',
        flexDirection: 'column',
        animation: 'ts-fadeIn 250ms ease both',
      }}
    >
      <style>{STYLES}</style>

      {/* ── Top bar ───────────────────────────────────────────────────── */}
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

      {/* ── Header block ─────────────────────────────────────────────── */}
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
          Step 02 / 03 · Select tractor
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
          Pick your tractor and implement
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
          Each spec trades horsepower for plow width and turn radius. The right choice depends on your field shape and algorithm strategy.
        </p>
      </div>

      {/* ── Main area ─────────────────────────────────────────────────── */}
      <div
        className="ts-main-layout"
        style={{
          flex: 1,
          display: 'flex',
          flexDirection: 'row',
          gap: 0,
          overflow: 'hidden',
          minHeight: 0,
        }}
      >
        {/* Cards area (3 columns) */}
        <div
          className="ts-cards-area"
          style={{
            flex: 1,
            display: 'flex',
            flexDirection: 'row',
            gap: 24,
            padding: '24px',
            overflow: 'hidden',
            minHeight: 0,
          }}
          role="radiogroup"
          aria-label="Tractor selection"
        >
          {OPTIONS.map((opt, i) => {
            const isSelected = opt.tractorKey === selectedKey;
            return (
              <button
                key={opt.tractorKey}
                ref={el => { cardRefs.current[i] = el; }}
                type="button"
                role="radio"
                aria-checked={isSelected}
                aria-label={`Select ${opt.label}: ${opt.hp} HP, ${opt.plowWidth}m plow`}
                className={`ts-card${isSelected ? ' ts-card--selected' : ''}`}
                tabIndex={0}
                onClick={() => setSelectedKey(opt.tractorKey)}
                style={{
                  flex: 1,
                }}
              >
                {/* Subtle grid background */}
                <SubtleGrid />

                {/* Selected badge */}
                {isSelected && (
                  <div
                    style={{
                      position: 'absolute',
                      top: 14,
                      right: 14,
                      zIndex: 2,
                      display: 'flex',
                      alignItems: 'center',
                      gap: 5,
                      background: '#00DF8F',
                      color: '#051208',
                      fontSize: 10,
                      fontFamily: MONO,
                      fontWeight: 700,
                      letterSpacing: '1px',
                      padding: '4px 10px',
                      borderRadius: 20,
                    }}
                  >
                    <svg width="10" height="10" viewBox="0 0 10 10" aria-hidden="true">
                      <path d="M2 5 L4 7.5 L8 3" stroke="#051208" strokeWidth="1.8" fill="none" strokeLinecap="round" strokeLinejoin="round" />
                    </svg>
                    SELECTED
                  </div>
                )}

                {/* Art area */}
                <div
                  style={{
                    flex: 1,
                    display: 'flex',
                    alignItems: 'center',
                    justifyContent: 'center',
                    padding: '16px 16px 8px',
                    minHeight: 0,
                    position: 'relative',
                    zIndex: 1,
                    overflow: 'hidden',
                  }}
                >
                  {/* Width constrained so heavy plow still fills more of the card */}
                  <div style={{ width: '80%', maxWidth: 200, height: '100%', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                    <TractorTopDown hp={opt.hp} plowWidth={opt.plowWidth} size="full" />
                  </div>
                </div>

                {/* Footer */}
                <div
                  style={{
                    flexShrink: 0,
                    padding: '16px 20px 20px',
                    borderTop: `1px solid ${isSelected ? 'rgba(0,223,143,0.3)' : '#243a32'}`,
                    textAlign: 'left',
                    position: 'relative',
                    zIndex: 1,
                  }}
                >
                  <div
                    style={{
                      fontFamily: SANS,
                      fontSize: 'clamp(28px, 3vw, 38px)',
                      fontWeight: 700,
                      color: isSelected ? '#00DF8F' : '#e6edf3',
                      letterSpacing: '-0.01em',
                      marginBottom: 14,
                      lineHeight: 1.1,
                    }}
                  >
                    {opt.label}
                  </div>

                  {/* Spec rows */}
                  {[
                    { label: 'HP',   value: String(opt.hp),              unit: '',  green: isSelected },
                    { label: 'PLOW', value: opt.plowWidth.toFixed(1),    unit: 'm', green: isSelected },
                    { label: 'TURN', value: opt.turnRadius.toFixed(1),   unit: 'm', green: isSelected },
                  ].map(row => (
                    <div
                      key={row.label}
                      style={{
                        display: 'flex',
                        justifyContent: 'space-between',
                        alignItems: 'flex-start',
                        paddingTop: 8,
                        paddingBottom: 4,
                        borderTop: '1px solid #243a32',
                        marginTop: 0,
                      }}
                    >
                      <span
                        style={{
                          fontFamily: SANS,
                          fontSize: 13,
                          fontWeight: 400,
                          color: '#9aa3a0',
                          letterSpacing: '0.18em',
                          textTransform: 'uppercase',
                          paddingTop: 4,
                        }}
                      >
                        {row.label}
                      </span>
                      <div className="ts-spec-row-value">
                        <span
                          style={{
                            fontFamily: SANS,
                            fontSize: 'clamp(22px, 2vw, 28px)',
                            fontWeight: 600,
                            color: row.green ? '#00DF8F' : '#e6edf3',
                            lineHeight: 1.1,
                          }}
                        >
                          {row.value}
                        </span>
                        {row.unit && (
                          <span
                            style={{
                              fontFamily: SANS,
                              fontSize: 13,
                              fontWeight: 500,
                              color: row.green ? '#00DF8F' : '#e6edf3',
                              marginLeft: 4,
                            }}
                          >
                            {row.unit}
                          </span>
                        )}
                      </div>
                    </div>
                  ))}
                </div>
              </button>
            );
          })}
        </div>

        {/* ── Right detail panel ─────────────────────────────────────── */}
        <div
          className="ts-right-panel"
          style={{
            flex: '0 0 380px',
            width: 380,
            borderLeft: '1px solid #243a32',
            background: '#060a08',
            display: 'flex',
            flexDirection: 'column',
            overflowY: 'auto',
          }}
        >
          {/* Panel header */}
          <div style={{ padding: '20px 20px 12px', flexShrink: 0 }}>
            <div
              style={{
                fontFamily: SANS,
                fontSize: 10,
                color: '#00DF8F',
                letterSpacing: '1.5px',
                marginBottom: 6,
                textTransform: 'uppercase',
              }}
            >
              Selected spec
            </div>
            <h2
              style={{
                fontFamily: SANS,
                fontSize: 28,
                fontWeight: 700,
                color: '#e6edf3',
                margin: 0,
                letterSpacing: '-0.01em',
              }}
            >
              {selectedOpt.label}
            </h2>
            <p
              style={{
                fontFamily: SANS,
                fontSize: 13,
                color: '#8b929c',
                lineHeight: 1.55,
                margin: '10px 0 0',
              }}
            >
              {selectedOpt.description}
            </p>
          </div>

          {/* Preview art tile */}
          <div
            style={{
              margin: '0 20px 16px',
              background: '#0b1410',
              border: '1px solid #1d2a24',
              borderRadius: 4,
              padding: '12px 8px',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'center',
              height: 200,
              position: 'relative',
              flexShrink: 0,
              overflow: 'hidden',
            }}
          >
            <SubtleGrid />
            <div style={{ position: 'relative', zIndex: 1, width: '100%', height: '100%' }}>
              <TractorTopDown hp={selectedOpt.hp} plowWidth={selectedOpt.plowWidth} size="mini" />
            </div>
          </div>

          {/* Full specification */}
          <div style={{ padding: '0 20px 16px', flexShrink: 0 }}>
            <div
              style={{
                fontFamily: SANS,
                fontSize: 10,
                color: '#7a8494',
                letterSpacing: '1px',
                textTransform: 'uppercase',
                marginBottom: 10,
              }}
            >
              Full specification
            </div>
            <div
              style={{
                display: 'grid',
                gridTemplateColumns: '1fr 1fr',
                gap: 1,
                background: '#243a32',
              }}
            >
              {[
                { label: 'Engine power', value: String(selectedOpt.hp),            unit: 'hp',   green: false },
                { label: 'Plow width',   value: selectedOpt.plowWidth.toFixed(1),  unit: 'm',    green: true  },
                { label: 'Max speed',    value: '7.0',                             unit: 'km/h', green: false },
                { label: 'Turn radius',  value: selectedOpt.turnRadius.toFixed(1), unit: 'm',    green: false },
                { label: 'Mass',         value: selectedOpt.mass.toFixed(1),       unit: 't',    green: false },
                { label: 'Lanes on Alpha', value: String(selectedOpt.lanesAlpha), unit: '',     green: false },
              ].map(cell => (
                <div key={cell.label} className="ts-spec-cell">
                  <span
                    style={{
                      fontFamily: SANS,
                      fontSize: 9,
                      color: '#7a8494',
                      letterSpacing: '0.5px',
                    }}
                  >
                    {cell.label}
                  </span>
                  <span
                    style={{
                      fontFamily: SANS,
                      fontSize: 16,
                      fontWeight: 700,
                      color: cell.green ? '#00DF8F' : '#e6edf3',
                    }}
                  >
                    {cell.value}
                    {cell.unit && (
                      <span
                        style={{
                          fontSize: 11,
                          fontWeight: 400,
                          color: '#7a8494',
                          marginLeft: 4,
                        }}
                      >
                        {cell.unit}
                      </span>
                    )}
                  </span>
                </div>
              ))}
            </div>
          </div>

          {/* Strategy note */}
          <div style={{ padding: '0 20px 20px', flexShrink: 0 }}>
            <div
              style={{
                fontFamily: SANS,
                fontSize: 10,
                color: '#7a8494',
                letterSpacing: '1px',
                textTransform: 'uppercase',
                marginBottom: 8,
              }}
            >
              Strategy note
            </div>
            <p
              style={{
                fontFamily: SANS,
                fontSize: 13,
                color: '#8b929c',
                lineHeight: 1.6,
                margin: 0,
              }}
            >
              {selectedOpt.strategyNote}
            </p>
          </div>

          {/* Spacer to push CTA to bottom */}
          <div style={{ flex: 1 }} />

          {/* CTA button */}
          <div
            style={{
              padding: '12px 20px 20px',
              flexShrink: 0,
              borderTop: '1px solid #1d2a24',
            }}
          >
            <button
              type="button"
              className="ts-launch-btn"
              disabled={!wsConnected}
              onClick={handleConfirm}
              style={{
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                gap: 8,
                width: '100%',
                padding: '18px 26px',
                borderRadius: 8,
                background: wsConnected
                  ? 'linear-gradient(180deg, #33e5a4 0%, #00DF8F 100%)'
                  : 'rgba(0,223,143,0.2)',
                border: '1px solid #00DF8F',
                boxShadow: wsConnected ? '0 8px 24px 0 rgba(0,223,143,0.22)' : 'none',
                color: '#051208',
                fontSize: 17,
                fontWeight: 700,
                fontFamily: SANS,
                cursor: wsConnected ? 'pointer' : 'not-allowed',
                letterSpacing: '0.5px',
              }}
            >
              <svg width="18" height="18" viewBox="0 0 14 14" fill="none" aria-hidden="true">
                <path d="M3 7 H11 M8 4 L11 7 L8 10" stroke="#051208" strokeWidth="1.8" strokeLinecap="round" strokeLinejoin="round" />
              </svg>
              Next
            </button>
          </div>
        </div>
      </div>
    </div>
  );
}
