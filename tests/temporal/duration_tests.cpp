#include <gtest/gtest.h>
#include <lines/temporal/duration.hpp>

using namespace Lines::Temporal;

TEST(DurationCompare, PositiveValues) {
    EXPECT_EQ(Minutes{1}, Seconds{60});
    EXPECT_EQ(Hours{1}, Minutes{60});
    EXPECT_EQ(Days{1}, Hours{24});
    EXPECT_NE(Days{28}, Months{1});
    EXPECT_LT(Hours{12}, Days{1});
    EXPECT_LE(Days{30}, Months{1});
    EXPECT_GE(Months{1}, Weeks{4});
    EXPECT_GT(Years{2}, Months{23});
}

TEST(DurationCompare, NegativeValues) {
    EXPECT_EQ(Minutes{-1}, Seconds{-60});
    EXPECT_EQ(Hours{-1}, Minutes{-60});
    EXPECT_EQ(Days{-1}, Hours{-24});
    EXPECT_NE(Days{-28}, Months{-1});
    EXPECT_GT(Hours{-12}, Days{-1});
    EXPECT_GE(Days{-30}, Months{-1});
    EXPECT_LE(Months{-1}, Weeks{-4});
    EXPECT_LT(Years{-2}, Months{-23});
}

TEST(DurationArithmetic, Addition) {
    EXPECT_EQ(Seconds{1} + Seconds{2}, Seconds{3});
    EXPECT_EQ(Minutes{1} + Seconds{2}, Seconds{62});
    EXPECT_EQ(Minutes{2} + Seconds{60}, Minutes{3});
    EXPECT_EQ(Hours{23} + Minutes{60}, Days{1});
}

TEST(DurationArithmetic, Substraction) {
    EXPECT_EQ(Seconds{3} - Seconds{2}, Seconds{1});
    EXPECT_EQ(Minutes{1} - Seconds{2}, Seconds{58});
    EXPECT_EQ(Seconds{60} - Minutes{1}, Seconds{0});
    EXPECT_EQ(Hours{3} - Minutes{180}, Days{0});
}

TEST(DurationArithmetic, Multiplication) {
    EXPECT_EQ(Seconds{2} * 3, Seconds{6});
    EXPECT_EQ(Seconds{6} * 10, Minutes{1});
    EXPECT_EQ(Minutes{20} * 3, Hours{1});
    EXPECT_EQ(Hours{12} * 4, Days{2});
    EXPECT_EQ(3 * Seconds{30}, Seconds{90});
}

TEST(DurationArithmetic, Division) {
    EXPECT_EQ(Seconds{6} / 3, Seconds{2});
    EXPECT_NE(Minutes{1} / 3, Seconds{20}); // Because arithmetic operators return value of minimal
                                            // period, 1m / 3 returns minutes
    EXPECT_EQ(Minutes{1} / Seconds{3}, 20); // This is
                                            // another case, at this moment minimal period is
                                            // seconds, then 1m / 3s returns 20
}

TEST(DurationArithmetic, DivisionByZero) {
    Seconds s{30};
    EXPECT_THROW(s / 0, std::invalid_argument);
    EXPECT_THROW(s /= 0, std::invalid_argument);
    EXPECT_THROW(s % 0, std::invalid_argument);
    EXPECT_THROW(s %= 0, std::invalid_argument);
    EXPECT_THROW(s / Seconds{0}, std::invalid_argument);
    EXPECT_THROW(s % Seconds{0}, std::invalid_argument);
    EXPECT_THROW(s %= Seconds{0}, std::invalid_argument);
}

TEST(DurationArithmetic, Mod) {
    EXPECT_EQ(Seconds{6} % 3, Seconds{0});
    EXPECT_NE(Minutes{1} % 3, Seconds{0});
    EXPECT_EQ(Minutes{1} % Seconds{3}, 0);
}

TEST(DurationArithmetic, NegativeValues) {
    EXPECT_EQ(Seconds{1} - Seconds{2}, Seconds{-1});
    EXPECT_EQ(Seconds{30} - Minutes{1}, Seconds{-30});
    EXPECT_EQ(Minutes{-1} * -60, Hours{1});
    EXPECT_EQ(Hours{-24} / 12, Hours{-2});
}

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

