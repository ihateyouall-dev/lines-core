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

void Lines::TaskCompletion::set_state(State state) noexcept { _state = state; }
void Lines::TaskCompletion::complete() noexcept { set_state(State::Completed); };
auto Lines::TaskCompletion::completed() const noexcept -> bool {
    return _state == State::Completed;
}

auto Lines::TaskCompletion::finished() const noexcept -> bool {
    return _state != State::NotCompleted;
}

void Lines::TaskCompletion::reset() noexcept { set_state(State::NotCompleted); }
void Lines::TaskCompletion::skip() noexcept { set_state(State::Skipped); }
auto Lines::TaskCompletion::skipped() const noexcept -> bool { return _state == State::Skipped; }

auto Lines::TaskCompletion::state() const noexcept -> State { return _state; }
