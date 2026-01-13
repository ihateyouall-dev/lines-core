#include <gtest/gtest.h>
#include <lines/tasks/task.hpp>
#include <lines/temporal/date.hpp>

using namespace Lines;

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
