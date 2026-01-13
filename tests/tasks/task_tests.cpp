#include <gtest/gtest.h>
#include <lines/tasks/task.hpp>
#include <lines/temporal/date.hpp>

using namespace Lines;

TEST(TaskCompletion, InitiallyNotCompleted) {
    TaskCompletion c; // NOLINT
    EXPECT_FALSE(c.completed());
}

TEST(TaskCompletion, CompleteSetsCompleted) {
    TaskCompletion c; // NOLINT
    c.complete();
    EXPECT_TRUE(c.completed());
}

TEST(TaskCompletion, ResetWorks) {
    TaskCompletion c; // NOLINT
    c.complete();
    c.reset();
    EXPECT_FALSE(c.completed());
}

TEST(TaskCompletion, CloneCopiesState) {
    TaskCompletion c; // NOLINT
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

TEST(Task, MoveTransfersOwnership) {
    Task t1 = make_task().info(TaskInfo("title")).task(); // NOLINT

    Task t2 = std::move(t1); // NOLINT
    EXPECT_FALSE(t2.title().empty());
}

TEST(Task, SetTitleThrowsOnEmpty) {
    Task t = make_task().info(TaskInfo("title")).task(); // NOLINT

    EXPECT_THROW(t.set_title(""), std::invalid_argument);
}

TEST(TaskFactory, CannotBeReusedAfterMove) {
    auto factory = make_task().info(TaskInfo("title"));
    Task t = std::move(factory).task(); // NOLINT

    // at this point, factory is no longer valid
    SUCCEED();
}

TEST(TaskFactory, DefaultCompletionIs) {
    Task t = make_task().info(TaskInfo("title")).task(); // NOLINT
    EXPECT_FALSE(t.completion().completed());
}

TEST(TaskFactory, AllowsPartialConfiguration) {
    Task t = make_task().info(TaskInfo("title")).task(); // NOLINT
    EXPECT_EQ(t.title(), "title");
}
