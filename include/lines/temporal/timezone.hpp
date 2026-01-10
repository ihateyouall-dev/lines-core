#pragma once

#include <lines/temporal/duration.hpp>
#include <lines/temporal/timepoint.hpp>

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
} // namespace Lines::Temporal