TEST(DurationCast, ZeroValues) {
    EXPECT_EQ(duration_cast<Seconds>(Minutes{0}), Seconds{0});
    EXPECT_EQ(duration_cast<Minutes>(Seconds{0}), Minutes{0});
    EXPECT_EQ(duration_cast<Hours>(Days{0}), Hours{0});
    EXPECT_EQ(duration_cast<Days>(Years{0}), Days{0});
}

TEST(DurationCast, Identity) {
    EXPECT_EQ(duration_cast<Seconds>(Seconds{42}), Seconds{42});
    EXPECT_EQ(duration_cast<Minutes>(Minutes{7}), Minutes{7});
    EXPECT_EQ(duration_cast<Years>(Years{3}), Years{3});
}

TEST(DurationCast, NegativeValues) {
    EXPECT_EQ(duration_cast<Seconds>(Minutes{-2}), Seconds{-120});
    EXPECT_EQ(duration_cast<Minutes>(Hours{-1}), Minutes{-60});
    EXPECT_EQ(duration_cast<Days>(Weeks{-3}), Days{-21});
}

TEST(DurationCast, Truncation) {
    EXPECT_EQ(duration_cast<Weeks>(Days{13}), Weeks{1});
    EXPECT_EQ(duration_cast<Days>(Months{1}), Days{30});
    EXPECT_EQ(duration_cast<Weeks>(Months{1}), Weeks{4});
}

TEST(DurationCast, ReversibleConversions) {
    auto d = Days{730}; // NOLINT
    EXPECT_NE(duration_cast<Years>(d), Years{2});
    EXPECT_NE(duration_cast<Days>(duration_cast<Years>(d)), d);

    auto m = Months{24}; // NOLINT
    EXPECT_EQ(duration_cast<Years>(m), Years{2});
    EXPECT_EQ(duration_cast<Months>(duration_cast<Years>(m)), m);
}

TEST(DurationCast, LargeValues) {
    EXPECT_EQ(duration_cast<Seconds>(Days{100000}), Seconds{100000LL * 24 * 60 * 60});
}

TEST(DurationFloor, Seconds) {
    EXPECT_EQ(floor<Minutes>(Seconds{61}), Minutes{1});
    EXPECT_EQ(floor<Minutes>(Seconds{119}), Minutes{1});
    EXPECT_EQ(floor<Hours>(Seconds{3601}), Hours{1});
    EXPECT_EQ(floor<Hours>(Seconds{7199}), Hours{1});
    EXPECT_EQ(floor<Days>(Seconds{86401}), Days{1});
    EXPECT_EQ(floor<Days>(Seconds{172799}), Days{1});
    EXPECT_EQ(floor<Weeks>(Seconds{604801}), Weeks{1});
    EXPECT_EQ(floor<Weeks>(Seconds{1209599}), Weeks{1});
}

TEST(DurationFloor, Minutes) {
    EXPECT_EQ(floor<Hours>(Minutes{61}), Hours{1});
    EXPECT_EQ(floor<Hours>(Minutes{119}), Hours{1});
    EXPECT_EQ(floor<Days>(Minutes{1441}), Days{1});
    EXPECT_EQ(floor<Days>(Minutes{2879}), Days{1});
    EXPECT_EQ(floor<Weeks>(Minutes{10081}), Weeks{1});
    EXPECT_EQ(floor<Weeks>(Minutes{20159}), Weeks{1});
}

TEST(DurationFloor, Hours) {
    EXPECT_EQ(floor<Days>(Hours{25}), Days{1});
    EXPECT_EQ(floor<Days>(Hours{47}), Days{1});
    EXPECT_EQ(floor<Weeks>(Hours{169}), Weeks{1});
    EXPECT_EQ(floor<Weeks>(Hours{335}), Weeks{1});
}

TEST(DurationFloor, Days) {
    EXPECT_EQ(floor<Weeks>(Days{8}), Weeks{1});
    EXPECT_EQ(floor<Weeks>(Days{13}), Weeks{1});
}

TEST(DurationFloor, Weeks) {
    EXPECT_EQ(floor<Weeks>(Weeks{1}), Weeks{1});
    EXPECT_EQ(floor<Weeks>(Weeks{3}), Weeks{3});
}

