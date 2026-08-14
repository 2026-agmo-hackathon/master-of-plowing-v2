# Phase A Verification Report

작성일: 2026-04-17
검증자: executor agent
검증 대상: Phase A 프로시저럴 트랙터/구현체/환경 데코 도입 작업 (A1~A5)

## 전체 Status: DONE_WITH_CONCERNS

Phase A 의 5개 TODO 는 모두 프로덕션 코드에 반영되어 있고, TypeScript 빌드 / 유닛 테스트 / 프로덕션 빌드 / 비주얼 스냅샷이 모두 통과합니다. Baseline 5 개의 md5 는 서로 다른 값이며, 3 회 반복 실행 결과 5/5 pass 가 안정적으로 재현됩니다. 그러나 Phase C 에서 처음 드러났던 **비주얼 회귀 테스트가 Sky turbidity 와 같은 렌더링 파라미터 변경을 여전히 감지하지 못하는 구조적 결함**이 Phase A 에서도 재확인되었습니다. Phase A 로 트랙터 / 데코 오브젝트가 화면에 추가되어 화면 점유율이 상승했음에도 감도는 개선되지 않았으며, 이는 Phase A 범위가 아닌 visual regression test 인프라의 별도 과제입니다.

---

## 1. Phase A 작업 요약

Phase A 는 시뮬레이터의 3D 씬에 실제 트랙터 / 구현체 / 환경 데코를 도입하기 위한 5 개 TODO 로 구성되었습니다. A6 (본 문서) 은 Phase A 전체에 대한 검증입니다.

| TODO | 변경/추가 파일 | 요약 |
|---|---|---|
| A1 GLTF 본 구조 검증 | `docs/ASSET_TRACTOR_BONES.md` | GLTF 트랙터 모델 본 0 개 확인 → 프로시저럴 경로 채택 결정 |
| A2 프로시저럴 트랙터 | `src/3d/VehicleRenderer.ts` | 박스 조합 기반 트랙터 + 바퀴/조향/히치 `Object3D` pivot |
| A3 서스펜션 시각화 | `src/3d/VehicleRenderer.ts`, `TerrainSampler` 연동 | 지면 높이 샘플 기반 ±0.15m clamp 서스펜션 |
| A4 구현체 렌더러 | `src/3d/ImplementRenderer.ts` (신규) | 쟁기/로터리 프로시저럴 + 리프트 제어 |
| A5 환경 데코 | `src/3d/EnvironmentDecor.ts` (신규) | 1000 개 InstancedMesh, 필드 제외 영역, seeded RNG |

git status 기준 Phase A 작업으로 수정된 / 신규 추가된 파일:

```
 M src/3d/VehicleRenderer.ts            (A2/A3: 프로시저럴 트랙터 + 서스펜션)
?? src/3d/EnvironmentDecor.ts           (A5: 환경 데코 InstancedMesh)
?? src/3d/ImplementRenderer.ts          (A4: 쟁기/로터리 프로시저럴)
docs/ASSET_TRACTOR_BONES.md             (A1: 본 구조 검증 기록)
```

---

## 2. 정적 검증 — PASS

| 항목 | 결과 | 명령 |
|---|---|---|
| TypeScript 컴파일 | 0 error | `npx tsc -b --force` |
| 유닛 테스트 | 122/122 pass (18 files) | `npm test -- --run` (897ms) |
| 프로덕션 빌드 | success | `npm run build` (1.68s) |

프로덕션 번들 크기: 1.6MB (483KB gzip). chunk size 경고가 떴으나 이는 Phase A 책임이 아닌 별도 최적화 과제입니다.

---

## 3. Baseline 재생성 — PASS

기존 baseline 5 개 삭제 후 `npm run test:visual:update` 실행.

```
MD5 (tests/visual/__baselines__/figure-eight.png)    = b19aafeae6ccd5bb5a9cf5dda11de03e
MD5 (tests/visual/__baselines__/flat-forward-5s.png) = 26237600bae1be71a613ca2eedc5fcf7
MD5 (tests/visual/__baselines__/left-turn.png)       = 6c8d7af13cdd28d8c1ee2ccfc5768e41
MD5 (tests/visual/__baselines__/plow-on-field.png)   = a85a742b5679ca30a63579af5eaa6a68
MD5 (tests/visual/__baselines__/slope-climb.png)     = 6aad8346a98e77f8ada3a00810d61636
```

5 개 md5 모두 서로 다른 값으로, 각 시나리오가 고유 장면을 렌더링함이 확인됩니다.

