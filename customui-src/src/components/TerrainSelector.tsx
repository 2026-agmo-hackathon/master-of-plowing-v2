import type { JSX } from 'react'
import { useSessionStore } from '../state/sessionStore'

interface Props {
  currentMapId: number | null
  onChange: (mapId: number) => void
}

export default function TerrainSelector({ currentMapId, onChange }: Props): JSX.Element {
  const mapList = useSessionStore((s) => s.mapList)
  const style: React.CSSProperties = {
    background: 'rgba(0,0,0,0.75)',
    color: '#ccc',
    border: '1px solid #555',
    borderRadius: 6,
    padding: '6px 10px',
    fontSize: 12,
    cursor: 'pointer',
    outline: 'none',
    width: '100%',
    boxSizing: 'border-box',
    fontFamily: 'inherit',
  }

  return (
    <select
      style={style}
      value={currentMapId ?? ''}
      onChange={(e) => onChange(Number(e.target.value))}
      aria-label="Select terrain map"
    >
      {mapList.map((entry) => (
        <option key={entry.id} value={entry.id}>
          {entry.label}
        </option>
      ))}
    </select>
  )
}
