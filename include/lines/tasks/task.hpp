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

namespace Lines {
class LINES_API Task {
    TaskInfo _info;
    TaskCompletion _completion;
    TaskRepeatRule _rule;

  public:
    explicit Task(TaskInfo info, TaskRepeatRule rule = {});
    Task(const Task &task) = default;
    auto operator=(const Task &task) -> Task & = default;
    Task(Task &&) = default;
    auto operator=(Task &&) -> Task & = default;
    ~Task() = default;

    auto completion() -> TaskCompletion &;
    LINES_NODISCARD auto completion() const -> const TaskCompletion &;

    void set_title(const std::string &title);
    void set_description(const std::string &description);
    void set_tags(std::vector<std::string> tags);
    void set_repeat_rule(const TaskRepeatRule &rule);

    LINES_NODISCARD auto title() const -> const std::string &;
    LINES_NODISCARD auto description() const -> const std::optional<std::string> &;
    LINES_NODISCARD auto tags() const -> const std::vector<std::string> &;
    LINES_NODISCARD auto next_date(const Temporal::Date &completed_at) const
        -> std::optional<Temporal::Date>;
};
} // namespace Lines
