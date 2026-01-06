#include "lines/tasks.h"
#include "lines/temporal.h"
#include <gtest/gtest.h>

using namespace Lines;

TEST(TaskInfo, CreatesWithValidTitle) {
    TaskInfo info("title");
    EXPECT_EQ(info.title, "title");
    EXPECT_FALSE(info.description.has_value());
    EXPECT_TRUE(info.tags.empty());
}

TEST(TaskInfo, ThrowsOnEmptyTitle) { EXPECT_THROW(TaskInfo(""), std::invalid_argument); }

TEST(BinaryTaskCompletion, InitiallyNotCompleted) {
    BinaryTaskCompletion c; // NOLINT
    EXPECT_FALSE(c.completed());
}

TEST(BinaryTaskCompletion, CompleteSetsCompleted) {
    BinaryTaskCompletion c; // NOLINT
    c.complete();
    EXPECT_TRUE(c.completed());
}

TEST(BinaryTaskCompletion, ResetWorks) {
    BinaryTaskCompletion c; // NOLINT
    c.complete();
    c.reset();
    EXPECT_FALSE(c.completed());
}

TEST(BinaryTaskCompletion, CloneCopiesState) {
    BinaryTaskCompletion c; // NOLINT
    c.complete();

    auto clone = c.clone();
    EXPECT_TRUE(clone->completed());
}

TEST(ProgressTaskCompletion, ThrowsIfMinGreaterThanMax) {
    EXPECT_THROW(ProgressTaskCompletion(10, 5), std::invalid_argument);
}

TEST(ProgressTaskCompletion, InitiallyNotCompleted) {
    ProgressTaskCompletion c(0, 10); // NOLINT
    EXPECT_FALSE(c.completed());
    EXPECT_EQ(c.current(), 0);
}

TEST(ProgressTaskCompletion, CompleteSetsToMax) {
    ProgressTaskCompletion c(0, 10); // NOLINT
    c.complete();
    EXPECT_TRUE(c.completed());
    EXPECT_EQ(c.current(), 10);
}

TEST(ProgressTaskCompletion, SetCurrentInRangeWorks) {
    ProgressTaskCompletion c(0, 10); // NOLINT
    c.set_current(5);                // NOLINT
    EXPECT_EQ(c.current(), 5);
}

TEST(ProgressTaskCompletion, SetCurrentOutOfRangeThrows) {
    ProgressTaskCompletion c(0, 10); // NOLINT
    EXPECT_THROW(c.set_current(11), std::out_of_range);
    EXPECT_THROW(c.set_current(100), std::out_of_range);
}

TEST(TaskVisibility, Always) {
    auto vis = Visibility::always();
    Temporal::Date date{Temporal::Days{1}};

    EXPECT_TRUE(vis.visible(date));
}

TEST(TaskVisibility, Never) {
    auto vis = Visibility::never();
    Temporal::Date date{Temporal::Days{1}};

    EXPECT_FALSE(vis.visible(date));
}

TEST(TaskVisibility, Weekdays) {
    auto vis = Visibility::weekdays();
    Temporal::Date date{Temporal::Days{7}}; // NOLINT
    EXPECT_TRUE(vis.visible(date));
    date += Temporal::Days{2};
    EXPECT_FALSE(vis.visible(date));
}

TEST(TaskVisibility, Weekends) {
    auto vis = Visibility::weekends();
    Temporal::Date date{Temporal::Days{7}}; // NOLINT
    EXPECT_FALSE(vis.visible(date));
    date += Temporal::Days{2};
    EXPECT_TRUE(vis.visible(date));
}

TEST(TaskVisibility, From) {
    auto date = Temporal::Date(Temporal::Days{14}); // NOLINT
    auto vis = Visibility::from(date);
    EXPECT_FALSE(vis.visible(Temporal::Date(Temporal::Days{13}))); // NOLINT
    EXPECT_TRUE(vis.visible(Temporal::Date(Temporal::Days{14})));  // NOLINT
    EXPECT_TRUE(vis.visible(Temporal::Date(Temporal::Days{15})));  // NOLINT
}

