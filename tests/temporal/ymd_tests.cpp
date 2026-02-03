/*
  #        #  #     #  # # # #  # # # #
  #        #  # #   #  #        #
  #        #  #   # #  # # # #  # # # #
  #        #  #     #  #              #
  # # # #  #  #     #  # # # #  # # # #
  Copyright (c) 2025-2026 I.H.Y.A.D.

  Lines Project, Core library.
  This file is licensed under GNU Lesser General Public License v3.0 or later.
  See LICENSE for more information.
  SPDX-License-Identifier: LGPL-3.0-or-later.
*/
#include "lines/temporal/duration.hpp"
#include <climits>
#include <gtest/gtest.h>
#include <lines/temporal/ymd.hpp>

using namespace Lines::Temporal;

TEST(Year, Validation) {
    Year yr(INT_MAX);
    EXPECT_TRUE(yr.ok());
}

TEST(Year, IntConversion) {
    Year yr{2026};
    EXPECT_EQ(int(yr), 2026);
}

TEST(Year, LeapYear) {
    EXPECT_TRUE(Year{2024}.is_leap());
    EXPECT_FALSE(Year{2022}.is_leap());
    EXPECT_TRUE(Year{2000}.is_leap());
    EXPECT_FALSE(Year{1900}.is_leap());
}

TEST(Year, Comparison) {
    EXPECT_LT(Year{2025}, Year{2026});
    EXPECT_GT(Year{2026}, Year{2025});
    EXPECT_LE(Year{2025}, Year{2026});
    EXPECT_LE(Year{2026}, Year{2026});
    EXPECT_GE(Year{2026}, Year{2025});
    EXPECT_GE(Year{2026}, Year{2026});
    EXPECT_NE(Year{0}, Year{-1});
}

TEST(Year, Arithmetic) {
    EXPECT_EQ(Year{2026} ++, Year{2026});
    EXPECT_EQ(++Year{2026}, Year{2027});
    EXPECT_EQ(Year{2026} --, Year{2026});
    EXPECT_EQ(--Year{2026}, Year{2025});
    EXPECT_EQ(Year{2026} + Years{2}, Year{2028});
    EXPECT_EQ(Year{2026} - Years{2}, Year{2024});
    EXPECT_EQ(Year{2026} - Year{2024}, Years{2});
    EXPECT_EQ(Years{2} + Year{2026}, Year{2028});

    Year yr{2026};

    EXPECT_EQ(yr += Years{2}, Year{2028});
    EXPECT_EQ(yr -= Years{2}, Year{2026});
    EXPECT_EQ(-yr, Year{-2026});
    EXPECT_EQ(+yr, Year{2026});
}

TEST(Month, Validation) {
    Month m{13};
    EXPECT_FALSE(m.ok());
    --m;
    EXPECT_TRUE(m.ok());
}

TEST(Month, UnsignedConversion) {
    Month m{12};
    EXPECT_EQ(unsigned(m), 12U);
}

TEST(Month, Comparison) {
    EXPECT_LT(Month{1}, Month{12});
    EXPECT_GT(Month{12}, Month{1});
    EXPECT_LE(Month{1}, Month{2});
    EXPECT_LE(Month{2}, Month{2});
    EXPECT_GE(Month{2}, Month{1});
    EXPECT_GE(Month{2}, Month{2});
    EXPECT_NE(Month{1}, Month{12});
}

TEST(Month, Arithmetic) {
    EXPECT_EQ(Month{11} ++, Month{11});
    EXPECT_EQ(++Month{11}, Month{12});
    EXPECT_EQ(Month{11} --, Month{11});
    EXPECT_EQ(--Month{11}, Month{10});
    EXPECT_EQ(Month{10} + Months{2}, Month{12});
    EXPECT_EQ(Month{12} - Months{2}, Month{10});
    EXPECT_EQ(Month{12} - Month{11}, Months{1});
    EXPECT_EQ(Months{2} + Month{10}, Month{12});

    Month m{10};

    EXPECT_EQ(m += Months{2}, Month{12});
    EXPECT_EQ(m -= Months{2}, Month{10});
}

TEST(Day, Validation) {
    Day m{32};
    EXPECT_FALSE(m.ok());
    --m;
    EXPECT_TRUE(m.ok());
}

TEST(Day, UnsignedConversion) {
    Day m{31};
    EXPECT_EQ(unsigned(m), 31U);
}

TEST(Day, Comparison) {
    EXPECT_LT(Day{1}, Day{31});
    EXPECT_GT(Day{31}, Day{1});
    EXPECT_LE(Day{1}, Day{2});
    EXPECT_LE(Day{2}, Day{2});
    EXPECT_GE(Day{2}, Day{1});
    EXPECT_GE(Day{2}, Day{2});
    EXPECT_NE(Day{1}, Day{31});
}

TEST(Day, Arithmetic) {
    EXPECT_EQ(Day{30} ++, Day{30});
    EXPECT_EQ(++Day{30}, Day{31});
    EXPECT_EQ(Day{30} --, Day{30});
    EXPECT_EQ(--Day{30}, Day{29});
    EXPECT_EQ(Day{29} + Days{2}, Day{31});
    EXPECT_EQ(Day{31} - Days{2}, Day{29});
    EXPECT_EQ(Day{31} - Day{30}, Days{1});
    EXPECT_EQ(Days{2} + Day{29}, Day{31});

    Day m{29};

    EXPECT_EQ(m += Days{2}, Day{31});
    EXPECT_EQ(m -= Days{2}, Day{29});
}
