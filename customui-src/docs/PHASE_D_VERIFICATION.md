# Phase D Verification Report

작성일: 2026-04-17 (재검증)
검증자: executor agent
검증 대상: simulator standalone 모드 + 시나리오 실행 + visual snapshot 회귀 테스트 (TODO D1~D9)

## 전체 Status: DONE_WITH_CONCERNS

Phase D의 race condition(App.tsx → SimulatorView scenario prop drilling)은 해소되어, 시나리오가 실제로 주행하는 baseline이 재생성되었습니다. 5개 baseline md5가 서로 다르고 시각적으로도 서로 다른 장면을 담고 있음이 확인되었습니다. 다만 회귀 감지 데모는 **시그널(Sky 색 변경)과 노이즈 플로어(baseline 재현 불안정성)가 겹치는 구간이 있어 run마다 결과가 달라지는 불안정한 상태**입니다. 근본 원인은 렌더러의 Math.random() 사용과 3000ms 타이밍 기반 캡처의 비결정성으로, Phase D 범위에서 해결이 어려운 구조적 문제입니다.

---

## 1. Baseline 재생성 결과 — PASS

scenario prop drilling 리팩토링(App.tsx → SimulatorView에 `scenario` prop 직접 전달) 이후 기존 baseline 삭제 후 `npm run test:visual:update` 로 재생성. 5개 모두 md5가 서로 다름.

```
MD5 (tests/visual/__baselines__/figure-eight.png)    = 4f509b25c27dc603d6d9d9fd0a45579f
MD5 (tests/visual/__baselines__/flat-forward-5s.png) = 31e4d1a4e6dda27f4f2fd790c7695c53
MD5 (tests/visual/__baselines__/left-turn.png)       = 4debb269d13dda4e10803eb332824c48
MD5 (tests/visual/__baselines__/plow-on-field.png)   = 59fe09ffc6db91fa5e36aed81c0d3cd4
MD5 (tests/visual/__baselines__/slope-climb.png)     = 8e19ff59b1a65a4dd34131935bb6728d
```

파일 크기도 분화됨 (138KB, 137KB, 137KB, 201KB, 199KB). plow-on-field는 쟁기 내림 궤적이 지면에 남아 다른 씬보다 픽셀 복잡도가 높아 PNG 압축 후에도 크기가 큼. 시각적 확인 결과:

- `flat-forward-5s` — 전방 주행, 지면 줄무늬, HDG 0°
- `left-turn` — HDG 360° 방향 전환 후 정면, 스티어링 스냅 후 정지 상태
- `plow-on-field` — 쟁기 내려가 밭이 갈린 줄무늬 씬
- 나머지 2개(slope-climb, figure-eight)도 md5 서로 다름 확인

즉, 기존 보고서의 "5개 baseline 모두 동일 (md5=d813bb1f...)" 결함은 해소됨.

## 2. 회귀 감지 데모 (Sky 0x87CEEB → 0xff0000) — UNSTABLE

`src/3d/SkyRenderer.ts` line 32의 `topColor: new THREE.Color(0x87CEEB)` → `new THREE.Color(0xff0000)` 로 변경 후 `npm run test:visual` 실행.

### 2a. 회귀 주입 실행

```
[visual] flat-forward-5s diff ratio=0.0294
[visual] left-turn        diff ratio=0.0595  ← FAIL (> 0.05)
[visual] slope-climb      diff ratio=0.0077
[visual] plow-on-field    diff ratio=0.0761  ← FAIL (> 0.05)
[visual] figure-eight     diff ratio=0.0436
Result: 2 failed, 3 passed
```

**2/5 fail** — 지시사항의 "최소 1개 fail" 기준은 충족.

### 2b. 원복 후 재실행

`git checkout -- src/3d/SkyRenderer.ts` 후 `npm run test:visual` 실행:

