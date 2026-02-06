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
#include <lines/tasks/task_completion.hpp>

void Lines::TaskCompletion::set_state(State state) LINES_NOEXCEPT { _state = state; }
void Lines::TaskCompletion::complete() LINES_NOEXCEPT { set_state(State::Completed); };
auto Lines::TaskCompletion::completed() const LINES_NOEXCEPT -> bool {
    return _state == State::Completed;
}

auto Lines::TaskCompletion::finished() const LINES_NOEXCEPT -> bool {
    return _state != State::NotCompleted;
}

void Lines::TaskCompletion::reset() LINES_NOEXCEPT { set_state(State::NotCompleted); }
void Lines::TaskCompletion::skip() LINES_NOEXCEPT { set_state(State::Skipped); }
auto Lines::TaskCompletion::skipped() const LINES_NOEXCEPT -> bool {
    return _state == State::Skipped;
}

auto Lines::TaskCompletion::state() const LINES_NOEXCEPT -> State { return _state; }