TEST(DurationFloor, Calendar) {
    EXPECT_EQ(floor<Years>(Months{13}), Years{1});
    EXPECT_EQ(floor<Years>(Months{23}), Years{1});
    EXPECT_EQ(floor<Months>(Days{31}), Months{1});
    EXPECT_EQ(floor<Months>(Days{92}), Months{3});
}

TEST(DurationFloor, ExactBoundaries) {
    EXPECT_EQ(floor<Minutes>(Seconds{60}), Minutes{1});
    EXPECT_EQ(floor<Hours>(Minutes{60}), Hours{1});
    EXPECT_EQ(floor<Days>(Hours{24}), Days{1});
    EXPECT_EQ(floor<Weeks>(Days{7}), Weeks{1});
}

TEST(DurationFloor, ZeroValues) {
    EXPECT_EQ(floor<Seconds>(Seconds{0}), Seconds{0});
    EXPECT_EQ(floor<Minutes>(Seconds{0}), Minutes{0});
    EXPECT_EQ(floor<Hours>(Minutes{0}), Hours{0});
    EXPECT_EQ(floor<Days>(Hours{0}), Days{0});
}

TEST(DurationFloor, LessThanOneUnit) {
    EXPECT_EQ(floor<Minutes>(Seconds{59}), Minutes{0});
    EXPECT_EQ(floor<Hours>(Minutes{59}), Hours{0});
    EXPECT_EQ(floor<Days>(Hours{23}), Days{0});
    EXPECT_EQ(floor<Weeks>(Days{6}), Weeks{0});
}

TEST(DurationFloor, Idempotent) {
    auto d = floor<Days>(Hours{49}); // NOLINT
    EXPECT_EQ(floor<Days>(d), d);

    auto w = floor<Weeks>(Days{20}); // NOLINT
    EXPECT_EQ(floor<Weeks>(w), w);
}

TEST(DurationFloor, MatchesDurationCastForLinearUnits) {
    EXPECT_EQ(floor<Minutes>(Seconds{119}), duration_cast<Minutes>(Seconds{119}));

    EXPECT_EQ(floor<Hours>(Minutes{3599}), duration_cast<Hours>(Minutes{3599}));
}

TEST(DurationFloor, LargeValues) {
    EXPECT_EQ(floor<Days>(Hours{24 * 365}), Days{365}); // NOLINT
    EXPECT_EQ(floor<Weeks>(Days{7 * 520}), Weeks{520}); // NOLINT
}

TEST(DurationFloor, NegativeValues) {
    EXPECT_EQ(floor<Minutes>(Seconds{-1}), Minutes{-1});
    EXPECT_EQ(floor<Minutes>(Seconds{-61}), Minutes{-2});
}

TEST(DurationFloor, Composition) {
    auto x = Seconds{90000}; // NOLINT
    EXPECT_EQ(floor<Days>(floor<Hours>(x)), floor<Days>(x));
}

TEST(DurationCeil, Seconds) {
    EXPECT_EQ(ceil<Minutes>(Seconds{1}), Minutes{1});
    EXPECT_EQ(ceil<Minutes>(Seconds{61}), Minutes{2});
    EXPECT_EQ(ceil<Hours>(Seconds{3601}), Hours{2});
    EXPECT_EQ(ceil<Hours>(Seconds{7199}), Hours{2});
    EXPECT_EQ(ceil<Days>(Seconds{86401}), Days{2});
    EXPECT_EQ(ceil<Days>(Seconds{172799}), Days{2});
    EXPECT_EQ(ceil<Weeks>(Seconds{604801}), Weeks{2});
    EXPECT_EQ(ceil<Weeks>(Seconds{1209599}), Weeks{2});
}

TEST(DurationCeil, Minutes) {
    EXPECT_EQ(ceil<Hours>(Minutes{1}), Hours{1});
    EXPECT_EQ(ceil<Hours>(Minutes{61}), Hours{2});
    EXPECT_EQ(ceil<Days>(Minutes{1441}), Days{2});
    EXPECT_EQ(ceil<Days>(Minutes{2879}), Days{2});
    EXPECT_EQ(ceil<Weeks>(Minutes{10081}), Weeks{2});
    EXPECT_EQ(ceil<Weeks>(Minutes{20159}), Weeks{2});
}

