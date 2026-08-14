# Tractor Physics Parameters (SSOT)

Reference vehicle: **John Deere 5050E** (50HP compact utility tractor)

## Vehicle Dimensions
| Parameter | Value | Source |
|-----------|-------|--------|
| Wheelbase | 2.05 m | JD 5050E spec (2,050mm) |
| Front track | 1.47 m | JD 5050E (1,474mm) |
| Rear track | 1.52 m | JD 5050E (~1,520mm) |
| Vehicle mass | 2,500 kg | Ballasted field weight |
| Min turning radius | 3.2 m | JD 5050E (3,181mm) |

## Engine (Diesel + Governor)
| Parameter | Value | Notes |
|-----------|-------|-------|
| Idle RPM | 850 | Governor-maintained minimum |
| Rated RPM | 2,100 | Maximum under governor control |
| RPM response tau | 0.5 s | First-order lag time constant |

## Transmission (4F gear)
| Gear | Max Speed (m/s) | Max Speed (km/h) |
|------|----------------|-------------------|
| 1 | 0.80 | 2.9 |
| 2 | 1.55 | 5.6 |
| 3 | 2.45 | 8.8 |
| 4 | 3.33 | 12.0 |

## Steering (KY170C Motor + Hydrostatic Power Steering)
| Parameter | Value | Notes |
|-----------|-------|-------|
| Max front wheel angle | 45° | JD 5050E spec: 55° |
| Steering ratio | 20 | Hydrostatic effective ratio |
| Max steering wheel angle | 900° | 45° × 20 = 2.5 turns each side |
| Lock-to-lock | ~5 turns | ~1,800° total |
| Steering wheel rate | 360 °/s | Lock-to-lock ~5s at max motor |
| Motor time constant | 0.35 s | Hydraulic system inertia |
| Self-centering rate | 0.7 /s | Caster + alignment torque |
| Motor peak RPM | 80 RPM | KY170C motor (50W, 12V DC) |

## Kinematic Model
Bicycle model: `headingRate = speed / wheelbase * tan(frontWheelAngle)`
- No slip model (adequate for <12 km/h on soil)
- Heading 0 = north (+Z), clockwise positive
- Position: x += sin(heading) * speed * dt, z += cos(heading) * speed * dt
- Front wheel angle = steeringWheelAngle / steeringRatio
