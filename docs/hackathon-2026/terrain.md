# 노면(지형·토양) 물리 모델 — 2026 Agritechnica 해커톤

시뮬레이터의 트랙터가 노면에서 받는 마찰·견인 한계·경작 저항의 실제 수식과 값입니다.
모든 값은 시뮬레이터 소스 코드에서 그대로 추출했으며, 각 값 옆에 소스 파일·상수명을 각주로 적습니다.

- 소스 루트: `simulator/src/simulation/`
- 대회 맵 4개(plain, agri-1-plain, agri-2-sloped, agri-3-patch)는 전부 토양 종류 `plain` 을 사용합니다 (`public/maps/*.json` 의 `soilType: "plain"`).

---

## 1. 토양 종류별 기본 물성

토양 물성 정의: `SoilType.ts` (`SOIL_TYPES`) + `tractors/types.ts` (`SHARED_SOIL_TYPES`, line 237)

| 토양 | 마찰계수 μ | 구름저항계수 | 침하 민감도 | 다짐 증가율 | 경운 저항 (N·m) | 견인 기준력 draftBaseN (N) | 초기 함수율 (0~1) |
|---|---|---|---|---|---|---|---|
| sandy | 0.5 | 0.03 | 0.00005 | 0.005 | 80 | 1,500 | 0.12 |
| loam | 0.6 | 0.04 | 0.0001 | 0.01 | 120 | 2,500 | 0.30 |
| clay | 0.7 | 0.06 | 0.0002 | 0.02 | 200 | 4,500 | 0.45 |
| wet | 0.35 | 0.08 | 0.0004 | 0.005 | 250 | 3,500 | 0.90 |
| wet-plain | 0.24 | 0.08 | 0 | 0 | 250 | 3,500 | 0.95 |
| hardpack | 0.75 | 0.025 | 0 | 0 | 400 | 6,000 | 0.05 |
| **plain (대회 맵)** | **0.65** | **0.04** | **0** | **0** | **120** | **2,500** | **0.22** |

- sandy/loam/clay/wet: `tractors/types.ts` `SHARED_SOIL_TYPES` (line 237)
- wet-plain: `SoilType.ts` `WetPlainSoil` (friction line 124, moisture line 127)
- hardpack: `SoilType.ts` `HardpackSoil` (line 141)
- plain: `SoilType.ts` `PlainSoil` (friction line 194, moisture line 201)

**대회 맵(`plain` 토양)의 특성** (`SoilType.ts` `PlainSoil` 주석):
- 바퀴 주행으로는 지형이 변하지 않음 (`wheelDeformable = false`, 침하·다짐 0)
- 작업기(쟁기 등)는 흙을 갈 수 있음 (`deformable = true`)
- μ=0.65 는 "다져진 마른 평지" 기준. 갈린(tilled) 셀 위에서는 아래 §2 식으로 μ≈0.53 까지 떨어짐

## 2. 유효 마찰계수 — 다짐·경작·수분 피드백

매 프레임 트랙터 위치 셀의 토양 상태로 유효 μ 를 계산합니다.

**적용 경로 (현행 물리, `GameLoopV2.ts` line 1365):**

```
μ = μ_base × (1 + compaction × 0.1) × (tilled ? 0.85 : 1.0) × (1 − moisture × 0.15)
```

- `μ_base` = 토양 종류의 friction (§1 표)
- `compaction` = 셀 다짐도 0~1 (plain 토양은 항상 0)
- `tilled` = 셀이 갈렸는가 — **갈린 흙 위에서는 접지가 15% 저하**
- `moisture` = 셀 함수율 0~1 — **완전 젖은 흙에서 15% 저하**

plain 토양 대입값:
| 상태 | 유효 μ |
|---|---|
| 미경작 (moisture 0.22) | 0.65 × (1 − 0.033) = **0.629** |
| 경작 후 (tilled) | 0.629 × 0.85 = **0.534** |

**레거시 kinematic 모델 (`DynamicVehicle.ts` line 560, 참고용):**

```
μ_eff = μ × (1 − moistureFrictionLoss × moisture)     // moistureFrictionLoss = 0.3
```

- `moistureFrictionLoss = 0.3`: `tractors/jd5050e.ts:119`, `jd6100m.ts:98`, `jd6155m.ts:111`
- 초기 μ = `soilFrictionDry = 0.6`: `tractors/jd5050e.ts:112`, `jd6100m.ts:91`, `jd6155m.ts:104`

## 3. 견인 한계 (traction limit)

구동력은 엔진 토크가 아니라 **접지 한계**로 잘립니다 (`DynamicVehicle.ts` line 337):

```
tractionLimit [N] = μ_rear × rearLoad [N] × tractionLimitRatio
tractionLimitRatio = 0.92
```

- `tractionLimitRatio = 0.92`: `tractors/jd5050e.ts:124`, `jd6100m.ts:103`, `jd6155m.ts:116`
- `rearLoad` = 차체 후축 하중 + 3점히치 무게 이동분 (`DynamicVehicle.ts` line 290)
- 요구 구동력 `driveTorque / tireRadius` 가 이 한계를 넘으면 클램프 → 휠슬립

**횡방향 스키딩**: 요구 횡가속 `|ω·v|` 이 `μ·g` 를 넘으면 옆으로 미끄러지기 시작합니다
(`DynamicVehicle.ts` `lateralSkidRatio` 주석 line 26, 판정은 RapierVehicle). 예: 경작 후 μ=0.534 → 한계 횡가속 ≈ 5.2 m/s².

## 4. 구름저항 — 침하·다짐·경작 페널티

`DynamicVehicle.ts` `effectiveRollingResistanceCoeff` (line 564):

```
compactedSink = sinkDepth × (1 − compactionRecoveryFactor × compaction)
rollingCoeff  = rollingResistanceBase + rollingResistanceSinkCoef × compactedSink × tilledPenalty
rollingR [N]  = 총중량 × rollingCoeff                       // TireModel.computeRollingResistance
점성 저항 [N] = −soilViscousDragCoef × moisture × v
```

| 상수 | 값 | 소스 |
|---|---|---|
| `rollingResistanceBase` | 0.015 | `tractors/jd5050e.ts:117` (전 기종 동일) |
| `rollingResistanceSinkCoef` | 0.5 | `tractors/jd5050e.ts:118` |
| `tilledPenalty` (갈린 흙 위) | 1.5 | `tractors/jd5050e.ts:121` |
| `compactionRecoveryFactor` | 0.7 | `tractors/jd5050e.ts:120` |
| `soilViscousDragCoef` | 450 N·s/m | `tractors/jd5050e.ts:122` |

대회 맵(plain)은 침하가 0 이므로 구름저항은 기본값 0.015 로 일정하고, 점성 저항만 moisture 0.22 비례로 걸립니다.

## 5. 작업기는 별도 문서

경작 저항(draft force), 관입 깊이, 작업기 카탈로그(작업 폭·`drag_coeff`·작업 깊이),
경작도 적립식은 [implement-specs.md](./implement-specs.md) 로 분리했습니다.
이 문서는 **노면 자체의 물성**(토양·마찰·견인 한계·구름저항)만 다룹니다.
