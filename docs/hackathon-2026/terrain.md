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

## 5. 경작 저항 (implement drag / draft force)

### 5.1 기본식 — ASABE D497 몰드보드 플라우 견인저항

`ImplementSystem.ts` `getDragN` (line 374) / `DRAG_SPEED_QUAD` (line 397):

```
D [N] = drag_coeff × depth [m] × (1 + 0.1014 × v²)      // v: 전진속도 m/s
DRAG_SPEED_QUAD = (5.1 × 3.6²) / 652 ≈ 0.1014
```

ASABE D497 원식 `D = F·(A + C·S²)·W·T` (A=652, C=5.1, S=km/h, F=0.7) 에서
폭·깊이를 `drag_coeff = F·A·W·100` 으로 빼낸 형태입니다. **정지 상태에서도 정적 저항이 걸립니다**
(속도항이 `1 + 0.1014v²` 이므로 v=0 에서도 `drag_coeff × depth`).

### 5.2 실제 적용값 — 토양 상태 배율

`ImplementSystem.ts` line 877 (`lastDraftForceN`) → `DynamicVehicle.setDraftForce` 로 전달:

```
draft [N] = drag_coeff × effectiveDepth × (1 + 0.1014·v²) × soilDraftMul

soilDraftMul = (draftBaseN / 2500) × (1 + 0.8 × 평균다짐) × typeCoeff
               // LOAM_DRAFT_BASE_N = 2500 (line 21), COMPACTION_DRAFT_GAIN = 0.8 (line 24)
typeCoeff    = plow: 1.0, land_plane(균평기): 0.9        // draftTypeCoeff (line 26)
```

관입 깊이도 흙이 허용하는 만큼만 들어갑니다 (`ImplementSystem.ts` line 830):

```
soilResistFactor = clamp(4500 / draftBaseN, 0.35, 1.15)
penetration      = clamp(soilResistFactor × (1 − 0.55·다짐) × (0.55 + 1.2·함수율), 0.15, 1)
effectiveDepth   = min(히치하강비 × 최대깊이, 최대깊이 × penetration)
```

### 5.3 작업기 카탈로그 (`implements/*.ts`)

| id | 종류 | 작업 폭 (m) | blade_height_m (히치 100% 깊이) | drag_coeff | 실용 작업 깊이 default_blade_depth_m (m) | 소스 |
|---|---|---|---|---|---|---|
| plow | plow | 1.6 | 0.22 | 73,024 | 0.163 | `implements/plow.ts:56-69` |
| plow-medium | plow | 2.2 | 0.22 | 100,408 | 0.163 | `implements/plowMedium.ts:20-33` |
| plow-large | plow | 3.6 | 0.22 | 164,304 | 0.163 | `implements/plowLarge.ts:24-37` |
| leveler | land_plane | 2.4 | 0.4 | 20,000 | 0 | `implements/leveler.ts:18-22` |
| leveler-small | land_plane | 1.8 | 0.4 | 20,000 | 0 | `implements/levelerSmall.ts:24-28` |

- 쟁기의 `drag_coeff = 0.7 × 652 × W × 100` (W = 작업 폭). 예: plow 1.6 m → 73,024
- `blade_height_m / default_blade_depth_m = 1.35` → **히치를 약 74% 이상 내리면 경작도 1.0** 에 도달 (`implements/plow.ts` 헤더 주석)
- 예시: plow(1.6 m) 를 실용 깊이 0.163 m 로 2 m/s 주행 시 draft ≈ 73,024 × 0.163 × 1.406 ≈ **16.7 kN**

### 5.4 차량에 걸리는 방식

`DynamicVehicle.ts`:
- draft 는 진행 방향 반대로 작용, |v| < 1e-3 m/s 에서는 무시(정지 차량을 못 끌어당기게) — line 349
- 균평기 블레이드 `implementDragN` 은 전진 시에만 감속력 — line 342

## 6. 경작(tilled) 판정 — 커버리지의 물리적 근거

쟁기가 지나간 셀의 "갈린 정도"는 깊이 기반으로 적립됩니다 (`ImplementSystem.ts` line 886~):

```
bladeDepth = 히치하강비 × 최대 관입 깊이
cut(셀)   = min(bladeDepth × 1.6, bladeDepth + (셀 원지반높이 − 트랙터 자리 원지반높이))
경작도    = min(1, Σmax cut / 0.163)          // 0.163 = default_blade_depth_m
```

- 기준면은 **트랙터 자리의 원지반(base)** — 3점히치 날의 높이는 트랙터 바퀴가 정하므로, 둔덕에서는 깊게·골에서는 얕게 갈림
- `MAX_CUT_OVERSHOOT = 1.6` (`ImplementSystem.ts:406`) — 둔덕에서도 명령 깊이의 1.6배 이상 파고들지 않음
- 셀 경작도 조회: `SoilGrid.getTillRatio` (`SoilGrid.ts:853`) — plow 는 깊이 비례, 그 외 작업기는 갈렸으면 1
- 대회 맵은 전부 평지라 실질적으로 `cut = bladeDepth` 가 그대로 걸립니다

경작 완료 기준·그리드 해상도·채점 면적은 [maps.md](./maps.md) §커버리지 참고.
