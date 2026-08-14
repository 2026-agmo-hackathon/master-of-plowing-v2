#include <gtest/gtest.h>
#include <AppMain/tracking/ClockTelemetry.hpp>

using namespace AppMain::tracking;

TEST(ClockTelemetryTest, ReportsTruthfulOneAndScaledCadence)
{
    for (const int multiplier : {1, 2, 4, 6}) {
        SampleClock clock;
        std::int64_t timestamp = 10000;
        for (int i = 0; i < 12; ++i) {
            timestamp += 100 * multiplier;
            clock.advance(timestamp, 0.1);
        }
        const auto telemetry = makeClockTelemetry(clock, timestamp,
                timestamp - 100 * multiplier, 100, 100);
        EXPECT_TRUE(telemetry.calibrated);
        EXPECT_TRUE(telemetry.valid);
        EXPECT_NEAR(telemetry.scale, multiplier, 0.05);
        EXPECT_EQ(telemetry.poseTimestampDeltaMs, 100 * multiplier);
        EXPECT_EQ(telemetry.trackingPeriodMs,
                normalizedPeriodMs(100, multiplier));
        EXPECT_EQ(telemetry.processTimerPeriodMs,
                normalizedPeriodMs(100, multiplier));
        EXPECT_STREQ(telemetry.status, "valid");
    }
}

TEST(ClockTelemetryTest, ReportsFaultAndNeverInventsRegressedPoseDelta)
{
    SampleClock clock;
    std::int64_t timestamp = 10000;
    for (int i = 0; i < 4; ++i) {
        timestamp += 400;
        clock.advance(timestamp, 0.1);
    }
    clock.advance(timestamp - 1, 0.1);
    const auto telemetry = makeClockTelemetry(clock, timestamp - 1,
            timestamp, 100, 100);
    EXPECT_FALSE(telemetry.valid);
    EXPECT_TRUE(telemetry.regression);
    EXPECT_EQ(telemetry.poseTimestampDeltaMs, 0);
    EXPECT_STREQ(telemetry.status, "regression");
}
