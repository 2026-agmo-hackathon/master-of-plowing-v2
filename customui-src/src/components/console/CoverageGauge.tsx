export interface CoverageGaugeProps {
  progress: number;   // 0..1
  goal: number;       // 0..100 (percent)
  className?: string;
  aux?: { areaDoneM2?: number; areaTotalM2?: number };
}

export default function CoverageGauge({
  progress,
  goal,
  className = '',
  aux,
}: CoverageGaugeProps) {
  const progressPercent = progress * 100;
  const clampedProgress = Math.min(progressPercent, 100);
  const displayProgress = progressPercent.toFixed(1);

  return (
    <div className={className} style={{ display: 'flex', flexDirection: 'column', gap: '6px' }}>
      {/* Header */}
      <div
        style={{
          display: 'flex',
          justifyContent: 'space-between',
          alignItems: 'center',
        }}
      >
        <div
          style={{
            fontSize: '11px',
            fontWeight: 600,
            color: '#f3f4f6',
            textTransform: 'uppercase',
            letterSpacing: '0.05em',
          }}
        >
          Coverage
        </div>
        <span style={{ fontSize: 11, color: '#9ca3af' }}>
          {aux && aux.areaDoneM2 != null
            ? `A:${aux.areaDoneM2.toFixed(1)} R:${Math.max((aux.areaTotalM2 ?? 0) - aux.areaDoneM2, 0).toLocaleString()} m²`
            : ''}
        </span>
      </div>

      {/* Big percentage number */}
      <div
        style={{
          fontSize: '36px',
          fontWeight: 700,
          color: '#34d399',
          lineHeight: 1,
        }}
      >
        {displayProgress}%
      </div>

      {/* Progress bar */}
      <div
        role="progressbar"
        aria-valuenow={Math.round(progress * 1000) / 10}
        aria-valuemin={0}
        aria-valuemax={100}
        aria-label={`Coverage ${(progress * 100).toFixed(1)} percent of goal ${goal}`}
        style={{
          position: 'relative',
          width: '100%',
          height: '6px',
          background: 'rgba(255, 255, 255, 0.08)',
          borderRadius: '3px',
        }}
      >
        {/* Inner fill */}
        <div
          style={{
            width: `${clampedProgress}%`,
            height: '100%',
            background: '#10b981',
            borderRadius: '3px',
          }}
        />

        {/* Goal marker */}
        <div
          style={{
            position: 'absolute',
            left: `${goal}%`,
            top: '-2px',
            width: '1px',
            height: '10px',
            background: '#6b7280',
          }}
        />
      </div>

      {/* Sub-text row */}
      <div
        style={{
          display: 'flex',
          justifyContent: 'space-between',
          fontSize: '11px',
          color: '#9ca3af',
        }}
      >
        <div>Goal {goal}%</div>
        <div>
          {displayProgress}→{goal.toFixed(1)}
        </div>
      </div>
    </div>
  );
}
