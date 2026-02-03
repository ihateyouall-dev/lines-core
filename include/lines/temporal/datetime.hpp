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

#include <lines/temporal/date.hpp>
#include <lines/temporal/timepoint.hpp>
#include <lines/temporal/timestamp.hpp>

namespace Lines::Temporal {
// Representation of TimePoint as Date + TimeStamp
class DateTime {
    TimePoint _tp;

  public:
    DateTime(const DateTime &) = default;
    DateTime(DateTime &&) = default;
    auto operator=(const DateTime &) -> DateTime & = default;
    auto operator=(DateTime &&) -> DateTime & = default;
    explicit DateTime(TimePoint tp) : _tp(tp) {}
    ~DateTime() = default;

    [[nodiscard]] auto date() const -> Date { return Date(floor<Days>(_tp.time_since_epoch())); }

    [[nodiscard]] auto time() const -> Timestamp { return Timestamp(_tp.time_since_epoch()); }
};
} // namespace Lines::Temporal
