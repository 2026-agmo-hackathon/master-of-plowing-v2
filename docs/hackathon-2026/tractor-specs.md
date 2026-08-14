# 트랙터 물리 제원 (2026 Agritechnica 해커톤)

시뮬레이터에 탑재된 트랙터 3종(소형/중형/대형)의 물리 제원 — 자율주행 알고리즘 설계 시 기준값으로 사용하십시오.

> 모든 값은 시뮬레이터 코드에서 추출됨 (커밋 기준: `835c785`, 트랙터 스펙 최종 변경 커밋 `ef07df6`, 2026-08-10).
> 소스: `simulator/src/simulation/tractors/` (jd5050e.ts / jd6100m.ts / jd6155m.ts, 타입·유도식은 types.ts)

## 트랙터 모델

| ID | 모델명 | 체급 |
|---|---|---|
| `compact` | JD 5050E | 소형 (기본 선택값[^default]) |
| `medium` | JD 6100M | 중형 |
| `large` | JD 6155M | 대형 4WD |

## 1. 체급별 섀시 제원

| 항목 | 단위 | 소형 (JD 5050E) | 중형 (JD 6100M) | 대형 (JD 6155M) |
|---|---|---:|---:|---:|
| 축거 (wheelbase)[^wheelbase] | m | 2.05 | 2.5 | 3.1 |
| 최소선회반경[^turnradius] | m | 3.2 | 4.0 | 4.9 |
| 최대 앞바퀴 조향각[^maxangle] | deg | 32.6 | 32.01 | 32.31 |
| 윤거 전/후 (track)[^track] | m | 1.251 / 1.182 | 1.526 / 1.441 | 1.892 / 1.787 |
| 총 질량 (밸러스트 포함)[^mass] | kg | 3,000 | 4,300 | 8,500 |
| 축하중 전/후[^axle] | kg | 1,000 / 1,500 | 1,700 / 2,600 | 3,400 / 5,100 |
| 전륜 하중비 (물리 모델)[^fwr] | — | 0.4 | 0.4 | 0.4 |
| 무게중심 높이[^cg] | m | 0.95 | 1.11 | 1.3 |
| 구동륜(후륜) 타이어 반경[^tirer] | m | 0.70 | 0.86 | 1.05 |
| 작업기 후방 부하 상한[^impl] | kg | 500 | 750 | 1,000 |

- 최대 조향각은 자전거 모델 기하로 유도됩니다: `δ_max = atan(축거 / 최소선회반경)`[^derive]. 선회반경 R 과 앞바퀴각 δ 의 관계는 `R = L / tan(δ)` (L = 축거).
- 소형의 축하중 합(2,500kg)은 총 질량(3,000kg)과 다릅니다 — 총 질량만 3,000kg 으로 상향된 이력이 있으며, 물리 모델의 전후 배분은 `frontWeightRatio = 0.4` 를 사용합니다[^fwr].

## 2. 조향 물성치

| 항목 | 단위 | 소형 | 중형 | 대형 |
|---|---|---:|---:|---:|
| 조향비 (핸들 ↔ 앞바퀴)[^ratio] | — | 20 : 1 | 20 : 1 | 20 : 1 |
| 핸들 최대 회전 각속도[^rate] | deg/s | 435 | 346 | 264 |
| 앞바퀴 환산 최대 각속도[^ratecalc] | deg/s | 21.75 | 17.3 | 13.2 |
| 핸들 락투락(lock-to-lock) 시간[^l2l] | s | 3.0 | 3.7 | 4.5 |
| 핸들 최대 회전각 (편측)[^maxsw] | deg | 652 | 640 | 646 |
| 핸들 응답 시정수 (τ)[^swtau] | s | 0.20 | 0.25 | 0.30 |
| 앞바퀴 응답 시정수 (τ)[^fwtau] | s | 0.05 | 0.07 | 0.09 |
| 소프트리밋 시작 (풀락 대비)[^softlim] | — | 80% | 80% | 80% |

- 핸들 명령 → 앞바퀴각은 1차 지연(τ) + 각속도 상한으로 모델링됩니다. 풀락의 80% 를 넘으면 조향 속도가 점감(최소 30%)합니다[^softlim].
- 예: 소형 전타→전타(앞바퀴 −32.6°→+32.6°)에 약 3.0초 소요.

## 3. 엔진 / 구동계

