#include <gtest/gtest.h>
#include <AppMain/tracking/ClockLog.hpp>

using namespace AppMain::tracking;

TEST(ClockLogTest, FormatsTruthfulSafeStructuredValuesWithoutSensitiveData)
{
    ClockLogValues values;
    values.clock.scale = 2.0;
    values.clock.calibrated = true;
    values.clock.valid = true;
    values.clock.status = "valid";
    values.clock.poseSampleId = 1234;
    values.clock.poseTimestampDeltaMs = 200;
    values.clock.trackingPeriodMs = 50;
    values.clock.processTimerPeriodMs = 50;
    values.outputAuthority = false;
    values.runState = "Idle";
    values.speedMps = 0.0;
    values.waypointIndex = -1;
    values.steerTargetDeg = 0.0;
    values.outputMotorCommand = 0;

    const std::string line = formatClockLog(values);
    EXPECT_NE(line.find("CLOCK_LOG scale=2.000 calibrated=1 valid=1"),
              std::string::npos);
    EXPECT_NE(line.find("pose_sample_id=1234 pose_delta_ms=200"),
              std::string::npos);
    EXPECT_NE(line.find("tracking_period_ms=50 process_timer_period_ms=50"),
              std::string::npos);
    EXPECT_NE(line.find("output_authority=0 run_state=Idle speed_mps=0.000"),
              std::string::npos);
    EXPECT_NE(line.find("waypoint=-1 controller_steer_target_deg=0.000 output_motor_cmd=0"),
              std::string::npos);
    EXPECT_EQ(line.find("team"), std::string::npos);
    EXPECT_EQ(line.find("token"), std::string::npos);
    EXPECT_EQ(line.find("leaderboard"), std::string::npos);
}

TEST(ClockLogTest, IdleReportsActualSafeOutputDespiteStaleControllerDebug)
{
    ClockLogValues values;
    values.runState = "Idle";
    values.steerTargetDeg = 18.0;       // cached controller debug from the run
    values.outputMotorCommand = 0;      // authoritative holdSafe MachineOutputs
    values.outputAuthority = false;
    const std::string line = formatClockLog(values);
    EXPECT_NE(line.find("controller_steer_target_deg=18.000"), std::string::npos);
    EXPECT_NE(line.find("output_motor_cmd=0"), std::string::npos);
    EXPECT_NE(line.find("output_authority=0"), std::string::npos);
}

TEST(ClockLogTest, EmitsAtMostOncePerWallSecond)
{
    ClockLogRateLimiter limiter;
    const auto start = std::chrono::steady_clock::time_point {};
    EXPECT_TRUE(limiter.shouldLog(start));
    EXPECT_FALSE(limiter.shouldLog(start + std::chrono::milliseconds(999)));
    EXPECT_TRUE(limiter.shouldLog(start + std::chrono::milliseconds(1000)));
    EXPECT_FALSE(limiter.shouldLog(start + std::chrono::milliseconds(1500)));
    EXPECT_TRUE(limiter.shouldLog(start + std::chrono::milliseconds(2000)));
}

TEST(ClockLogTest, RunResetCannotReopenTheProcessLifetimeRateGate)
{
    ClockLogRateLimiter limiter;
    const auto beforeReset = std::chrono::steady_clock::time_point {} +
            std::chrono::seconds(10);
    EXPECT_TRUE(limiter.shouldLog(beforeReset));
    // TrackingLoop::resetRunState deliberately has no rate-limiter reset. A run
    // transition immediately after the prior line therefore stays suppressed.
    EXPECT_FALSE(limiter.shouldLog(beforeReset + std::chrono::milliseconds(1)));
    EXPECT_TRUE(limiter.shouldLog(beforeReset + std::chrono::seconds(1)));
}