```
[visual] flat-forward-5s diff ratio=0.0252
[visual] left-turn        diff ratio=0.0362
[visual] slope-climb      diff ratio=0.0157
[visual] plow-on-field    diff ratio=0.0707  ← FAIL (> 0.05)
[visual] figure-eight     diff ratio=0.0369
Result: 1 failed, 4 passed
```

**원복 후에도 plow-on-field가 0.0707 > THRESHOLD 0.05 로 fail**. 이는 baseline 자체가 재현 불가능하다는 뜻 — SkyRenderer 변경과 무관하게 실행마다 diff가 흔들림. 추가 2회 확인:

```
Run A (원복 상태): plow-on-field=0.1316, left-turn=0.0827
Run B (원복 상태): plow-on-field=0.0944, left-turn=0.0499
```

즉 **baseline 노이즈 플로어가 THRESHOLD(0.05)를 주기적으로 초과**함. Sky 색 변경의 diff 크기(0.008~0.076)와 겹치는 구간이 있어 시그널/노이즈 분리가 불가능.

### 2c. 근본 원인

- `src/3d/TerrainRenderer.ts` 63–138 line: 식생/제방 배치가 `Math.random()` 기반. 각 페이지 로드마다 다른 배치.
- `src/3d/VehicleRenderer.ts` 101–108, `src/3d/DustParticles.ts` 47–53: 먼지 파티클도 매 프레임 `Math.random()` 호출.
- `ScenarioRunner`가 `requestAnimationFrame` 기반이라 wall-clock 타이밍 의존. 3000ms waiting 시점에 시뮬레이션이 정확히 같은 상태에 있으리라는 보장 없음.
- `plow-on-field`가 특히 심한 것은 쟁기 궤적의 누적 효과 때문으로 추정.

## 3. git status 확인 — PASS

`git status --porcelain src/` 출력에 SkyRenderer.ts 변경 없음. src/ 내 변경/미추적 파일은 전부 Phase D 전/중 작업한 것이며, 이번 TODO에서 추가/수정한 src 파일은 없음.

```
 M simulator/src/App.tsx
 M simulator/src/components/SimulatorView.tsx
 M simulator/src/simulation/DynamicVehicle.ts
 M simulator/src/simulation/GameLoop.ts
 M simulator/src/simulation/KinematicVehicle.ts
 M simulator/src/simulation/MqttClient.ts
 M simulator/src/simulation/VehicleSystem.ts
 M simulator/src/vite-env.d.ts
?? simulator/src/components/MockSink.tsx
?? simulator/src/components/SceneCatalog.tsx
?? simulator/src/components/SensorOverlay.tsx
?? simulator/src/simulation/ApiBridge.ts
?? simulator/src/simulation/ScenarioRunner.ts
?? simulator/src/simulation/ScenarioTypes.ts
?? simulator/src/simulation/SignalControllerClient.ts
?? simulator/src/simulation/StandaloneMode.ts
?? simulator/src/simulation/__tests__/ScenarioTypes.test.ts
?? simulator/src/simulation/__tests__/StandaloneMode.test.ts
```

이번 TODO에서는 `tests/visual/__baselines__/*.png` (5개 재생성) 과 `docs/PHASE_D_VERIFICATION.md` (본 문서) 만 변경.

---

## 남은 우려사항

1. **HIGH — Visual snapshot 노이즈 플로어가 THRESHOLD에 근접/초과**: 현재 THRESHOLD=0.05에서 baseline 대비 자체 노이즈가 0.01~0.13 범위를 오감. 특히 `plow-on-field`와 `left-turn`은 run마다 THRESHOLD를 넘기도 함. 이 상태로는 "형식상 pass/fail"로 회귀 여부를 판단할 수 없음. Phase C 진입 전 또는 Phase D를 완전히 닫기 전에 아래 중 하나 이상이 필요:
   - 렌더러에 **seeded RNG** 도입 (Math.random → seedrandom 등), standalone 모드에서만 고정 시드
   - 시나리오 완전 결정론화: `requestAnimationFrame` 대신 Playwright 제어 가능한 virtual clock (Three.js clock.oldTime 고정)
   - 캡처 전 `page.waitForFunction(() => window.__scenarioComplete === true)` 같은 동기화 훅 도입

