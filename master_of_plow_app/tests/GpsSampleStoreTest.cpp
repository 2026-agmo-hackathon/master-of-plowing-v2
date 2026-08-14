#include <gtest/gtest.h>
#include <AppMain/tracking/GpsSampleStore.hpp>

#include <atomic>
#include <thread>

using AppMain::tracking::GpsSampleStore;

TEST(GpsSampleStoreTest, SnapshotIsCoherentAndSequencesIdenticalValues)
{
    GpsSampleStore store;
    EXPECT_FALSE(store.snapshot().valid);

    store.update(1.0, 2.0, 3.0, 4.0, 5.0);
    const auto first = store.snapshot();
    store.update(1.0, 2.0, 3.0, 4.0, 5.0);
    const auto second = store.snapshot();

    EXPECT_TRUE(second.valid);
    EXPECT_EQ(second.sequence, first.sequence + 1);
    EXPECT_DOUBLE_EQ(second.lat, 1.0);
    EXPECT_DOUBLE_EQ(second.alt, 5.0);
}

TEST(GpsSampleStoreTest, InterfaceTimestampDefinesFreshnessAndCannotRegress)
{
    GpsSampleStore store;
    store.update(1.0, 2.0, 3.0, 4.0, 5.0, 1000);
    store.update(1.0, 2.0, 3.0, 4.0, 5.0, 1000);
    EXPECT_EQ(store.snapshot().sequence, 1000)
        << "a held provider object is not a fresh GPS frame";

    store.update(9.0, 18.0, 27.0, 36.0, 45.0, 999);
    EXPECT_DOUBLE_EQ(store.snapshot().lat, 1.0)
        << "a regressing interface timestamp must not replace the pose";

    store.update(6.0, 12.0, 18.0, 24.0, 30.0, 1100);
    EXPECT_EQ(store.snapshot().sequence, 1100);
    EXPECT_DOUBLE_EQ(store.snapshot().lat, 6.0);
}

TEST(GpsSampleStoreTest, ConcurrentReaderNeverObservesTornTuple)
{
    GpsSampleStore store;
    std::atomic<bool> finished{false};
    std::atomic<bool> torn{false};
    std::thread writer([&] {
        for (int i = 1; i <= 20000; ++i) {
            const double v = static_cast<double>(i);
            store.update(v, 2.0 * v, 3.0 * v, 4.0 * v, 5.0 * v);
        }
        finished = true;
    });
    while (!finished) {
        const auto sample = store.snapshot();
        if (sample.valid &&
                (sample.lon != 2.0 * sample.lat ||
                 sample.heading != 3.0 * sample.lat ||
                 sample.speed != 4.0 * sample.lat ||
                 sample.alt != 5.0 * sample.lat)) {
            torn = true;
        }
    }
    writer.join();
    EXPECT_FALSE(torn);
}
