import React from 'react';
import FieldArtSvg from './FieldArtSvg';

interface Props {
  codeName: string;
  subLabel: string;
  description?: string;
  area_m2?: number;
  polygons: [number, number][][];
  mapId: string;
  onLaunch: () => void;
  maxSpeed?: number;
  startLat?: number;
  startLon?: number;
  coverage?: string;
  reverseRule?: string;
}

// Small inline SVG icons for RULES cells
const IconArea: React.FC = () => (
  <svg width="10" height="10" viewBox="0 0 10 10" fill="none" aria-hidden="true">
    <rect x="1" y="1" width="8" height="8" stroke="currentColor" strokeWidth="1.2" fill="none" />
  </svg>
);

const IconSpeed: React.FC = () => (
  <svg width="10" height="10" viewBox="0 0 10 10" fill="none" aria-hidden="true">
    <path d="M5 1v4M5 5l3 3" stroke="currentColor" strokeWidth="1.2" strokeLinecap="round" />
  </svg>
);

const IconStart: React.FC = () => (
  <svg width="10" height="10" viewBox="0 0 10 10" fill="none" aria-hidden="true">
    <path d="M2 2l6 3-6 3V2z" fill="currentColor" />
  </svg>
);

const IconCoverage: React.FC = () => (
  <svg width="10" height="10" viewBox="0 0 10 10" fill="none" aria-hidden="true">
    <circle cx="5" cy="5" r="4" stroke="currentColor" strokeWidth="1.2" fill="none" />
    <circle cx="5" cy="5" r="1.5" fill="currentColor" />
  </svg>
);

const IconWarning: React.FC = () => (
  <svg width="10" height="10" viewBox="0 0 10 10" fill="none" aria-hidden="true">
    <path d="M5 1L9.5 9H0.5L5 1z" stroke="currentColor" strokeWidth="1.2" strokeLinejoin="round" fill="none" />
    <path d="M5 4v2" stroke="currentColor" strokeWidth="1.2" strokeLinecap="round" />
    <circle cx="5" cy="7.5" r="0.5" fill="currentColor" />
  </svg>
);

const LaunchArrow: React.FC = () => (
  <svg width="16" height="16" viewBox="0 0 14 14" fill="none" aria-hidden="true">
    <path d="M3 7 H11 M8 4 L11 7 L8 10" stroke="currentColor" strokeWidth="1.8" strokeLinecap="round" strokeLinejoin="round" />
  </svg>
);

const hudLabelStyle: React.CSSProperties = {
  fontFamily: "'Geist', sans-serif",
  fontSize: '10px',
  letterSpacing: '2.5px',
  color: '#7a8494',
  textTransform: 'uppercase',
};

