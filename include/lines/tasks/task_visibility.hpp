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

class ITaskVisibilityRule {
  public:
    ITaskVisibilityRule() = default;
    ITaskVisibilityRule(const ITaskVisibilityRule &) = default;
    ITaskVisibilityRule(ITaskVisibilityRule &&) = default;
    auto operator=(const ITaskVisibilityRule &) -> ITaskVisibilityRule & = default;
    auto operator=(ITaskVisibilityRule &&) -> ITaskVisibilityRule & = default;
    virtual auto operator()(const Lines::Temporal::Date &date) const -> bool = 0;
    [[nodiscard]] virtual auto tag() const -> std::string_view = 0;
    virtual ~ITaskVisibilityRule() = default;
    [[nodiscard]] virtual auto clone() const -> std::unique_ptr<ITaskVisibilityRule> = 0;
};

namespace TaskVisibility {
class Always : public ITaskVisibilityRule {
  public:
    static auto args() -> std::tuple<> { return {}; }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override { // NOLINT
        return true;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "always"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<Always>(*this);
    }
};

class Never : public ITaskVisibilityRule {
  public:
    static auto args() -> std::tuple<> { return {}; }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override { // NOLINT
        return false;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "never"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<Never>(*this);
    }
};

class From : public ITaskVisibilityRule {
    const Lines::Temporal::Date _from; // NOLINT

  public:
    explicit From(Lines::Temporal::Date from) : _from(from) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Date> { return std::tie(_from); }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override {
        return date >= _from;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "from"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<From>(*this);
    }
};

class Until : public ITaskVisibilityRule {
    const Lines::Temporal::Date _until; // NOLINT

  public:
    explicit Until(Lines::Temporal::Date until) : _until(until) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Date> {
        return std::tie(_until);
    }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override {
        return date <= _until;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "until"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<Until>(*this);
    }
};

class Between : public ITaskVisibilityRule {
    const Lines::Temporal::Date _from;  // NOLINT
    const Lines::Temporal::Date _until; // NOLINT

  public:
    Between(Lines::Temporal::Date from, Lines::Temporal::Date until) // NOLINT
        : _from(from), _until(until) {
        if (_from > _until) {
            throw std::invalid_argument("TaskVisibility::Between: from > until");
        }
    }

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Date, Lines::Temporal::Date> {
        return std::tie(_from, _until);
    }

    auto operator()(const Lines::Temporal::Date &date) const -> bool override {
        return date >= _from && date <= _until;
    }

    [[nodiscard]] auto tag() const -> std::string_view override { return "between"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<Between>(*this);
    }
};

class Weekdays : public ITaskVisibilityRule {
  public:
    static auto args() -> std::tuple<> { return {}; }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override {
        auto weekday = date.weekday();
        return weekday >= Lines::Temporal::Weekday::Monday &&
               weekday <= Lines::Temporal::Weekday::Friday;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "weekdays"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<Weekdays>(*this);
    }
};

class Weekends : public ITaskVisibilityRule {
  public:
    static auto args() -> std::tuple<> { return {}; }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override {
        auto weekday = date.weekday();
        return weekday == Lines::Temporal::Weekday::Saturday ||
               weekday == Lines::Temporal::Weekday::Sunday;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "weekends"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<Weekends>(*this);
    }
};

class Once : public ITaskVisibilityRule {
    const Lines::Temporal::Date _target; // NOLINT

  public:
    explicit Once(Lines::Temporal::Date target) : _target(target) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Date> {
        return std::tie(_target);
    }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override {
        return date == _target;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "once"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<Once>(*this);
    }
};

class EveryMonthDay : public ITaskVisibilityRule {
    const Lines::Temporal::Day _day; // NOLINT

  public:
    explicit EveryMonthDay(Lines::Temporal::Day day) : _day(day) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Day> { return std::tie(_day); }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override {
        return date.day() == _day;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "every_month_day"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<EveryMonthDay>(*this);
    }
};

class EveryNthDay : public ITaskVisibilityRule {
    const uint _n;                      // NOLINT
    const Lines::Temporal::Date _start; // NOLINT

  public:
    EveryNthDay(uint n, Lines::Temporal::Date start) : _n(n), _start(start) {
        if (_n == 0) {
            throw std::invalid_argument("TaskVisibility::EveryNthDay: n must not be 0");
        }
    }

    [[nodiscard]] auto args() const -> std::tuple<uint, Lines::Temporal::Date> {
        return std::tie(_n, _start);
    }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override {
        if (date < _start) {
            return false;
        }
        return (date - _start).count() % _n == 0;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "every_nth_day"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<EveryNthDay>(*this);
    }
};

class EveryWeekday : public ITaskVisibilityRule {
    Lines::Temporal::Weekday _weekday;

  public:
    explicit EveryWeekday(Lines::Temporal::Weekday weekday) : _weekday(weekday) {}

    [[nodiscard]] auto args() const -> std::tuple<Lines::Temporal::Weekday> {
        return std::tie(_weekday);
    }
    auto operator()(const Lines::Temporal::Date &date) const -> bool override {
        return date.weekday() == _weekday;
    }
    [[nodiscard]] auto tag() const -> std::string_view override { return "every_weekday"; }

    [[nodiscard]] auto clone() const -> std::unique_ptr<ITaskVisibilityRule> override {
        return std::make_unique<EveryWeekday>(*this);
    }
};
} // namespace TaskVisibility
} // namespace Lines
