#include "latertemporal.h"
#include <gtest/gtest.h>

using namespace Later::Temporal;

TEST(TimePointConstruction, TimeSinceEpoch) {
    EXPECT_EQ(TimePoint(Seconds{0}).time_since_epoch(), Seconds{0});
    EXPECT_EQ(TimePoint(Seconds{42}).time_since_epoch(), Seconds{42});
}

TEST(TimePointComparison, Ordering) {
    EXPECT_LT(TimePoint(Seconds{10}), TimePoint(Seconds{20}));
    EXPECT_GT(TimePoint(Seconds{20}), TimePoint(Seconds{10}));
    EXPECT_EQ(TimePoint(Seconds{10}), TimePoint(Seconds{10}));
}

TEST(TimePointArithmetic, IncrementDecrement) {
    EXPECT_EQ(++TimePoint(Seconds{10}), TimePoint(Seconds{11}));
    EXPECT_EQ(TimePoint(Seconds{10})++, TimePoint(Seconds{10}));
    EXPECT_EQ(--TimePoint(Seconds{10}), TimePoint(Seconds{9}));
    EXPECT_EQ(TimePoint(Seconds{10})--, TimePoint(Seconds{10}));
}

TEST(TimePointArithmetic, AddDuration) {
    EXPECT_EQ(TimePoint(Seconds{100}) + Seconds{20}, TimePoint(Seconds{120}));
    EXPECT_EQ(TimePoint(Seconds{100}) + Minutes{1}, TimePoint(Seconds{160}));
    EXPECT_EQ(Seconds{15} + TimePoint(Seconds{30}), TimePoint(Seconds{45}));
}

TEST(TimePointArithmetic, SubDuration) {
    EXPECT_EQ(TimePoint(Seconds{100}) - Seconds{40}, TimePoint(Seconds{60}));
    EXPECT_EQ(TimePoint(Seconds{0}) - Seconds{1}, TimePoint(Seconds{-1}));
}

TEST(TimePointArithmetic, Difference) {
    EXPECT_EQ(TimePoint(Seconds{200}) - TimePoint(Seconds{50}), Seconds{150});
    EXPECT_EQ(TimePoint(Seconds{50}) - TimePoint(Seconds{200}), Seconds{-150});
}

TEST(TimePointWrapping, DateTime) {
    EXPECT_EQ(DateTime{TimePoint{Seconds{86401}}}.date(), Date{Days{1}});
    EXPECT_EQ(DateTime{TimePoint{Seconds{86401}}}.time(), Timestamp{Seconds{1}});
}
