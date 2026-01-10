#pragma once

#include "lines/detail/macro.h"
#include <chrono>
#include <concepts>
#include <cstdint>
#include <string>

namespace Lines::Temporal {
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
