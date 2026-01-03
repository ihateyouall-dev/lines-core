#include "latertemporal.h"
#include <gtest/gtest.h>

using namespace Later::Temporal;

TEST(DateConstruction, FromDays) {
    Date d{Days{0}}; // NOLINT
    EXPECT_EQ(d.year(), Year{1970});
    EXPECT_EQ(d.month(), Month{1});
    EXPECT_EQ(d.day(), Day{1});
}

TEST(DateConstruction, FromYMD) {
    Date d{Year{2024}, Month{2}, Day{29}}; // NOLINT
    EXPECT_EQ(d.year(), Year{2024});
    EXPECT_EQ(d.month(), Month{2});
    EXPECT_EQ(d.day(), Day{29});
}

TEST(DateConstruction, InvalidDateThrows) {
    EXPECT_THROW(Date(Year{2024}, Month{2}, Day{30}), std::invalid_argument);

    EXPECT_THROW(Date(Year{2023}, Month{2}, Day{29}), std::invalid_argument);

    EXPECT_THROW(Date(Year{2024}, Month{13}, Day{1}), std::invalid_argument);
}

TEST(DateComparison, OrderingAndEquality) {
    Date a{Year{2024}, Month{1}, Day{1}}; // NOLINT
    Date b{Year{2024}, Month{1}, Day{2}}; // NOLINT
    Date c{Year{2024}, Month{1}, Day{1}}; // NOLINT

    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_EQ(a, c);
}

TEST(DateArithmetic, DifferenceInDays) {
    Date a{Year{2024}, Month{1}, Day{10}}; // NOLINT
    Date b{Year{2024}, Month{1}, Day{1}};  // NOLINT

    EXPECT_EQ(a - b, Days{9});
    EXPECT_EQ(b - a, Days{-9});
}

TEST(DateArithmetic, AddSubtractDays) {
    Date d{Year{2024}, Month{1}, Day{1}}; // NOLINT

    EXPECT_EQ((d + Days{1}).day(), Day{2});
    EXPECT_EQ((d + Days{31}).month(), Month{2});
    EXPECT_EQ((d - Days{1}).day(), Day{31});
    EXPECT_EQ((d - Days{1}).month(), Month{12});
}

TEST(DateArithmetic, IncrementDecrement) {
    Date d{Year{2024}, Month{1}, Day{31}}; // NOLINT

    EXPECT_EQ(++d, Date(Year{2024}, Month{2}, Day{1}));
    EXPECT_EQ(d++, Date(Year{2024}, Month{2}, Day{1}));
    EXPECT_EQ(d, Date(Year{2024}, Month{2}, Day{2}));

    EXPECT_EQ(--d, Date(Year{2024}, Month{2}, Day{1}));
    EXPECT_EQ(d--, Date(Year{2024}, Month{2}, Day{1}));
    EXPECT_EQ(d, Date(Year{2024}, Month{1}, Day{31}));
}

TEST(DateAccessors, WeekdayStable) {
    Date d1{Year{1970}, Month{1}, Day{1}}; // NOLINT
    Date d2 = d1 + Days{7};                // NOLINT

    EXPECT_EQ(d1.weekday(), d2.weekday());
}
