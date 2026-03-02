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
#pragma once

#include "lines/detail/macro.h"
#include <lines/tasks/task_completion.hpp>
#include <lines/tasks/task_info.hpp>
#include <lines/tasks/task_repeat.hpp>
#include <lines/temporal/date.hpp>

namespace Lines {
class LINES_API Task {
    TaskInfo _info;
    TaskCompletion _completion;
    TaskRepeatRule _rule;
    std::optional<Temporal::Date> _deadline;

  public:
    explicit Task(TaskInfo info, TaskRepeatRule rule = {});
    Task(const Task &task) = default;
    auto operator=(const Task &task) -> Task & = default;
    Task(Task &&) = default;
    auto operator=(Task &&) -> Task & = default;
    ~Task() = default;

    LINES_NODISCARD auto finished() const -> bool;

    LINES_NODISCARD auto skipped() const -> bool;
    void skip();

    LINES_NODISCARD auto completed() const -> bool;
    void complete();

    LINES_NODISCARD auto completion_state() const -> TaskCompletion::State;

    void set_completion_state(TaskCompletion::State state);

    void completion_reset();

    void set_title(const std::string &title);
    void set_description(const std::string &description);
    void set_tags(std::vector<std::string> tags);
    void set_repeat_rule(const TaskRepeatRule &rule);

    LINES_NODISCARD auto deadline() const -> const std::optional<Temporal::Date> &;

    LINES_NODISCARD auto title() const -> const std::string &;
    LINES_NODISCARD auto description() const -> const std::optional<std::string> &;
    LINES_NODISCARD auto tags() const -> const std::vector<std::string> &;

    LINES_NODISCARD auto next_deadline(const Temporal::Date &completed_at) const
        -> std::optional<Temporal::Date>;
    void advance_deadline(const Temporal::Date &completed_at);
    LINES_NODISCARD auto actual(const Temporal::Date &date) const -> bool;
};
} // namespace Lines
