import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import Dashboard from './dashboard/Dashboard'

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 엔트리가 App(내장 3D 시뮬레이터)에서 Dashboard(자율주행 관측)로 바뀌었습니다.
// 시뮬레이션은 이제 SeamOS 머신 인터페이스 뒤의 외부 시뮬레이터가 담당하고,
// 이 UI 는 텔레메트리를 받아 보여주기만 합니다.
//
// src/ 아래의 옛 시뮬레이터 파일들(3d/, simulation/, components/, state/,
// protocol/)은 남겨 두었습니다 — ui/ 는 git 이 추적하지 않아 지우면 되돌릴 수
// 없기 때문입니다. 빌드에는 포함되지 않습니다(여기서 import 하지 않으므로).
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// The entry point moved from App (the built-in 3D simulator) to Dashboard. The
// simulation now lives in an external simulator behind the SeamOS machine
// interfaces, and this UI only displays telemetry.
//
// The old simulator sources under src/ are left in place because ui/ is not under
// git and deleting them would be unrecoverable. They are not built, since nothing
// imports them from here.
// └────────────────────────────────────────────────────────────────────────────┘
createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <Dashboard />
  </StrictMode>,
)
