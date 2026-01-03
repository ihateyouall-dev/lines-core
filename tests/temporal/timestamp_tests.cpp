#include "lines/temporal.h"
#include <gtest/gtest.h>

using namespace Lines::Temporal;

TEST(TimestampArithmetic, Normalization) {
    EXPECT_EQ(++Timestamp(Seconds{86399}), Timestamp(Seconds{0}));
    EXPECT_EQ(--Timestamp(Seconds{0}), Timestamp(Seconds{86399}));
    EXPECT_EQ(Timestamp(Seconds{86400}), Timestamp(Seconds{0}));
    EXPECT_EQ(Timestamp(Seconds{-1}), Timestamp(Seconds{86399}));
    EXPECT_EQ(Timestamp(Seconds{(86400 * 2) + 42}), Timestamp(Seconds{42}));
}

TEST(TimestampArithmetic, IncrementDecrement) {
    Timestamp t{Seconds{10}}; // NOLINT

    EXPECT_EQ(++t, Timestamp(Seconds{11}));
    EXPECT_EQ(t, Timestamp(Seconds{11}));
    EXPECT_EQ(t++, Timestamp(Seconds{11}));
    EXPECT_EQ(t, Timestamp(Seconds{12}));
    EXPECT_EQ(--t, Timestamp(Seconds{11}));
    EXPECT_EQ(t--, Timestamp(Seconds{11}));
    EXPECT_EQ(t, Timestamp(Seconds{10}));
}

TEST(TimestampArithmetic, AddDuration) {
    EXPECT_EQ(Timestamp(Seconds{86300}) + Seconds{200}, Timestamp(Seconds{100}));
    EXPECT_EQ(Timestamp(Seconds{100}) + Minutes{1}, Timestamp(Seconds{160}));
    EXPECT_EQ(Minutes{1} + Timestamp(Seconds{10}), Timestamp(Seconds{70}));
}

TEST(TimestampArithmetic, SubDuration) {
    EXPECT_EQ(Timestamp(Seconds{100}) - Seconds{200}, Timestamp(Seconds{86300}));
    EXPECT_EQ(Timestamp(Seconds{0}) - Seconds{1}, Timestamp(Seconds{86399}));
}

TEST(TimestampArithmetic, Difference) {
    EXPECT_EQ(Timestamp(Seconds{100}) - Timestamp(Seconds{40}), Seconds{60});
    EXPECT_EQ(Timestamp(Seconds{40}) - Timestamp(Seconds{100}), Seconds{-60});
}

TEST(TimestampAccessors, HMS) {
    Timestamp t{Hours{1}, Minutes{2}, Seconds{3}}; // NOLINT

    EXPECT_EQ(t.hours(), Hours{1});
    EXPECT_EQ(t.minutes(), Minutes{2});
    EXPECT_EQ(t.seconds(), Seconds{3});
}

TEST(TimestampFormatting, HHMMSS) {
    EXPECT_EQ(Timestamp(Hours{1}, Minutes{2}, Seconds{3}).hh_mm_ss(), "01:02:03");
    EXPECT_EQ(Timestamp(Seconds{0}).hh_mm_ss(), "00:00:00");
    EXPECT_EQ(Timestamp(Seconds{86399}).hh_mm_ss(), "23:59:59");
}
