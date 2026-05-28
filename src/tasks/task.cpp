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

#include "lines/tasks/task_repeat.hpp"
#include "lines/temporal/clocks.hpp"
#include "lines/temporal/timepoint.hpp"

#include <optional>
#include <utility>

Lines::Task::Task(TaskInfo info, std::optional<TaskRepeatRule> rule)
    : _info(std::move(info)), _repeat_rule(std::move(rule)) {}

void Lines::Task::set_title(const std::string &title) {
    if (title.empty()) {
        throw TaskError("ERROR: Title must not be empty");
    }
    _info.title = title;
}

void Lines::Task::set_description(const std::string &description) {
    _info.description = description;
}

void Lines::Task::set_tags(std::vector<std::string> tags) { _info.tags = std::move(tags); }

void Lines::Task::set_repeat_rule(const std::optional<TaskRepeatRule> &rule) {
    if (!_due) {
        set_due(Temporal::LocalClock::now());
    }
    _repeat_rule = rule;
    advance_due();
}

void Lines::Task::set_repeat_rule_raw(const std::optional<TaskRepeatRule> &rule) {
    _repeat_rule = rule;
}

auto Lines::Task::title() const -> const std::string & { return _info.title; }

auto Lines::Task::description() const -> const std::optional<std::string> & {
    return _info.description;
}

auto Lines::Task::tags() const -> const std::vector<std::string> & { return _info.tags; }

auto Lines::Task::next_due(const Temporal::TimePoint &completed_at) const
    -> std::optional<Temporal::TimePoint> {
    // Returns the next due for the task after completion.
    // Non-repeating tasks keep their current due if it has not passed.
    // Repeating tasks compute the next due using the repeat rule.
    if (!_due) {
        return std::nullopt;
    }
    if (!_repeat_rule) {
        if (completed_at >= *_due) {
            return std::nullopt;
        }
        return _due;
    }
    return _repeat_rule->next_due(completed_at);
}

auto Lines::Task::due() const -> const std::optional<Temporal::TimePoint> & { return _due; };

void Lines::Task::complete() {
    if (_repeat_rule) {
        advance_due();
        return;
    }
    _completed = true;
}

void Lines::Task::advance_due(const Temporal::TimePoint &completed_at) {
    _due = next_due(completed_at);
}

auto Lines::Task::is_active(const Temporal::TimePoint &tp) const -> bool {
    // A task is active on a given timepoint if it isn't completed
    // and its due has not yet passed. Tasks without dues
    // remain active until completed.
    return !_completed && (!_due || tp <= *_due);
}

void Lines::Task::uncomplete() { _completed = false; };

LINES_NODISCARD auto Lines::Task::completed() const -> bool { return _completed; };

void Lines::Task::set_due(const std::optional<Temporal::TimePoint> &due) {
    if (!due && _repeat_rule) {
        throw TaskError("ERROR: Cannot disable due to repeating task");
    }
    _due = due;
}

LINES_NODISCARD auto Lines::Task::next_due() const -> std::optional<Temporal::TimePoint> {
    return _due ? next_due(*_due) : std::nullopt;
}

void Lines::Task::advance_due() { _due = next_due(); }

LINES_NODISCARD auto Lines::Task::repeat_rule() const
    -> const std::optional<Lines::TaskRepeatRule> & {
    return _repeat_rule;
}

Lines::TaskError::TaskError(std::string_view what) : _what(what) {}

[[nodiscard]] auto Lines::TaskError::what() const noexcept -> const char * { return _what.c_str(); }

void Lines::Task::set_repeat_end(const std::optional<Temporal::TimePoint> &end) {
    if (!_repeat_rule) {
        throw TaskError("ERROR: Cannot assign repeat end to task without repeat rule");
    }
    _repeat_rule->end = end;
}

LINES_NODISCARD auto Lines::Task::repeat_end() const
    -> const std::optional<Lines::Temporal::TimePoint> & {
    return _repeat_rule->end;
}
