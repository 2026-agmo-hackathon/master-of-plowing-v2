#include <gtest/gtest.h>

#include <chrono>

#include <AppMain/tracking/SignalFreshness.hpp>

using AppMain::tracking::SignalFreshness;

TEST(SignalFreshnessTest, AdvancingTimestampMakesZeroPayloadUsable)
{
    SignalFreshness freshness;
    const auto now = SignalFreshness::Clock::time_point{};

    // The SDK reports false for the zero payload, but the new timestamp proves
    // that the encoder frame arrived and zero is a real angle.
    EXPECT_TRUE(freshness.observe(1000, false, now));
    EXPECT_TRUE(freshness.observe(1100, false, now + std::chrono::milliseconds(100)));
}

TEST(SignalFreshnessTest, HeldTimestampExpires)
{
    SignalFreshness freshness(std::chrono::milliseconds(350));
    const auto now = SignalFreshness::Clock::time_point{};

    EXPECT_TRUE(freshness.observe(1000, false, now));
    EXPECT_TRUE(freshness.observe(1000, false, now + std::chrono::milliseconds(350)));
    EXPECT_FALSE(freshness.observe(1000, false, now + std::chrono::milliseconds(351)));
}

TEST(SignalFreshnessTest, MissingTimestampUsesSdkValidityOnlyAtStartup)
{
    SignalFreshness freshness;
    const auto now = SignalFreshness::Clock::time_point{};

    EXPECT_FALSE(freshness.observe(-1, false, now));
    EXPECT_TRUE(freshness.observe(-1, true, now));

    EXPECT_TRUE(freshness.observe(1000, false, now));
    EXPECT_FALSE(freshness.observe(1000, true,
                                   now + std::chrono::milliseconds(351)));
}
