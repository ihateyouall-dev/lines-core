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
#include "lines/temporal/clocks.hpp"
#include "lines/temporal/duration.hpp"
#include "lines/temporal/timepoint.hpp"

#include "gtest/gtest.h"

using namespace Lines;

TEST(TaskAccessors, Getters) {
    Task task{TaskInfo{"title", "description", {"tag1", "tag2"}}};

    EXPECT_EQ(task.title(), "title");
    EXPECT_EQ(task.description().value(), "description");
    EXPECT_FALSE(task.tags().empty());

    using namespace Temporal::Literals;
    TaskRepeatRule repeat_rule{.repeat_type = Lines::TaskRepeat::EveryUnit{.interval = 3600_s}};

    task.set_repeat_rule(repeat_rule);

    auto rr = *task.repeat_rule();

    EXPECT_EQ(std::get<Lines::TaskRepeat::EveryUnit>(rr.repeat_type).interval, 1_h);
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
    EXPECT_THROW(Task{TaskInfo{""}}, TaskInfoError);

    Task task = Task{TaskInfo{"not empty title"}};

    EXPECT_THROW(task.set_title(""), TaskError);
}

TEST(TaskInvariants, Repeat) {
    Task task{TaskInfo{"task"}};
    TaskRepeatRule rr{
        .repeat_type = TaskRepeat::EveryUnit{
            .interval = Temporal::duration_cast<Temporal::Seconds>(Temporal::Days{1})}};
    task.set_repeat_rule(rr);
    EXPECT_EQ(*task.due(), Temporal::LocalClock::now() + Temporal::Days{1});

    task.set_repeat_rule(std::nullopt);

    EXPECT_THROW(task.set_repeat_end(Temporal::TimePoint{Temporal::Seconds{1}}), TaskError);
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

    task.set_due(Temporal::TimePoint{Temporal::Days{7}});
    EXPECT_TRUE(task.is_active(*task.due() - Temporal::Days{1}));
    EXPECT_TRUE(task.is_active(*task.due()));
    EXPECT_FALSE(task.is_active(*task.due() + Temporal::Days{1}));

    task.set_due(std::nullopt);
    EXPECT_TRUE(task.is_active(Temporal::TimePoint{Temporal::Days{8}}));

    task.complete();
    EXPECT_FALSE(task.is_active(Temporal::TimePoint{Temporal::Days{8}}));
};

TEST(Task, NextDue) {
    Task task{TaskInfo{"task"}};

    task.set_due(Temporal::TimePoint{Temporal::Days{7}});

    EXPECT_EQ(task.next_due(*task.due() - Temporal::Days{1}), *task.due());
    EXPECT_FALSE(task.next_due());
    EXPECT_FALSE(task.next_due(*task.due() + Temporal::Days{1}));

    TaskRepeatRule rule{
        .repeat_type = TaskRepeat::EveryUnit{
            .interval = Temporal::duration_cast<Temporal::Seconds>(Temporal::Days{1})}};
    task.set_repeat_rule(rule);

    EXPECT_EQ(task.next_due(), *task.due() + Temporal::Days{1});

    task.set_due(std::nullopt);

    EXPECT_FALSE(task.next_due());
}

TEST(Task, AdvanceDue) {
    Task task{TaskInfo{"task"}};
    task.set_due(Temporal::TimePoint{Temporal::Days{7}});

    TaskRepeatRule rule{
        .repeat_type = TaskRepeat::EveryUnit{
            .interval = Temporal::duration_cast<Temporal::Seconds>(Temporal::Days{1})}};
    task.set_repeat_rule(rule);
    // One initial advance_due calls in set_repeat_rule

    task.advance_due();
    EXPECT_EQ(*task.due(), Temporal::TimePoint{Temporal::Days{9}});

    task.advance_due(Temporal::TimePoint{Temporal::Days{15}});
    EXPECT_EQ(*task.due(), Temporal::TimePoint{Temporal::Days{16}});
}
