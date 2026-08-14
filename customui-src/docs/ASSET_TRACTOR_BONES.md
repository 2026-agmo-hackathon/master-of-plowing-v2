# Asset: tractor.glb — Bone 구조 조사 보고

## 메타데이터

- **조사 일자**: 2026-04-17
- **파일 경로**: `/Users/hobeen/agmo/simulationworld/simulator/public/models/tractor.glb`
- **파일 크기**: 1,083,880 bytes (약 1.03 MB)
- **포맷**: glTF binary model, version 2 (length 1083880 bytes)
- **추가 커밋**: `24f8051 Update simulator integration and control logging` (2026-04-09)
- **원본 출처 추정**: Sketchfab (씬 루트 노드 이름이 `Sketchfab_Scene`, `Sketchfab_model`)
- **라이선스 기록**: 프로젝트 내부에 별도 기록 없음 (후속 과제로 라이선스/출처 확인 필요)

## 조사 방법

`/Users/hobeen/agmo/simulationworld/simulator/scripts/inspect-gltf.mjs` 스크립트로
`three` + `GLTFLoader`를 Node ESM 환경에서 돌려 씬 트리를 덤프했습니다.

실행 명령:
```
cd /Users/hobeen/agmo/simulationworld/simulator
node scripts/inspect-gltf.mjs public/models/tractor.glb
```

> 참고: Node 환경에서는 GLTFLoader가 텍스처 디코드 시 `self`(브라우저 전역)을 참조하므로
> 스크립트 상단에서 `globalThis.self = globalThis` 폴리필을 적용했습니다.
> 텍스처 blob 디코드 경고는 구조 분석에 영향을 주지 않습니다.

## 실행 결과 (원본 stdout)

```
THREE.GLTFLoader: Couldn't load texture blob:nodedata:d2b486fc-21e1-4037-94a6-9bfcb01d2a37
# GLTF: /Users/hobeen/agmo/simulationworld/simulator/public/models/tractor.glb
# Scenes: 1, Animations: 0
## Bones
(no Bone nodes found)
## Meshes
- Object_4 (geo=BufferGeometry) material=Material.005
## Animations
(no animation clips)
## Hierarchy
- Sketchfab_Scene [Group]
  - Sketchfab_model [Object3D]
    - root [Object3D]
      - GLTF_SceneRootNode [Object3D]
        - Tractor_0 [Object3D]
          - Object_4 [Mesh]
```

## Bone 목록

| Bone | 존재 여부 |
| --- | --- |
| (없음) | — |

glb 내부에 `Bone` 타입 노드가 하나도 없습니다. 스켈레톤/스킨 없이
단일 mesh(`Object_4`)로 모든 트랙터 지오메트리가 병합되어 있습니다.

## Mesh 목록

| Mesh | Geometry | Material |
| --- | --- | --- |
| `Object_4` | BufferGeometry | `Material.005` |

## Animation 목록

없음 (애니메이션 클립 0개).

## 본 계층 트리

```
Sketchfab_Scene [Group]
└── Sketchfab_model [Object3D]
    └── root [Object3D]
        └── GLTF_SceneRootNode [Object3D]
            └── Tractor_0 [Object3D]
                └── Object_4 [Mesh]
```

## 필수 본 6개 체크

| 필수 본 | 존재 여부 |
| --- | --- |
| `steering_wheel` | 없음 |
| `front_wheel_left` | 없음 |
| `front_wheel_right` | 없음 |
| `rear_wheel_left` | 없음 |
| `rear_wheel_right` | 없음 |
| `hitch` | 없음 |

**결과: 0 / 6 — 모든 필수 본이 누락**

## 판정

**Status: DONE_WITH_CONCERNS**

- 스크립트 자체는 정상 동작했고 glb 구조도 정상적으로 파싱되었습니다.
- 그러나 본 6개 모두 부재 + 스켈레톤 자체가 없음 → A2(본 기반 제어)로 바로 진행 불가.

## 다음 단계 권고

본이 전혀 없으므로 A2로 진행하기 전에 **에셋 계층 재구성**이 필요합니다.
권고 옵션은 다음과 같습니다.

### 옵션 1. 에셋 교체 (권장)
- 바퀴/스티어링/히치가 별도 오브젝트로 분리된 트랙터 glb를 수급
- 라이선스/출처 명확한 소스 확보
- **BLOCKED — 외부 에셋 수급 필요**

### 옵션 2. 단일 mesh를 런타임에 부위별로 분리
- 현재 `Object_4` 단일 메쉬이므로 이름 기반 매칭 불가
- 부위 분리하려면 버텍스 그룹/서브메쉬 재작업이 필요 (Blender 등 외부 도구)
- 실질적으로 옵션 1과 동급 비용

### 옵션 3. 프로시저럴 본/프록시 생성
- 본 없이 빈 Object3D(예: `steeringPivot`, `frontLeftWheelPivot`)를
  트랙터 로컬 좌표 기준 상대 위치에 직접 코드로 추가
- 시각적 회전은 프록시 Object3D 회전으로 대체 (mesh 자체는 회전 안 함)
- 장점: 에셋 수급 없이 즉시 A2 착수 가능
- 단점: 바퀴/스티어링이 시각적으로는 회전하지 않고, 논리적 기준점만 존재

### 결론
- 시각 피드백이 필요한 A2 범위라면 → **옵션 1(에셋 교체)** 선행이 맞습니다.
- 시각 피드백 없이 논리적 pivot만 필요하면 → **옵션 3**으로 일시 진행, 이후 에셋 교체.
