# 시스템 요구사양과 설치 — 2026 Agritechnica 해커톤

SeamOS World(3D 시뮬레이터 + 대시보드 + CCU 가상머신)를 참가자 PC 에서 돌리기 위한 기준과
설치 절차입니다. **지원 OS 는 macOS(Apple Silicon)와 Ubuntu 두 가지**이고, 둘의 성능 특성과
설치 방법이 다르므로 나눠서 적었습니다.

---

## 1. 왜 OS 에 따라 성능이 갈리는가

CCU 가상머신의 게스트가 **aarch64(ARM)** 입니다. NEVONEX 런타임 바이너리가 ARM 전용이기
때문입니다. 그래서 호스트 CPU 아키텍처가 성능을 가릅니다.

| 호스트 | 가상화 방식 | 결과 |
|--------|-------------|------|
| **macOS (Apple Silicon)** | 네이티브 가상화 (호스트도 ARM) | 가장 빠름. 앱 신호 주기가 기준값 100 ms 에 정확히 붙음 |
| **Ubuntu (arm64)** | KVM | 빠름 |
| **Ubuntu (x86_64)** | TCG 소프트웨어 에뮬레이션 (**KVM 불가**) | 같은 등급이라도 CPU 를 더 씀. 코어 수·클럭이 높을수록 유리 |

x86_64 호스트에서는 런처가 경고를 띄웁니다. TCG 라도 MTTCG + 번역블록 캐시 튜닝이 기본으로
걸려 있어 24코어 기준 부팅 75초 정도입니다. 3D 시뮬레이터는 호스트 브라우저가 렌더하므로
TCG 와 무관합니다.

---

## 2. macOS (Apple Silicon)

### 요구사양

| 항목 | 최소 | 권장 |
|---|---|---|
| 칩 | Apple Silicon **M1 이상** | M2 이상 |
| RAM | 8 GB | 16 GB 이상 |
| 저장장치 | 여유 20 GB | 여유 40 GB |
| GPU | 내장 (외장 불필요) | 내장 |
| 네트워크 | 인터넷 연결 (설치·VM 이미지 다운로드·리더보드 제출) | 동일 |

> Intel Mac 은 지원 대상이 아닙니다 — Apple Silicon 이 아니면 x86_64 이므로 TCG 로 떨어집니다.

### 설치

설치되는 것은 **런처 스크립트뿐**입니다. 4.4 GB VM 이미지는 첫 `seamosworld start` 에서
자동으로 받습니다(최초 1회). 따로 받아 둘 필요 없습니다.

```bash
brew install agmo-inc/seamosworld/seamosworld
seamosworld start
```

### 업데이트

런처와 VM 이미지는 별개입니다. **둘 다** 갱신해야 합니다 — 한쪽만 새로 받으면 hostfwd 포트나
VM 안 서비스가 어긋납니다.

```bash
brew upgrade seamosworld
seamosworld stop && seamosworld update && seamosworld start
```

---

## 3. Ubuntu

### 요구사양

| 항목 | 최소 (실측 검증) | 권장 |
|---|---|---|
| CPU | 8코어 최신 모바일급 (AMD Ryzen 7 5825U 동급 이상) | 데스크탑 8코어 3 GHz+ (Ryzen 7 데스크탑급) |
| RAM | 8 GB (가용 5.6 GB 실측) | 16 GB 이상 |
| 저장장치 | NVMe SSD, 여유 20 GB | NVMe SSD, 여유 40 GB |
| GPU | 내장 GPU (AMD Radeon iGPU 실측) | 내장 GPU 이상 (외장 불필요) |
| OS | Ubuntu **22.04 이상** | Ubuntu 22.04/24.04 데스크탑 |
| 네트워크 | 인터넷 연결 | 동일 |

최소사양은 실측 기준입니다 — 위 "최소" 열의 노트북(AMD Ryzen 7 5825U, RAM 8 GB)에서
v1.4.0 전체 스택 구동과 자동조향 주행을 실검증했습니다.

x86_64 라면 TCG 로 돌기 때문에 **코어 수와 클럭이 곧 성능**입니다. arm64 Ubuntu 라면 KVM 이
붙어 훨씬 여유롭습니다.

### 설치

첫 줄은 apt 에게 이 패키지가 어디 있는지 알려주는 저장소 등록입니다. **한 번만** 하면 되고,
이후 업데이트에는 필요 없습니다.

```bash
echo 'deb [trusted=yes] https://seamosworld-dist-795591862191.s3.ap-northeast-2.amazonaws.com/apt stable main' \
  | sudo tee /etc/apt/sources.list.d/seamosworld.list
sudo apt update && sudo apt install seamosworld
seamosworld start
```

### 업데이트

```bash
sudo apt update && sudo apt install --only-upgrade seamosworld
seamosworld stop && seamosworld update && seamosworld start
```

---

## 4. 첫 실행 확인

```bash
seamosworld start     # 최초 1회는 VM 이미지 4.4 GB 다운로드가 함께 진행됩니다
seamosworld status    # 서비스 상태 확인
```

브라우저에서 `http://localhost:3000` 으로 대시보드에 접속합니다.

전체 CLI 명령은 `seamosworld --help` 로 확인할 수 있습니다.

---

## 5. 최소사양에서의 체감 (실측)

- 전체 스택(시뮬레이터 3D + 대시보드 + CCU VM) 정상 구동, 자동조향 주행 가능합니다.
- 앱 신호 주기가 기준 100 ms 대비 110~140 ms 로 흔들릴 수 있습니다 (CPU 여유에 따라).
- 배속은 낮은 배수 위주로 쓰는 것이 안전합니다. 고배속은 CPU 가 따라가지 못하면 화면이
  툭툭 끊길 수 있습니다. 어차피 REC 의 `duration_ms` 는 심(sim) 시간이라 배속으로 기록이
  좋아지지 않습니다.
- RAM 8 GB 에서는 다른 무거운 프로그램(브라우저 탭 다수 등)을 함께 띄우지 않는 것을
  권합니다 — CCU 가상머신이 2.5~4 GB 를 사용합니다.
