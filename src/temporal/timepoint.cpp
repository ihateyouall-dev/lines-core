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
#include <lines/temporal/timepoint.hpp>

Lines::Temporal::TimePoint::TimePoint(const Duration &rep) : _rep(rep) {}

auto Lines::Temporal::TimePoint::operator++() -> TimePoint & {
    ++_rep;
    return *this;
}

auto Lines::Temporal::TimePoint::operator++(int) -> TimePoint {
    auto temp = *this;
    ++*this;
    return temp;
}

auto Lines::Temporal::TimePoint::operator--() -> TimePoint & {
    --_rep;
    return *this;
}

auto Lines::Temporal::TimePoint::operator--(int) -> TimePoint {
    auto temp = *this;
    --*this;
    return temp;
}

auto Lines::Temporal::TimePoint::operator-(const TimePoint &tp) const -> Duration {
    return _rep - tp._rep;
}

auto Lines::Temporal::TimePoint::time_since_epoch() const -> Duration { return _rep; }
