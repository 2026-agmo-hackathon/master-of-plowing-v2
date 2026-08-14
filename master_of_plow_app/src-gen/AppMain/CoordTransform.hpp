/*
 * AppMain/CoordTransform.hpp
 * WGS84 <-> UTM (EPSG:32652) coordinate transform utility.
 */

#ifndef APPMAIN_COORDTRANSFORM_HPP
#define APPMAIN_COORDTRANSFORM_HPP

namespace AppMain
{

    struct UtmCoord
    {
        double easting;
        double northing;
    };

    struct EnuCoord
    {
        double x;
        double z;
    };

    class CoordTransform
    {
    public:
        // Lat/Lon (decimal degrees) -> UTM zone 52N (meters), EPSG:32652.
        static UtmCoord wgs84ToUtm(double lat, double lon);

        // UTM -> simulator ENU (meters).
        // Simulator coordinate convention: X = east(+), Z = south(+).
        // Therefore z = originNorthing - northing (north -> south positive).
        static EnuCoord utmToEnu(double easting, double northing,
                                 double originEasting, double originNorthing);
    };

} // namespace AppMain

#endif // APPMAIN_COORDTRANSFORM_HPP
