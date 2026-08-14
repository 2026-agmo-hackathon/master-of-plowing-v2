import { useEffect, useRef, useState } from 'react'

const LS_KEY = 'simulator.speedWarning.dismissed'

export function isSpeedWarningDismissed(): boolean {
  try {
    return localStorage.getItem(LS_KEY) === 'true'
  } catch {
    return false
  }
}

export interface SpeedWarningDialogProps {
  open: boolean
  onClose: (dontShowAgain: boolean) => void
}

export default function SpeedWarningDialog({ open, onClose }: SpeedWarningDialogProps) {
  const [checked, setChecked] = useState(false)
  const okBtnRef = useRef<HTMLButtonElement>(null)
  const checkboxRef = useRef<HTMLInputElement>(null)

  // Focus OK button whenever dialog opens; reset checkbox state
  useEffect(() => {
    if (open) {
      setChecked(false)
      okBtnRef.current?.focus()
    }
  }, [open])

  if (!open) return null

  const handleClose = () => {
    if (checked) {
      try {
        localStorage.setItem(LS_KEY, 'true')
      } catch {
        // ignore
      }
    }
    onClose(checked)
  }

  const handleKeyDown = (e: React.KeyboardEvent) => {
    if (e.key === 'Escape') {
      e.preventDefault()
      handleClose()
      return
    }
    // Focus trap: Tab/Shift+Tab cycles between checkbox and OK button
    if (e.key === 'Tab') {
      const focusables = [checkboxRef.current, okBtnRef.current].filter(Boolean) as HTMLElement[]
      if (focusables.length === 0) return
      const idx = focusables.findIndex((el) => el === document.activeElement)
      if (e.shiftKey && idx <= 0) {
        e.preventDefault()
        focusables[focusables.length - 1].focus()
      } else if (!e.shiftKey && idx === focusables.length - 1) {
        e.preventDefault()
        focusables[0].focus()
      }
    }
  }

  return (
    <div style={overlayStyle} onClick={handleClose} onKeyDown={handleKeyDown}>
      <div
        role="dialog"
        aria-modal={true}
        aria-label="Speed multiplier notice"
        style={dialogStyle}
        onClick={(e) => e.stopPropagation()}
      >
        <h3 style={titleStyle}>
          ⚠&nbsp; Speed multiplier notice
        </h3>
        <p style={bodyStyle}>
          At 2x or higher, driving performance may vary depending on
          network latency between this browser and the device. For
          consistent results, use 1x or run the simulator UI locally
          on the device.
        </p>
        <label style={checkboxRowStyle}>
          <input
            ref={checkboxRef}
            type="checkbox"
            checked={checked}
            onChange={(e) => setChecked(e.target.checked)}
            style={{ marginRight: 8, accentColor: '#00DF8F', cursor: 'pointer' }}
          />
          <span style={{ fontFamily: "'Geist', system-ui, sans-serif", fontSize: 13, color: '#cbd5e1' }}>
            Don&apos;t show this again
          </span>
        </label>
        <div style={{ display: 'flex', justifyContent: 'flex-end', marginTop: 20 }}>
          <button
            ref={okBtnRef}
            type="button"
            onClick={handleClose}
            style={okButtonStyle}
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
            OK
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
  marginBottom: 20,
  fontFamily: "'Geist', system-ui, sans-serif",
  fontSize: 13,
  lineHeight: 1.6,
  color: '#94a3b8',
}

const checkboxRowStyle: React.CSSProperties = {
  display: 'flex',
  alignItems: 'center',
  cursor: 'pointer',
}

const okButtonStyle: React.CSSProperties = {
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
