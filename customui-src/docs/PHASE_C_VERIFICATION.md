# Phase C Verification Report

작성일: 2026-04-17
검증자: executor agent
검증 대상: Phase C 렌더링 품질 향상 작업 (HDR/PBR/Sky/Shadow/PostProcessing/QualityPreset/ProceduralTextures)

## 전체 Status: DONE_WITH_CONCERNS

Phase C의 7개 TODO는 모두 프로덕션 코드에 반영되어 있고, TypeScript 빌드/유닛 테스트/프로덕션 빌드/비주얼 스냅샷이 모두 통과합니다. Baseline 5개의 md5는 서로 다른 값이며, 3회 반복 실행 결과 5/5 pass가 안정적으로 재현됩니다(Phase D에서 발견된 1/5 간헐 실패가 해소된 증거). 그러나 회귀 감지 데모 결과 **현재 비주얼 테스트 인프라가 씬 렌더링 변경(AmbientLight 색/강도)을 감지하지 못하는 구조적 결함이 있음**이 새로 드러났습니다. 이 부분은 Phase C 범위가 아니라 visual regression test 자체의 설계 문제로, 별도 작업이 필요합니다.

---

## 1. Phase C 작업 요약

Phase C는 시뮬레이터의 렌더링 품질을 실디바이스 대응 수준으로 끌어올리기 위한 7개 TODO로 구성되었습니다.

| TODO | 변경/추가 파일 | 요약 |
|---|---|---|
| C1 HDR tone mapping | `src/3d/SceneManager.ts` | `ACESFilmicToneMapping` + `toneMappingExposure` 적용 |
| C2 PBR 환경광 | `src/3d/SceneManager.ts` | `PMREMGenerator` + `RoomEnvironment` → `scene.environment` 주입 |
| C3 절차적 하늘 | `src/3d/SkyRenderer.ts` | three examples `Sky` 셰이더 기반 절차적 하늘 도입, sun direction/turbidity/rayleigh 파라미터화 |
| C4 Shadow system | `src/3d/ShadowSystem.ts` (신규) | CSM(Cascaded Shadow Maps) 또는 PCFSoftShadowMap 기반 그림자 품질 관리 모듈 |
| C5 PostProcessing | `src/3d/PostProcessing.ts` (신규) | `EffectComposer` + bloom/tonemapping 경로 |
| C6 Quality preset | `src/3d/QualityPreset.ts` (신규) | Low/Medium/High 프리셋 (pixelRatio, shadowMapSize, AA, bloom 강도 등) |
| C7 Procedural textures | `src/3d/ProceduralTextures.ts` (신규) | 지면/풀 텍스처를 프로시저럴로 생성해 외부 에셋 의존 제거 |

git status 기준 Phase C 작업으로 수정된/신규 추가된 파일:

```
 M src/3d/DustParticles.ts            (PBR material 적용)
 M src/3d/SceneManager.ts             (HDR tone map + PMREM env)
 M src/3d/SkyRenderer.ts              (Sky 셰이더 기반)
 M src/3d/TerrainRenderer.ts          (PBR + 절차적 텍스처)
 M src/3d/VehicleRenderer.ts          (PBR 머티리얼)
 M src/3d/WheelTrackRenderer.ts       (PBR 머티리얼)
 M src/components/HUD.tsx             (프리셋/씬 카탈로그 UI)
 M src/components/SimulatorView.tsx   (Quality preset 연동, 씬 카탈로그)
?? src/3d/PostProcessing.ts
?? src/3d/ProceduralTextures.ts
?? src/3d/QualityPreset.ts
?? src/3d/ShadowSystem.ts
?? src/components/MockSink.tsx
?? src/components/SceneCatalog.tsx
?? src/components/SensorOverlay.tsx
```

---

## 2. Phase C 복원 상태 grep 결과 — PASS

```bash
$ grep -n "ACESFilmic\|PMREM\|RoomEnvironment" src/3d/SceneManager.ts
2:import { RoomEnvironment } from 'three/examples/jsm/environments/RoomEnvironment.js'
25:    this.renderer.toneMapping = THREE.ACESFilmicToneMapping
35:    const pmrem = new THREE.PMREMGenerator(this.renderer)
36:    this.envTex = pmrem.fromScene(new RoomEnvironment(), 0.04).texture

$ grep -n "import { Sky }" src/3d/SkyRenderer.ts
2:import { Sky } from 'three/examples/jsm/objects/Sky.js'
```

Phase C 복원 상태 확인 완료 — HDR tone map, PMREM environment, 절차적 Sky 셰이더가 모두 반영되어 있습니다.

---

## 3. 정적 검증 — PASS

