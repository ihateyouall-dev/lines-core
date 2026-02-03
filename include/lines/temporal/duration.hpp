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

#include "lines/detail/macro.h"

#include <chrono>
#include <concepts>
#include <cstdint>

namespace Lines::Temporal {
template <uint32_t Period, std::integral Rep = int64_t> class Duration {
    Rep _rep;
    uint32_t _period = Period;

  public:
    constexpr Duration() = default;
    static constexpr uint32_t period = Period;
    Duration(const Duration &) = default;
    Duration(Duration &&) = default;
    auto operator=(const Duration &) -> Duration & = default;
    auto operator=(Duration &&) -> Duration & = default;
    explicit constexpr Duration(Rep rep) : _rep(std::move(rep)) {}
    template <uint32_t P, std::integral R>
    explicit constexpr Duration(const Duration<P, R> &dur) : _rep(dur.count() * P / period) {}
    template <class R, class P>
    explicit constexpr Duration(std::chrono::duration<R, P> const &dur)
        : _rep(std::chrono::duration_cast<std::chrono::duration<Rep, std::ratio<Period>>>(dur)
                   .count()) {}
    ~Duration() = default;

    constexpr auto operator+() const noexcept -> Duration { return *this; }

    constexpr auto operator-() const noexcept -> Duration { return Duration{-_rep}; }

    constexpr auto operator++() -> Duration & {
        ++_rep;
        return *this;
    }

    constexpr auto operator++(int) -> Duration {
        auto temp = *this;
        ++*this;
        return temp;
    }

    constexpr auto operator--() -> Duration & {
        --_rep;
        return *this;
    }

    constexpr auto operator--(int) -> Duration {
        auto temp = *this;
        --*this;
        return temp;
    }

    constexpr auto operator+(const Duration &dur) const -> Duration {
        auto temp = *this;
        temp += dur;
        return temp;
    }

    constexpr auto operator+=(const Duration &dur) -> Duration & {
        _rep += dur._rep;
        return *this;
    }

    constexpr auto operator-(const Duration &dur) const -> Duration {
        auto temp = *this;
        temp -= dur;
        return temp;
    }

    constexpr auto operator-=(const Duration &dur) -> Duration & {
        _rep -= dur._rep;
        return *this;
    }

    template <std::integral T> constexpr auto operator*(T num) const -> Duration {
        auto temp = *this;
        temp *= num;
        return temp;
    }

    template <std::integral T> constexpr auto operator*=(T num) -> Duration & {
        _rep *= num;
        return *this;
    }

    template <std::integral T> constexpr friend auto operator*(T num, Duration dur) -> Duration {
        return dur * num;
    }

    template <std::integral T> constexpr auto operator/(T num) const -> Duration {
        if (num == 0) {
            throw std::invalid_argument("Duration::operator/: division by zero");
        }
        auto temp = *this;
        temp /= num;
        return temp;
    }

    template <std::integral T> constexpr auto operator/=(T num) -> Duration & {
        if (num == 0) {
            throw std::invalid_argument("Duration::operator/=: division by zero");
        }
        _rep /= num;
        return *this;
    }

    constexpr auto operator/(const Duration &dur) -> Rep {
        if (dur == Duration{0}) {
            throw std::invalid_argument("Duration::operator/: division by zero");
        }
        auto temp = *this;
        temp /= dur._rep;
        return temp._rep;
    }

    template <std::integral T> constexpr auto operator%(T num) const -> Duration {
        if (num == 0) {
            throw std::invalid_argument("Duration::operator%: division by zero");
        }
        auto temp = *this;
        temp %= num;
        return temp;
    }

    template <std::integral T> constexpr auto operator%=(T num) -> Duration & {
        if (num == 0) {
            throw std::invalid_argument("Duration::operator%=: division by zero");
        }
        _rep %= num;
        return *this;
    }

    constexpr auto operator%(const Duration &dur) const -> Rep {
        if (dur == Duration{0}) {
            throw std::invalid_argument("Duration::operator%: division by zero");
        }
        auto temp = *this;
        temp %= dur._rep;
        return temp._rep;
    }

    constexpr auto operator%=(const Duration &dur) -> Duration & {
        if (dur == Duration{0}) {
            throw std::invalid_argument("Duration::operator%=: division by zero");
        }
        _rep %= dur._rep;
        return *this;
    }

    [[nodiscard]] constexpr auto count() const noexcept -> Rep { return _rep; }

    template <typename ChronoDuration> constexpr auto to_chrono() const -> ChronoDuration {
        return ChronoDuration{_rep * period / ChronoDuration::period::num};
    }
};

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
constexpr auto operator<=>(const Duration<P1, R1> &lhs, const Duration<P2, R2> &rhs) {
    using Lhs = Duration<P1, R1>;
    using Rhs = Duration<P2, R2>;

    if constexpr (Lhs::period > Rhs::period) {
        return duration_cast<Rhs>(lhs).count() <=> rhs.count();
    } else if constexpr (Lhs::period < Rhs::period) {
        return lhs.count() <=> duration_cast<Lhs>(rhs).count();
    } else {
        return lhs.count() <=> rhs.count();
    }
}

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
constexpr auto operator==(const Duration<P1, R1> &lhs, const Duration<P2, R2> &rhs) -> bool {
    return (lhs <=> rhs) == 0;
}

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
    requires(!(P1 == P2))
constexpr auto operator+(const Duration<P1, R1> &lhs, const Duration<P2, R2> &rhs) {
    using Lhs = Duration<P1, R1>;
    using Rhs = Duration<P2, R2>;

    if constexpr (Lhs::period > Rhs::period) {
        return duration_cast<Rhs>(lhs) + rhs;
    }

    if constexpr (Lhs::period < Rhs::period) {
        return lhs + duration_cast<Lhs>(rhs);
    }
    LINES_UNREACHABLE();
}

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
    requires(!(P1 == P2))
constexpr auto operator+=(Duration<P1, R1> &lhs, const Duration<P2, R2> &rhs) {
    lhs += duration_cast<Duration<P1, R1>>(rhs);
    return lhs;
}

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
    requires(!(P1 == P2))
constexpr auto operator-(const Duration<P1, R1> &lhs, const Duration<P2, R2> &rhs) {
    using Lhs = Duration<P1, R1>;
    using Rhs = Duration<P2, R2>;

    if constexpr (Lhs::period > Rhs::period) {
        return duration_cast<Rhs>(lhs) - rhs;
    }

    if constexpr (Lhs::period < Rhs::period) {
        return lhs - duration_cast<Lhs>(rhs);
    }
    LINES_UNREACHABLE();
}

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
    requires(!(P1 == P2))
constexpr auto operator-=(Duration<P1, R1> &lhs, const Duration<P2, R2> &rhs) {
    lhs -= duration_cast<Duration<P1, R1>>(rhs);
    return lhs;
}

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
    requires(!(P1 == P2))
constexpr auto operator/(const Duration<P1, R1> &lhs, const Duration<P2, R2> &rhs) {
    using Lhs = Duration<P1, R1>;
    using Rhs = Duration<P2, R2>;

    if (rhs == Rhs{0}) {
        throw std::invalid_argument("Duration::operator/: division by zero");
    }

    if constexpr (Lhs::period > Rhs::period) {
        return duration_cast<Rhs>(lhs) / rhs;
    }

    if constexpr (Lhs::period < Rhs::period) {
        return lhs / duration_cast<Lhs>(rhs);
    }
    LINES_UNREACHABLE();
}

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
    requires(!(P1 == P2))
constexpr auto operator%(const Duration<P1, R1> &lhs, const Duration<P2, R2> &rhs) {
    using Lhs = Duration<P1, R1>;
    using Rhs = Duration<P2, R2>;

    if (rhs == Rhs{0}) {
        throw std::invalid_argument("Duration::operator%: division by zero");
    }

    if constexpr (Lhs::period > Rhs::period) {
        return duration_cast<Rhs>(lhs) % rhs;
    }

    if constexpr (Lhs::period < Rhs::period) {
        return lhs % duration_cast<Lhs>(rhs);
    }
    LINES_UNREACHABLE();
}

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
    requires(!(P1 == P2))
constexpr auto operator%=(Duration<P1, R1> &lhs, const Duration<P2, R2> &rhs) {
    if (rhs == Duration<P2, R2>{0}) {
        throw std::invalid_argument("Duration::operator%=: division by zero");
    }
    lhs %= duration_cast<Duration<P1, R1>>(rhs);
    return lhs;
}

using Seconds = Duration<1>;
using Minutes = Duration<60>;     // NOLINT
using Hours = Duration<3600>;     // NOLINT
using Days = Duration<86400>;     // NOLINT
using Weeks = Duration<604800>;   // NOLINT
using Months = Duration<2629746>; // NOLINT
using Years = Duration<31556952>; // NOLINT

template <typename To, uint32_t Period, std::integral Rep>
constexpr auto duration_cast(const Duration<Period, Rep> &dur) -> To {
    return To(dur.count() * Period / To::period);
}

template <typename To, uint32_t Period, std::integral Rep>
constexpr auto floor(const Duration<Period, Rep> &dur) -> To {
    To to_dur = duration_cast<To>(dur);
    if (to_dur > dur) {
        return to_dur - To{1};
    }
    return to_dur;
}

template <typename To, uint32_t Period, std::integral Rep>
constexpr auto ceil(const Duration<Period, Rep> &dur) -> To {
    To to_dur = duration_cast<To>(dur);
    if (to_dur < dur) {
        return to_dur + To{1};
    }
    return to_dur;
}

template <typename To, uint32_t Period, std::integral Rep>
constexpr auto round(const Duration<Period, Rep> &dur) -> To {
    To to0 = floor<To>(dur);
    To to1 = to0 + To{1};
    auto diff0 = dur - to0;
    auto diff1 = to1 - dur;
    if (diff0 == diff1) {
        if (to0.count() & 1) {
            return to1;
        }
        return to0;
    }

    if (diff0 < diff1) {
        return to0;
    }
    return to1;
}
} // namespace Lines::Temporal
