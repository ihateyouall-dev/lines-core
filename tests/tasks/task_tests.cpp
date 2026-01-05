#include "lines/tasks.h"
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

TEST(TaskVisibility, AlwaysVisible) {
    auto vis = Visibility::always();
    Temporal::Date date{Temporal::Days{1}};

    EXPECT_TRUE(vis.visible(date));
}

TEST(TaskVisibility, CustomPredicateRespectsStartDate) {
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
    Task t(TaskInfo("title"), std::make_unique<BinaryTaskCompletion>(),
           Visibility::always()); // NOLINT

    EXPECT_THROW(t.set_title(""), std::invalid_argument);
}
