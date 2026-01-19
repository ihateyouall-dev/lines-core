#include "lines/tasks/task_info.hpp"
#include <gtest/gtest.h>
#include <lines/tasks/task.hpp>
#include <lines/temporal/date.hpp>

using namespace Lines;

TEST(TaskAccessors, Getters) {
    Task task = make_task().info(TaskInfo{"title", "description", {"tag1", "tag2"}}).task();

    EXPECT_EQ(task.title(), "title");
    EXPECT_EQ(task.description().value(), "description");
    EXPECT_FALSE(task.tags().empty());
}

TEST(TaskAccessors, Setters) {
    Task task = make_task().info(TaskInfo{"title", "description", {"tag1", "tag2"}}).task();

    task.set_title("whatever");
    task.set_description("whatever");
    task.set_tags({});

    EXPECT_EQ(task.title(), "whatever");
    EXPECT_EQ(task.description().value(), "whatever");
    EXPECT_TRUE(task.tags().empty());
};

TEST(TaskInvariants, Title) {
    // Task title cannot be empty, it must have at least 1 character
    EXPECT_THROW(make_task().info(TaskInfo{""}).task(), std::invalid_argument);

    Task task = make_task().info(TaskInfo{"not empty title"}).task();

    EXPECT_THROW(task.set_title(""), std::invalid_argument);
}

TEST(TaskSpecialMembers, Copy) {
    Task task1 = make_task().info(TaskInfo{"first"}).task();
    Task task2 = make_task().info(TaskInfo{"second"}).task();

    task1 = task2;

    Task task3(task2); // NOLINT

    EXPECT_EQ(task1.title(), "second");
    EXPECT_EQ(task3.title(), "second");

    task2 = task2; // Nothing happens
}

TEST(TaskCompletion, ConstAndNonConst) {
    const Task const_task = make_task().info(TaskInfo{"const"}).task();
    Task task = make_task().info(TaskInfo{"non const"}).task();

    EXPECT_FALSE(const_task.completion().completed());
    task.completion().complete();

    EXPECT_TRUE(task.completion().completed());
}
