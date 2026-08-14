import { useEffect, useState } from 'react';
import CoverageGauge from './CoverageGauge';
import Sparkline from './Sparkline';
import Minimap from './Minimap';

export interface RightConsoleProps {
  progress: number;          // 0..1
  elapsedS: number;
  speed: number;             // m/s
  heading: number;           // degrees [0, 360)
  posX: number;
  posY: number;
  crossTrackError: number;
  headingDrift: number;
  vehicleX: number;
  vehicleZ: number;
  vehicleHeading: number;
  coverageGoal: number;      // 0..100
  areaDoneM2?: number;
  areaTotalM2?: number;
  trackingCrUnit?: number;
  overheadScaleKm?: number;
}

function formatElapsed(elapsedS: number): string {
  const m = Math.floor(elapsedS / 60);
  const s = Math.floor(elapsedS % 60);
  return `${m.toString().padStart(2, '0')}:${s.toString().padStart(2, '0')}`;
}

function getCardinal(deg: number): string {
  const n = ((deg % 360) + 360) % 360;
  if (n >= 337.5 || n < 22.5) return 'N';
  if (n < 67.5) return 'NE';
  if (n < 112.5) return 'E';
  if (n < 157.5) return 'SE';
  if (n < 202.5) return 'S';
  if (n < 247.5) return 'SW';
  if (n < 292.5) return 'W';
  return 'NW';
}


function SectionHeader({ label, aux }: { label: string; aux?: string }) {
  return (
    <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: 4 }}>
      <div style={{ fontSize: 11, color: '#f3f4f6', letterSpacing: '0.05em', textTransform: 'uppercase' as const }}>
        {label}
      </div>
      {aux !== undefined && (
        <span style={{ fontSize: 11, color: '#9ca3af' }}>{aux}</span>
      )}
    </div>
  );
}

const sectionHeaderStyle: React.CSSProperties = {
  fontSize: 11,
  color: '#f3f4f6',
  letterSpacing: '0.05em',
  textTransform: 'uppercase',
  marginBottom: 4,
};

const bigNumberStyle: React.CSSProperties = {
  fontSize: 22,
  fontWeight: 700,
  color: '#f3f4f6',
  lineHeight: 1,
};

const subTextStyle: React.CSSProperties = {
  fontSize: 11,
  color: '#6b7280',
};

const halfColStyle: React.CSSProperties = {
  display: 'flex',
  flexDirection: 'column',
  gap: 2,
  flex: 1,
};

