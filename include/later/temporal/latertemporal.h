#pragma once

#include <chrono>
#include <concepts>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace Later::Temporal {

template <uint32_t Period, std::integral Rep = int64_t> class Duration {
    Rep _value;
    uint32_t _period = Period;

  public:
    static constexpr uint32_t period = Period;
    Duration(const Duration &) = default;
    Duration(Duration &&) = default;
    auto operator=(const Duration &) -> Duration & = default;
    auto operator=(Duration &&) -> Duration & = default;
    explicit Duration(Rep value) : _value(std::move(value)) {}
    ~Duration() = default;

    constexpr auto operator<=>(const Duration &) const = default;

    auto operator++() -> Duration & {
        ++_value;
        return *this;
    }

    auto operator++(int) -> Duration {
        auto temp = *this;
        ++*this;
        return temp;
    }

    auto operator--() -> Duration & {
        --_value;
        return *this;
    }

    auto operator--(int) -> Duration {
        auto temp = *this;
        --*this;
        return temp;
    }

    auto operator+(const Duration &dur) const -> Duration {
        auto temp = *this;
        temp += dur;
        return temp;
    }

    auto operator+=(const Duration &dur) -> Duration & {
        _value += dur._value;
        return *this;
    }

    auto operator-(const Duration &dur) const -> Duration {
        auto temp = *this;
        temp -= dur;
        return temp;
    }

    auto operator-=(const Duration &dur) -> Duration & {
        _value -= dur._value;
        return *this;
    }

    template <std::integral T> auto operator*(T num) const -> Duration {
        auto temp = *this;
        temp *= num;
        return temp;
    }

    template <std::integral T> auto operator*=(T num) -> Duration & {
        _value *= num;
        return *this;
    }

    template <std::integral T> friend auto operator*(T num, Duration dur) -> Duration {
        return dur * num;
    }

    template <std::integral T> auto operator/(T num) const -> Duration {
        auto temp = *this;
        temp /= num;
        return temp;
    }

    template <std::integral T> auto operator/=(T num) -> Duration & {
        _value /= num;
        return *this;
    }

    template <std::integral T> auto operator%(T num) const -> Duration {
        auto temp = *this;
        temp %= num;
        return temp;
    }

    template <std::integral T> auto operator%=(T num) -> Duration & {
        _value %= num;
        return *this;
    }

    auto operator%(const Duration &dur) const -> Duration {
        auto temp = *this;
        temp %= dur;
        return temp;
    }

    auto operator%=(const Duration &dur) -> Duration & {
        _value %= dur;
        return *this;
    }

    [[nodiscard]] auto count() const noexcept -> Rep { return _value; }
};

template <uint32_t P1, uint32_t P2, std::integral R1, std::integral R2>
    requires(!(P1 == P2))
constexpr auto operator<=>(const Duration<P1, R1> &dur1, const Duration<P2, R2> &dur2) {
    using Dur1 = Duration<P1, R1>;
    using Dur2 = Duration<P2, R2>;

    if constexpr (Dur1::period > Dur2::period) {
        return duration_cast<Dur2>(dur1) <=> dur2;
    }

    if constexpr (Dur1::period < Dur2::period) {
        return dur1 <=> duration_cast<Dur1>(dur2);
    }
    __builtin_unreachable();
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

class Time {
    Seconds _rep = Seconds{0};

  public:
    Time() = default;
    explicit Time(Seconds rep) : _rep(rep) {}

    [[nodiscard]] auto count() const -> Seconds { return _rep; }

    auto operator<=>(const Time &) const = default;

    [[nodiscard]] auto hh_mm_ss() const -> std::string {
        std::ostringstream hh_mm_ss;

        auto hours = floor<Hours>(_rep);
        auto minutes = floor<Minutes>(_rep - duration_cast<Seconds>(hours));
        auto seconds = _rep - duration_cast<Seconds>(minutes) - duration_cast<Seconds>(hours);

        hh_mm_ss << std::setw(2) << std::setfill('0') << hours.count() << ':' << std::setw(2)
                 << minutes.count() << ':' << std::setw(2) << seconds.count();

        return hh_mm_ss.str();
    }

    static auto now() -> Time {
        auto now = std::chrono::system_clock::now();
        auto absolute_rep =
            std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        return Time(Seconds{static_cast<long>(absolute_rep)});
    }
};

class Date {
    std::chrono::days _days;

    [[nodiscard]] auto ymd() const -> std::chrono::year_month_day {
        return std::chrono::sys_days(_days);
    }

  public:
    explicit Date(std::chrono::days days) : _days(days) {}

    auto operator<=>(const Date &) const = default;

    auto operator+=(std::chrono::days days) -> Date & {
        _days += days;
        return *this;
    }
    auto operator-=(std::chrono::days days) -> Date & {
        _days -= days;
        return *this;
    }

    auto operator++() -> Date & {
        ++_days;
        return *this;
    }
    auto operator++(int) -> Date {
        Date tmp = *this;
        ++*this;
        return tmp;
    }

    auto operator--() -> Date & {
        --_days;
        return *this;
    }
    auto operator--(int) -> Date {
        Date tmp = *this;
        --*this;
        return tmp;
    }

    static auto today() -> Date {
        auto now = std::chrono::system_clock::now();
        auto days = std::chrono::floor<std::chrono::days>(now).time_since_epoch();
        return Date(days);
    }

    [[nodiscard]] auto year() const -> int { return int(ymd().year()); }
    [[nodiscard]] auto month() const -> unsigned { return unsigned(ymd().month()); }
    [[nodiscard]] auto day() const -> unsigned { return unsigned(ymd().day()); }
};
} // namespace Later::Temporal
