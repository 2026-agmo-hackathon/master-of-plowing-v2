# 작업기(Implement) 제원과 경작 물리 — 2026 Agritechnica 해커톤

시뮬레이터에 실린 작업기 5종의 제원과, 그 작업기가 흙에 걸리는 힘·갈리는 깊이의
실제 수식과 값입니다. 노면 자체의 물성(토양·마찰·견인 한계·구름저항)은
[terrain.md](./terrain.md) 에 있습니다.

작업기는 한 주행에 **하나만** 쓸 수 있습니다 — 녹화 중에 바꾸면 그 주행은 무효입니다.
작업 폭 `W` 는 REC CSV 의 `# implement` 줄로 나가고 채점의 Time 항목에 그대로 들어갑니다.

## 1. 경작 저항 (implement drag / draft force)

### 1.1 기본식 — ASABE D497 몰드보드 플라우 견인저항

`ImplementSystem.ts` `getDragN` (line 374) / `DRAG_SPEED_QUAD` (line 397):

```
D [N] = drag_coeff × depth [m] × (1 + 0.1014 × v²)      // v: 전진속도 m/s
DRAG_SPEED_QUAD = (5.1 × 3.6²) / 652 ≈ 0.1014
```

ASABE D497 원식 `D = F·(A + C·S²)·W·T` (A=652, C=5.1, S=km/h, F=0.7) 에서
폭·깊이를 `drag_coeff = F·A·W·100` 으로 빼낸 형태입니다. **정지 상태에서도 정적 저항이 걸립니다**
(속도항이 `1 + 0.1014v²` 이므로 v=0 에서도 `drag_coeff × depth`).

### 1.2 실제 적용값 — 토양 상태 배율

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

### 1.3 작업기 카탈로그 (`implements/*.ts`)

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

### 1.4 차량에 걸리는 방식

`DynamicVehicle.ts`:
- draft 는 진행 방향 반대로 작용, |v| < 1e-3 m/s 에서는 무시(정지 차량을 못 끌어당기게) — line 349
- 균평기 블레이드 `implementDragN` 은 전진 시에만 감속력 — line 342

## 2. 경작(tilled) 판정 — 커버리지의 물리적 근거

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
