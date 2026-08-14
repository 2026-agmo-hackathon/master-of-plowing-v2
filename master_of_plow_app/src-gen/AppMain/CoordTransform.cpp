/*
 * AppMain/CoordTransform.cpp
 * WGS84 -> UTM (EPSG:32652) using Karney/Kruger n-series (6th order, µm-accurate).
 * Replaces boost::geometry::srs to keep compile time low while matching reference output.
 */

#include <AppMain/CoordTransform.hpp>
#include <cmath>

namespace AppMain
{
    namespace
    {
        constexpr double kPi = 3.14159265358979323846;

        // WGS84
        constexpr double kA  = 6378137.0;
        constexpr double kF  = 1.0 / 298.257223563;
        constexpr double kK0 = 0.9996;
        constexpr double kFalseEasting  = 500000.0;
        constexpr double kFalseNorthing = 0.0;
        constexpr double kLon0Deg = 129.0;
        constexpr double kLon0 = kLon0Deg * kPi / 180.0;

        // Third flattening n = f/(2-f)
        constexpr double kN  = kF / (2.0 - kF);
        constexpr double kN2 = kN  * kN;
        constexpr double kN3 = kN2 * kN;
        constexpr double kN4 = kN3 * kN;
        constexpr double kN5 = kN4 * kN;
        constexpr double kN6 = kN5 * kN;

        // A-bar: rectifying radius
        constexpr double kABar = (kA / (1.0 + kN)) *
            (1.0 + kN2/4.0 + kN4/64.0 + kN6/256.0);

        // Kruger series alpha coefficients (6th order)
        constexpr double kAlpha1 = kN/2.0 - 2.0*kN2/3.0 + 5.0*kN3/16.0
                                 + 41.0*kN4/180.0 - 127.0*kN5/288.0
                                 + 7891.0*kN6/37800.0;
        constexpr double kAlpha2 = 13.0*kN2/48.0 - 3.0*kN3/5.0
                                 + 557.0*kN4/1440.0 + 281.0*kN5/630.0
                                 - 1983433.0*kN6/1935360.0;
        constexpr double kAlpha3 = 61.0*kN3/240.0 - 103.0*kN4/140.0
                                 + 15061.0*kN5/26880.0 + 167603.0*kN6/181440.0;
        constexpr double kAlpha4 = 49561.0*kN4/161280.0 - 179.0*kN5/168.0
                                 + 6601661.0*kN6/7257600.0;
        constexpr double kAlpha5 = 34729.0*kN5/80640.0 - 3418889.0*kN6/1995840.0;
        constexpr double kAlpha6 = 212378941.0*kN6/319334400.0;
    }

    UtmCoord CoordTransform::wgs84ToUtm(double lat, double lon)
    {
        const double phi    = lat * kPi / 180.0;
        const double lambda = lon * kPi / 180.0;
        const double dLambda = lambda - kLon0;

        const double sinPhi = std::sin(phi);
        const double sqrtN_over_1plusN = 2.0 * std::sqrt(kN) / (1.0 + kN);

        // conformal latitude -> t
        const double t = std::sinh(
              std::atanh(sinPhi)
            - sqrtN_over_1plusN * std::atanh(sqrtN_over_1plusN * sinPhi));

        const double cosDLambda = std::cos(dLambda);
        const double sinDLambda = std::sin(dLambda);
        const double sqrtOneT2  = std::sqrt(1.0 + t * t);

        const double xiP  = std::atan2(t, cosDLambda);
        const double etaP = std::atanh(sinDLambda / sqrtOneT2);

        double xi  = xiP;
        double eta = etaP;
        // j = 1..6
        const double alpha[6] = { kAlpha1, kAlpha2, kAlpha3, kAlpha4, kAlpha5, kAlpha6 };
        for (int j = 1; j <= 6; ++j) {
            const double twoJ = 2.0 * j;
            xi  += alpha[j-1] * std::sin(twoJ * xiP) * std::cosh(twoJ * etaP);
            eta += alpha[j-1] * std::cos(twoJ * xiP) * std::sinh(twoJ * etaP);
        }

        UtmCoord result;
        result.easting  = kK0 * kABar * eta + kFalseEasting;
        result.northing = kK0 * kABar * xi  + kFalseNorthing;
        return result;
    }

    EnuCoord CoordTransform::utmToEnu(double easting, double northing,
                                      double originEasting, double originNorthing)
    {
        EnuCoord result;
        result.x = easting - originEasting;
        // Simulator Z axis points south.
        result.z = originNorthing - northing;
        return result;
    }

} // namespace AppMain
