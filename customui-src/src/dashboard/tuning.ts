// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 튜닝 파라미터의 단일 정의표. 백엔드 web/TuningProtocol.cpp 의 PARAM_FIELDS 와
// 키가 1:1 로 대응한다(키는 계약 — 화면 라벨은 바꿔도 키는 그대로 병기한다).
// 기본값은 백엔드 TrackerParams 기본값과 동일해야 "기본값과 다름" 표시와
// 「기본값 복귀」가 맞는다. step 은 진동하는 기계 위에서 의미 있는 최소 조정폭.
//
// Single definition table for the tuning parameters. Keys map 1:1 to
// PARAM_FIELDS in web/TuningProtocol.cpp (keys are the contract; labels are
// presentation). Defaults must equal the backend TrackerParams defaults so the
// modified indicator and reset-to-default stay truthful. Steps are the smallest
// adjustment that matters on a vibrating machine.
// └────────────────────────────────────────────────────────────────────────────┘

export const TRACKER_LABELS: Record<string, string> = {
  pure_pursuit: 'Pure Pursuit',
  stanley: 'Stanley',
}

export interface ParamSpec {
  /** 백엔드 JSON 키 (계약) / backend JSON key (contract) */
  key: string
  label: string
  step: number
  defaultValue: number
  /** 표시 소수 자리 / display decimals */
  digits: number
  unit?: string
}

export interface ParamGroup {
  title: string
  /** 이 그룹이 속한 알고리즘 이름. null = 알고리즘과 무관(항상 활성).
   *  Algorithm this group belongs to; null = algorithm-independent. */
  tracker: string | null
  note?: string
  params: ParamSpec[]
}

export const PARAM_GROUPS: ParamGroup[] = [
  {
    title: 'Pure Pursuit',
    tracker: 'pure_pursuit',
    params: [
      { key: 'ppLookaheadMinM', label: '전방주시 최소', step: 0.5, defaultValue: 5.5, digits: 1, unit: 'm' },
      { key: 'ppLookaheadMaxM', label: '전방주시 최대', step: 0.5, defaultValue: 8.0, digits: 1, unit: 'm' },
      { key: 'ppLookaheadTimeS', label: '전방주시 시간', step: 0.1, defaultValue: 2.5, digits: 1, unit: 's' },
      { key: 'ppKLtd', label: 'LTD 게인', step: 0.05, defaultValue: 0.5, digits: 2 },
      { key: 'ppKHde', label: 'HDE 게인', step: 0.05, defaultValue: 1.0, digits: 2 },
      { key: 'ppWeightPp', label: 'PP 항 가중치', step: 0.05, defaultValue: 0.45, digits: 2 },
      { key: 'ppKYawDamp', label: '요 감쇠 (D)', step: 0.1, defaultValue: 1.5, digits: 1 },
    ],
  },
  {
    title: 'Stanley',
    tracker: 'stanley',
    params: [
      { key: 'stKLtd', label: 'LTD 게인', step: 0.05, defaultValue: 1.0, digits: 2 },
      { key: 'stSofteningMps', label: '저속 완화항', step: 0.05, defaultValue: 0.5, digits: 2, unit: 'm/s' },
    ],
  },
  {
    title: '조향 PID',
    tracker: null,
    // 게인 스케일이 도(degree) 기준임을 화면에 명시 — 라디안 착각 방지.
    // The gain scale is on a degree error; stated on screen to prevent the
    // radian mistake.
    note: '오차 단위는 도(degree)입니다 — 라디안으로 착각하면 57배 어긋납니다.',
    params: [
      { key: 'steerKp', label: 'KP', step: 5.0, defaultValue: 50.0, digits: 1 },
      { key: 'steerKi', label: 'KI', step: 0.05, defaultValue: 0.1, digits: 2 },
      { key: 'steerKd', label: 'KD', step: 0.5, defaultValue: 5.0, digits: 1 },
    ],
  },
  {
    title: '종방향 PID',
    tracker: null,
    params: [
      { key: 'longKp', label: 'KP', step: 0.1, defaultValue: 1.5, digits: 1 },
      { key: 'longKi', label: 'KI', step: 0.01, defaultValue: 0.05, digits: 2 },
      { key: 'longKd', label: 'KD', step: 0.05, defaultValue: 0.1, digits: 2 },
    ],
  },
]