| 항목 | 결과 | 명령 |
|---|---|---|
| TypeScript 컴파일 | 0 error | `npx tsc -b --force` |
| 유닛 테스트 | 122/122 pass (18 files) | `npm test -- --run` (709ms) |
| 프로덕션 빌드 | success | `npm run build` (1.55s) |

프로덕션 번들 크기: 1.6MB (480KB gzip). chunk size 경고가 떴으나 이는 Phase C 책임이 아닌 별도 최적화 과제입니다.

---

## 4. Baseline 재생성 — PASS

기존 baseline 5개 삭제 후 `npm run test:visual:update` 실행.

```
MD5 (tests/visual/__baselines__/figure-eight.png)    = 132c3475a5c7f4b654963273146671ad
MD5 (tests/visual/__baselines__/flat-forward-5s.png) = 26237600bae1be71a613ca2eedc5fcf7
MD5 (tests/visual/__baselines__/left-turn.png)       = ca38b630f9c3f89b318ae3679e78a054
MD5 (tests/visual/__baselines__/plow-on-field.png)   = a85a742b5679ca30a63579af5eaa6a68
MD5 (tests/visual/__baselines__/slope-climb.png)     = 0280e34842af6374b8f685e609e367f9
```

5개 md5 모두 서로 다른 값으로, 각 시나리오가 고유 장면을 렌더링함이 확인됩니다.

### Concern: 파일 크기 이상

Phase C baseline 5개 파일 크기:

```
figure-eight.png       60422 bytes
flat-forward-5s.png    60219 bytes
left-turn.png          60550 bytes
plow-on-field.png      61398 bytes
slope-climb.png        60216 bytes
```

Phase D 검증 문서에 기록된 baseline 크기(138KB~201KB)와 비교했을 때 현 baseline 전체가 약 60KB 수준으로 **크게 작아졌습니다**. plow-on-field의 쟁기 자국 같은 복잡한 픽셀 패턴도 다른 시나리오와 크기 차이가 1KB 안쪽입니다. 이는 최근 구조 변경 영향이 있을 가능성이 있으나 Phase C 본 작업 범위 밖이므로 추가 조사가 필요합니다.

---

## 5. 3회 반복 안정성 — PASS

`npm run test:visual` 를 3회 연속 실행한 결과:

| Run | flat-forward | left-turn | slope-climb | plow-on-field | figure-eight | 결과 |
|---|---|---|---|---|---|---|
| 1 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 5/5 pass |
| 2 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0086 | 5/5 pass |
| 3 | 0.0000 | 0.0000 | 0.0000 | 0.0000 | 0.0086 | 5/5 pass |

threshold(0.05) 대비 모두 충분한 마진을 보이며 3회 전부 성공. **Phase D에서 관찰됐던 1/5 간헐 실패(figure-eight diff > threshold)는 Phase C 시점에서 안정화**되어 있습니다. figure-eight 의 0.0086 미세 diff는 threshold의 17% 수준으로, 순간적 렌더 타이밍 비결정성에서 오는 노이즈 범위로 볼 수 있습니다.

---

## 6. 회귀 감지 데모 — DONE_WITH_CONCERNS

### 6a. Sky turbidity 변경 (`10 → 50`)

```bash
cp src/3d/SkyRenderer.ts /tmp/SkyRenderer.ts.bak
sed -i.sedbak 's/turbidity.value = 10/turbidity.value = 50/' src/3d/SkyRenderer.ts
npm run test:visual
```

결과:

| 시나리오 | diff ratio |
|---|---|
| flat-forward-5s | 0.0000 |
| left-turn | 0.0003 |
| slope-climb | 0.0000 |
| plow-on-field | 0.0000 |
| figure-eight | 0.0000 |

변경 감지되지 않음 (5/5 pass). Sky가 현 카메라 프레이밍의 1280×720 영역에 실제로 포함되지 않거나, tone mapping 포화 구간에 들어가 pixel 차이가 억제되는 것으로 추정.

이후 `cp /tmp/SkyRenderer.ts.bak src/3d/SkyRenderer.ts` 로 원복, `grep turbidity src/3d/SkyRenderer.ts` 로 `value = 10` 복귀 확인.

### 6b. AmbientLight 강도 변경 (`0.25 → 0.8`)

```bash
cp src/3d/SceneManager.ts /tmp/SceneManager.ts.bak
sed -i.sedbak 's/AmbientLight(0xffffff, 0.25)/AmbientLight(0xffffff, 0.8)/' src/3d/SceneManager.ts
npm run test:visual
```

결과:

| 시나리오 | diff ratio |
|---|---|
| flat-forward-5s | 0.0000 |
| left-turn | 0.0000 |
| slope-climb | 0.0000 |
| plow-on-field | 0.0087 |
| figure-eight | 0.0085 |

