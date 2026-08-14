import React, { useMemo } from 'react';
import { useSessionStore } from '../../state/sessionStore';
import { gpsToEnu } from '../../simulation/GpsOrigin';

export interface MinimapProps {
  vehicleX: number;
  vehicleZ: number;
  vehicleHeading: number; // radians (heading in scene)
  width?: number;         // default 280
  height?: number;        // default 160
  className?: string;
}

const Minimap: React.FC<MinimapProps> = ({
  vehicleX,
  vehicleZ,
  width = 280,
  height = 200,
  className,
}) => {
  const selectedMapId = useSessionStore((s) => s.selectedMapId);
  const mapList = useSessionStore((s) => s.mapList);
  const originVersion = useSessionStore((s) => s.originVersion);

  // Resolve the polygon for the currently selected map only.
  const activePolygonLL = useMemo(() => {
    if (selectedMapId == null) return null;
    const entry = mapList.find((m) => m.id === selectedMapId);
    if (!entry || !entry.polygonLL || entry.polygonLL.length < 3) return null;
    return entry.polygonLL as [number, number][];
  }, [selectedMapId, mapList]);

  // Convert the single active polygon from lat/lon to ENU.
  const activePolygonENU = useMemo(() => {
    if (!activePolygonLL) return null;
    return activePolygonLL.map(([lat, lon]) => {
      const { x, z } = gpsToEnu(lat, lon);
      return [x, z] as [number, number];
    });
  }, [activePolygonLL, originVersion]);

  if (!activePolygonENU) {
    return (
      <svg
        width={width}
        height={height}
        viewBox={'0 0 ' + width + ' ' + height}
        style={{ background: 'rgba(255,255,255,0.02)', borderRadius: 6 }}
        className={className}
        role="img"
        aria-label="No map data available"
      >
        <text
          x={width / 2}
          y={height / 2}
          fill="#6b7280"
          textAnchor="middle"
          dominantBaseline="middle"
          fontSize="11"
        >
          No map
        </text>
      </svg>
    );
  }

  // Compute bounding box for the active polygon
  let minX = Infinity;
  let maxX = -Infinity;
  let minZ = Infinity;
  let maxZ = -Infinity;

  for (const [px, pz] of activePolygonENU) {
    if (px < minX) minX = px;
    if (px > maxX) maxX = px;
    if (pz < minZ) minZ = pz;
    if (pz > maxZ) maxZ = pz;
  }

  // Add 10% padding on each side
  const padX = (maxX - minX) * 0.1;
  const padZ = (maxZ - minZ) * 0.1;

  const bboxW = (maxX + padX) - (minX - padX);
  const bboxH = (maxZ + padZ) - (minZ - padZ);

  const toScreen = (x: number, z: number): [number, number] => [
    ((x - (minX - padX)) / bboxW) * width,
    ((z - (minZ - padZ)) / bboxH) * height,
  ];

  // Build SVG path string for the active polygon
  const parts = activePolygonENU.map(([px, pz], idx) => {
    const [sx, sz] = toScreen(px, pz);
    return (idx === 0 ? 'M' : 'L') + ' ' + sx + ' ' + sz;
  });
  const polygonPath = parts.join(' ') + ' Z';

  // Vehicle marker position
  const [sx, sz] = toScreen(vehicleX, vehicleZ);

  return (
    <svg
      width={width}
      height={height}
      viewBox={'0 0 ' + width + ' ' + height}
      style={{ background: 'rgba(255,255,255,0.02)', borderRadius: 6 }}
      className={className}
      role="img"
      aria-label="Overhead minimap showing vehicle position on field"
    >
      <path
        d={polygonPath}
        fill="rgba(176, 136, 90, 0.85)"
        stroke="#7a5a38"
        strokeWidth={1}
      />
      <circle
        cx={sx}
        cy={sz}
        r={14}
        fill="rgba(52, 211, 153, 0.18)"
      />
      <circle
        cx={sx}
        cy={sz}
        r={7}
        fill="#34d399"
        stroke="#0a0f0d"
        strokeWidth={2}
      />
    </svg>
  );
};

export default Minimap;
