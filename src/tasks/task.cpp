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
#include <optional>
#include <utility>

Lines::Task::Task(TaskInfo info, TaskRepeatRule rule)
    : _info(std::move(info)), _rule(std::move(rule)) {}

void Lines::Task::set_title(const std::string &title) {
    if (title.empty()) {
        throw std::invalid_argument("Lines::Task: title must not be empty");
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

auto Lines::Task::next_deadline(const Temporal::Date &completed_at) const
    -> std::optional<Temporal::Date> {
    return _rule.next_date(completed_at);
}

auto Lines::Task::deadline() const -> const std::optional<Temporal::Date> & { return _deadline; };

void Lines::Task::complete() { _completed = true; }

void Lines::Task::advance_deadline(const Temporal::Date &completed_at) {
    _deadline = _rule.next_date(completed_at);
}

auto Lines::Task::actual(const Temporal::Date &date) const -> bool {
    return !_completed && (!_deadline || *_deadline <= date);
}

void Lines::Task::uncomplete() { _completed = false; };

LINES_NODISCARD auto Lines::Task::completed() const -> bool { return _completed; };