TEST(DurationCeil, Hours) {
    EXPECT_EQ(ceil<Days>(Hours{1}), Days{1});
    EXPECT_EQ(ceil<Days>(Hours{25}), Days{2});
    EXPECT_EQ(ceil<Weeks>(Hours{169}), Weeks{2});
    EXPECT_EQ(ceil<Weeks>(Hours{335}), Weeks{2});
}

TEST(DurationCeil, Days) {
    EXPECT_EQ(ceil<Weeks>(Days{1}), Weeks{1});
    EXPECT_EQ(ceil<Weeks>(Days{8}), Weeks{2});
    EXPECT_EQ(ceil<Weeks>(Days{13}), Weeks{2});
}

TEST(DurationCeil, Weeks) {
    EXPECT_EQ(ceil<Weeks>(Weeks{1}), Weeks{1});
    EXPECT_EQ(ceil<Weeks>(Weeks{3}), Weeks{3});
}

TEST(DurationCeil, Calendar) {
    EXPECT_EQ(ceil<Years>(Months{1}), Years{1});
    EXPECT_EQ(ceil<Years>(Months{13}), Years{2});
    EXPECT_EQ(ceil<Months>(Days{1}), Months{1});
    EXPECT_EQ(ceil<Months>(Days{31}), Months{2});
    EXPECT_EQ(ceil<Months>(Days{92}), Months{4});
}

TEST(DurationCeil, ExactBoundaries) {
    EXPECT_EQ(ceil<Minutes>(Seconds{60}), Minutes{1});
    EXPECT_EQ(ceil<Hours>(Minutes{60}), Hours{1});
    EXPECT_EQ(ceil<Days>(Hours{24}), Days{1});
    EXPECT_EQ(ceil<Weeks>(Days{7}), Weeks{1});
}

TEST(DurationCeil, ZeroValues) {
    EXPECT_EQ(ceil<Seconds>(Seconds{0}), Seconds{0});
    EXPECT_EQ(ceil<Minutes>(Seconds{0}), Minutes{0});
    EXPECT_EQ(ceil<Hours>(Minutes{0}), Hours{0});
    EXPECT_EQ(ceil<Days>(Hours{0}), Days{0});
}

TEST(DurationCeil, LessThanOneUnit) {
    EXPECT_EQ(ceil<Minutes>(Seconds{1}), Minutes{1});
    EXPECT_EQ(ceil<Hours>(Minutes{1}), Hours{1});
    EXPECT_EQ(ceil<Days>(Hours{1}), Days{1});
    EXPECT_EQ(ceil<Weeks>(Days{1}), Weeks{1});
}

TEST(DurationCeil, Idempotent) {
    auto d = ceil<Days>(Hours{49}); // NOLINT
    EXPECT_EQ(ceil<Days>(d), d);

    auto w = ceil<Weeks>(Days{20}); // NOLINT
    EXPECT_EQ(ceil<Weeks>(w), w);
}

TEST(DurationCeil, MatchesDurationCastForLinearUnitsWhenExact) {
    EXPECT_EQ(ceil<Minutes>(Seconds{120}), duration_cast<Minutes>(Seconds{120}));
    EXPECT_EQ(ceil<Hours>(Minutes{120}), duration_cast<Hours>(Minutes{120}));
}

TEST(DurationCeil, LargeValues) {
    EXPECT_EQ(ceil<Days>(Hours{24 * 365}), Days{365}); // NOLINT
    EXPECT_EQ(ceil<Weeks>(Days{7 * 520}), Weeks{520}); // NOLINT
}

TEST(DurationCeil, NegativeValues) {
    EXPECT_EQ(ceil<Minutes>(Seconds{-1}), Minutes{0});
    EXPECT_EQ(ceil<Minutes>(Seconds{-61}), Minutes{-1});
}

TEST(DurationCeil, Composition) {
    auto x = Seconds{90000}; // NOLINT
    EXPECT_EQ(ceil<Days>(ceil<Hours>(x)), ceil<Days>(x));
}

TEST(DurationFloorCeil, Invariant) {
    auto s = Seconds{61}; // NOLINT
    EXPECT_LE(floor<Minutes>(s), ceil<Minutes>(s));
}

