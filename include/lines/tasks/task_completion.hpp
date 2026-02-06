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

#include <cstdint>
#include <lines/detail/macro.h>

namespace Lines {

class LINES_API TaskCompletion {
  public:
    enum class State : uint8_t { NotCompleted, Completed, Skipped };

  private:
    State _state = State::NotCompleted;

    void set_state(State state) LINES_NOEXCEPT;

  public:
    TaskCompletion() = default;
    TaskCompletion(const TaskCompletion &) = default; // LCOV_EXCL_LINE
    TaskCompletion(TaskCompletion &&) = default;
    auto operator=(const TaskCompletion &) -> TaskCompletion & = default;
    auto operator=(TaskCompletion &&) -> TaskCompletion & = default;
    ~TaskCompletion() = default;

    void complete() LINES_NOEXCEPT;
    LINES_NODISCARD auto completed() const LINES_NOEXCEPT -> bool;
    LINES_NODISCARD auto finished() const LINES_NOEXCEPT -> bool;

    void reset() LINES_NOEXCEPT;
    void skip() LINES_NOEXCEPT;
    LINES_NODISCARD auto skipped() const LINES_NOEXCEPT -> bool;
    LINES_NODISCARD auto state() const LINES_NOEXCEPT -> State;
};
} // namespace Lines
