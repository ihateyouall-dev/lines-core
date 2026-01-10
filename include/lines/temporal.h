#pragma once

#include "detail/macro.h"
#include <chrono>
#include <concepts>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

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

class Timestamp {
    // 0 <= _rep < 24h
    Seconds _rep{0};

    void normalize() {
        _rep %= Days::period;
        if (_rep < Seconds{0}) {
            _rep += Seconds{Days::period};
        }
    }

  public:
    Timestamp() = default;
    explicit Timestamp(Seconds rep) : _rep(rep) { normalize(); }
    Timestamp(const Hours &hours, const Minutes &minutes, const Seconds &seconds)
        : _rep(duration_cast<Seconds>(hours) + duration_cast<Seconds>(minutes) + seconds) {
        normalize();
    }

    [[nodiscard]] auto time_since_midnight() const -> Seconds { return _rep; }

    auto operator<=>(const Timestamp &) const = default;

    auto operator++() -> Timestamp & {
        ++_rep;
        normalize();
        return *this;
    }

    auto operator++(int) -> Timestamp {
        auto temp = *this;
        ++*this;
        normalize();
        return temp;
    }

    auto operator--() -> Timestamp & {
        --_rep;
        normalize();
        return *this;
    }

    auto operator--(int) -> Timestamp {
        auto temp = *this;
        --*this;
        normalize();
        return temp;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator+(const Duration<Period, Rep> &dur) const -> Timestamp {
        auto temp = *this;
        temp._rep += dur;
        temp.normalize();
        return temp;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator+=(const Duration<Period, Rep> &dur) -> Timestamp & {
        _rep += dur.count();
        normalize();
        return *this;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator-(const Duration<Period, Rep> &dur) const -> Timestamp {
        auto temp = *this;
        temp._rep -= dur;
        temp.normalize();
        return temp;
    }

    auto operator-(const Timestamp &time) const -> decltype(_rep) {
        auto temp = _rep;
        temp -= time._rep;
        return temp;
    }

    template <uint32_t Period, std::integral Rep>
    auto operator-=(const Duration<Period, Rep> &dur) -> Timestamp & {
        _rep -= dur;
        normalize();
        return *this;
    }

    [[nodiscard]] auto hh_mm_ss() const -> std::string {
        std::ostringstream hh_mm_ss;

        auto hours = floor<Hours>(_rep);
        auto minutes = floor<Minutes>(_rep - hours);
        auto seconds = _rep - floor<Minutes>(_rep);

        hh_mm_ss << std::setw(2) << std::setfill('0') << hours.count() << ':' << std::setw(2)
                 << minutes.count() << ':' << std::setw(2) << seconds.count();

        return hh_mm_ss.str();
    }

    [[nodiscard]] auto hours() const -> Hours { return floor<Hours>(_rep); }

    [[nodiscard]] auto minutes() const -> Minutes {
        return floor<Minutes>(_rep - floor<Hours>(_rep));
    }

    [[nodiscard]] auto seconds() const -> Seconds { return _rep - floor<Minutes>(_rep); }
};

template <uint32_t Period, std::integral Rep>
auto operator+(const Duration<Period, Rep> &dur, const Timestamp &time) -> Timestamp {
    return time + dur;
}

class TimePoint {
    // Unlike Timestamp, this class does not have a strong invariant.
    // It represents absolute time since the epoch.
    Seconds _rep{0};

  public:
    using Duration = decltype(_rep);
    explicit TimePoint(Duration rep) : _rep(rep) {}
    TimePoint(const TimePoint &) = default;
    TimePoint(TimePoint &&) = default;
    auto operator=(const TimePoint &) -> TimePoint & = default;
    auto operator=(TimePoint &&) -> TimePoint & = default;
    ~TimePoint() = default;

    auto operator<=>(const TimePoint &) const = default;

    auto operator++() -> TimePoint & {
        ++_rep;
        return *this;
    }

    auto operator++(int) -> TimePoint {
        auto temp = *this;
        ++*this;
        return temp;
    }

    auto operator--() -> TimePoint & {
        --_rep;
        return *this;
    }

    auto operator--(int) -> TimePoint {
        auto temp = *this;
        --*this;
        return temp;
    }

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

    auto operator-(const TimePoint &tp) const -> Duration { return _rep - tp._rep; } // NOLINT

    [[nodiscard]] auto time_since_epoch() const -> Duration { return _rep; }
};

template <uint32_t Period, std::integral Rep>
auto operator+(const Duration<Period, Rep> &lhs, const TimePoint &rhs) -> TimePoint {
    return rhs + lhs;
}

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

    constexpr explicit operator int() const noexcept { return _rep; }

    auto operator<=>(const Year &) const = default;

    auto operator+() const noexcept -> Year { return *this; }

    auto operator-() const noexcept -> Year { return Year{-_rep}; }

    auto operator+=(const Years &yrs) -> Year & {
        _rep += static_cast<Rep>(yrs.count());
        return *this;
    }

    auto operator-=(const Years &yrs) -> Year & {
        _rep -= static_cast<Rep>(yrs.count());
        return *this;
    }

    auto operator++() -> Year & {
        ++_rep;
        return *this;
    }

    auto operator++(int) -> Year {
        auto temp = *this;
        ++*this;
        return temp;
    }

    auto operator--() -> Year & {
        --_rep;
        return *this;
    }

    auto operator--(int) -> Year {
        auto temp = *this;
        --*this;
        return temp;
    }

    auto operator+(const Years &yrs) const -> Year {
        auto temp = *this;
        temp += yrs;
        return temp;
    }

    auto operator-(const Years &yrs) const -> Year {
        auto temp = *this;
        temp -= yrs;
        return temp;
    }

    auto operator-(const Year &year) const -> Years { return Years{_rep - year._rep}; }

    [[nodiscard]] constexpr auto is_leap() const noexcept -> bool {
        if (_rep % 400 == 0) { // NOLINT
            return true;
        }
        if (_rep % 100 == 0) { // NOLINT
            return false;
        }
        return _rep % 4 == 0;
    }

    [[nodiscard]] auto ok() const noexcept -> bool {
        return _rep != std::numeric_limits<Rep>::min();
    }
};

inline auto operator+(const Years &lhs, const Year &rhs) -> Year { return rhs + lhs; }

class Month {
    // 1 <= _rep <= 12. Not strong invariant
    uint8_t _rep;

  public:
    Month(const Month &) = default;
    Month(Month &&) = default;
    auto operator=(const Month &) -> Month & = default;
    auto operator=(Month &&) -> Month & = default;
    explicit Month(uint32_t rep) : _rep(rep) {} // NOLINT
    ~Month() = default;

    auto operator<=>(const Month &) const = default;

    explicit constexpr operator unsigned() const { return _rep; }

    auto operator++() -> Month & {
        ++_rep;
        return *this;
    }

    auto operator++(int) -> Month {
        auto temp = *this;
        ++*this;
        return temp;
    }

    auto operator--() -> Month & {
        --_rep;
        return *this;
    }

    auto operator--(int) -> Month {
        auto temp = *this;
        --*this;
        return temp;
    }

    auto operator+=(const Months &months) -> Month & {
        _rep += months.count();
        return *this;
    }

    auto operator-=(const Months &months) -> Month & {
        _rep -= months.count();
        return *this;
    }

    auto operator+(const Months &months) const -> Month {
        auto temp = *this;
        temp += months;
        return temp;
    }

    auto operator-(const Months &months) const -> Month {
        auto temp = *this;
        temp -= months;
        return temp;
    }

    auto operator-(const Month &month) const -> Months { return Months{_rep - month._rep}; }

    [[nodiscard]] auto ok() const -> bool {
        return _rep >= 1 && _rep <= 12; // NOLINT
    }
};

inline auto operator+(const Months &lhs, const Month &rhs) -> Month { return rhs + lhs; }

class Day {
    // 1 <= _rep <= 31. Not strong invariant
    uint8_t _rep;

  public:
    Day(const Day &) = default;
    Day(Day &&) = default;
    auto operator=(const Day &) -> Day & = default;
    auto operator=(Day &&) -> Day & = default;
    explicit Day(uint32_t rep) : _rep(rep) {} // NOLINT
    ~Day() = default;

    auto operator<=>(const Day &) const = default;

    explicit constexpr operator unsigned() const { return _rep; }

    auto operator++() -> Day & {
        ++_rep;
        return *this;
    }

    auto operator++(int) -> Day {
        auto temp = *this;
        ++*this;
        return temp;
    }

    auto operator--() -> Day & {
        --_rep;
        return *this;
    }

    auto operator--(int) -> Day {
        auto temp = *this;
        --*this;
        return temp;
    }

    auto operator+=(const Days &days) -> Day & {
        _rep += days.count();
        return *this;
    }

    auto operator-=(const Days &days) -> Day & {
        _rep -= days.count();
        return *this;
    }

    auto operator+(const Days &days) const -> Day {
        auto temp = *this;
        temp += days;
        return temp;
    }

    auto operator-(const Days &days) const -> Day {
        auto temp = *this;
        temp -= days;
        return temp;
    }

    auto operator-(const Day &day) const -> Days { return Days{_rep - day._rep}; }

    [[nodiscard]] auto ok() const -> bool {
        return _rep >= 1 && _rep <= 31; // NOLINT
    }
};

inline auto operator+(const Days &lhs, const Day &rhs) -> Day { return rhs + lhs; }

enum class Weekday : int8_t {
    Monday = 0,
    Tuesday = 1,
    Wednesday = 2,
    Thursday = 3,
    Friday = 4,
    Saturday = 5,
    Sunday = 6
};

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
#pragma clang coverage off
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
#pragma clang coverage on
} // namespace Lines::Temporal
