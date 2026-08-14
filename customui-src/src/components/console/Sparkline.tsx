export interface SparklineProps {
  data: number[];
  width?: number;
  height?: number;
  color?: string;
  className?: string;
  strokeWidth?: number;
}

export default function Sparkline(props: SparklineProps) {
  const {
    data,
    width = 120,
    height = 24,
    color = '#10b981',
    className = '',
    strokeWidth = 1.5,
  } = props;

  // Handle empty or single-point data: render flat line at center
  if (data.length <= 1) {
    const centerY = height / 2;
    return (
      <svg
        width={width}
        height={height}
        viewBox={`0 0 ${width} ${height}`}
        className={className}
        aria-hidden="true"
      >
        <polyline
          points={`0,${centerY} ${width},${centerY}`}
          fill="none"
          stroke={color}
          strokeWidth={strokeWidth}
        />
      </svg>
    );
  }

  // Calculate min and max
  let min = Math.min(...data);
  let max = Math.max(...data);

  // Avoid divide-by-zero when all values are the same
  if (min === max) {
    min = min - 1;
    max = max + 1;
  }

  // Normalize data points
  const range = max - min;
  const points = data
    .map((value, i) => {
      const x = (i / (data.length - 1)) * width;
      const y = height - ((value - min) / range) * height;
      return `${x},${y}`;
    })
    .join(' ');

  return (
    <svg
      width={width}
      height={height}
      viewBox={`0 0 ${width} ${height}`}
      className={className}
      aria-hidden="true"
    >
      <polyline
        points={points}
        fill="none"
        stroke={color}
        strokeWidth={strokeWidth}
      />
    </svg>
  );
}
