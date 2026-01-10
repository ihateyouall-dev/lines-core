#include <lines/temporal/timepoint.hpp>

Lines::Temporal::TimePoint::TimePoint(const Duration &rep) : _rep(rep) {}

auto Lines::Temporal::TimePoint::operator++() -> TimePoint & {
    ++_rep;
    return *this;
}

auto Lines::Temporal::TimePoint::operator++(int) -> TimePoint {
    auto temp = *this;
    ++*this;
    return temp;
}

auto Lines::Temporal::TimePoint::operator--() -> TimePoint & {
    --_rep;
    return *this;
}

auto Lines::Temporal::TimePoint::operator--(int) -> TimePoint {
    auto temp = *this;
    --*this;
    return temp;
}

auto Lines::Temporal::TimePoint::operator-(const TimePoint &tp) const -> Duration {
    return _rep - tp._rep;
}

auto Lines::Temporal::TimePoint::time_since_epoch() const -> Duration { return _rep; }
