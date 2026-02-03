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
#include <iomanip>
#include <lines/temporal/timestamp.hpp>

void Lines::Temporal::Timestamp::normalize() {
    _rep %= Days::period;
    if (_rep < Seconds{0}) {
        _rep += Seconds{Days::period};
    }
}

Lines::Temporal::Timestamp::Timestamp(const Hours &hours, const Minutes &minutes,
                                      const Seconds &seconds)
    : _rep(duration_cast<Seconds>(hours) + duration_cast<Seconds>(minutes) + seconds) {
    normalize();
}

auto Lines::Temporal::Timestamp::operator++() -> Timestamp & {
    ++_rep;
    normalize();
    return *this;
}

Lines::Temporal::Timestamp::Timestamp(const Seconds &rep) : _rep(rep) { normalize(); }

auto Lines::Temporal::Timestamp::operator++(int) -> Timestamp {
    auto temp = *this;
    ++*this;
    normalize();
    return temp;
}

auto Lines::Temporal::Timestamp::operator--() -> Timestamp & {
    --_rep;
    normalize();
    return *this;
}

auto Lines::Temporal::Timestamp::operator--(int) -> Timestamp {
    auto temp = *this;
    --*this;
    normalize();
    return temp;
}

auto Lines::Temporal::Timestamp::operator-(const Timestamp &time) const -> decltype(_rep) {
    auto temp = _rep;
    temp -= time._rep;
    return temp;
}

auto Lines::Temporal::Timestamp::hh_mm_ss() const -> std::string {
    std::ostringstream hh_mm_ss;

    auto hours = floor<Hours>(_rep);
    auto minutes = floor<Minutes>(_rep - hours);
    auto seconds = _rep - floor<Minutes>(_rep);

    hh_mm_ss << std::setw(2) << std::setfill('0') << hours.count() << ':' << std::setw(2)
             << minutes.count() << ':' << std::setw(2) << seconds.count();

    return hh_mm_ss.str();
}

auto Lines::Temporal::Timestamp::hours() const -> Hours { return floor<Hours>(_rep); }

auto Lines::Temporal::Timestamp::minutes() const -> Minutes {
    return floor<Minutes>(_rep - floor<Hours>(_rep));
}

auto Lines::Temporal::Timestamp::seconds() const -> Seconds { return _rep - floor<Minutes>(_rep); }
