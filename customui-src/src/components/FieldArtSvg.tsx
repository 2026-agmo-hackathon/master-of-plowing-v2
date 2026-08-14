import React, { useMemo } from 'react';

interface Props {
  polygons: [number, number][][];
  width: number;
  height: number;
  mapId: string;
}

// Compute bounding box from all coordinates across all polygons
function computeBBox(polygons: [number, number][][]): {
  minX: number;
  minY: number;
  maxX: number;
  maxY: number;
} {
  let minX = Infinity;
  let minY = Infinity;
  let maxX = -Infinity;
  let maxY = -Infinity;

  for (const poly of polygons) {
    for (const [x, y] of poly) {
      if (x < minX) minX = x;
      if (y < minY) minY = y;
      if (x > maxX) maxX = x;
      if (y > maxY) maxY = y;
    }
  }

  return { minX, minY, maxX, maxY };
}

// Normalize geographic coordinates into SVG canvas space.
// Y axis is flipped: geographic north (higher lat) maps to lower SVG y.
function normalizePoints(
  polygon: [number, number][],
  minX: number,
  minY: number,
  maxX: number,
  maxY: number,
  padX: number,
  padY: number,
  svgW: number,
  svgH: number,
): string {
  const rangeX = maxX - minX || 1;
  const rangeY = maxY - minY || 1;
  const drawW = svgW - padX * 2;
  const drawH = svgH - padY * 2;

  return polygon
    .map(([x, y]) => {
      const nx = padX + ((x - minX) / rangeX) * drawW;
      // Flip Y: geographic higher value = SVG top = smaller SVG y
      const ny = padY + (1 - (y - minY) / rangeY) * drawH;
      return `${nx.toFixed(2)},${ny.toFixed(2)}`;
    })
    .join(' ');
}

// Build a deterministic fallback polygon based on mapId char code sum
function buildFallbackPoints(
  seed: number,
  svgW: number,
  svgH: number,
): string {
  // Produce a slightly varied hexagon-like shape using the seed
  const cx = svgW / 2;
  const cy = svgH / 2;
  const rx = svgW * 0.38;
  const ry = svgH * 0.38;
  const sides = 6;
  const angleOffset = ((seed % 30) - 15) * (Math.PI / 180);

  const points: string[] = [];
  for (let i = 0; i < sides; i++) {
    const angle = (i / sides) * Math.PI * 2 + angleOffset;
    // Slightly distort each vertex by a small deterministic amount
    const distort = 1 + ((seed * (i + 1)) % 9) * 0.012;
    const px = cx + rx * distort * Math.cos(angle);
    const py = cy + ry * distort * Math.sin(angle);
    points.push(`${px.toFixed(2)},${py.toFixed(2)}`);
  }
  return points.join(' ');
}

const FieldArtSvg: React.FC<Props> = ({
  polygons,
  width,
  height,
  mapId,
}) => {
  const seed = useMemo(
    () => Array.from(mapId).reduce((acc, ch) => acc + ch.charCodeAt(0), 0),
    [mapId],
  );

  // Memoize bounding box so start marker can reuse it
  const bbox = useMemo(() => {
    if (polygons.length === 0) return null;
    return computeBBox(polygons);
  }, [polygons]);

  // Memoize normalized polygon point strings
  const normalizedPolygons = useMemo(() => {
    if (!bbox) return [];

    const { minX, minY, maxX, maxY } = bbox;
    // 5% padding on each side
    const padX = width * 0.05;
    const padY = height * 0.05;

    return polygons.map((poly) =>
      normalizePoints(poly, minX, minY, maxX, maxY, padX, padY, width, height),
    );
  }, [polygons, bbox, width, height]);

  const fallbackPoints = useMemo(() => {
    if (polygons.length > 0) return '';
    return buildFallbackPoints(seed, width, height);
  }, [polygons.length, seed, width, height]);

  return (
    <svg
      xmlns="http://www.w3.org/2000/svg"
      viewBox={`0 0 ${width} ${height}`}
      width={width}
      height={height}
      aria-hidden="true"
    >
      {/* Dark background */}
      <rect width={width} height={height} fill="#0a120e" />

      {polygons.length > 0
        ? normalizedPolygons.map((pts, i) => (
            <polygon
              key={i}
              points={pts}
              fill="#2b2a20"
              stroke="#b89c5a"
              strokeWidth="1.4"
              strokeDasharray="3 3"
              strokeLinejoin="round"
            />
          ))
        : (
            <polygon
              points={fallbackPoints}
              fill="#2b2a20"
              stroke="#b89c5a"
              strokeWidth="1.4"
              strokeDasharray="3 3"
              strokeLinejoin="round"
            />
          )}
    </svg>
  );
};

export default FieldArtSvg;
