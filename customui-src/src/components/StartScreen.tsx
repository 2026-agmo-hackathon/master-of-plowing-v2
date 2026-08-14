import { useState } from 'react';
import { useSessionStore } from '../state/sessionStore';
import { getSessionClient } from '../protocol/SessionClient';
import { IMPLEMENT_PRESETS, ImplementPresetKey } from '../protocol/SessionProtocol';
import TractorSelectStep from './TractorSelectStep';
import FieldSelectStep from './FieldSelectStep';

// ── Inline CSS (hover, keyframes, card classes) ───────────────────────────────
const STYLES = `
  @keyframes pulse {
    0%, 100% { opacity: 1; }
    50%       { opacity: 0.25; }
  }

  .mop-back-btn {
    width: 48px;
    height: 48px;
    display: flex;
    align-items: center;
    justify-content: center;
    background: transparent;
    border: 1px solid #4a525c;
    color: #e8eaed;
    font-size: 24px;
    cursor: pointer;
    position: fixed;
    top: 24px;
    left: 24px;
    z-index: 10;
    padding: 0;
  }
  .mop-back-btn:hover {
    border-color: #f59e0b;
    color: #f59e0b;
  }

  .mop-step-card {
    height: 70vh;
    flex: 1 1 0;
    min-width: 200px;
    max-width: 400px;
    display: flex;
    flex-direction: column;
    background: transparent;
    border: 1px solid #4a525c;
    color: #e8eaed;
    font-family: 'JetBrains Mono', monospace;
    cursor: pointer;
    padding: 0;
    transition: border-color 160ms ease, background 160ms ease, transform 160ms ease;
    position: relative;
  }
  .mop-step-card:hover { border-color: #f59e0b; background: rgba(245,158,11,0.04); }
  .mop-step-card:focus-visible { outline: 2px solid #f59e0b; outline-offset: 4px; }
  .mop-step-card.selected { border-color: #f59e0b; background: rgba(245,158,11,0.08); }
  .mop-step-card-art { flex: 1; display: flex; align-items: center; justify-content: center; padding: 8px; min-height: 0; }
  .mop-step-card-art > svg { width: 100%; height: 100%; max-height: 100%; }
  .mop-step-card-meta { padding: 20px 24px 24px; border-top: 1px solid rgba(74,82,92,0.5); text-align: center; }
  .mop-step-card-title { font-size: 24px; letter-spacing: 4px; color: #e8eaed; }
  .mop-step-card-sub { font-size: 13px; color: #8b929c; margin-top: 6px; letter-spacing: 2px; }
  @media (max-width: 1280px) {
    .mop-step-card { height: 60vh; max-width: 320px; }
    .mop-step-card-title { font-size: 20px; }
  }

  .mop-start-hero {
    cursor: pointer;
    transition: box-shadow 200ms ease, background 200ms ease, transform 200ms ease;
  }
  .mop-start-hero:hover:not(:disabled) {
    background: rgba(245,158,11,0.08);
    box-shadow: 0 0 80px rgba(245,158,11,0.35);
  }
  .mop-start-hero:focus-visible { outline: 2px solid #f59e0b; outline-offset: 6px; }
  .mop-start-hero:disabled { opacity: 0.4; cursor: not-allowed; }

  .mop-step-row {
    position: relative;
    display: flex;
    justify-content: center;
    align-items: center;
    width: 100vw;
    height: 100vh;
    gap: 24px;
    padding: 80px 24px 24px;
    box-sizing: border-box;
    z-index: 4;
  }
  .mop-step-row--wrap {
    flex-wrap: wrap;
    align-content: center;
  }
  .mop-step-row--wrap .mop-step-card { height: 45vh; max-width: 280px; }
  @media (max-width: 1280px) {
    .mop-step-row { gap: 16px; padding: 72px 16px 16px; }
  }

  @media (prefers-reduced-motion: reduce) {
    .mop-step-card, .mop-start-hero, .mop-back-btn { transition: none !important; }
  }

  .tc-cta-btn {
    transition: transform 160ms ease, box-shadow 160ms ease;
  }
  .tc-cta-btn:hover:not(:disabled) {
    transform: translateY(-1px);
    box-shadow: 0 12px 32px 0 rgba(0,223,143,0.36);
  }
  .tc-cta-btn:focus-visible {
    outline: 2px solid #00DF8F;
    outline-offset: 4px;
  }
`;

