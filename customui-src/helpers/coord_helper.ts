import proj4 from "proj4";

const DEFAULT_EPSG = 32652; // UTM zone 52N + WGS84 (Korea)

function ensureProj(epsg: number): string {
  const def = `EPSG:${epsg}`;
  if (!proj4.defs(def)) {
    if (epsg === 32652) {
      proj4.defs(def, "+proj=utm +zone=52 +datum=WGS84 +units=m +no_defs");
    } else {
      throw new Error(`EPSG:${epsg} definition not registered.`);
    }
  }
  return def;
}

export function wgs84ToUtm(
  lat: number,
  lon: number,
  epsg: number = DEFAULT_EPSG,
): { easting: number; northing: number } {
  const utm = ensureProj(epsg);
  const [easting, northing] = proj4("EPSG:4326", utm, [lon, lat]);
  return { easting, northing };
}

export function utmToEnu(
  easting: number,
  northing: number,
  originE: number,
  originN: number,
): { x: number; z: number } {
  return { x: easting - originE, z: originN - northing };
}