export default function RightConsole(props: RightConsoleProps) {
  const {
    progress,
    elapsedS,
    speed,
    heading,
    posX,
    posY,
    crossTrackError,
    headingDrift,
    vehicleX,
    vehicleZ,
    vehicleHeading,
    coverageGoal,
    areaDoneM2 = 0,
    areaTotalM2 = 0,
    trackingCrUnit = 0,
    overheadScaleKm = 0,
  } = props;

  const [crossTrackHistory, setCrossTrackHistory] = useState<number[]>([]);
  const [headingDriftHistory, setHeadingDriftHistory] = useState<number[]>([]);

  useEffect(() => {
    setCrossTrackHistory((prev) => {
      const next = [...prev, crossTrackError];
      return next.length > 60 ? next.slice(next.length - 60) : next;
    });
  }, [crossTrackError]);

  useEffect(() => {
    setHeadingDriftHistory((prev) => {
      const next = [...prev, headingDrift];
      return next.length > 60 ? next.slice(next.length - 60) : next;
    });
  }, [headingDrift]);

  const headingDeg = heading;
  const cardinal = getCardinal(headingDeg);

  return (
    <div
      style={{
        width: 320,
        flexShrink: 0,
        height: '100%',
        background: '#0a0f0d',
        borderLeft: '1px solid rgba(255,255,255,0.06)',
        padding: 16,
        gap: 14,
        display: 'flex',
        flexDirection: 'column',
        overflowY: 'auto',
        color: '#e5e7eb',
        fontFamily: 'system-ui, -apple-system, sans-serif',
      }}
    >
      {/* Section 1: Coverage */}
      <div style={{ borderBottom: '1px solid rgba(255,255,255,0.06)', paddingBottom: 12 }}>
        <CoverageGauge progress={progress} goal={coverageGoal} aux={{ areaDoneM2, areaTotalM2 }} />
      </div>

      {/* Section 2: Elapsed */}
      <div style={{ borderBottom: '1px solid rgba(255,255,255,0.06)', paddingBottom: 12 }}>
        <SectionHeader label="Elapsed" />
        <div aria-hidden="true" style={{ fontSize: 32, fontWeight: 700, color: '#f3f4f6', lineHeight: 1 }}>
          {formatElapsed(elapsedS)}
        </div>
      </div>

      {/* Section 3: Speed / Heading row */}
      <div style={{ borderBottom: '1px solid rgba(255,255,255,0.06)', paddingBottom: 12 }}>
        <div style={{ display: 'flex', gap: 12 }}>
          {/* Speed */}
          <div style={halfColStyle}>
            <div style={sectionHeaderStyle}>Speed</div>
            <div aria-hidden="true" style={{ display: 'flex', alignItems: 'baseline', gap: 2 }}>
              <span style={bigNumberStyle}>{(speed * 3.6).toFixed(2)}</span>
              <span style={{ fontSize: 12, color: '#9ca3af' }}> km/h</span>
            </div>
          </div>

          {/* Heading */}
          <div style={halfColStyle}>
            <div style={sectionHeaderStyle}>Heading</div>
            <div aria-hidden="true" style={{ display: 'flex', alignItems: 'baseline', gap: 2 }}>
              <span style={bigNumberStyle}>{headingDeg.toFixed(0)}°</span>
              <span style={{ fontSize: 12, color: '#9ca3af' }}> {cardinal}</span>
            </div>
            <div style={subTextStyle}>&nbsp;</div>
          </div>
        </div>
      </div>

      {/* Section 4: Position X / Y row */}
      <div style={{ borderBottom: '1px solid rgba(255,255,255,0.06)', paddingBottom: 12 }}>
        <div style={{ display: 'flex', gap: 12 }}>
          {/* Position X */}
          <div style={halfColStyle}>
            <div style={sectionHeaderStyle}>Position X</div>
            <span aria-hidden="true" style={bigNumberStyle}>{posX.toFixed(2)}</span>
          </div>

          {/* Position Y */}
          <div style={halfColStyle}>
            <div style={sectionHeaderStyle}>Position Y</div>
            <span aria-hidden="true" style={bigNumberStyle}>{posY.toFixed(2)}</span>
          </div>
        </div>
      </div>

      {/* Section 5: Tracking */}
      <div style={{ display: 'flex', flexDirection: 'column', gap: 8, borderBottom: '1px solid rgba(255,255,255,0.06)', paddingBottom: 12 }}>
        <SectionHeader label="Tracking" aux={`${trackingCrUnit.toFixed(2)} cr-unit`} />

        {/* Cross-track */}
        <div>
          <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
            <span style={{ fontSize: 11, color: '#9ca3af' }}>Cross-track</span>
            <span style={{ fontSize: 12, color: '#f3f4f6' }}>
              ±{Math.abs(crossTrackError).toFixed(2)} m
            </span>
          </div>
          <Sparkline data={[...crossTrackHistory]} width={288} height={24} />
        </div>

        {/* Heading drift */}
        <div>
          <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
            <span style={{ fontSize: 11, color: '#9ca3af' }}>Heading drift</span>
            <span style={{ fontSize: 12, color: '#f3f4f6' }}>
              {headingDrift.toFixed(1)}°
            </span>
          </div>
          <Sparkline data={[...headingDriftHistory]} width={288} height={24} />
        </div>
      </div>

      {/* Section 6: Overhead (Minimap) */}
      <div>
        <SectionHeader label="Overhead" aux={overheadScaleKm > 0 ? `~${overheadScaleKm.toFixed(1)}km` : ''} />
        <Minimap
          vehicleX={vehicleX}
          vehicleZ={vehicleZ}
          vehicleHeading={vehicleHeading}
          width={288}
          height={200}
        />
      </div>
    </div>
  );
}
