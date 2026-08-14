import { describe, it, expect } from "vitest";
import { wgs84ToUtm, utmToEnu } from "../../../helpers/coord_helper";

const SEOUL_LAT = 37.5665;
const SEOUL_LON = 126.978;
// Reference values — pyproj/EPSG:32652 based (validated in round 1B)
const REF_EASTING = 321424.2862;
const REF_NORTHING = 4159640.6414;
const TOLERANCE_M = 0.1;

describe("Coord transform consistency (proj4js ↔ reference)", () => {
  it("wgs84ToUtm matches pyproj reference within 0.1m", () => {
    const { easting, northing } = wgs84ToUtm(SEOUL_LAT, SEOUL_LON);
    expect(Math.abs(easting - REF_EASTING)).toBeLessThan(TOLERANCE_M);
    expect(Math.abs(northing - REF_NORTHING)).toBeLessThan(TOLERANCE_M);
  });

  it("utmToEnu returns (0,0) when origin equals point", () => {
    const utm = wgs84ToUtm(SEOUL_LAT, SEOUL_LON);
    const enu = utmToEnu(utm.easting, utm.northing, utm.easting, utm.northing);
    expect(enu.x).toBeCloseTo(0, 6);
    expect(enu.z).toBeCloseTo(0, 6);
  });

  it("repeated conversion is deterministic", () => {
    const a = wgs84ToUtm(SEOUL_LAT, SEOUL_LON);
    const b = wgs84ToUtm(SEOUL_LAT, SEOUL_LON);
    expect(a.easting).toBe(b.easting);
    expect(a.northing).toBe(b.northing);
  });
});
