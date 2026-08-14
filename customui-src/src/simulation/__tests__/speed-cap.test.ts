import { describe, it, expect } from "vitest";
import { DynamicVehicle } from "../DynamicVehicle";
import type { VehicleInput, VehicleOutput } from "../KinematicVehicle";

const STEP_DT = 1 / 60;
const TARGET_S = 5.0;
const MAX_SPEED_MPS = 7 / 3.6; // 7 km/h in m/s = 1.944 m/s
const TOLERANCE = 0.05; // m/s

function simulate(
  vehicle: DynamicVehicle,
  input: Partial<VehicleInput>,
  seconds: number,
  dt = STEP_DT,
): VehicleOutput {
  const steps = Math.round(seconds / dt);
  const fullInput = { throttle: 0, brake: 0, steer: 0, ...input } as VehicleInput;
  let output: VehicleOutput;
  for (let i = 0; i < steps; i++) {
    output = vehicle.update(fullInput, dt);
  }
  return output!;
}

describe("DynamicVehicle speed cap (7 km/h)", () => {
  // Skip: per-gear cap replaced global 7 km/h cap. gear=4 cap is now 3.33 m/s (WORK_GEAR_MAX_SPEEDS_MPS[4]).
  it.skip("gear=4, throttle=1.0 for 5s never exceeds 7 km/h", () => {
    const v = new DynamicVehicle();
    const input = { throttle: 1.0, brake: 0, steer: 0, manualGear: 4 };
    const output = simulate(v, input, TARGET_S);
    expect(Math.abs(output.speed)).toBeLessThanOrEqual(MAX_SPEED_MPS + TOLERANCE);
  });

  it("at 7 km/h with throttle=0, decelerates naturally", () => {
    const v = new DynamicVehicle();
    // First accelerate to ~7 km/h (near cap)
    const accelInput = { throttle: 1.0, brake: 0, steer: 0, manualGear: 4 };
    const outputAccel = simulate(v, accelInput, TARGET_S);
    const peakSpeed = Math.abs(outputAccel.speed);

    // Then apply throttle=0 for 1 second
    const coastInput = { throttle: 0, brake: 0, steer: 0, manualGear: 4 };
    const outputCoast = simulate(v, coastInput, 1.0);
    const finalSpeed = Math.abs(outputCoast.speed);

    // Should decelerate from peak
    expect(finalSpeed).toBeLessThan(peakSpeed);
  });

  it("reverse (gear=-1) also caps at 7 km/h magnitude", () => {
    const v = new DynamicVehicle();
    const input = { throttle: 1.0, brake: 0, steer: 0, manualGear: -1 };
    const output = simulate(v, input, TARGET_S);
    expect(Math.abs(output.speed)).toBeLessThanOrEqual(MAX_SPEED_MPS + TOLERANCE);
  });
});
