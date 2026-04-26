/*
  #        #  #     #  # # # #  # # # #
  #        #  # #   #  #        #
  #        #  #   # #  # # # #  # # # #
  #        #  #     #  #              #
  # # # #  #  #     #  # # # #  # # # #
  Copyright (c) 2025-2026 I.H.Y.A.D.

  Lines Project, Core library.
  This file is licensed under GNU Lesser General Public License v3.0 or later.
  See LICENSE for more information.
  SPDX-License-Identifier: LGPL-3.0-or-later.
*/
#include "lines/tasks/task.hpp"
#include "lines/tasks/task_info.hpp"
#include "lines/tasks/task_repeat.hpp"
#include "lines/temporal/duration.hpp"
#include "lines/temporal/timepoint.hpp"

#include "gtest/gtest.h"

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

TEST(TaskCompletion, Completion) {
    const Task const_task = Task{TaskInfo{"const"}};
    Task task = Task{TaskInfo{"non const"}};

    EXPECT_FALSE(const_task.completed());

    task.complete();
    EXPECT_TRUE(task.completed());

    task.uncomplete();
    EXPECT_FALSE(task.completed());
}

TEST(Task, IsActive) {
    Task task{TaskInfo{"task"}};

    task.set_deadline(Temporal::TimePoint{Temporal::Days{7}});
    EXPECT_TRUE(task.is_active(*task.deadline() - Temporal::Days{1}));
    EXPECT_TRUE(task.is_active(*task.deadline()));
    EXPECT_FALSE(task.is_active(*task.deadline() + Temporal::Days{1}));

    task.set_deadline(std::nullopt);
    EXPECT_TRUE(task.is_active(Temporal::TimePoint{Temporal::Days{8}}));

    task.complete();
    EXPECT_FALSE(task.is_active(Temporal::TimePoint{Temporal::Days{8}}));
};

TEST(Task, NextDeadline) {
    Task task{TaskInfo{"task"}};

    task.set_deadline(Temporal::TimePoint{Temporal::Days{7}});

    EXPECT_EQ(task.next_deadline(*task.deadline() - Temporal::Days{1}), *task.deadline());
    EXPECT_FALSE(task.next_deadline());
    EXPECT_FALSE(task.next_deadline(*task.deadline() + Temporal::Days{1}));

    TaskRepeatRule rule{
        .repeat_type = TaskRepeat::EveryUnit{
            .interval = Temporal::duration_cast<Temporal::Seconds>(Temporal::Days{1})}};
    task.set_repeat_rule(rule);

    EXPECT_EQ(task.next_deadline(), *task.deadline() + Temporal::Days{1});

    task.set_deadline(std::nullopt);

    EXPECT_FALSE(task.next_deadline());
}

TEST(Task, AdvanceDeadline) {
    Task task{TaskInfo{"task"}};
    task.set_deadline(Temporal::TimePoint{Temporal::Days{7}});

    TaskRepeatRule rule{
        .repeat_type = TaskRepeat::EveryUnit{
            .interval = Temporal::duration_cast<Temporal::Seconds>(Temporal::Days{1})}};
    task.set_repeat_rule(rule);

    task.advance_deadline();
    EXPECT_EQ(*task.deadline(), Temporal::TimePoint{Temporal::Days{8}});

    task.advance_deadline(Temporal::TimePoint{Temporal::Days{14}});
    EXPECT_EQ(*task.deadline(), Temporal::TimePoint{Temporal::Days{15}});
}