TEST(TaskVisibility, Until) {
    auto date = Temporal::Date(Temporal::Days{14}); // NOLINT
    auto vis = Visibility::until(date);
    EXPECT_TRUE(vis.visible(Temporal::Date(Temporal::Days{13})));  // NOLINT
    EXPECT_TRUE(vis.visible(Temporal::Date(Temporal::Days{14})));  // NOLINT
    EXPECT_FALSE(vis.visible(Temporal::Date(Temporal::Days{15}))); // NOLINT
}

TEST(TaskVisibility, Between) {
    auto start = Temporal::Date(Temporal::Days{14}); // NOLINT
    auto end = Temporal::Date(Temporal::Days{16});   // NOLINT
    auto vis = Visibility::between(start, end);
    EXPECT_FALSE(vis.visible(Temporal::Date(Temporal::Days{13}))); // NOLINT
    EXPECT_TRUE(vis.visible(Temporal::Date(Temporal::Days{14})));  // NOLINT
    EXPECT_TRUE(vis.visible(Temporal::Date(Temporal::Days{15})));  // NOLINT
    EXPECT_TRUE(vis.visible(Temporal::Date(Temporal::Days{16})));  // NOLINT
}

TEST(TaskVisibility, Once) {
    auto vis = Visibility::once(Temporal::Date{Temporal::Days{1}});
    Temporal::Date date{Temporal::Days{1}};
    EXPECT_TRUE(vis.visible(date));
    date += Temporal::Days{1};
    EXPECT_FALSE(vis.visible(date));
    date += Temporal::Days{1};
    EXPECT_FALSE(vis.visible(date));
}

TEST(TaskVisibility, EveryMonthDay) {
    auto vis = Visibility::every_month_day(Temporal::Day{1});
    Temporal::Date date{Temporal::Days{0}};                                          // 1970-01-01
    Temporal::Date next(Temporal::Year{2026}, Temporal::Month{2}, Temporal::Day{1}); // NOLINT
    EXPECT_TRUE(vis.visible(date));
    date += Temporal::Days{1};
    EXPECT_FALSE(vis.visible(date));
    EXPECT_TRUE(vis.visible(next));
}

TEST(TaskVisibility, EveryWeekday) {
    auto vis = Visibility::every_weekday(Temporal::Weekday::Thursday);
    Temporal::Date date{Temporal::Days{0}};
    EXPECT_TRUE(vis.visible(date));
    date += Temporal::Days{1};
    EXPECT_FALSE(vis.visible(date));
}

TEST(TaskVisibility, EveryNthDay) {
    auto vis = Visibility::every_nth_day(2, Temporal::Date{Temporal::Days{1}});
    Temporal::Date date{Temporal::Days{1}};
    EXPECT_TRUE(vis.visible(date));
    date += Temporal::Days{1};
    EXPECT_FALSE(vis.visible(date));
    date += Temporal::Days{1};
    EXPECT_TRUE(vis.visible(date));
}

TEST(TaskVisibility, CustomPredicate) {
    Temporal::Date start{Temporal::Days{1}};
    Temporal::Date before{start - Temporal::Days{1}};
    Temporal::Date after{start + Temporal::Days{1}};

    TaskVisibility vis([start](const Temporal::Date &d) { // NOLINT
        return d >= start;
    });

    EXPECT_FALSE(vis.visible(before));
    EXPECT_TRUE(vis.visible(after));
}

TEST(Task, ThrowsIfCompletionIsNull) {
    TaskInfo info("title");
    EXPECT_THROW(Task(info, nullptr, Visibility::always()), std::invalid_argument);
}

TEST(Task, CopyClonesCompletion) {
    Task original(TaskInfo("title"), std::make_unique<BinaryTaskCompletion>(),
                  Visibility::always());

    original.completion().complete();

    Task copy = original;

    EXPECT_TRUE(copy.completion().completed());
    EXPECT_NE(&original.completion(), &copy.completion());
}

TEST(Task, MoveTransfersOwnership) {
    Task t1( // NOLINT
        TaskInfo("title"), std::make_unique<BinaryTaskCompletion>(), Visibility::always());

    Task t2 = std::move(t1); // NOLINT
    EXPECT_FALSE(t2.title().empty());
}

TEST(Task, SetTitleThrowsOnEmpty) {
    Task t(TaskInfo("title"), std::make_unique<BinaryTaskCompletion>(), // NOLINT
           Visibility::always());

    EXPECT_THROW(t.set_title(""), std::invalid_argument);
}
