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
