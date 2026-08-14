import { useEffect, useRef } from 'react'
import { useSessionStore } from '../state/sessionStore'
import { getSessionClient } from '../protocol/SessionClient'

export default function RddfPendingDialog() {
  const queue = useSessionStore((s) => s.rddfPendingQueue)
  const cancelBtnRef = useRef<HTMLButtonElement>(null)
  const applyBtnRef = useRef<HTMLButtonElement>(null)
  const head = queue[0]

  // Move focus to Cancel button whenever the dialog head changes
  useEffect(() => {
    if (head) cancelBtnRef.current?.focus()
  }, [head?.mapId])

  if (!head) return null

  const client = getSessionClient()
  const handleApply = () => client.sendRddfApply(head.mapId)
  const handleCancel = () => client.sendRddfCancel(head.mapId)

  const handleKeyDown = (e: React.KeyboardEvent) => {
    // ESC closes the dialog
    if (e.key === 'Escape') {
      e.preventDefault()
      handleCancel()
      return
    }
    // Focus trap: Tab/Shift+Tab cycles between the two buttons
    if (e.key === 'Tab') {
      const buttons = [cancelBtnRef.current, applyBtnRef.current].filter(Boolean) as HTMLButtonElement[]
      if (buttons.length === 0) return
      const idx = buttons.findIndex((b) => b === document.activeElement)
      if (e.shiftKey && idx <= 0) {
        // Shift+Tab on first button → wrap to last
        e.preventDefault()
        buttons[buttons.length - 1].focus()
      } else if (!e.shiftKey && idx === buttons.length - 1) {
        // Tab on last button → wrap to first
        e.preventDefault()
        buttons[0].focus()
      }
    }
  }

  return (
    <div style={overlayStyle} onKeyDown={handleKeyDown}>
      <div role="dialog" aria-modal={true} aria-label="RDDF pending confirmation" style={dialogStyle}>
        <h3 style={titleStyle}>New RDDF available</h3>
        <p style={bodyStyle}>
          Map: <span style={accentStyle}>{head.mapId}</span>
        </p>
        <p style={{ ...bodyStyle, marginBottom: 20 }}>Received: {head.receivedAt}</p>
        <div style={buttonRowStyle}>
          <button
            ref={cancelBtnRef}
            type="button"
            onClick={handleCancel}
            style={cancelButtonStyle}
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
            Cancel
          </button>
          <button
            ref={applyBtnRef}
            type="button"
            onClick={handleApply}
            style={applyButtonStyle}
            onMouseEnter={(e) => {
              const el = e.currentTarget as HTMLButtonElement
              el.style.background = '#00bf7a'
              el.style.borderColor = '#00bf7a'
            }}
            onMouseLeave={(e) => {
              const el = e.currentTarget as HTMLButtonElement
              el.style.background = '#00DF8F'
              el.style.borderColor = '#00DF8F'
            }}
          >
            Apply
          </button>
        </div>
      </div>
    </div>
  )
}

const overlayStyle: React.CSSProperties = {
  position: 'fixed',
  inset: 0,
  background: 'rgba(0,0,0,0.75)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  zIndex: 1000,
}

const dialogStyle: React.CSSProperties = {
  background: '#0d1a14',
  border: '1px solid #1d2a24',
  padding: '24px 28px',
  borderRadius: 8,
  minWidth: 340,
  maxWidth: 440,
  boxShadow: '0 4px 24px rgba(0,0,0,0.6)',
}

const titleStyle: React.CSSProperties = {
  margin: 0,
  marginBottom: 14,
  fontFamily: "'Geist', system-ui, sans-serif",
  fontSize: 16,
  fontWeight: 700,
  color: '#e6edf3',
}

const bodyStyle: React.CSSProperties = {
  margin: 0,
  marginBottom: 4,
  fontFamily: "'Geist', system-ui, sans-serif",
  fontSize: 13,
  lineHeight: 1.6,
  color: '#94a3b8',
}

const accentStyle: React.CSSProperties = {
  color: '#e6edf3',
  fontWeight: 600,
}

const buttonRowStyle: React.CSSProperties = {
  display: 'flex',
  justifyContent: 'flex-end',
  gap: 8,
}

const cancelButtonStyle: React.CSSProperties = {
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

const applyButtonStyle: React.CSSProperties = {
  padding: '7px 28px',
  fontFamily: "'JetBrains Mono', ui-monospace, monospace",
  fontSize: 12,
  fontWeight: 700,
  letterSpacing: '0.5px',
  border: '1px solid #00DF8F',
  borderRadius: 4,
  background: '#00DF8F',
  color: '#04140d',
  cursor: 'pointer',
}
