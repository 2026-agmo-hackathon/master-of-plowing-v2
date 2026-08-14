/**
 * Pass-through adapter. Backend's steer-motor convention matches simulator's
 * vehicle frame (+1 = right turn). Kept as a function for future convention changes.
 */
export function appSteerCommandToVehicleNormalized(appSteerNormalized: number): number {
  return appSteerNormalized
}
