#include <gtest/gtest.h>
#include <lines/tasks/task_completion.hpp>

using namespace Lines;

TEST(TaskCompletion, InitiallyNotCompleted) {
    TaskCompletion c;
    EXPECT_FALSE(c.completed());
}

TEST(TaskCompletion, CompleteSetsCompleted) {
    TaskCompletion c;
    c.complete();
    EXPECT_TRUE(c.completed());
}

TEST(TaskCompletion, ResetWorks) {
    TaskCompletion c;
    c.complete();
    c.reset();
    EXPECT_FALSE(c.completed());
}

TEST(TaskCompletion, CloneCopiesState) {
    TaskCompletion c;
    c.complete();

    auto clone = c.clone();
    EXPECT_TRUE(clone->completed());
}

TEST(ProgressTaskCompletion, ThrowsIfMinGreaterThanMax) {
    EXPECT_THROW(ProgressTaskCompletion(10, 5), std::invalid_argument);
}

TEST(ProgressTaskCompletion, InitiallyNotCompleted) {
    ProgressTaskCompletion c(0, 10);
    EXPECT_FALSE(c.completed());
    EXPECT_EQ(c.current(), 0);
}

TEST(ProgressTaskCompletion, CompleteSetsToMax) {
    ProgressTaskCompletion c(0, 10);
    c.complete();
    EXPECT_TRUE(c.completed());
    EXPECT_EQ(c.current(), 10);
}

TEST(ProgressTaskCompletion, SetCurrentInRangeWorks) {
    ProgressTaskCompletion c(0, 10);
    c.set_current(5);
    EXPECT_EQ(c.current(), 5);
}

TEST(ProgressTaskCompletion, SetCurrentOutOfRangeThrows) {
    ProgressTaskCompletion c(0, 10);
    EXPECT_THROW(c.set_current(11), std::out_of_range);
    EXPECT_THROW(c.set_current(100), std::out_of_range);
}
