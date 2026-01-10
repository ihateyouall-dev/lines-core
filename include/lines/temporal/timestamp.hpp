#pragma once

#include <lines/temporal/duration.hpp>

namespace Lines::Temporal {
class Timestamp {
    // 0 <= _rep < 24h
    Seconds _rep{0};

    void normalize();

  public:
    Timestamp() = default;
    explicit Timestamp(const Seconds &rep);
    Timestamp(const Hours &hours, const Minutes &minutes, const Seconds &seconds);

    [[nodiscard]] auto time_since_midnight() const -> Seconds { return _rep; }

    auto operator<=>(const Timestamp &) const = default;

    auto operator++() -> Timestamp &;

    auto operator++(int) -> Timestamp;

    auto operator--() -> Timestamp &;

    auto operator--(int) -> Timestamp;

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

    auto operator-(const Timestamp &time) const -> decltype(_rep);

    template <uint32_t Period, std::integral Rep>
    auto operator-=(const Duration<Period, Rep> &dur) -> Timestamp & {
        _rep -= dur;
        normalize();
        return *this;
    }

    [[nodiscard]] auto hh_mm_ss() const -> std::string;

    [[nodiscard]] auto hours() const -> Hours;

    [[nodiscard]] auto minutes() const -> Minutes;

    [[nodiscard]] auto seconds() const -> Seconds;
};

template <uint32_t Period, std::integral Rep>
auto operator+(const Duration<Period, Rep> &dur, const Timestamp &time) -> Timestamp {
    return time + dur;
}
} // namespace Lines::Temporal
