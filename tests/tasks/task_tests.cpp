#include "lines/tasks/task_info.hpp"
#include "lines/tasks/task_repeat.hpp"
#include "lines/temporal/duration.hpp"
#include <gtest/gtest.h>
#include <lines/tasks/task.hpp>
#include <lines/temporal/date.hpp>

using namespace Lines;

TEST(TaskAccessors, Getters) {
    Task task{TaskInfo{"title", "description", {"tag1", "tag2"}}};

    EXPECT_EQ(task.title(), "title");
    EXPECT_EQ(task.description().value(), "description");
    EXPECT_FALSE(task.tags().empty());
}

TEST(TaskAccessors, Setters) {
    Task task{TaskInfo{"title", "description", {"tag1", "tag2"}}};

    task.set_title("whatever");
    task.set_description("whatever");
    task.set_tags({});

    EXPECT_EQ(task.title(), "whatever");
    EXPECT_EQ(task.description().value(), "whatever");
    EXPECT_TRUE(task.tags().empty());
};

TEST(TaskInvariants, Title) {
    // Task title cannot be empty, it must have at least 1 character
    EXPECT_THROW(Task{TaskInfo{""}}, std::invalid_argument);

    Task task = Task{TaskInfo{"not empty title"}};

    EXPECT_THROW(task.set_title(""), std::invalid_argument);
}

TEST(TaskSpecialMembers, Copy) {
    Task task1 = Task{TaskInfo{"first"}};
    Task task2 = Task{TaskInfo{"second"}};

    task1 = task2;

    Task task3(task2); // NOLINT

    EXPECT_EQ(task1.title(), "second");
    EXPECT_EQ(task3.title(), "second");

    task2 = task2; // Nothing happens
}

TEST(TaskCompletion, ConstAndNonConst) {
    const Task const_task = Task{TaskInfo{"const"}};
    Task task = Task{TaskInfo{"non const"}};

    EXPECT_FALSE(const_task.completion().completed());
    task.completion().complete();

    EXPECT_TRUE(task.completion().completed());
}

TEST(TaskRepeat, NextDate) {
    TaskRepeatRule rule{.repeat_type =
                            EveryUnit(duration_cast<Temporal::Minutes>(Temporal::Days{1}))};
    Task task{TaskInfo{"title"}, {}, rule};
    EXPECT_EQ(task.next_date(Temporal::Date{Temporal::Days{1}}), Temporal::Date{Temporal::Days{2}});
    task.set_repeat_rule(TaskRepeatRule{
        .repeat_type = EveryUnit{duration_cast<Temporal::Minutes>(Temporal::Days{2})}});
    EXPECT_EQ(task.next_date(Temporal::Date{Temporal::Days{1}}), Temporal::Date{Temporal::Days{3}});
};
