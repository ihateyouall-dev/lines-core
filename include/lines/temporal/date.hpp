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
#include <lines/temporal/ymd.hpp>

namespace Lines::Temporal {
class Date {
    Days _rep;

    std::chrono::year_month_day _ymd;

    [[nodiscard]] auto update_ymd() const -> std::chrono::year_month_day;

  public:
    explicit Date(Days rep);
    Date(const Year &year, const Month &month, const Day &day);

    auto operator<=>(const Date &date) const -> std::strong_ordering;

    auto operator==(const Date &date) const -> bool;

    template <uint32_t Period, std::integral Rep>
    auto operator+=(const Duration<Period, Rep> &dur) -> Date & {
        _rep += duration_cast<Days>(dur);
        _ymd = update_ymd();
        return *this;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator+(const Duration<Period, Rep> &dur) const -> Date {
        auto temp = *this;
        temp += dur;
        return temp;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator-=(const Duration<Period, Rep> &dur) -> Date & {
        _rep -= duration_cast<Days>(dur);
        _ymd = update_ymd();
        return *this;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator-(const Duration<Period, Rep> &dur) const -> Date {
        auto temp = *this;
        temp -= dur;
        return temp;
    }

    auto operator-(const Date &date) const -> Days;

    auto operator++() -> Date &;

    auto operator++(int) -> Date;

    auto operator--() -> Date &;

    auto operator--(int) -> Date;

    [[nodiscard]] auto time_since_epoch() const -> Days;
    [[nodiscard]] auto year() const -> Year;
    [[nodiscard]] auto month() const -> Month;
    [[nodiscard]] auto day() const -> Day;
    [[nodiscard]] auto weekday() const -> Weekday;

    [[nodiscard]] auto yyyy_mm_dd() const -> std::string;
};

template <uint32_t Period, std::integral Rep>
auto operator+(const Duration<Period, Rep> &lhs, const Date &rhs) -> Date {
    return rhs + lhs;
}
} // namespace Lines::Temporal