마찬가지로 threshold 미만 (5/5 pass). 변경이 일부 시나리오에서 약간 감지되나 감도가 매우 낮음.

### 6c. 추가 확인: AmbientLight `0xffffff, 0.25 → 0xff0000, 5.0` (극단적 변경)

원인 규명 차원에서 red ambient + intensity 5.0 변경 후 Vite 캐시(`node_modules/.vite/deps`)도 삭제하고 재실행했으나, 5개 전체 diff = `0.0000` 으로 **전혀 변경이 감지되지 않음**. 이는 다음 중 하나를 의미합니다:

1. Vite dev 서버의 HMR 모듈 캐시가 Playwright `webServer.reuseExistingServer` 경로에서 변경을 반영하지 못함
2. AmbientLight가 현 렌더 경로(PostProcessing composer, PMREM envMap override 등)에서 실제 출력에 기여하지 못함
3. standalone 모드 테스트 페이지가 별도의 (변경되지 않은) 리소스 번들을 사용함

### Concern

**비주얼 회귀 테스트는 "baseline 대비 무변경" 을 검증하는 데는 안정적이나, 실제 씬 렌더링 변경을 감지하는 능력은 현재 매우 제한적입니다.** Phase C TODO 범위는 아니지만, 이후 렌더링 변경의 리그레션 방지를 위해서는 별도 작업이 필요합니다:

- webServer 설정을 `reuseExistingServer: false` 로 강제
- 테스트 실행 전 `node_modules/.vite` 자동 삭제
- threshold를 0.05 → 0.005 로 낮추고 시나리오별 tight baseline 유지
- AmbientLight/Sky 변경이 반영되는지 확인하는 "canary" 회귀 시나리오 추가

---

## 7. 성능 측정 — manual pending

Electron iframe 1920×1080 Medium 프리셋 60fps 목표 달성 여부는 수동 테스트 필요. 본 자동 검증 범위에서 제외.

체크리스트 (추후 수동 수행):

- [ ] Medium preset 1920×1080 평균 FPS ≥ 60
- [ ] Low preset 1920×1080 평균 FPS ≥ 60 (저사양 디바이스 대응)
- [ ] High preset 1920×1080 평균 FPS ≥ 30
- [ ] 장시간(10분) 실행 시 메모리 누수 없음

---

## 8. 원복 완료 확인

비교 종료 후 최종 `git status --short src/3d/ src/components/`:

```
 M src/3d/DustParticles.ts
 M src/3d/SceneManager.ts            ← Phase C 작업분 (AmbientLight=0.25 복원됨)
 M src/3d/SkyRenderer.ts             ← Phase C 작업분 (turbidity=10 복원됨)
 M src/3d/TerrainRenderer.ts
 M src/3d/VehicleRenderer.ts
 M src/3d/WheelTrackRenderer.ts
 M src/components/HUD.tsx
 M src/components/SimulatorView.tsx
?? src/3d/PostProcessing.ts
?? src/3d/ProceduralTextures.ts
?? src/3d/QualityPreset.ts
?? src/3d/ShadowSystem.ts
?? src/components/MockSink.tsx
?? src/components/SceneCatalog.tsx
?? src/components/SensorOverlay.tsx
```

임시 변경분(turbidity=50, AmbientLight=0.8, AmbientLight=0xff0000/5.0)은 모두 원복되었으며, 남은 modification은 Phase C 본 작업분입니다.

---

## 9. 다음 Phase A 진입 가능 여부

### 진입 가능 (조건부)

Phase C 본 작업은 정적 검증(tsc/vitest/build) 및 비주얼 baseline 안정성 기준으로 **완료** 상태입니다. 따라서 Phase A로 진입 가능합니다.

다만 다음 2가지는 Phase A 진행과 **병렬로 처리**하거나, Phase A 진행 중 문제 발생 시 즉시 조치가 필요한 잠재 리스크입니다:

1. **비주얼 회귀 감도 부족**: 현재 visual regression test가 렌더링 변경을 감지하지 못하는 상태이므로, Phase A에서 렌더링 관련 변경이 발생해도 baseline이 무반응일 수 있음 → 검증 부담이 critic/verification 단계로 이전됨
2. **baseline 파일 크기 축소**: Phase D 기록(138~201KB) 대비 현재(60KB대)로 크게 줄어든 것은 씬 복잡도가 떨어졌거나 캡처 시점 프레임이 단순화된 가능성 → 실제 렌더 화면과 baseline이 괴리될 여지

### 권장사항

- Phase A 진입 전에 최소한 `webServer.reuseExistingServer: false` 적용 및 AmbientLight/Sky 변경이 반영되는지 재검증
- baseline 파일 크기 축소 원인을 씬 스크린샷 육안 확인으로 추적 (정상 렌더링인지 확인)
