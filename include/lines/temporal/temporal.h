#pragma once

#include "lines/detail/macro.h"
#include <chrono>
#include <concepts>
#include <cstdint>
#include <string>

namespace Lines::Temporal {
class Date {
    Days _rep;

    std::chrono::year_month_day _ymd;

    [[nodiscard]] auto update_ymd() const -> std::chrono::year_month_day {
        return std::chrono::sys_days(std::chrono::days{_rep.count()});
    }

  public:
    explicit Date(Days rep) : _rep(rep), _ymd(update_ymd()) {}
    Date(const Year &year, const Month &month, const Day &day)
        : _rep(Days{std::chrono::sys_days{
              std::chrono::year{int(year)} / std::chrono::month{unsigned(month)} /
              std::chrono::day{unsigned(
                  day)}}.time_since_epoch()}),
          _ymd(update_ymd()) {
        if (!_ymd.ok()) {
            throw std::invalid_argument("Date::Date: invalid arguments for constructor");
        }
    }

    auto operator<=>(const Date &date) const { return _rep <=> date._rep; }

    auto operator==(const Date &date) const -> bool { return (*this <=> date) == 0; }

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

    auto operator-(const Date &date) const -> Days { return _rep - date._rep; }

    auto operator++() -> Date & {
        ++_rep;
        _ymd = update_ymd();
        return *this;
    }
    auto operator++(int) -> Date {
        Date tmp = *this;
        ++*this;
        return tmp;
    }

    auto operator--() -> Date & {
        --_rep;
        _ymd = update_ymd();
        return *this;
    }

    auto operator--(int) -> Date {
        Date tmp = *this;
        --*this;
        return tmp;
    }

    [[nodiscard]] auto time_since_epoch() const -> Days { return _rep; }
    [[nodiscard]] auto year() const -> Year { return Year{int(_ymd.year())}; }
    [[nodiscard]] auto month() const -> Month { return Month{unsigned(_ymd.month())}; }
    [[nodiscard]] auto day() const -> Day { return Day{unsigned(_ymd.day())}; }
    [[nodiscard]] auto weekday() const -> Weekday {
        return static_cast<Weekday>(((_rep + Days{3}) % 7).count()); // NOLINT
    }

    [[nodiscard]] auto yyyy_mm_dd() const -> std::string {
        return std::format("{}-{:02}-{:02}", int(year()), unsigned(month()), unsigned(day()));
    }
};

template <uint32_t Period, std::integral Rep>
auto operator+(const Duration<Period, Rep> &lhs, const Date &rhs) -> Date {
    return rhs + lhs;
}

// Representation of TimePoint as Date + TimeStamp
class DateTime {
    TimePoint _tp;

  public:
    DateTime(const DateTime &) = default;
    DateTime(DateTime &&) = default;
    auto operator=(const DateTime &) -> DateTime & = default;
    auto operator=(DateTime &&) -> DateTime & = default;
    explicit DateTime(TimePoint tp) : _tp(tp) {} // NOLINT
    ~DateTime() = default;

    [[nodiscard]] auto date() const -> Date { return Date(floor<Days>(_tp.time_since_epoch())); }

    [[nodiscard]] auto time() const -> Timestamp { return Timestamp(_tp.time_since_epoch()); }
};

// This class represents a fixed offset from UTC.
// It is NOT equivalent to std::chrono::time_zone.
class TimeZone {
    Seconds _offset;

  public:
    TimeZone(const TimeZone &) = default;
    TimeZone(TimeZone &&) = default;
    auto operator=(const TimeZone &) -> TimeZone & = default;
    auto operator=(TimeZone &&) -> TimeZone & = default;
    explicit TimeZone(Seconds offset) : _offset(offset) {}
    explicit TimeZone(Hours hours) : _offset(Seconds{duration_cast<Seconds>(hours)}) {}
    ~TimeZone() = default;

    [[nodiscard]] auto offset() const -> Seconds { return _offset; }
};

// Wraps TimePoint and TimeZone into an object representing a zoned time
class ZonedTime {
    TimeZone _tz;
    TimePoint _tp;

  public:
    ZonedTime(const ZonedTime &) = default;
    ZonedTime(ZonedTime &&) = default;
    auto operator=(const ZonedTime &) -> ZonedTime & = default;
    auto operator=(ZonedTime &&) -> ZonedTime & = default;
    explicit ZonedTime(TimePoint tp, TimeZone tz) : _tz(tz), _tp(tp) {} // NOLINT
    ~ZonedTime() = default;

    auto get_local_time() const -> TimePoint { return _tp + _tz.offset(); } // NOLINT
    auto get_sys_time() const -> TimePoint { return _tp; }                  // NOLINT
};

// Depends on reality, cannot be unit tested
struct UTCClock final {
    static auto now() noexcept -> TimePoint {
        auto now = std::chrono::system_clock::now();
        return TimePoint(Seconds{now.time_since_epoch()});
    }

    static auto since_midnight() -> Timestamp { return Timestamp(now().time_since_epoch()); }

    static auto today() noexcept -> Date {
        auto days = floor<Days>(now().time_since_epoch());
        return Date(Days{days});
    }
};

struct LocalClock final {
    static auto now() noexcept -> TimePoint {
        auto now = UTCClock::now();
        return ZonedTime(now, current_zone()).get_local_time();
    }

    static auto since_midnight() -> Timestamp { return Timestamp(now().time_since_epoch()); }

    static auto today() noexcept -> Date {
        auto days = floor<Days>(now().time_since_epoch());
        return Date(days);
    }

    [[nodiscard]] static auto current_zone() -> TimeZone {
        std::time_t now = std::time(nullptr);
        std::tm local{};
        std::tm utc{};
#if defined(LINES_WINDOWSNT)
        localtime_s(&local, &now);
        gmtime_s(&utc, &now);
#else
        localtime_r(&now, &local);
        gmtime_r(&now, &utc);
#endif

        auto local_sec = std::mktime(&local);
        auto utc_sec = std::mktime(&utc);
        return TimeZone(Seconds{local_sec - utc_sec});
    }
};
} // namespace Lines::Temporal