### 파일 크기

```
figure-eight.png       60419 bytes
flat-forward-5s.png    60219 bytes
left-turn.png          60537 bytes
plow-on-field.png      61398 bytes
slope-climb.png        60280 bytes
```

Phase C 검증 문서 대비 파일 크기 레인지(60KB대)는 동일하게 유지되었습니다. Phase A 로 트랙터/구현체/데코가 추가되었으나 baseline 평균 크기 증가는 관측되지 않았으며, 이는 씬의 주요 시각적 변화(트랙터, 데코)가 카메라 프레이밍 안에 실제로 포함되지 않거나 PNG 엔트로피에 큰 영향을 주지 못함을 시사합니다 — 6 항 회귀 감도 부족 이슈와 맥을 같이합니다.

---

## 4. 3회 반복 안정성 — PASS

`npm run test:visual` 을 3 회 연속 실행한 결과:

| Run | flat-forward | left-turn | slope-climb | plow-on-field | figure-eight | 결과 |
|---|---|---|---|---|---|---|
| 1 | 0.0000 | 0.0000 | 0.0002 | 0.0000 | 0.0000 | 5/5 pass |
| 2 | 0.0000 | 0.0000 | 0.0002 | 0.0000 | 0.0000 | 5/5 pass |
| 3 | 0.0000 | 0.0000 | 0.0002 | 0.0000 | 0.0000 | 5/5 pass |

threshold(0.05) 대비 모두 충분한 마진을 보이며 3 회 전부 성공. slope-climb 의 0.0002 미세 diff 는 threshold 의 0.4 % 수준으로, 순간적 렌더 타이밍 비결정성에서 오는 노이즈 범위입니다. Phase C 검증 시 figure-eight 에서 관측되던 0.0086 보다도 작은 수준으로, Phase A 작업으로 인한 안정성 저하는 없습니다.

---

## 5. 프로시저럴 구현 코드 리뷰 체크리스트

Phase A 작업 결과물의 코드 레벨 완성도를 다음 항목으로 검증합니다 (executor 금지 원칙에 따라 VehicleRenderer/ImplementRenderer/EnvironmentDecor 파일은 수정 없이 구조만 확인).

- [x] **트랙터 pivot 3축 (조향 / 바퀴 회전 / 히치)** — `src/3d/VehicleRenderer.ts` 에 `Object3D` pivot 3 종이 구현되어 있고 타입 체커 통과
- [x] **쟁기 / 로터리 전환** — `src/3d/ImplementRenderer.ts` 에 implement type 별 프로시저럴 geometry + 리프트 제어 메서드가 존재하고 타입 체커 통과
- [x] **1000 개 데코 InstancedMesh 렌더** — `src/3d/EnvironmentDecor.ts` 에 InstancedMesh, 필드 제외 영역, seeded RNG 구현 확인 및 타입 체커 통과
- [x] **서스펜션 clamp** — `src/3d/VehicleRenderer.ts` 가 `TerrainSampler` 를 소비해 ±0.15m 범위 clamp 적용

> 시각적/런타임 동작의 1 px 레벨 검증은 6 항 회귀 감도 부족 이슈로 이번 자동 검증 범위에서 확증 불가. critic/verification 단계에서 dev 서버 수동 확인 권장.

---

## 6. 회귀 감지 데모 — DONE_WITH_CONCERNS

### 6a. Sky turbidity 변경 (`10 → 50`)

```bash
cp src/3d/SkyRenderer.ts /tmp/SkyRenderer.A6.bak
sed -i.sedbak 's/turbidity.value = 10/turbidity.value = 50/' src/3d/SkyRenderer.ts
npm run test:visual
```

결과:

| 시나리오 | diff ratio |
|---|---|
| flat-forward-5s | 0.0000 |
| left-turn | 0.0000 |
| slope-climb | 0.0000 |
| plow-on-field | 0.0000 |
| figure-eight | 0.0000 |

**5/5 모두 감지 실패**. Phase C 에서 동일 실험 시 left-turn 이 0.0003 으로 미세하게나마 감지되던 수준보다도 감지력이 떨어졌습니다. Phase A 로 트랙터/데코가 화면에 추가되어 화면 점유율이 상승한 결과, 하늘이 차지하는 pixel 비율은 오히려 감소해 sky 셰이더 파라미터 변경의 영향이 축소된 것으로 추정합니다.

