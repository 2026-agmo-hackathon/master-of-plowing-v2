#ifndef APPMAIN_TRACKING_CLOCKLOG_HPP
#define APPMAIN_TRACKING_CLOCKLOG_HPP

#include <AppMain/tracking/ClockTelemetry.hpp>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

namespace AppMain { namespace tracking {

struct ClockLogValues
{
    ClockTelemetry clock;
    bool outputAuthority = false;
    const char* runState = "Idle";
    double speedMps = 0.0;
    int waypointIndex = -1;
    double steerTargetDeg = 0.0;
    int outputMotorCommand = 0;
};

inline std::string formatClockLog(const ClockLogValues& values)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(3)
        << "CLOCK_LOG"
        << " scale=" << values.clock.scale
        << " calibrated=" << (values.clock.calibrated ? 1 : 0)
        << " valid=" << (values.clock.valid ? 1 : 0)
        << " source_loss=" << (values.clock.sourceLoss ? 1 : 0)
        << " regression=" << (values.clock.regression ? 1 : 0)
        << " implausible_jump=" << (values.clock.implausibleJump ? 1 : 0)
        << " status=" << values.clock.status
        << " pose_sample_id=" << values.clock.poseSampleId
        << " pose_delta_ms=" << values.clock.poseTimestampDeltaMs
        << " tracking_period_ms=" << values.clock.trackingPeriodMs
        << " process_timer_period_ms=" << values.clock.processTimerPeriodMs
        << " output_authority=" << (values.outputAuthority ? 1 : 0)
        << " run_state=" << values.runState
        << " speed_mps=" << values.speedMps
        << " waypoint=" << values.waypointIndex
        << " controller_steer_target_deg=" << values.steerTargetDeg
        << " output_motor_cmd=" << values.outputMotorCommand;
    return out.str();
}

class ClockLogRateLimiter
{
public:
    explicit ClockLogRateLimiter(std::chrono::milliseconds interval =
            std::chrono::milliseconds(1000))
        : interval_(interval)
    {
    }

    bool shouldLog(std::chrono::steady_clock::time_point now)
    {
        if (!started_ || now - last_ >= interval_) {
            started_ = true;
            last_ = now;
            return true;
        }
        return false;
    }

private:
    std::chrono::milliseconds interval_;
    std::chrono::steady_clock::time_point last_ {};
    bool started_ = false;
};

}} // namespace AppMain::tracking
#endif
