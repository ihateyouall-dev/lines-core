#pragma once

#include <lines/temporal/date.hpp>
#include <tuple>

namespace Lines {
template <typename R>
concept TaskVisibilityRule = requires(const R &r, const Lines::Temporal::Date &d) {
    { r(d) } -> std::same_as<bool>;
    { r.args() } -> std::same_as<decltype(r.args())>;
    { R::tag() } -> std::same_as<std::string_view>;
};

namespace TaskVisibility {
struct Always {
    static auto args() -> std::tuple<> { return {}; }
    auto operator()(const Lines::Temporal::Date &date) const -> bool { return true; } // NOLINT
    static constexpr auto tag() -> std::string_view { return "always"; }
};

struct Never {
    static auto args() -> std::tuple<> { return {}; }
    auto operator()(const Lines::Temporal::Date &date) const -> bool { return false; } // NOLINT
    static constexpr auto tag() -> std::string_view { return "never"; }
};

struct From {
    const Lines::Temporal::Date _from; // NOLINT

    explicit From(Lines::Temporal::Date from) : _from(from) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Date> { return std::tie(_from); }
    auto operator()(const Lines::Temporal::Date &date) const -> bool { return date >= _from; }
    static constexpr auto tag() -> std::string_view { return "from"; }
};

struct Until {
    const Lines::Temporal::Date _until; // NOLINT

    explicit Until(Lines::Temporal::Date until) : _until(until) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Date> {
        return std::tie(_until);
    }
    auto operator()(const Lines::Temporal::Date &date) const -> bool { return date <= _until; }
    static constexpr auto tag() -> std::string_view { return "until"; }
};

struct Between {
    const Lines::Temporal::Date _from;  // NOLINT
    const Lines::Temporal::Date _until; // NOLINT

    Between(Lines::Temporal::Date from, Lines::Temporal::Date until) // NOLINT
        : _from(from), _until(until) {
        if (_from > _until) {
            throw std::invalid_argument("TaskVisibility::Between: from > until");
        }
    }

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Date, Lines::Temporal::Date> {
        return std::tie(_from, _until);
    }

    auto operator()(const Lines::Temporal::Date &date) const -> bool {
        return date >= _from && date <= _until;
    }

    static constexpr auto tag() -> std::string_view { return "between"; }
};

struct Weekdays {
    static auto args() -> std::tuple<> { return {}; }
    auto operator()(const Lines::Temporal::Date &date) const -> bool {
        auto weekday = date.weekday();
        return weekday >= Lines::Temporal::Weekday::Monday &&
               weekday <= Lines::Temporal::Weekday::Friday;
    }
    static constexpr auto tag() -> std::string_view { return "weekdays"; }
};

struct Weekends {
    static auto args() -> std::tuple<> { return {}; }
    auto operator()(const Lines::Temporal::Date &date) const -> bool {
        auto weekday = date.weekday();
        return weekday == Lines::Temporal::Weekday::Saturday ||
               weekday == Lines::Temporal::Weekday::Sunday;
    }
    static constexpr auto tag() -> std::string_view { return "weekends"; }
};

struct Once {
    const Lines::Temporal::Date _target; // NOLINT

    explicit Once(Lines::Temporal::Date target) : _target(target) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Date> {
        return std::tie(_target);
    }
    auto operator()(const Lines::Temporal::Date &date) const -> bool { return date == _target; }
    static constexpr auto tag() -> std::string_view { return "once"; }
};

struct EveryMonthDay {
    const Lines::Temporal::Day _day; // NOLINT

    explicit EveryMonthDay(Lines::Temporal::Day day) : _day(day) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Day> { return std::tie(_day); }
    auto operator()(const Lines::Temporal::Date &date) const -> bool { return date.day() == _day; }
    static constexpr auto tag() -> std::string_view { return "every_month_day"; }
};

struct EveryNthDay {
    const uint _n;                      // NOLINT
    const Lines::Temporal::Date _start; // NOLINT

    EveryNthDay(uint n, Lines::Temporal::Date start) : _n(n), _start(start) {
        if (_n == 0) {
            throw std::invalid_argument("TaskVisibility::EveryNthDay: n must not be 0");
        }
    }

    [[nodiscard]] auto args() const -> std::tuple<uint, Lines::Temporal::Date> {
        return std::tie(_n, _start);
    }
    auto operator()(const Lines::Temporal::Date &date) const -> bool {
        if (date < _start) {
            return false;
        }
        return (date - _start).count() % _n == 0;
    }
    static constexpr auto tag() -> std::string_view { return "every_nth_day"; }
};

struct EveryWeekday {
    Lines::Temporal::Weekday _weekday;

    explicit EveryWeekday(Lines::Temporal::Weekday weekday) : _weekday(weekday) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Weekday> {
        return std::tie(_weekday);
    }
    auto operator()(const Lines::Temporal::Date &date) const -> bool {
        return date.weekday() == _weekday;
    }
    static constexpr auto tag() -> std::string_view { return "every_weekday"; }
};
} // namespace TaskVisibility
} // namespace Lines
