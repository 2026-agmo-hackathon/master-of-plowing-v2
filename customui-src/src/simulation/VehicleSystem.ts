export interface VehicleState {
  latitude: number; longitude: number; altitude: number
  heading: number; speed: number; numSat: number  // internal simulator heading: 0=north, clockwise-positive
  rpm: number; throttle: number; brake: number
  steerAngle: number   // legacy alias for vehicleFrontWheelAngleDeg
  measuredSteerAngleDeg: number   // internal steering-wheel angle in simulator sign convention
  vehicleFrontWheelAngleDeg: number   // internal front-wheel angle used by the vehicle model
  motorCmdRpm: number   // raw steer motor command from MQTT, rpm-equivalent [-500, 500]
  accel_x: number; accel_y: number; gyro_z: number
  roll: number; pitch: number; yaw: number
  gear: number
  appSteerCommand: number  // legacy alias for motorCmdRpm
  steerFeedbackActive: boolean
  running: boolean
  // Optional debug telemetry — order: [FL, FR, RL, RR]
  slip?: [number, number, number, number]       // slip ratio (dimensionless), 0 = no slip
  tireLoad?: [number, number, number, number]   // normal force per wheel (N)
  soilType?: string                              // current soil type name
  soilMoisture?: number                          // current soil moisture 0..1
  // Optional wheel rotational speed (rad/s). Consumers may derive from `speed` if absent.
  wheelSpeed?: number
  wheelOmega?: number        // raw wheel angular velocity (rad/s) from drive train model (average L+R)
  wheelOmegaL?: number       // left rear wheel angular velocity (rad/s)
  wheelOmegaR?: number       // right rear wheel angular velocity (rad/s)
  wheelEquivRpm?: number     // wheel-equivalent RPM = |wheelOmega| × driveRatio × 60/(2π)
  terrainPitchRad?: number   // 지형 경사 pitch (rad), GameLoop 10Hz가 SoilGrid 4륜 샘플링으로 계산
  terrainRollRad?: number    // 지형 경사 roll (rad), 시각화 전용
  localX?: number            // world-local X position in meters (east)
  localZ?: number            // world-local Z position in meters (north)
  headingRad?: number        // heading in radians
  draftForceN?: number       // implement draft force (N)
  ptoEngaged?: boolean       // PTO engaged state
  ptoLoad?: number           // PTO load torque (Nm)
}
