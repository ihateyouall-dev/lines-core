#pragma once

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace Later::Temporal {
namespace detail {
struct TemporalTag {};

struct LinearTimeTag : TemporalTag {};
struct CalendarTimeTag : TemporalTag {};

struct SecondsTag : LinearTimeTag {};
struct MinutesTag : LinearTimeTag {};
struct HoursTag : LinearTimeTag {};

struct DaysTag : CalendarTimeTag {};
struct MonthsTag : CalendarTimeTag {};
struct YearsTag : CalendarTimeTag {};
}; // namespace detail

class Time {
    using uint = std::uint32_t;
    uint _time;

    static constexpr uint seconds_in_hour = 3600;
    static constexpr uint seconds_in_minute = 60;
    static constexpr uint seconds_in_day = 24 * seconds_in_hour;

  public:
    Time() = default;
    explicit Time(uint time) : _time(time % seconds_in_day) {}
    Time(uint hours, uint minutes, uint seconds)
        : _time(((hours * seconds_in_hour) + (minutes * seconds_in_minute) + seconds) %
                seconds_in_day) {}

    [[nodiscard]] auto time() const -> uint { return _time; }

    auto operator<=>(const Time &) const = default;

    [[nodiscard]] auto hh_mm_ss() const -> std::string {
        std::ostringstream hh_mm_ss;

        uint hours = static_cast<uint>(_time / seconds_in_hour);
        const uint minutes = (_time / seconds_in_minute) % 60;
        const uint seconds = _time % 60;

        hh_mm_ss << std::setw(2) << std::setfill('0') << hours << ':' << std::setw(2) << minutes
                 << ':' << std::setw(2) << seconds;

        return hh_mm_ss.str();
    }

    static auto now() -> Time {
        auto now = std::chrono::system_clock::now();
        auto absolute_time =
            std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        return Time(static_cast<uint>(absolute_time) % seconds_in_day);
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