2. **MEDIUM — Sky 변경의 diff 기여도가 작음**: 현재 카메라 시점은 3인칭 오버헤드로 뷰의 상단 ~20%만 Sky. Fog 색(0xb7d4e8)이 여전히 지면 영역을 덮고 있어 Sky 색 변경은 local diff일 뿐. 회귀 데모를 안정화하려면 "지면 색 변경" 같이 뷰 대부분을 덮는 회귀로 바꾸거나, diff ratio가 아닌 per-region diff 로 평가해야 함.

3. **MEDIUM — THRESHOLD 0.05 는 지나치게 관대하지만, 낮출 수 없음**: THRESHOLD=0.01로 낮추면 Sky 색 변경을 감지할 수 있지만 baseline 재현 불가능성 때문에 평소에도 5/5 fail. 즉 현재 인프라로는 "Sky 변경을 감지하면서 baseline을 재현"할 수 있는 단일 THRESHOLD가 존재하지 않음.

4. `playwright.config.ts` 의 `reuseExistingServer: !process.env.CI` 는 여전히 유효. 로컬에서 `npm run dev` 띄운 상태로 test:visual 실행 시 구 코드 서빙 위험 있음.

5. 기존 Phase D 보고서의 다른 우려사항 (onGear no-op, chunk size 경고, MqttClient traffic listener 설계 등) 은 해당 분야 변경 없어 그대로 유효.

---

## 다음 Phase C 진입 가능 여부: **CONDITIONAL_YES**

핵심 기능(scenario 자동 실행, baseline이 실제 주행 장면 반영)은 동작함. Race condition 해소가 가장 큰 결함이었고 그 부분은 복구됨. 그러나 회귀 감지 데모는 **구조적 비결정성 때문에 안정화되지 않은 상태**이며 이는 Phase D 인프라의 한계.

- Phase C로의 진입은 **가능**하되, 위 우려사항 1번 (렌더러 결정론화) 은 Phase C 병행 또는 직후에 반드시 처리.
- 현재 `test:visual` 은 "red-flag 알람" 정도로만 신뢰 가능 — 1~2개 fail이 나오면 주의 깊게 원인 분석 필요, 하지만 5/5 pass 도 "회귀 없음"을 보장하지 않음.
- CI에서 `test:visual` 을 `required` 로 승격하려면 1번 해결 후여야 함.

---

## 증거 파일 경로 (absolute)

- 재생성된 baseline (5개 서로 다른 md5): `/Users/hobeen/agmo/simulationworld/simulator/tests/visual/__baselines__/*.png`
- snapshot spec (THRESHOLD=0.05 유지): `/Users/hobeen/agmo/simulationworld/simulator/tests/visual/snapshot.spec.ts`
- compare 유틸 (pixelmatch): `/Users/hobeen/agmo/simulationworld/simulator/tests/visual/compare.ts`
- scenario prop drilling 수정된 App: `/Users/hobeen/agmo/simulationworld/simulator/src/App.tsx`
- scenario prop 수신 SimulatorView: `/Users/hobeen/agmo/simulationworld/simulator/src/components/SimulatorView.tsx`
- 비결정성 원천 (Math.random): `/Users/hobeen/agmo/simulationworld/simulator/src/3d/TerrainRenderer.ts`, `/Users/hobeen/agmo/simulationworld/simulator/src/3d/DustParticles.ts`, `/Users/hobeen/agmo/simulationworld/simulator/src/3d/VehicleRenderer.ts`
- 회귀 주입 대상: `/Users/hobeen/agmo/simulationworld/simulator/src/3d/SkyRenderer.ts` (line 32 topColor)
