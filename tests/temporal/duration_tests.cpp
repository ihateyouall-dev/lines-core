#include "latertemporal.h"
#include <gtest/gtest.h>

using namespace Later::Temporal;

TEST(DurationCast, Seconds) {
    EXPECT_EQ(duration_cast<Seconds>(Minutes{2}), Seconds{120});
    EXPECT_EQ(duration_cast<Seconds>(Hours{2}), Seconds{7200});
    EXPECT_EQ(duration_cast<Seconds>(Days{2}), Seconds{172800});
    EXPECT_EQ(duration_cast<Seconds>(Weeks{2}), Seconds{1209600});
}

TEST(DurationCast, Minutes) {
    EXPECT_EQ(duration_cast<Minutes>(Seconds{120}), Minutes{2});
    EXPECT_EQ(duration_cast<Minutes>(Hours{2}), Minutes{120});
    EXPECT_EQ(duration_cast<Minutes>(Days{2}), Minutes{2880});
    EXPECT_EQ(duration_cast<Minutes>(Weeks{2}), Minutes{20160});
}

TEST(DurationCast, Hours) {
    EXPECT_EQ(duration_cast<Hours>(Seconds{7200}), Hours{2});
    EXPECT_EQ(duration_cast<Hours>(Minutes{120}), Hours{2});
    EXPECT_EQ(duration_cast<Hours>(Days{2}), Hours{48});
    EXPECT_EQ(duration_cast<Hours>(Weeks{2}), Hours{336});
}

TEST(DurationCast, Days) {
    EXPECT_EQ(duration_cast<Days>(Seconds{172800}), Days{2});
    EXPECT_EQ(duration_cast<Days>(Minutes{2880}), Days{2});
    EXPECT_EQ(duration_cast<Days>(Hours{48}), Days{2});
    EXPECT_EQ(duration_cast<Days>(Weeks{2}), Days{14});
    EXPECT_EQ(duration_cast<Days>(Months{2}), Days{60});
    EXPECT_EQ(duration_cast<Days>(Months{3}), Days{30} * 3 + Days{1});
    EXPECT_EQ(duration_cast<Days>(Years{2}), Days{730});
    EXPECT_EQ(duration_cast<Days>(Years{5}), Days{365} * 5 + Days{1});
}

TEST(DurationCast, Weeks) {
    EXPECT_EQ(duration_cast<Weeks>(Seconds{1209600}), Weeks{2});
    EXPECT_EQ(duration_cast<Weeks>(Minutes{20160}), Weeks{2});
    EXPECT_EQ(duration_cast<Weeks>(Hours{336}), Weeks{2});
    EXPECT_EQ(duration_cast<Weeks>(Days{14}), Weeks{2});
    EXPECT_EQ(duration_cast<Weeks>(Months{2}), Weeks{8});
    EXPECT_EQ(duration_cast<Weeks>(Months{3}), Weeks{4} * 3 + Weeks{1});
    EXPECT_EQ(duration_cast<Weeks>(Years{2}), Weeks{104});
    EXPECT_EQ(duration_cast<Weeks>(Years{11}), Weeks{52} * 11 + Weeks{1});
}

TEST(DurationCast, Calendar) {
    EXPECT_EQ(duration_cast<Months>(Years{2}), Months{24});
    EXPECT_EQ(duration_cast<Years>(Months{24}), Years{2});
}
