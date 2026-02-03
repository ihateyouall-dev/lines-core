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
#include "lines/tasks/task_repeat.hpp"
#include <lines/tasks/task.hpp>
#include <utility>

Lines::Task::Task(TaskInfo info, TaskRepeatRule rule)
    : _info(std::move(info)), _rule(std::move(rule)) {}

auto Lines::Task::completion() -> TaskCompletion & { return _completion; }

auto Lines::Task::completion() const -> const TaskCompletion & { return _completion; }

void Lines::Task::set_title(const std::string &title) {
    if (title.empty()) {
        throw std::invalid_argument("Task: title must not be empty");
    }
    _info.title = title;
}

void Lines::Task::set_description(const std::string &description) {
    _info.description = description;
}

void Lines::Task::set_tags(std::vector<std::string> tags) { _info.tags = std::move(tags); }

void Lines::Task::set_repeat_rule(const TaskRepeatRule &rule) { _rule = rule; }

auto Lines::Task::title() const -> const std::string & { return _info.title; }

auto Lines::Task::description() const -> const std::optional<std::string> & {
    return _info.description;
}

auto Lines::Task::tags() const -> const std::vector<std::string> & { return _info.tags; }

auto Lines::Task::next_date(const Temporal::Date &completed_at) const
    -> std::optional<Temporal::Date> {
    return _rule.next_date(completed_at);
}
