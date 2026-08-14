#include <gtest/gtest.h>
#include <AppMain/tracking/ControlTimeGate.hpp>

using namespace AppMain::tracking;

TEST(ControlTimeGateTest, IdleHeldEpochsCalibrateBeforeStartCanActuate)
{
    SampleClock clock;
    std::int64_t timestamp = 10000;
    for (int epoch = 0; epoch < 2; ++epoch) {
        for (int held = 0; held < 9; ++held) {
            const auto step = advanceControlTime(clock, timestamp, 0.1, false);
            EXPECT_FALSE(step.allowActuation);
        }
        timestamp += 4000;
        EXPECT_FALSE(advanceControlTime(clock, timestamp, 0.1, false)
                .allowActuation);
    }

    const auto running = advanceControlTime(clock, timestamp, 0.025, true);
    EXPECT_TRUE(running.allowActuation);
    EXPECT_NEAR(running.scale, 4.0, 0.05);
}

TEST(ControlTimeGateTest, MissingTimestampNeverAllowsStart)
{
    SampleClock clock;
    for (int i = 0; i < 40; ++i) {
        EXPECT_FALSE(advanceControlTime(clock, -1, 0.1, true).allowActuation);
    }
}

TEST(ControlTimeGateTest, RegressionSuppressesActuationOnSameTick)
{
    SampleClock clock;
    std::int64_t timestamp = 10000;
    for (int i = 0; i < 12; ++i) {
        timestamp += 400;
        advanceControlTime(clock, timestamp, 0.1, false);
    }
    ASSERT_TRUE(advanceControlTime(clock, timestamp, 0.025, true).allowActuation);
    EXPECT_FALSE(advanceControlTime(clock, timestamp - 1, 0.025, true)
            .allowActuation);
}

TEST(ControlTimeGateTest, SharedSchedulerNormalizesTrackerAndProcessTimerCadence)
{
    EXPECT_EQ(normalizedPeriodMs(100, 1.0), 100);
    EXPECT_EQ(normalizedPeriodMs(100, 2.0), 50);
    EXPECT_EQ(normalizedPeriodMs(100, 4.0), 25);
    EXPECT_EQ(normalizedPeriodMs(100, 6.0), 17);
    EXPECT_EQ(normalizedPeriodMs(100, 100.0), 17)
        << "untrusted scales are bounded to the supported 6x domain";
}