TEST(DurationRound, PositiveValues) {
    EXPECT_EQ(round<Minutes>(Seconds{31}), Minutes{1});
    EXPECT_EQ(round<Minutes>(Seconds{30}), Minutes{0});

    EXPECT_EQ(round<Hours>(Minutes{90}), Hours{2});
    EXPECT_EQ(round<Hours>(Minutes{89}), Hours{1});

    EXPECT_EQ(round<Days>(Hours{36}), Days{2});
    EXPECT_EQ(round<Days>(Hours{35}), Days{1});

    EXPECT_EQ(round<Weeks>(Days{4}), Weeks{1});
    EXPECT_EQ(round<Weeks>(Days{3}), Weeks{0});
}

TEST(DurationRound, NegativeValues) {
    EXPECT_EQ(round<Minutes>(Seconds{-31}), Minutes{-1});
    EXPECT_EQ(round<Minutes>(Seconds{-30}), Minutes{0});

    EXPECT_EQ(round<Hours>(Minutes{-90}), Hours{-2});
    EXPECT_EQ(round<Hours>(Minutes{-89}), Hours{-1});

    EXPECT_EQ(round<Days>(Hours{-36}), Days{-2});
    EXPECT_EQ(round<Days>(Hours{-35}), Days{-1});

    EXPECT_EQ(round<Weeks>(Days{-4}), Weeks{-1});
    EXPECT_EQ(round<Weeks>(Days{-3}), Weeks{0});
}

TEST(DurationRound, HalfwayCases) {
    EXPECT_EQ(round<Minutes>(Seconds{31}), Minutes{1});
    EXPECT_EQ(round<Minutes>(Seconds{-31}), Minutes{-1});

    EXPECT_EQ(round<Hours>(Minutes{31}), Hours{1});
    EXPECT_EQ(round<Hours>(Minutes{-31}), Hours{-1});
}

TEST(DurationRound, SamePeriod) {
    EXPECT_EQ(round<Seconds>(Seconds{10}), Seconds{10});
    EXPECT_EQ(round<Minutes>(Minutes{-5}), Minutes{-5});
    EXPECT_EQ(round<Days>(Days{3}), Days{3});
}

TEST(DurationRound, Calendar) {
    EXPECT_EQ(round<Months>(Days{16}), Months{1});
    EXPECT_EQ(round<Months>(Days{15}), Months{0});

    EXPECT_EQ(round<Years>(Months{18}), Years{2});
    EXPECT_EQ(round<Years>(Months{17}), Years{1});

    EXPECT_EQ(round<Years>(Months{-18}), Years{-2});
    EXPECT_EQ(round<Years>(Months{-17}), Years{-1});
}

TEST(DurationCompatibility, ToChrono) {
    EXPECT_EQ(Seconds{10}.to_chrono<std::chrono::seconds>(), std::chrono::seconds{10});
    EXPECT_EQ(Minutes{10}.to_chrono<std::chrono::minutes>(), std::chrono::minutes{10});
    EXPECT_EQ(Hours{10}.to_chrono<std::chrono::hours>(), std::chrono::hours{10});
    EXPECT_EQ(Days{10}.to_chrono<std::chrono::days>(), std::chrono::days{10});
    EXPECT_EQ(Weeks{10}.to_chrono<std::chrono::weeks>(), std::chrono::weeks{10});
    EXPECT_EQ(Months{10}.to_chrono<std::chrono::months>(), std::chrono::months{10});
    EXPECT_EQ(Years{10}.to_chrono<std::chrono::years>(), std::chrono::years{10});
}

TEST(DurationCompatibility, FromChrono) {
    EXPECT_EQ(Seconds{std::chrono::seconds{10}}, Seconds{10});
    EXPECT_EQ(Minutes{std::chrono::minutes{10}}, Minutes{10});
    EXPECT_EQ(Hours{std::chrono::hours{10}}, Hours{10});
    EXPECT_EQ(Days{std::chrono::days{10}}, Days{10});
    EXPECT_EQ(Weeks{std::chrono::weeks{10}}, Weeks{10});
    EXPECT_EQ(Months{std::chrono::months{10}}, Months{10});
    EXPECT_EQ(Years{std::chrono::years{10}}, Years{10});
}

TEST(DurationCompatibility, FromDifferentPeriod) {
    EXPECT_EQ(Seconds{Minutes{1}}, Seconds{60});
    EXPECT_EQ(Hours{Days{1}}, Hours{24});
}
