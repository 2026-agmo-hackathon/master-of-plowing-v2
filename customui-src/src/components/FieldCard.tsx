import React from 'react';
import FieldArtSvg from './FieldArtSvg';

interface Props {
  codeName: string;
  subLabel: string;
  area_m2?: number;
  selected: boolean;
  onClick: () => void;
  polygons: [number, number][][];
  mapId: string;
}

const CheckIcon: React.FC = () => (
  <svg width="9" height="9" viewBox="0 0 9 9" fill="none" aria-hidden="true">
    <path
      d="M1.5 4.5l2 2L7.5 2.5"
      stroke="#0a1a14"
      strokeWidth="1.5"
      strokeLinecap="round"
      strokeLinejoin="round"
    />
  </svg>
);


const FieldCard: React.FC<Props> = ({
  codeName,
  subLabel,
  area_m2,
  selected,
  onClick,
  polygons,
  mapId,
}) => {
  const wrapperStyle: React.CSSProperties = {
    background: '#151b23',
    border: `1px solid ${selected ? '#00DF8F' : '#1f2630'}`,
    boxShadow: selected
      ? '0 0 0 1px #00DF8F, 0 0 24px rgba(0,223,143,0.15)'
      : 'none',
    borderRadius: '4px',
    padding: 0,
    display: 'flex',
    flexDirection: 'column',
    cursor: 'pointer',
    overflow: 'hidden',
    transition: 'border-color 120ms, box-shadow 120ms',
  };

  const minimapContainerStyle: React.CSSProperties = {
    width: '100%',
    height: '140px',
    background: '#1a1815',
    position: 'relative',
    padding: '12px',
    boxSizing: 'border-box',
    display: 'flex',
    alignItems: 'stretch',
  };

  const textAreaStyle: React.CSSProperties = {
    padding: '16px',
    background: '#151b23',
    display: 'flex',
    flexDirection: 'column',
    gap: '4px',
  };

  const codeNameStyle: React.CSSProperties = {
    fontFamily: "'Geist', sans-serif",
    fontWeight: 700,
    fontSize: '22px',
    letterSpacing: '2px',
    color: selected ? '#00DF8F' : '#e6edf3',
  };

  const subLabelStyle: React.CSSProperties = {
    fontSize: '11px',
    color: '#7a8494',
    letterSpacing: '0.5px',
  };

  const areaLabelStyle: React.CSSProperties = {
    marginTop: '8px',
    fontSize: '10px',
    color: '#7a8494',
    letterSpacing: '2px',
    textTransform: 'uppercase' as const,
  };

  const areaValueStyle: React.CSSProperties = {
    fontFamily: "'Geist', sans-serif",
    fontSize: '18px',
    fontWeight: 600,
    color: '#e6edf3',
  };

  const selectedBadgeStyle: React.CSSProperties = {
    display: 'flex',
    alignItems: 'center',
    gap: '4px',
    background: '#00DF8F',
    color: '#0a1a14',
    fontFamily: "'Geist', sans-serif",
    fontSize: '9px',
    fontWeight: 700,
    letterSpacing: '1.5px',
    padding: '3px 8px',
    borderRadius: '2px',
    position: 'absolute',
    top: '8px',
    right: '8px',
  };

  const areaText =
    area_m2 != null ? `${Math.round(area_m2).toLocaleString('en-US')} m²` : '—';

  return (
    <div
      style={wrapperStyle}
      className="mop-field-card"
      role="radio"
      aria-checked={selected}
      tabIndex={0}
      onClick={onClick}
    >
      {/* Minimap area */}
      <div style={minimapContainerStyle}>
        <FieldArtSvg
          polygons={polygons}
          width={280}
          height={130}
          mapId={mapId}
        />

        {/* Selected badge */}
        {selected && (
          <div style={selectedBadgeStyle}>
            <CheckIcon />
            SELECTED
          </div>
        )}
      </div>

      {/* Text area */}
      <div style={textAreaStyle}>
        <div style={codeNameStyle}>{codeName}</div>
        <div style={subLabelStyle}>{subLabel}</div>
        <div style={areaLabelStyle}>AREA</div>
        <div style={areaValueStyle}>{areaText}</div>
      </div>
    </div>
  );
};

export default FieldCard;
