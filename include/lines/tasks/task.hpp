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

#include <lines/detail/macro.h>
#include <lines/tasks/task_info.hpp>
#include <lines/tasks/task_repeat.hpp>
#include <lines/temporal/date.hpp>

namespace Lines {
class LINES_API Task {
    TaskInfo _info;
    std::optional<TaskRepeatRule<>> _repeat_rule;
    std::optional<Temporal::Date> _deadline;
    bool _completed{};

  public:
    explicit Task(TaskInfo info, std::optional<TaskRepeatRule<>> rule = std::nullopt);
    Task(const Task &task) = default;
    auto operator=(const Task &task) -> Task & = default;
    Task(Task &&) = default;
    auto operator=(Task &&) -> Task & = default;
    ~Task() = default;

    LINES_NODISCARD auto completed() const -> bool;
    void complete();
    void uncomplete();

    void set_title(const std::string &title);
    void set_description(const std::string &description);
    void set_tags(std::vector<std::string> tags);
    void set_repeat_rule(const std::optional<TaskRepeatRule<>> &rule);

    LINES_NODISCARD auto deadline() const -> const std::optional<Temporal::Date> &;

    LINES_NODISCARD auto title() const -> const std::string &;
    LINES_NODISCARD auto description() const -> const std::optional<std::string> &;
    LINES_NODISCARD auto tags() const -> const std::vector<std::string> &;

    LINES_NODISCARD auto next_deadline(const Temporal::Date &completed_at) const
        -> std::optional<Temporal::Date>;
    void advance_deadline(const Temporal::Date &completed_at);
    LINES_NODISCARD auto next_deadline() const -> std::optional<Temporal::Date>;
    void advance_deadline();
    void set_deadline(const std::optional<Temporal::Date> &deadline);
    LINES_NODISCARD auto is_active(const Temporal::Date &date) const -> bool;
};
} // namespace Lines
