#include <gtest/gtest.h>
#include <AppMain/tracking/SampleClock.hpp>

using AppMain::tracking::SampleClock;

TEST(SampleClockTest, LearnsMillisecondsAtOneTwoFourAndSixTimes)
{
    for (const int multiplier : {1, 2, 4, 6}) {
        SampleClock clock;
        std::int64_t timestamp = 100000;
        double dt = 0.0;
        for (int i = 0; i < 12; ++i) {
            timestamp += 100 * multiplier;
            dt = clock.advance(timestamp, 0.1);
        }
        EXPECT_NEAR(clock.scale(), static_cast<double>(multiplier), 0.05);
        EXPECT_NEAR(dt, 0.1 * multiplier, 0.01);
    }
}

TEST(SampleClockTest, RejectsUnknownUnitsAndNonMonotonicSamples)
{
    SampleClock clock;
    EXPECT_DOUBLE_EQ(clock.advance(1, 0.1), 0.1);
    EXPECT_DOUBLE_EQ(clock.advance(2, 0.1), 0.1)
        << "a sequence counter must not be mistaken for milliseconds";
    EXPECT_DOUBLE_EQ(clock.advance(1, 0.1), 0.1)
        << "a regressing clock must fail closed to wall time";
    EXPECT_DOUBLE_EQ(clock.scale(), 1.0);
}

TEST(SampleClockTest, HeldFrameAdvancesAtLastValidatedMachineRate)
{
    SampleClock clock;
    std::int64_t timestamp = 10000;
    for (int i = 0; i < 12; ++i) {
        timestamp += 400;
        clock.advance(timestamp, 0.1);
    }
    EXPECT_NEAR(clock.advance(timestamp, 0.1), 0.4, 0.01);
}

TEST(SampleClockTest, LearnsAcrossOneHertzHeldEpochs)
{
    for (const int multiplier : {1, 2, 4, 6}) {
        SampleClock clock;
        std::int64_t timestamp = 10000;
        clock.advance(timestamp, 0.1);
        for (int epoch = 0; epoch < 12; ++epoch) {
            for (int held = 0; held < 9; ++held) {
                clock.advance(timestamp, 0.1);
            }
            timestamp += 1000 * multiplier;
            clock.advance(timestamp, 0.1);
        }
        EXPECT_NEAR(clock.scale(), static_cast<double>(multiplier), 0.05);
    }
}

TEST(SampleClockTest, JitterAndSkippedEpochsRemainBounded)
{
    SampleClock clock;
    std::int64_t timestamp = 10000;
    clock.advance(timestamp, 0.1);
    for (int epoch = 0; epoch < 16; ++epoch) {
        const int heldTicks = epoch == 7 ? 19 : (epoch % 2 == 0 ? 8 : 10);
        for (int held = 0; held < heldTicks; ++held) {
            clock.advance(timestamp, 0.1);
        }
        timestamp += (epoch == 7 ? 2000 : 1000) * 4;
        clock.advance(timestamp, 0.1);
    }
    EXPECT_NEAR(clock.scale(), 4.0, 0.35);
}

TEST(SampleClockTest, RegressionAndImplausibleJumpImmediatelyFailSafe)
{
    SampleClock clock;
    std::int64_t timestamp = 10000;
    for (int i = 0; i < 12; ++i) {
        timestamp += 400;
        clock.advance(timestamp, 0.1);
    }
    ASSERT_NEAR(clock.scale(), 4.0, 0.05);
    EXPECT_DOUBLE_EQ(clock.advance(timestamp - 1, 0.1), 0.1);
    EXPECT_DOUBLE_EQ(clock.scale(), 1.0);
    EXPECT_FALSE(clock.calibrated())
        << "actuation must be suppressed on the regression tick";
    EXPECT_TRUE(clock.regressed());
    EXPECT_FALSE(clock.valid());
    EXPECT_STREQ(clock.statusName(), "regression");

    for (int i = 0; i < 12; ++i) {
        timestamp += 400;
        clock.advance(timestamp, 0.1);
    }
    ASSERT_NEAR(clock.scale(), 4.0, 0.05);
    EXPECT_DOUBLE_EQ(clock.advance(timestamp + 1000000, 0.1), 0.1);
    EXPECT_DOUBLE_EQ(clock.scale(), 1.0);
    EXPECT_FALSE(clock.calibrated())
        << "actuation must be suppressed on the implausible-jump tick";
    EXPECT_TRUE(clock.implausibleJump());
    EXPECT_STREQ(clock.statusName(), "implausible_jump");
}

TEST(SampleClockTest, MissingSourceExpiresLearnedScale)
{
    SampleClock clock;
    std::int64_t timestamp = 10000;
    for (int i = 0; i < 12; ++i) {
        timestamp += 600;
        clock.advance(timestamp, 0.1);
    }
    ASSERT_NEAR(clock.scale(), 6.0, 0.05);
    for (int i = 0; i < 26; ++i) {
        clock.advance(-1, 0.1);
    }
    EXPECT_DOUBLE_EQ(clock.scale(), 1.0);
    EXPECT_FALSE(clock.calibrated())
        << "actuation must be suppressed on the source-loss tick";
    EXPECT_TRUE(clock.sourceLost());
    EXPECT_FALSE(clock.valid());
    EXPECT_STREQ(clock.statusName(), "source_loss");
}

TEST(SampleClockTest, FaultStatusClearsOnlyAfterFreshRecalibration)
{
    SampleClock clock;
    std::int64_t timestamp = 10000;
    for (int i = 0; i < 4; ++i) {
        timestamp += 200;
        clock.advance(timestamp, 0.1);
    }
    ASSERT_TRUE(clock.valid());
    ASSERT_STREQ(clock.statusName(), "valid");

    clock.advance(timestamp - 1, 0.1);
    ASSERT_TRUE(clock.regressed());
    timestamp += 200;
    clock.advance(timestamp, 0.1);
    EXPECT_TRUE(clock.regressed()) << "one sample cannot clear a time-domain fault";
    timestamp += 200;
    clock.advance(timestamp, 0.1);
    EXPECT_TRUE(clock.valid());
    EXPECT_STREQ(clock.statusName(), "valid");
}

TEST(SampleClockTest, LearnedScaleNormalizesOneHundredMillisecondScheduler)
{
    for (const int multiplier : {1, 2, 4, 6}) {
        SampleClock clock;
        std::int64_t timestamp = 10000;
        for (int i = 0; i < 12; ++i) {
            timestamp += 100 * multiplier;
            clock.advance(timestamp, 0.1);
        }
        const int periodMs = std::max(16, static_cast<int>(std::lround(
                100.0 / clock.scale())));
        EXPECT_NEAR(1000.0 / periodMs / multiplier, 10.0, 0.25)
            << "control/output cadence must stay near 10 Hz per machine second";
    }
}