// ── IntroStep ─────────────────────────────────────────────────────────────────
function IntroStep({ onStart }: { onStart: () => void }) {
  const teamName    = useSessionStore(s => s.teamName);
  const setTeamName = useSessionStore(s => s.setTeamName);
  const [nameError, setNameError] = useState(false);

  function handleStart() {
    if (teamName.trim().length < 1) {
      setNameError(true);
      return;
    }
    onStart();
  }

  return (
    <div
      style={{
        position: 'relative',
        minHeight: '100vh',
        width: '100%',
        overflow: 'hidden',
        background: '#0d1117',
        fontFamily: "'Geist', system-ui, sans-serif",
        isolation: 'isolate',
      }}
    >
      {/* 1. Background field image — right side, heavy dark filter */}
      <img
        src="./title-card/bg-field.jpg"
        alt=""
        aria-hidden="true"
        style={{
          position: 'absolute',
          right: 0,
          top: 0,
          width: '62%',
          height: '100%',
          objectFit: 'cover',
          filter: 'brightness(0.42) contrast(1.1) saturate(0.55) hue-rotate(-10deg)',
          pointerEvents: 'none',
        }}
      />

      {/* 2. Multiply gradient over photo region — darken further with navy tone */}
      <div
        style={{
          position: 'absolute',
          right: 0,
          top: 0,
          width: '62%',
          height: '100%',
          background:
            'linear-gradient(rgba(20,32,44,0.55) 0%, rgba(13,24,32,0.20) 40%, rgba(8,16,22,0.60) 100%)',
          mixBlendMode: 'multiply',
          pointerEvents: 'none',
        }}
      />

      {/* 3. Radial vignette over photo region */}
      <div
        style={{
          position: 'absolute',
          right: 0,
          top: 0,
          width: '62%',
          height: '100%',
          background:
            'radial-gradient(ellipse at 50% 55%, transparent 38%, rgba(8,11,16,0.72) 100%)',
          pointerEvents: 'none',
        }}
      />

      {/* 4. Left side fade-to-dark — seamless transition to solid dark text area */}
      <div
        style={{
          position: 'absolute',
          inset: 0,
          background: 'linear-gradient(to left, transparent 35%, #0d1117 62%)',
          pointerEvents: 'none',
        }}
      />

      {/* Cloud SVG — top right */}
      <svg
        style={{ position: 'absolute', inset: 0, width: '100%', height: '100%', pointerEvents: 'none' }}
        viewBox="0 0 1600 1000"
        preserveAspectRatio="xMaxYMin slice"
        aria-hidden="true"
      >
        <defs>
          <radialGradient id="cloudGrad">
            <stop offset="0%" stopColor="rgba(255,255,255,0.25)" />
            <stop offset="100%" stopColor="rgba(255,255,255,0)" />
          </radialGradient>
        </defs>
        <ellipse cx="1300" cy="180" rx="320" ry="90" fill="url(#cloudGrad)" />
        <ellipse cx="1450" cy="240" rx="220" ry="70" fill="url(#cloudGrad)" />
        <ellipse cx="1150" cy="120" rx="240" ry="60" fill="url(#cloudGrad)" />
      </svg>

      {/* Logo — top left */}
      <img
        src="./title-card/logo.png"
        alt="SeamOS"
        style={{
          position: 'absolute',
          top: 28,
          left: 32,
          height: 28,
          width: 'auto',
          zIndex: 10,
        }}
      />

      {/* Main content — bottom left */}
      <div
        style={{
          position: 'absolute',
          left: 'clamp(32px, 6vw, 90px)',
          bottom: 140,
          zIndex: 10,
        }}
      >
        {/* Eyebrow badge */}
        <div
          style={{
            display: 'flex',
            flexDirection: 'row',
            alignItems: 'center',
            gap: 12,
            marginBottom: 20,
          }}
        >
          <div
            aria-hidden="true"
            style={{
              width: 32,
              height: 1,
              background: '#00DF8F',
              flexShrink: 0,
            }}
          />
          <span
            style={{
              fontSize: 12,
              fontWeight: 600,
              color: '#00DF8F',
              letterSpacing: '2.16px',
              textTransform: 'uppercase',
            }}
          >
            SEAMOS HACKATHON 2026
          </span>
        </div>

        {/* H1 title */}
        <h1
          style={{
            fontFamily: "'Geist', system-ui, sans-serif",
            fontWeight: 700,
            fontSize: 'clamp(64px, 9vw, 116px)',
            lineHeight: 0.92,
            color: '#e6edf3',
            letterSpacing: '-0.02em',
            margin: 0,
          }}
        >
          Master of<br />
          Plowing<span style={{ color: '#00DF8F' }}>.</span>
        </h1>

        {/* Description */}
        <p
          style={{
            fontSize: 20,
            fontWeight: 400,
            color: '#b8c2d0',
            lineHeight: 1.45,
            maxWidth: 560,
            letterSpacing: '-0.005em',
            marginTop: 24,
            marginBottom: 0,
          }}
        >
          The land is dry. Storms are closing in. Write the algorithm that plows the field before the rain falls — and the fastest team wins.
        </p>

        {/* Team name input */}
        <div style={{ marginTop: 28 }}>
          <input
            type="text"
            placeholder="Enter team name"
            value={teamName}
            onChange={(e) => {
              setTeamName(e.target.value);
              if (nameError && e.target.value.trim().length >= 1) setNameError(false);
            }}
            onKeyDown={(e) => {
              if (e.key === 'Enter') handleStart();
            }}
            aria-label="Team name"
            aria-invalid={nameError}
            aria-describedby={nameError ? 'team-name-error' : undefined}
            style={{
              width: '100%',
              maxWidth: 320,
              height: 48,
              padding: '0 16px',
              background: 'rgba(13,17,23,0.85)',
              border: nameError ? '1px solid #ef4444' : '1px solid rgba(255,255,255,0.12)',
              borderRadius: 8,
              color: '#e6edf3',
              fontSize: 16,
              fontWeight: 500,
              fontFamily: "'Geist', system-ui, sans-serif",
              outline: 'none',
              boxSizing: 'border-box',
              transition: 'border-color 160ms ease',
            }}
          />
          {nameError && (
            <p
              id="team-name-error"
              role="alert"
              style={{
                margin: '6px 0 0',
                fontSize: 12,
                color: '#ef4444',
                fontFamily: "'Geist', system-ui, sans-serif",
                letterSpacing: '0.02em',
              }}
            >
              Please enter your team name.
            </p>
          )}
        </div>

        {/* CTA button */}
        <button
          type="button"
          onClick={handleStart}
          className="tc-cta-btn"
          style={{
            display: 'flex',
            alignItems: 'center',
            gap: 8,
            marginTop: 16,
            padding: '14px 26px 14px 22px',
            width: 200,
            height: 58,
            borderRadius: 8,
            background: 'linear-gradient(180deg, #33e5a4 0%, #00DF8F 100%)',
            border: '1px solid #00DF8F',
            boxShadow: '0 8px 24px 0 rgba(0,223,143,0.22)',
            color: '#0a1a14',
            fontSize: 18,
            fontWeight: 700,
            fontFamily: "'Geist', system-ui, sans-serif",
            cursor: 'pointer',
            boxSizing: 'border-box',
          }}
        >
          <svg width="20" height="20" viewBox="0 0 15 15" fill="none" aria-hidden="true">
            <path d="M4 3 L12 7.5 L4 12 Z" fill="#0a1a14" />
          </svg>
          Start mission
        </button>

        {/* 3-step cards */}
        <div
          style={{
            display: 'flex',
            flexDirection: 'row',
            flexWrap: 'wrap',
            gap: 24,
            marginTop: 40,
            maxWidth: 632,
          }}
        >
          {[
            { num: '01', title: 'Build', desc: 'Wire your control loop in the SDK' },
            { num: '02', title: 'Run',   desc: 'Plow ≥ 95 % of field Alpha' },
            { num: '03', title: 'Submit', desc: 'Fastest qualifying time wins' },
          ].map((card) => (
            <div
              key={card.num}
              style={{
                background: 'rgba(21,27,35,0.82)',
                backdropFilter: 'blur(8px)',
                WebkitBackdropFilter: 'blur(8px)',
                border: '1px solid rgba(255,255,255,0.06)',
                borderRadius: 10,
                padding: '14px 16px',
                minWidth: 192,
                flex: '0 0 auto',
              }}
            >
              <div style={{ display: 'flex', alignItems: 'baseline', gap: 10 }}>
                <span
                  style={{
                    fontFamily: "'IBM Plex Mono', monospace",
                    fontSize: 11,
                    fontWeight: 600,
                    color: '#00DF8F',
                    letterSpacing: '0.04em',
                  }}
                >
                  {card.num}
                </span>
                <span
                  style={{
                    fontSize: 14,
                    fontWeight: 600,
                    color: '#e6edf3',
                    fontFamily: "'Geist', system-ui, sans-serif",
                  }}
                >
                  {card.title}
                </span>
              </div>
              <p
                style={{
                  fontSize: 12.5,
                  fontWeight: 400,
                  color: '#b8c2d0',
                  lineHeight: 1.4,
                  marginTop: 6,
                  marginBottom: 0,
                }}
              >
                {card.desc}
              </p>
            </div>
          ))}
        </div>
      </div>

      {/* Footer bar */}
      <div
        style={{
          position: 'absolute',
          bottom: 28,
          left: 32,
          right: 32,
          display: 'flex',
          justifyContent: 'space-between',
          alignItems: 'center',
          zIndex: 10,
        }}
      >
        <span style={{ fontSize: 11, fontWeight: 400, color: '#7a8494' }}>
          Max 7 km/h · ≥ 95 % coverage · reverse unplows
        </span>
      </div>
    </div>
  );
}

// ── StartScreen ───────────────────────────────────────────────────────────────
type Step = 'intro' | 'setup' | 'field';

export function StartScreen() {
  const [step, setStep] = useState<Step>('intro');

  const client          = getSessionClient();

  return (
    <>
      {/* Injected styles */}
      <style>{STYLES}</style>

      {step === 'intro' && (
        <IntroStep
          onStart={() => setStep('setup')}
        />
      )}

      {step === 'setup' && (
        <TractorSelectStep
          onBack={() => setStep('intro')}
          onConfirm={(tractorKey, implementKey) => {
            client.sendSetTractorPreset(tractorKey);
            client.sendSetImplementPreset(implementKey);
            setStep('field');
          }}
        />
      )}

      {step === 'field' && (
        <FieldSelectStep
          onBack={() => setStep('setup')}
          onLaunch={(_mapId: number) => {
            const { teamName, implementPreset, setLaunching } = useSessionStore.getState();
            const implementWidth = IMPLEMENT_PRESETS[implementPreset as ImplementPresetKey]?.width ?? 0;
            setLaunching(true);
            try {
              client.sendStartSession(teamName, implementWidth);
            } catch {
              setLaunching(false);
            }
          }}
        />
      )}
    </>
  );
}

export default StartScreen;