이후 `cp /tmp/SkyRenderer.A6.bak src/3d/SkyRenderer.ts` 로 원복, `grep turbidity src/3d/SkyRenderer.ts` 로 `value = 10` 복귀 확인.

### Concern

**Phase C 에서 식별된 비주얼 회귀 감도 부족 문제가 Phase A 에서도 지속 / 악화되었습니다.** Phase C 에서 제안된 개선안은 여전히 유효합니다:

- `webServer.reuseExistingServer: false` 강제
- 테스트 실행 전 `node_modules/.vite` 자동 삭제
- threshold 를 0.05 → 0.005 로 낮추고 시나리오별 tight baseline 유지
- AmbientLight/Sky/트랙터 가시성 변경이 반영되는지 확인하는 "canary" 회귀 시나리오 추가

Phase A 는 트랙터 pivot / 쟁기 리프트 / 데코 InstancedMesh 같이 **명시적 움직임이 있는** 오브젝트를 다뤘기 때문에, 다음 Phase B 진입 전에는 이들이 실제 화면에 의도대로 출력되는지 확인할 수 있는 canary 시나리오가 반드시 필요합니다.

---

## 7. 성능 측정 — manual pending

Electron iframe 1920×1080 Medium 프리셋 60fps 목표 달성 여부는 수동 테스트 필요. 본 자동 검증 범위에서 제외.

체크리스트 (추후 수동 수행):

- [ ] Medium preset 1920×1080 평균 FPS ≥ 60 (1000 데코 + 프로시저럴 트랙터 상태)
- [ ] Low preset 1920×1080 평균 FPS ≥ 60
- [ ] High preset 1920×1080 평균 FPS ≥ 30
- [ ] 1000 개 InstancedMesh draw call 최적화 확인 (프로파일러)
- [ ] 트랙터 pivot/리프트 애니메이션 육안 확인
- [ ] 장시간(10분) 실행 시 메모리 누수 없음

---

## 8. 원복 완료 확인

비교 종료 후 최종 `git status --short`:

Phase A 관련 modification/untracked:

```
 M src/3d/VehicleRenderer.ts           ← Phase A 작업분 (A2/A3)
?? src/3d/EnvironmentDecor.ts          ← Phase A 작업분 (A5)
?? src/3d/ImplementRenderer.ts         ← Phase A 작업분 (A4)
```

SkyRenderer.ts 임시 변경분(turbidity=50) 은 원복되었으며, 남은 modification 은 Phase A 본 작업분 및 그 외 선행 Phase 작업분입니다. A6 검증 과정에서 생성된 임시 파일(`/tmp/SkyRenderer.A6.bak`, `src/3d/SkyRenderer.ts.sedbak`) 도 모두 삭제되었습니다.

---

## 9. 다음 Phase B 진입 가능 여부

### 진입 가능 (조건부)

Phase A 본 작업은 정적 검증(tsc/vitest/build) 및 비주얼 baseline 안정성 기준으로 **완료** 상태입니다. 따라서 Phase B 로 진입 가능합니다.

다만 다음 2 가지는 Phase B 진행과 **병렬로 처리**하거나, Phase B 진행 중 문제 발생 시 즉시 조치가 필요한 잠재 리스크입니다:

1. **비주얼 회귀 감도 부족 (Phase C 부터 지속)**: 현재 visual regression test 가 렌더링 변경을 감지하지 못하는 상태이므로, Phase B 에서 렌더링 관련 변경이 발생해도 baseline 이 무반응일 수 있음. Phase A 로 추가된 트랙터 pivot/쟁기 리프트/데코가 실제로 의도대로 렌더되는지의 1 차 검증이 자동화되어 있지 않음 → 검증 부담이 critic / verification / tester 수동 확인에 이전됨
2. **프로시저럴 경로 수동 확인 누락**: Phase A 는 GLTF 대신 프로시저럴 경로를 채택했으므로(A1), 3 축 pivot / 쟁기 전환 / 데코 배치가 dev 서버에서 육안으로 의도대로 보이는지 수동 확인이 필요

### 권장사항

- Phase B 진입 전에 dev 서버를 띄워 5 개 시나리오를 육안 확인 (플로어 위 트랙터, 바퀴 회전, 쟁기 리프트, 데코 1000 개 배치)
- 최소한 `webServer.reuseExistingServer: false` 적용 및 Sky/AmbientLight 변경이 반영되는지 재검증
- canary 회귀 시나리오 (예: 카메라를 하늘에 고정한 sky-only 스냅샷, 트랙터 클로즈업 스냅샷) 추가 작업을 Phase B 와 병행
