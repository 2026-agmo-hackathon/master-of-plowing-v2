#ifndef APPMAIN_TRACKING_GPSSAMPLESTORE_HPP
#define APPMAIN_TRACKING_GPSSAMPLESTORE_HPP

#include <cstdint>
#include <mutex>

namespace AppMain { namespace tracking {

struct GpsSample
{
    double lat = 0.0;
    double lon = 0.0;
    double heading = 0.0;
    double speed = 0.0;
    double alt = 0.0;
    std::int64_t sequence = -1;
    bool valid = false;
};

/** Publishes one coherent pose tuple between the SDK and tracking threads. */
class GpsSampleStore
{
public:
    void update(double lat, double lon, double heading, double speed, double alt,
                std::int64_t interfaceTimestamp = -1)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        if (interfaceTimestamp >= 0 && sample_.valid &&
                interfaceTimestamp < sample_.sequence) {
            return;
        }
        const std::int64_t sequence = interfaceTimestamp >= 0
                ? interfaceTimestamp : sample_.sequence + 1;
        sample_ = {lat, lon, heading, speed, alt, sequence, true};
    }

    GpsSample snapshot() const
    {
        std::lock_guard<std::mutex> guard(mutex_);
        return sample_;
    }

private:
    mutable std::mutex mutex_;
    GpsSample sample_;
};

}} // namespace AppMain::tracking
#endif
