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
#include <lines/temporal/ymd.hpp>

// Year
auto Lines::Temporal::Year::operator+() const noexcept -> Year { return *this; }

auto Lines::Temporal::Year::operator-() const noexcept -> Year { return Year{-_rep}; }

auto Lines::Temporal::Year::operator+=(const Years &yrs) -> Year & {
    _rep += static_cast<Rep>(yrs.count());
    return *this;
}

auto Lines::Temporal::Year::operator-=(const Years &yrs) -> Year & {
    _rep -= static_cast<Rep>(yrs.count());
    return *this;
}

auto Lines::Temporal::Year::operator++() -> Year & {
    ++_rep;
    return *this;
}

auto Lines::Temporal::Year::operator++(int) -> Year {
    auto temp = *this;
    ++*this;
    return temp;
}

auto Lines::Temporal::Year::operator--() -> Year & {
    --_rep;
    return *this;
}

auto Lines::Temporal::Year::operator--(int) -> Year {
    auto temp = *this;
    --*this;
    return temp;
}

auto Lines::Temporal::Year::operator+(const Years &yrs) const -> Year {
    auto temp = *this;
    temp += yrs;
    return temp;
}

auto Lines::Temporal::Year::operator-(const Years &yrs) const -> Year {
    auto temp = *this;
    temp -= yrs;
    return temp;
}

auto Lines::Temporal::Year::operator-(const Year &year) const -> Years {
    return Years{_rep - year._rep};
}

auto Lines::Temporal::Year::ok() const noexcept -> bool {
    return _rep != std::numeric_limits<Rep>::min();
}

auto Lines::Temporal::operator+(const Years &lhs, const Year &rhs) -> Year { return rhs + lhs; }

// Month
Lines::Temporal::Month::Month(uint32_t rep) : _rep(rep) {}

auto Lines::Temporal::Month::operator++() -> Month & {
    ++_rep;
    return *this;
}

auto Lines::Temporal::Month::operator++(int) -> Month {
    auto temp = *this;
    ++*this;
    return temp;
}

auto Lines::Temporal::Month::operator--() -> Month & {
    --_rep;
    return *this;
}

auto Lines::Temporal::Month::operator--(int) -> Month {
    auto temp = *this;
    --*this;
    return temp;
}

auto Lines::Temporal::Month::operator+=(const Months &months) -> Month & {
    _rep += months.count();
    return *this;
}

auto Lines::Temporal::Month::operator-=(const Months &months) -> Month & {
    _rep -= months.count();
    return *this;
}

auto Lines::Temporal::Month::operator+(const Months &months) const -> Month {
    auto temp = *this;
    temp += months;
    return temp;
}

auto Lines::Temporal::Month::operator-(const Months &months) const -> Month {
    auto temp = *this;
    temp -= months;
    return temp;
}

auto Lines::Temporal::Month::operator-(const Month &month) const -> Months {
    return Months{_rep - month._rep};
}

auto Lines::Temporal::Month::ok() const -> bool {
    return _rep >= 1 && _rep <= 12; // NOLINT
}

auto Lines::Temporal::operator+(const Months &lhs, const Month &rhs) -> Month { return rhs + lhs; }

// Day
Lines::Temporal::Day::Day(uint32_t rep) : _rep(rep) {}

auto Lines::Temporal::Day::operator++() -> Day & {
    ++_rep;
    return *this;
}

auto Lines::Temporal::Day::operator++(int) -> Day {
    auto temp = *this;
    ++*this;
    return temp;
}

auto Lines::Temporal::Day::operator--() -> Day & {
    --_rep;
    return *this;
}

auto Lines::Temporal::Day::operator--(int) -> Day {
    auto temp = *this;
    --*this;
    return temp;
}

auto Lines::Temporal::Day::operator+=(const Days &days) -> Day & {
    _rep += days.count();
    return *this;
}

auto Lines::Temporal::Day::operator-=(const Days &days) -> Day & {
    _rep -= days.count();
    return *this;
}

auto Lines::Temporal::Day::operator+(const Days &days) const -> Day {
    auto temp = *this;
    temp += days;
    return temp;
}

auto Lines::Temporal::Day::operator-(const Days &days) const -> Day {
    auto temp = *this;
    temp -= days;
    return temp;
}

auto Lines::Temporal::Day::operator-(const Day &day) const -> Days { return Days{_rep - day._rep}; }

auto Lines::Temporal::Day::ok() const -> bool {
    return _rep >= 1 && _rep <= 31; // NOLINT
}

auto Lines::Temporal::operator+(const Days &lhs, const Day &rhs) -> Day { return rhs + lhs; }
