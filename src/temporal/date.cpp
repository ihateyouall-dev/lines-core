#include <lines/temporal/date.hpp>

Lines::Temporal::Date::Date(const Year &year, const Month &month, const Day &day)
    : _rep(Days{std::chrono::sys_days{
          std::chrono::year{int(year)} / std::chrono::month{unsigned(month)} /
          std::chrono::day{unsigned(
              day)}}.time_since_epoch()}),
      _ymd(update_ymd()) {
    if (!_ymd.ok()) {
        throw std::invalid_argument("Date::Date: invalid arguments for constructor");
    }
}
auto Lines::Temporal::Date::update_ymd() const -> std::chrono::year_month_day {
    return std::chrono::sys_days(std::chrono::days{_rep.count()});
}

Lines::Temporal::Date::Date(Days rep) : _rep(rep), _ymd(update_ymd()) {}
auto Lines::Temporal::Date::operator<=>(const Date &date) const { return _rep <=> date._rep; }

auto Lines::Temporal::Date::operator==(const Date &date) const -> bool {
    return (*this <=> date) == 0;
}

auto Lines::Temporal::Date::operator-(const Date &date) const -> Days { return _rep - date._rep; }

auto Lines::Temporal::Date::operator++() -> Date & {
    ++_rep;
    _ymd = update_ymd();
    return *this;
}

auto Lines::Temporal::Date::operator++(int) -> Date {
    Date tmp = *this;
    ++*this;
    return tmp;
}

auto Lines::Temporal::Date::operator--() -> Date & {
    --_rep;
    _ymd = update_ymd();
    return *this;
}

auto Lines::Temporal::Date::operator--(int) -> Date {
    Date tmp = *this;
    --*this;
    return tmp;
}

auto Lines::Temporal::Date::time_since_epoch() const -> Days { return _rep; }

auto Lines::Temporal::Date::year() const -> Year { return Year{int(_ymd.year())}; }

auto Lines::Temporal::Date::month() const -> Month { return Month{unsigned(_ymd.month())}; }

auto Lines::Temporal::Date::day() const -> Day { return Day{unsigned(_ymd.day())}; }

auto Lines::Temporal::Date::weekday() const -> Weekday {
    return static_cast<Weekday>(((_rep + Days{3}) % 7).count());
}

auto Lines::Temporal::Date::yyyy_mm_dd() const -> std::string {
    return std::format("{}-{:02}-{:02}", int(year()), unsigned(month()), unsigned(day()));
}
