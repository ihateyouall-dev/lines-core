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
#include "lines/temporal/duration.hpp"
#include "lines/temporal/timepoint.hpp"
#include "lines/temporal/ymd.hpp"
#include <gtest/gtest.h>
#include <lines/tasks/task_repeat.hpp>

using namespace Lines;

TEST(TaskRepeat, EveryUnit) {
    TaskRepeatRule rule{
        .repeat_type = TaskRepeat::EveryUnit{
            .interval = Temporal::duration_cast<Temporal::Seconds>(Temporal::Days{1})}};

    EXPECT_EQ(rule.next_deadline(Temporal::TimePoint{Temporal::Seconds{Temporal::Days{1}}}),
              Temporal::TimePoint{Temporal::Seconds{Temporal::Days{2}}});
}

TEST(TaskRepeat, EveryUnitWithEnd) {
    TaskRepeatRule rule{
        .repeat_type = TaskRepeat::EveryUnit{.interval = Temporal::duration_cast<Temporal::Seconds>(
                                                 Temporal::Days{1})},
        .end = Temporal::TimePoint(Temporal::Days{5})};

    EXPECT_EQ(rule.next_deadline(Temporal::TimePoint{Temporal::Days{1}}),
              Temporal::TimePoint{Temporal::Days{2}});
    EXPECT_EQ(rule.next_deadline(Temporal::TimePoint{Temporal::Days{2}}),
              Temporal::TimePoint{Temporal::Days{3}});
    EXPECT_EQ(rule.next_deadline(Temporal::TimePoint{Temporal::Days{5}}), std::nullopt);
}

TEST(TaskRepeat, EveryWeekday) {
    TaskRepeatRule rule{.repeat_type =
                            TaskRepeat::EveryWeekday{.weekdays = {Temporal::Weekday::Monday}}};

    EXPECT_EQ(rule.next_deadline(Temporal::TimePoint{Temporal::Days{1}}),
              Temporal::TimePoint{Temporal::Days{4}});

    TaskRepeatRule rule2{.repeat_type = TaskRepeat::EveryWeekday{
                             .weekdays = {Temporal::Weekday::Monday, Temporal::Weekday::Tuesday}}};

    EXPECT_EQ(rule2.next_deadline(Temporal::TimePoint{Temporal::Days{1}}),
              Temporal::TimePoint{Temporal::Days{4}});

    EXPECT_EQ(rule2.next_deadline(Temporal::TimePoint{Temporal::Days{4}}),
              Temporal::TimePoint{Temporal::Days{5}});
}

TEST(TaskRepeat, EveryWeekdayWithEnd) {
    TaskRepeatRule rule{.repeat_type =
                            TaskRepeat::EveryWeekday{.weekdays = {Temporal::Weekday::Monday}},
                        .end = Temporal::TimePoint{Temporal::Days{5}}};

    EXPECT_EQ(rule.next_deadline(Temporal::TimePoint{Temporal::Days{1}}),
              Temporal::TimePoint{Temporal::Days{4}});

    EXPECT_EQ(rule.next_deadline(Temporal::TimePoint{Temporal::Days{4}}), std::nullopt);
}
