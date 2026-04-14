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
#include "lines/temporal/timepoint.hpp"
#include <lines/tasks/task.hpp>
#include <optional>
#include <utility>

Lines::Task::Task(TaskInfo info, std::optional<TaskRepeatRule> rule)
    : _info(std::move(info)), _repeat_rule(std::move(rule)) {}

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

void Lines::Task::set_repeat_rule(const std::optional<TaskRepeatRule> &rule) {
    _repeat_rule = rule;
}

auto Lines::Task::title() const -> const std::string & { return _info.title; }

auto Lines::Task::description() const -> const std::optional<std::string> & {
    return _info.description;
}

auto Lines::Task::tags() const -> const std::vector<std::string> & { return _info.tags; }

auto Lines::Task::next_deadline(const Temporal::TimePoint &completed_at) const
    -> std::optional<Temporal::TimePoint> {
    // Returns the next deadline for the task after completion.
    // Non-repeating tasks keep their current deadline if it has not passed.
    // Repeating tasks compute the next deadline using the repeat rule.
    if (!_repeat_rule) {
        if (_deadline) {
            if (completed_at >= *_deadline) {
                return std::nullopt;
            }
            return _deadline;
        }
        return std::nullopt;
    }
    return _repeat_rule->next_deadline(completed_at);
}

auto Lines::Task::deadline() const -> const std::optional<Temporal::TimePoint> & {
    return _deadline;
};

void Lines::Task::complete() { _completed = true; }

void Lines::Task::advance_deadline(const Temporal::TimePoint &completed_at) {
    _deadline = next_deadline(completed_at);
}

auto Lines::Task::is_active(const Temporal::TimePoint &tp) const -> bool {
    // A task is active on a given timepoint if it isn't completed
    // and its deadline has not yet passed. Tasks without deadlines
    // remain active until completed.
    return !_completed && (!_deadline || tp <= *_deadline);
}

void Lines::Task::uncomplete() { _completed = false; };

LINES_NODISCARD auto Lines::Task::completed() const -> bool { return _completed; };

void Lines::Task::set_deadline(const std::optional<Temporal::TimePoint> &deadline) {
    _deadline = deadline;
}
LINES_NODISCARD auto Lines::Task::next_deadline() const -> std::optional<Temporal::TimePoint> {
    return next_deadline(*_deadline);
}

void Lines::Task::advance_deadline() { _deadline = next_deadline(); }