const MissionBriefingPanel: React.FC<Props> = ({
  codeName,
  subLabel,
  description,
  area_m2,
  polygons,
  mapId,
  onLaunch,
  maxSpeed,
  startLat,
  startLon,
  coverage,
  reverseRule,
}) => {
  const isEmpty = codeName === '';

  const totalArea =
    area_m2 != null ? Math.round(area_m2).toLocaleString('en-US') + ' m²' : '—';
  const speedVal = `${maxSpeed ?? 7} km/h`;
  const coverageVal = coverage ?? '≥ 95 %';
  const reverseVal = reverseRule ?? 'UNPLOWS';

  const cellStyle: React.CSSProperties = {
    background: '#151b23',
    padding: '12px 14px',
    display: 'flex',
    flexDirection: 'column',
    gap: '6px',
  };

  const cellLabelRowStyle: React.CSSProperties = {
    display: 'flex',
    alignItems: 'center',
    gap: '6px',
    color: '#7a8494',
  };

  const cellLabelTextStyle: React.CSSProperties = {
    fontSize: '9px',
    letterSpacing: '1.5px',
    color: '#7a8494',
    textTransform: 'uppercase',
  };

  const cellValueStyle: React.CSSProperties = {
    fontFamily: "'Geist', sans-serif",
    fontSize: '16px',
    fontWeight: 600,
    color: '#e6edf3',
  };

  return (
    <div
      style={{
        background: '#0d1117',
        border: '1px solid #1f2630',
        borderRadius: '4px',
        padding: '20px',
        display: 'flex',
        flexDirection: 'column',
        gap: '14px',
        height: '100%',
        boxSizing: 'border-box',
        overflowY: 'auto',
      }}
    >
      {/* MISSION BRIEFING label */}
      <div style={hudLabelStyle}>MISSION BRIEFING</div>

      {isEmpty ? (
        <div style={{ fontSize: '14px', color: '#7a8494' }}>Select a field</div>
      ) : (
        <>
          {/* Inline header: ALPHA · Hackathon Course · Official */}
          <h2
            style={{
              fontFamily: "'Geist', sans-serif",
              fontWeight: 700,
              fontSize: '26px',
              color: '#e6edf3',
              margin: 0,
              lineHeight: 1.2,
            }}
          >
            <span style={{ color: '#00DF8F' }}>{codeName}</span>
            <span
              style={{
                fontWeight: 400,
                fontSize: '14px',
                color: '#e6edf3',
                marginLeft: '8px',
              }}
            >
              · {subLabel}
            </span>
          </h2>

          {/* Description */}
          {description && (
            <p
              style={{
                fontSize: '13px',
                color: '#b8c2d0',
                lineHeight: 1.6,
                margin: 0,
              }}
            >
              {description}
            </p>
          )}

          {/* Large minimap */}
          <div
            style={{
              width: '100%',
              height: '140px',
              background: '#1a1815',
              borderRadius: '4px',
              padding: '16px',
              boxSizing: 'border-box',
              display: 'flex',
              alignItems: 'stretch',
            }}
          >
            <FieldArtSvg
              polygons={polygons}
              width={400}
              height={140}
              mapId={mapId}
            />
          </div>
        </>
      )}

      {/* MISSION RULES label */}
      <div style={{ ...hudLabelStyle, marginTop: '4px' }}>MISSION RULES</div>

      {/* Rules grid */}
      <div
        style={{
          display: 'grid',
          gridTemplateColumns: '1fr 1fr',
          gap: '1px',
          background: '#1f2630',
          borderRadius: '4px',
          overflow: 'hidden',
        }}
      >
        {/* TOTAL AREA */}
        <div style={cellStyle}>
          <div style={cellLabelRowStyle}>
            <IconArea />
            <span style={cellLabelTextStyle}>TOTAL AREA</span>
          </div>
          <span style={cellValueStyle}>{totalArea}</span>
        </div>

        {/* MAX SPEED */}
        <div style={cellStyle}>
          <div style={cellLabelRowStyle}>
            <IconSpeed />
            <span style={cellLabelTextStyle}>MAX SPEED</span>
          </div>
          <span style={cellValueStyle}>{speedVal}</span>
        </div>

        {/* START */}
        <div style={cellStyle}>
          <div style={cellLabelRowStyle}>
            <IconStart />
            <span style={cellLabelTextStyle}>START</span>
          </div>
          {startLat != null && startLon != null ? (
            <div style={{ ...cellValueStyle, fontSize: '13px', lineHeight: 1.4 }}>
              <div>{startLat.toFixed(3)}° N</div>
              <div>{startLon.toFixed(3)}° E</div>
            </div>
          ) : (
            <span style={cellValueStyle}>—</span>
          )}
        </div>

        {/* COVERAGE */}
        <div style={cellStyle}>
          <div style={cellLabelRowStyle}>
            <IconCoverage />
            <span style={cellLabelTextStyle}>COVERAGE</span>
          </div>
          <span style={cellValueStyle}>{coverageVal}</span>
        </div>

        {/* REVERSE — full width, orange accent */}
        <div
          style={{
            ...cellStyle,
            gridColumn: '1 / -1',
            borderLeft: '3px solid #e0a23a',
          }}
        >
          <div style={{ ...cellLabelRowStyle, color: '#e0a23a' }}>
            <IconWarning />
            <span style={{ ...cellLabelTextStyle, color: '#e0a23a' }}>REVERSE</span>
          </div>
          <span style={cellValueStyle}>{reverseVal}</span>
        </div>
      </div>

      {/* LAUNCH MISSION button */}
      <button
        className="mop-launch-btn"
        onClick={isEmpty ? undefined : onLaunch}
        disabled={isEmpty}
        style={{
          background: 'linear-gradient(180deg, #33e5a4, #00DF8F)',
          color: '#0a1a14',
          fontFamily: "'Geist', sans-serif",
          fontWeight: 700,
          fontSize: '17px',
          letterSpacing: '1px',
          padding: '18px 28px',
          width: '100%',
          border: 'none',
          borderRadius: '4px',
          cursor: isEmpty ? 'not-allowed' : 'pointer',
          display: 'flex',
          justifyContent: 'center',
          alignItems: 'center',
          gap: '8px',
          opacity: isEmpty ? 0.4 : 1,
        }}
      >
        <LaunchArrow />
        Next
      </button>
    </div>
  );
};

export default MissionBriefingPanel;
