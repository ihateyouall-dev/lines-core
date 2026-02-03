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
class Year {
    int _rep;

    using Rep = decltype(_rep);

  public:
    Year(const Year &) = default;
    Year(Year &&) = default;
    auto operator=(const Year &) -> Year & = default;
    auto operator=(Year &&) -> Year & = default;
    explicit Year(int rep) : _rep(rep) {}
    ~Year() = default;

    constexpr explicit operator int() const noexcept { return _rep; };

    auto operator<=>(const Year &) const = default;

    auto operator+() const noexcept -> Year;

    auto operator-() const noexcept -> Year;

    auto operator+=(const Years &yrs) -> Year &;

    auto operator-=(const Years &yrs) -> Year &;

    auto operator++() -> Year &;

    auto operator++(int) -> Year;

    auto operator--() -> Year &;

    auto operator--(int) -> Year;

    auto operator+(const Years &yrs) const -> Year;

    auto operator-(const Years &yrs) const -> Year;

    auto operator-(const Year &year) const -> Years;

    [[nodiscard]] constexpr auto is_leap() const noexcept -> bool {
        if (_rep % 400 == 0) {
            return true;
        }
        if (_rep % 100 == 0) {
            return false;
        }
        return _rep % 4 == 0;
    }

    [[nodiscard]] auto ok() const noexcept -> bool;
};

auto operator+(const Years &lhs, const Year &rhs) -> Year;

class Month {
    // 1 <= _rep <= 12. Not strong invariant
    uint8_t _rep;

  public:
    Month(const Month &) = default;
    Month(Month &&) = default;
    auto operator=(const Month &) -> Month & = default;
    auto operator=(Month &&) -> Month & = default;
    explicit Month(uint32_t rep);
    ~Month() = default;

    auto operator<=>(const Month &) const = default;

    constexpr explicit operator unsigned() const { return _rep; }

    auto operator++() -> Month &;

    auto operator++(int) -> Month;

    auto operator--() -> Month &;

    auto operator--(int) -> Month;

    auto operator+=(const Months &months) -> Month &;

    auto operator-=(const Months &months) -> Month &;

    auto operator+(const Months &months) const -> Month;

    auto operator-(const Months &months) const -> Month;

    auto operator-(const Month &month) const -> Months;

    [[nodiscard]] auto ok() const -> bool;
};

auto operator+(const Months &lhs, const Month &rhs) -> Month;

class Day {
    // 1 <= _rep <= 31. Not strong invariant
    uint8_t _rep;

  public:
    Day(const Day &) = default;
    Day(Day &&) = default;
    auto operator=(const Day &) -> Day & = default;
    auto operator=(Day &&) -> Day & = default;
    explicit Day(uint32_t rep);
    ~Day() = default;

    auto operator<=>(const Day &) const = default;

    explicit constexpr operator unsigned() const { return _rep; };

    auto operator++() -> Day &;

    auto operator++(int) -> Day;

    auto operator--() -> Day &;

    auto operator--(int) -> Day;

    auto operator+=(const Days &days) -> Day &;

    auto operator-=(const Days &days) -> Day &;

    auto operator+(const Days &days) const -> Day;

    auto operator-(const Days &days) const -> Day;

    auto operator-(const Day &day) const -> Days;

    [[nodiscard]] auto ok() const -> bool;
};

auto operator+(const Days &lhs, const Day &rhs) -> Day;

enum class Weekday : int8_t {
    Monday = 0,
    Tuesday = 1,
    Wednesday = 2,
    Thursday = 3,
    Friday = 4,
    Saturday = 5,
    Sunday = 6
};
} // namespace Lines::Temporal