| 항목 | 단위 | 소형 | 중형 | 대형 |
|---|---|---:|---:|---:|
| 정격 출력[^hp] | hp | 50 | 100 | 210 |
| 아이들 rpm[^idle] | rpm | 850 | 800 | 750 |
| 정격 rpm[^rated] | rpm | 2,100 | 2,200 | 2,000 |
| 스톨 rpm[^stall] | rpm | 600 | 580 | 550 |
| 최대(피크) 토크[^curve] | Nm @ rpm | 220 @ 1,400 | 420 @ 1,500 | 1,000 @ 1,400 |
| 정격점 토크[^curve] | Nm @ rpm | 180 @ 2,100 | 324 @ 2,200 | 750 @ 2,000 |
| 종감속비[^final] | — | 8.5 | 8.8 | 9.2 |
| 전진 기어 단수[^gears] | — | 4 | 5 | 6 |
| 최고 작업속도 (최상단 기어)[^speeds] | m/s (km/h) | 3.33 (12.0) | 4.2 (15.1) | 5.2 (18.7) |
| 후진 최고속도[^rev] | m/s (km/h) | 1.0 (3.6) | 1.0 (3.6) | 1.0 (3.6) |
| 필드 최대 가속도[^accel] | m/s² | 0.8 | 0.68 | 0.55 |
| 필드 최대 감속도(브레이크)[^brake] | m/s² | 1.5 | 1.35 | 1.2 |
| 최대 제동 토크 (4륜 합산)[^brktq] | Nm | 9,000 | 14,800 | 28,500 |
| 최대 횡가속도[^lat] | m/s² | 1.1 | 1.05 | 1.0 |

### 엔진 토크 커브[^curve]

토크는 rpm-토크 점 사이 선형 보간으로 계산됩니다[^interp].

| rpm → 토크(Nm) | 소형 | 중형 | 대형 |
|---|---|---|---|
| 점 1 | 850 → 160 | 800 → 260 | 750 → 820 |
| 점 2 | 1,200 → 210 | 1,200 → 370 | 1,100 → 950 |
| 점 3 | 1,400 → 220 | 1,500 → 420 | 1,400 → 1,000 |
| 점 4 | 1,800 → 200 | 1,800 → 390 | 1,700 → 950 |
| 점 5 (정격) | 2,100 → 180 | 2,200 → 324 | 2,000 → 750 |
| 점 6 | 2,300 → 0 | 2,400 → 0 | 2,200 → 0 |

### 기어별 최고 작업속도와 기어비

기어비는 손으로 정하지 않고 단별 최고속에서 유도됩니다: `기어비 = (정격rpm/60 × 2π × 타이어반경) / (최고속 × 종감속비)`[^gearderive]. 아래 기어비는 그 식의 계산 결과입니다.

| 기어 | 소형 m/s (기어비) | 중형 m/s (기어비) | 대형 m/s (기어비) |
|---|---|---|---|
| 1단 | 0.80 (22.6) | 0.68 (33.1) | 0.60 (39.8) |
| 2단 | 1.55 (11.7) | 1.30 (17.3) | 1.15 (20.8) |
| 3단 | 2.45 (7.4) | 2.05 (11.0) | 1.85 (12.9) |
| 4단 | 3.33 (5.4) | 3.05 (7.4) | 2.75 (8.7) |
| 5단 | — | 4.20 (5.4) | 3.90 (6.1) |
| 6단 | — | — | 5.20 (4.6) |

- 오토시프트: 각 기어 최고속의 약 82~85% 도달 시 승급, 아랫단 최고속의 70~72% 아래로 떨어지면 강등[^shift].
- 후진은 항상 1단 기어비를 사용합니다[^revgear].

## 4. 공통 세계 설정 (참고)

| 항목 | 값 | 소스 |
|---|---|---|
| 물리 고정 스텝 | 1/120 s | `SHARED_WORLD_CONFIG.physicsFixedStepSec` (types.ts) |
| 고속 신호 루프 주기 | 50 ms | `SHARED_WORLD_CONFIG.fastLoopIntervalMs` (types.ts) |
| 저속 신호 루프 주기 | 1,000 ms | `SHARED_WORLD_CONFIG.slowLoopIntervalMs` (types.ts) |
| 중력가속도 | 9.81 m/s² | `SHARED_WORLD_CONFIG.gravityMps2` (types.ts) |
| GPS 원점 | 37.5665°N, 126.9780°E | `SHARED_WORLD_CONFIG.gpsOriginLatDeg/LonDeg` (types.ts) |

---

각주의 파일 경로는 모두 `simulator/src/simulation/` 기준입니다. 소형=`tractors/jd5050e.ts`, 중형=`tractors/jd6100m.ts`, 대형=`tractors/jd6155m.ts`.

