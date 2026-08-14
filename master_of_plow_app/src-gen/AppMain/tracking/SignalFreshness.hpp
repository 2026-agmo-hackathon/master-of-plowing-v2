#ifndef APPMAIN_TRACKING_SIGNALFRESHNESS_HPP
#define APPMAIN_TRACKING_SIGNALFRESHNESS_HPP

#include <chrono>
#include <cstdint>

namespace AppMain {
namespace tracking {

/**
 * Turns an SDK interface timestamp into a bounded frame-freshness signal.
 *
 * Some generated SDK decoders report a numeric zero payload as JSON null and
 * consequently mark the interface invalid.  The interface timestamp still
 * advances for every received frame, so a changed timestamp is authoritative
 * evidence that the retained value (including zero) is current.  A held
 * timestamp expires instead of latching validity after the CAN stream stops.
 */
class SignalFreshness
{
public:
    using Clock = std::chrono::steady_clock;

    explicit SignalFreshness(std::chrono::milliseconds maxAge =
                                     std::chrono::milliseconds(350))
        : maxAge_(maxAge)
    {
    }

    bool observe(std::int64_t interfaceTimestamp,
                 bool sdkReportedValid,
                 Clock::time_point now = Clock::now())
    {
        if (interfaceTimestamp >= 0 &&
            (!hasTimestamp_ || interfaceTimestamp != lastTimestamp_))
        {
            hasTimestamp_ = true;
            lastTimestamp_ = interfaceTimestamp;
            lastArrival_ = now;
        }

        // Retain the SDK's legacy validity behavior only until this process has
        // observed a timestamp.  Afterwards freshness is bounded by frame time.
        if (!hasTimestamp_)
            return sdkReportedValid;

        return now - lastArrival_ <= maxAge_;
    }

private:
    std::chrono::milliseconds maxAge_;
    bool hasTimestamp_ = false;
    std::int64_t lastTimestamp_ = -1;
    Clock::time_point lastArrival_;
};

} // namespace tracking
} // namespace AppMain

#endif
