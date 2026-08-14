import { TRACTOR_CONFIG } from './TractorConfig';

const DEG_TO_RAD = Math.PI / 180;
const METERS_PER_DEG_LAT = 111320;

let _originLat = TRACTOR_CONFIG.gpsOriginLatDeg;
let _originLon = TRACTOR_CONFIG.gpsOriginLonDeg;
let _metersPerDegLon = 111320 * Math.cos(_originLat * DEG_TO_RAD);
let _isDefault = true;

let _setOriginCallCount = 0;
export function setOrigin(lat: number, lon: number): void {
    _originLat = lat;
    _originLon = lon;
    _metersPerDegLon = 111320 * Math.cos(lat * DEG_TO_RAD);
    _isDefault = false;
    _setOriginCallCount += 1;
    console.log(`[GpsOrigin] setOrigin(${lat}, ${lon})`);
}

export function getOriginLat(): number { return _originLat; }
export function getOriginLon(): number { return _originLon; }
export function getMetersPerDegLat(): number { return METERS_PER_DEG_LAT; }
export function getMetersPerDegLon(): number { return _metersPerDegLon; }

export function resetOrigin(): void {
    _originLat = TRACTOR_CONFIG.gpsOriginLatDeg;
    _originLon = TRACTOR_CONFIG.gpsOriginLonDeg;
    _metersPerDegLon = 111320 * Math.cos(_originLat * DEG_TO_RAD);
    _isDefault = true;
    console.log('[GpsOrigin] resetOrigin (back to default Seoul)');
}

// Convert WGS84 (lat, lon) to ENU (x: east, z: north) relative to current origin.
// Convention matches GameLoop.ts: z = (lat - originLat) * METERS_PER_DEG_LAT (north positive).
let _lastDefaultWarnMs = 0;
export function gpsToEnu(lat: number, lon: number): { x: number; z: number } {
    if (_isDefault) {
        const now = Date.now();
        if (now - _lastDefaultWarnMs > 5000) {
            _lastDefaultWarnMs = now;
            console.warn('[GpsOrigin] gpsToEnu called while origin is still default Seoul — likely a race; result will be wrong frame', { lat, lon });
        }
    }
    return {
        x: (lon - _originLon) * _metersPerDegLon,
        z: (lat - _originLat) * METERS_PER_DEG_LAT,
    };
}

// Convert ENU (x: east, z: north) to WGS84 (lat, lon) relative to current origin.
export function enuToGps(x: number, z: number): { lat: number; lon: number } {
    return {
        lat: _originLat + z / METERS_PER_DEG_LAT,
        lon: _originLon + x / _metersPerDegLon,
    };
}