[^default]: `tractors/specs.ts` `DEFAULT_TRACTOR_ID = 'compact'`
[^wheelbase]: 각 스펙 파일 `wheelbaseM`
[^turnradius]: 각 스펙 파일 `minimumTurningRadiusM`
[^maxangle]: 소형은 `jd5050e.ts` `maxFrontWheelAngleDeg: 32.6` (리터럴, 레거시 보존), 중형·대형은 `deriveSteeringGeometry(wheelbase, minTurnRadius, 20)` 계산값 — atan(2.5/4.0)=32.005°, atan(3.1/4.9)=32.31°
[^track]: 각 스펙 파일 `frontTrackM` / `rearTrackM` — 3D 모델(tractor.glb) 실측 기반 값(카탈로그 제원 아님)
[^mass]: 각 스펙 파일 `massKg`
[^axle]: 각 스펙 파일 `frontAxleKg` / `rearAxleKg`
[^fwr]: 각 스펙 파일 `frontWeightRatio`
[^cg]: 각 스펙 파일 `cgHeightM`
[^tirer]: 각 스펙 파일 `tireRadiusM` (레거시 물리 모델용. 레이캐스트 휠은 `wheelRadiusM` 0.679/0.828/1.027 별도)
[^impl]: 각 스펙 파일 `implementRearLoadKg`
[^derive]: `tractors/types.ts` `deriveSteeringGeometry()`
[^ratio]: 각 스펙 파일 `steeringRatio`
[^rate]: 각 스펙 파일 `steeringWheelMaxRateDegPerSec`
[^ratecalc]: `steeringWheelMaxRateDegPerSec ÷ steeringRatio` 계산값
[^l2l]: 락투락 각(2 × 최대핸들각) ÷ 핸들 최대 각속도 — 각 스펙 파일 `steeringWheelMaxRateDegPerSec` 주석의 유도 근거(3.0/3.7/4.5초)와 일치
[^maxsw]: `maxSteeringWheelAngleDeg` = 최대 앞바퀴각 × 조향비 (소형 32.6×20=652, 중형·대형은 deriveSteeringGeometry 계산값)
[^swtau]: 각 스펙 파일 `steeringWheelTauSec`
[^fwtau]: 각 스펙 파일 `frontWheelTauSec` (속도 의존 보정 `frontWheelTauSpeedScale: 0.05` 공통)
[^softlim]: 각 스펙 파일 `steeringSoftLimitStartRatio` / `steeringSoftLimitMinRateScale`
[^hp]: 각 스펙 파일 `ratedPowerHp` — 토크커브 정격점과 일치 검증됨(`__tests__/TractorConfig.test.ts`)
[^idle]: 각 스펙 파일 `idleRpm`
[^rated]: 각 스펙 파일 `ratedRpm`
[^stall]: 각 스펙 파일 `stallRpm` — 이 rpm 미만으로 떨어지면 엔진 정지(`EngineTorque.ts` `updateEngineRpm()`)
[^curve]: 각 스펙 파일 `engineTorqueCurve`
[^interp]: `EngineTorque.ts` `interpolateTorque()` — 커브 범위 밖 rpm 은 토크 0
[^final]: 각 스펙 파일 `finalDriveRatio`
[^gears]: 각 스펙 파일 `workGearMaxSpeedsMps` 길이 − 1 (index 0 = 중립)
[^speeds]: 각 스펙 파일 `workGearMaxSpeedsMps` 마지막 원소
[^rev]: 각 스펙 파일 `reverseGearMaxSpeedMps`
[^accel]: 각 스펙 파일 `accelMaxFieldMps2`
[^brake]: 각 스펙 파일 `brakeMaxFieldMps2`
[^brktq]: 각 스펙 파일 `maxBrakeTorqueNm`
[^lat]: 각 스펙 파일 `lateralAccelMaxMps2`
[^gearderive]: `tractors/gearing.ts` `deriveGearRatios()` — `workGearMaxSpeedsMps` 를 정본으로 기어비 유도
[^shift]: 각 스펙 파일 `gearShiftRatios` (소형 0.85/0.7, 중형 0.84/0.71, 대형 0.82/0.72) 및 `gearUpshiftThresholdsMps` / `gearDownshiftThresholdsMps`
[^revgear]: `tractors/types.ts` `gearRatios` JSDoc — 후진은 `gearRatios[1]` 사용(EngineCore 규약)
