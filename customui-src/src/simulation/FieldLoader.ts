import { wgs84ToUtm, utmToEnu } from "../../helpers/coord_helper";

type GeoJsonFeatureCollection = {
  type: "FeatureCollection";
  features: Array<{
    type: "Feature";
    properties: Record<string, unknown>;
    geometry: { type: "Polygon" | "Point"; coordinates: unknown };
  }>;
};

export type LoadedField = {
  polygon: [number, number][]; // [x, z] meters, ENU
  startPoint: { x: number; z: number }; // origin (always [0, 0])
  startHeadingDeg: number;
  fieldId: string;
};

export function loadFieldFromGeoJson(geojson: GeoJsonFeatureCollection): LoadedField {
  const polygonFeature = geojson.features.find(f => f.geometry.type === "Polygon");
  const pointFeature = geojson.features.find(f => f.geometry.type === "Point");

  if (!polygonFeature || !pointFeature) {
    throw new Error("FieldLoader: GeoJSON must contain Polygon and Point features");
  }

  const [startLon, startLat] = pointFeature.geometry.coordinates as [number, number];
  const startUtm = wgs84ToUtm(startLat, startLon);

  const ringLL = (polygonFeature.geometry.coordinates as [number, number][][])[0];
  const polygon: [number, number][] = ringLL.map(([lon, lat]) => {
    const utm = wgs84ToUtm(lat, lon);
    const enu = utmToEnu(utm.easting, utm.northing, startUtm.easting, startUtm.northing);
    return [enu.x, enu.z];
  });

  // GeoJSON polygons are closed (last point == first point); remove duplicate if present
  if (polygon.length > 1) {
    const first = polygon[0];
    const last = polygon[polygon.length - 1];
    if (Math.abs(first[0] - last[0]) < 1e-6 && Math.abs(first[1] - last[1]) < 1e-6) {
      polygon.pop();
    }
  }

  const startHeadingDeg = (pointFeature.properties?.start_heading_deg as number) ?? 0;
  const fieldId =
    (pointFeature.properties?.field_id as string) ??
    (polygonFeature.properties?.field_id as string) ??
    "unknown";

  return {
    polygon,
    startPoint: { x: 0, z: 0 }, // origin = start point
    startHeadingDeg,
    fieldId,
  };
}
