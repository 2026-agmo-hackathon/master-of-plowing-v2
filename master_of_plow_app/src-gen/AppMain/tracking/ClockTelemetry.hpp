#ifndef APPMAIN_TRACKING_CLOCKTELEMETRY_HPP
#define APPMAIN_TRACKING_CLOCKTELEMETRY_HPP

#include <AppMain/tracking/ControlTimeGate.hpp>

#include <cstdint>

namespace AppMain { namespace tracking {

struct ClockTelemetry
{
    double scale = 1.0;
    bool calibrated = false;
    bool valid = false;
    bool sourceLoss = false;
    bool regression = false;
    bool implausibleJump = false;
    const char* status = "calibrating";
    std::int64_t poseSampleId = -1;
    std::int64_t poseTimestampDeltaMs = 0;
    int trackingPeriodMs = 100;
    int processTimerPeriodMs = 100;
};

inline ClockTelemetry makeClockTelemetry(const SampleClock& clock,
        std::int64_t poseSampleId, std::int64_t previousPoseSampleId,
        int trackingBasePeriodMs, int processBasePeriodMs)
{
    ClockTelemetry result;
    result.scale = clock.scale();
    result.calibrated = clock.calibrated();
    result.valid = clock.valid();
    result.sourceLoss = clock.sourceLost();
    result.regression = clock.regressed();
    result.implausibleJump = clock.implausibleJump();
    result.status = clock.statusName();
    result.poseSampleId = poseSampleId;
    result.poseTimestampDeltaMs = previousPoseSampleId >= 0 &&
            poseSampleId >= previousPoseSampleId
            ? poseSampleId - previousPoseSampleId : 0;
    result.trackingPeriodMs = normalizedPeriodMs(trackingBasePeriodMs, result.scale);
    result.processTimerPeriodMs = normalizedPeriodMs(processBasePeriodMs, result.scale);
    return result;
}

}} // namespace AppMain::tracking
#endif
