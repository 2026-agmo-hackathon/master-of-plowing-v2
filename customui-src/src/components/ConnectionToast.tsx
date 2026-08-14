import { useSessionStore } from '../state/sessionStore';

export function ConnectionToast() {
  const wsConnected = useSessionStore(s => s.wsConnected);
  if (wsConnected) return null;
  return (
    <div
      style={{
        position: 'fixed',
        top: 0,
        left: 0,
        right: 0,
        background: '#a92020',
        color: 'white',
        padding: '8px 16px',
        textAlign: 'center',
        fontSize: 14,
        zIndex: 1000,
        fontFamily: 'system-ui, -apple-system, sans-serif',
      }}
      role="status"
      aria-live="polite"
    >
      Connection lost. Reconnecting...
    </div>
  );
}

export default ConnectionToast;
