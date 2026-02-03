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

#include <lines/temporal/duration.hpp>

namespace Lines::Temporal {
class TimePoint {
    // Unlike Timestamp, this class does not have a strong invariant.
    // It represents absolute time since the epoch.
    Seconds _rep{0};

  public:
    using Duration = decltype(_rep);
    explicit TimePoint(const Duration &rep);
    TimePoint(const TimePoint &) = default;
    TimePoint(TimePoint &&) = default;
    auto operator=(const TimePoint &) -> TimePoint & = default;
    auto operator=(TimePoint &&) -> TimePoint & = default;
    ~TimePoint() = default;

    auto operator<=>(const TimePoint &) const = default;

    auto operator++() -> TimePoint &;

    auto operator++(int) -> TimePoint;

    auto operator--() -> TimePoint &;

    auto operator--(int) -> TimePoint;

    template <uint32_t Period, std::integral Rep>
    auto operator+=(const Lines::Temporal::Duration<Period, Rep> &dur) -> TimePoint & {
        _rep += dur;
        return *this;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator-=(const Lines::Temporal::Duration<Period, Rep> &dur) -> TimePoint & {
        _rep -= dur;
        return *this;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator+(const Lines::Temporal::Duration<Period, Rep> &dur) const -> TimePoint {
        auto temp = *this;
        temp += dur;
        return temp;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator-(const Lines::Temporal::Duration<Period, Rep> &dur) const -> TimePoint {
        auto temp = *this;
        temp -= dur;
        return temp;
    }

    auto operator-(const TimePoint &tp) const -> Duration; // NOLINT

    [[nodiscard]] auto time_since_epoch() const -> Duration;
};

template <uint32_t Period, std::integral Rep>
auto operator+(const Duration<Period, Rep> &lhs, const TimePoint &rhs) -> TimePoint {
    return rhs + lhs;
}
} // namespace Lines::Temporal
