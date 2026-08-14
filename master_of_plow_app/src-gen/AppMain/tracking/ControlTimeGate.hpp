#ifndef APPMAIN_TRACKING_CONTROLTIMEGATE_HPP
#define APPMAIN_TRACKING_CONTROLTIMEGATE_HPP

#include <AppMain/tracking/SampleClock.hpp>

#include <algorithm>
#include <cmath>

namespace AppMain { namespace tracking {

struct ControlTimeStep
{
    double dtS = 0.0;
    double scale = 1.0;
    bool allowActuation = false;
};

inline ControlTimeStep advanceControlTime(SampleClock& clock,
        std::int64_t sampleId, double wallDtS, bool driving)
{
    const double dtS = clock.advance(sampleId, wallDtS);
    return {dtS, clock.scale(), driving && clock.calibrated()};
}

inline int normalizedPeriodMs(int basePeriodMs, double scale)
{
    const double boundedScale = std::clamp(scale, 1.0, 6.0);
    return std::max(16, static_cast<int>(std::lround(
            static_cast<double>(basePeriodMs) / boundedScale)));
}

}} // namespace AppMain::tracking
#endif
