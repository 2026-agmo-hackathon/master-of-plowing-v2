#include <gtest/gtest.h>
#include <AppMain/orchestration/LaunchLifecycle.hpp>

#include <atomic>
#include <chrono>
#include <future>
#include <thread>
#include <vector>

using AppMain::orchestration::LaunchLifecycle;

TEST(LaunchLifecycleTest, CloseWaitsForLaunchClaimedBeforeMemberAccess)
{
    LaunchLifecycle gate;
    std::promise<void> claimed,release;
    auto releaseFuture=release.get_future().share();
    std::thread launcher([&]{auto token=gate.tryEnter();ASSERT_TRUE(token);claimed.set_value();releaseFuture.wait();});
    claimed.get_future().wait();
    auto closer=std::async(std::launch::async,[&]{gate.closeAndWait();});
    EXPECT_EQ(closer.wait_for(std::chrono::milliseconds(50)),std::future_status::timeout);
    EXPECT_FALSE(gate.tryEnter());
    release.set_value();launcher.join();
    EXPECT_EQ(closer.wait_for(std::chrono::seconds(1)),std::future_status::ready);
}

TEST(LaunchLifecycleTest, RepeatedLaunchCloseRaceIsTsanFriendly)
{
    for(int iteration=0;iteration<100;++iteration){
        LaunchLifecycle gate;std::atomic<int> inside{0};std::vector<std::thread> launchers;
        for(int i=0;i<8;++i)launchers.emplace_back([&]{auto token=gate.tryEnter();if(token){++inside;std::this_thread::yield();--inside;}});
        gate.closeAndWait();
        for(auto& t:launchers)t.join();
        EXPECT_EQ(inside.load(),0);EXPECT_FALSE(gate.tryEnter());
    }
}
