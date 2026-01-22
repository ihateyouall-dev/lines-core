#include <gtest/gtest.h>
#include <lines/tasks/task_completion.hpp>

using namespace Lines;

TEST(TaskCompletion, States) {
    TaskCompletion c;
    c.complete();
    EXPECT_TRUE(c.completed());
    EXPECT_FALSE(c.skipped());
    EXPECT_TRUE(c.finished());
    EXPECT_EQ(c.state(), TaskCompletion::State::Completed);
    c.skip();
    EXPECT_FALSE(c.completed());
    EXPECT_TRUE(c.skipped());
    EXPECT_TRUE(c.finished());
    EXPECT_EQ(c.state(), TaskCompletion::State::Skipped);
    c.reset();
    EXPECT_FALSE(c.finished());
    EXPECT_FALSE(c.completed());
    EXPECT_FALSE(c.skipped());
    EXPECT_EQ(c.state(), TaskCompletion::State::NotCompleted);
}
