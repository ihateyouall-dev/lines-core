#include "lines/temporal.h"
#include <gtest/gtest.h>

using namespace Lines::Temporal;

TEST(DateConstruction, FromDays) {
    Date d{Days{0}}; // NOLINT
    EXPECT_EQ(d.year(), Year{1970});
    EXPECT_EQ(d.month(), Month{1});
    EXPECT_EQ(d.day(), Day{1});
}

TEST(DateConstruction, FromYMD) {
    Date d{Year{2028}, Month{2}, Day{29}}; // NOLINT
    EXPECT_EQ(d.year(), Year{2028});
    EXPECT_EQ(d.month(), Month{2});
    EXPECT_EQ(d.day(), Day{29});
}

TEST(DateComparison, OrderingAndEquality) {
    Date a{Year{2028}, Month{1}, Day{1}}; // NOLINT
    Date b{Year{2028}, Month{1}, Day{2}}; // NOLINT
    Date c{Year{2028}, Month{1}, Day{1}}; // NOLINT

    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_EQ(a, c);
}

TEST(DateArithmetic, DifferenceInDays) {
    Date a{Year{2028}, Month{1}, Day{10}}; // NOLINT
    Date b{Year{2028}, Month{1}, Day{1}};  // NOLINT

    EXPECT_EQ(a - b, Days{9});
    EXPECT_EQ(b - a, Days{-9});
}

TEST(DateArithmetic, AddSubtractDays) {
    Date d{Year{2028}, Month{1}, Day{1}}; // NOLINT

    EXPECT_EQ((d + Days{1}).day(), Day{2});
    EXPECT_EQ((d + Days{31}).month(), Month{2});
    EXPECT_EQ((d - Days{1}).day(), Day{31});
    EXPECT_EQ((d - Days{1}).month(), Month{12});
}

TEST(DateArithmetic, IncrementDecrement) {
    Date d{Year{2028}, Month{1}, Day{31}}; // NOLINT

    EXPECT_EQ(++d, Date(Year{2028}, Month{2}, Day{1}));
    EXPECT_EQ(d++, Date(Year{2028}, Month{2}, Day{1}));
    EXPECT_EQ(d, Date(Year{2028}, Month{2}, Day{2}));

    EXPECT_EQ(--d, Date(Year{2028}, Month{2}, Day{1}));
    EXPECT_EQ(d--, Date(Year{2028}, Month{2}, Day{1}));
    EXPECT_EQ(d, Date(Year{2028}, Month{1}, Day{31}));
}

TEST(DateAccessors, WeekdayStable) {
    Date d1{Year{1970}, Month{1}, Day{7}}; // NOLINT
    Date d2 = d1 + Days{7};                // NOLINT

    EXPECT_EQ(d1.weekday(), d2.weekday());
}

TEST(DateFormatting, YYYYMMDD) {
    Date d{Year{2028}, Month{1}, Day{1}}; // NOLINT
    EXPECT_EQ(d.yyyy_mm_dd(), "2028-01-01");
}
