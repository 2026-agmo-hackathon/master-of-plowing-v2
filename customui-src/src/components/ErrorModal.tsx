import type { JSX } from 'react'
import { useSessionStore } from '../state/sessionStore'

export interface ApiErrorState {
  source: 'map_list' | 'leaderboard' | 'start_session' | 'rddf_validation'
  reason: string
  code?: string
  status?: number
  op?: string
  runId?: string
}

export default function ErrorModal(): JSX.Element | null {
  const apiError = useSessionStore((s) => (s as unknown as { apiError: ApiErrorState | null }).apiError)
  const dismissApiError = useSessionStore(
    (s) => (s as unknown as { dismissApiError: () => void }).dismissApiError
  )

  if (!apiError) return null

  const title =
    apiError.source === 'leaderboard'      ? 'Leaderboard Error' :
    apiError.source === 'start_session'    ? 'Start Error' :
    apiError.source === 'rddf_validation'  ? 'RDDF Validation Error' :
    'Map List Error'

  return (
    <div style={overlayStyle} role="alertdialog" aria-modal={true} aria-labelledby="errmod-title">
      <div style={dialogStyle}>
        <h2 id="errmod-title" style={titleStyle}>
          {title}
        </h2>
        <p style={bodyStyle}>{apiError.reason}</p>
        {apiError.status != null && apiError.status > 0 && (
          <p style={metaStyle}>
            HTTP {apiError.status}
            {apiError.code ? ` · ${apiError.code}` : ''}
          </p>
        )}
        {apiError.op && (
          <p style={metaStyle}>
            Operation: {apiError.op}
            {apiError.runId ? ` · run ${apiError.runId.slice(0, 8)}…` : ''}
          </p>
        )}
        <div style={buttonRowStyle}>
          <button
            type="button"
            onClick={dismissApiError}
            style={closeButtonStyle}
            onMouseEnter={(e) => {
              const el = e.currentTarget as HTMLButtonElement
              el.style.borderColor = '#2d3a34'
              el.style.color = '#c0c8d0'
            }}
            onMouseLeave={(e) => {
              const el = e.currentTarget as HTMLButtonElement
              el.style.borderColor = '#1d2a24'
              el.style.color = '#94a3b8'
            }}
          >
            Close
          </button>
        </div>
      </div>
    </div>
  )
}

const overlayStyle: React.CSSProperties = {
  position: 'fixed',
  inset: 0,
  zIndex: 2000,
  background: 'rgba(0,0,0,0.75)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
}

const dialogStyle: React.CSSProperties = {
  background: '#0d1a14',
  border: '1px solid #1d2a24',
  borderRadius: 8,
  padding: '24px 28px',
  maxWidth: 480,
  width: '92%',
  boxShadow: '0 4px 24px rgba(0,0,0,0.6)',
}

const titleStyle: React.CSSProperties = {
  margin: 0,
  marginBottom: 12,
  fontFamily: "'Geist', system-ui, sans-serif",
  fontSize: 18,
  fontWeight: 600,
  color: '#e6edf3',
}

const bodyStyle: React.CSSProperties = {
  margin: 0,
  fontFamily: "'Geist', system-ui, sans-serif",
  fontSize: 14,
  lineHeight: 1.6,
  color: '#94a3b8',
}

const metaStyle: React.CSSProperties = {
  marginTop: 8,
  marginBottom: 0,
  fontFamily: "'JetBrains Mono', ui-monospace, monospace",
  fontSize: 12,
  color: '#7a8494',
}

const buttonRowStyle: React.CSSProperties = {
  marginTop: 20,
  display: 'flex',
  justifyContent: 'flex-end',
}

const closeButtonStyle: React.CSSProperties = {
  padding: '7px 20px',
  fontFamily: "'JetBrains Mono', ui-monospace, monospace",
  fontSize: 12,
  fontWeight: 600,
  letterSpacing: '0.5px',
  border: '1px solid #1d2a24',
  borderRadius: 4,
  background: 'transparent',
  color: '#94a3b8',
  cursor: 'pointer',
}
