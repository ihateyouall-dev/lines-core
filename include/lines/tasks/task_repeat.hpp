/*
  #        #  #     #  # # # #  # # # #
  #        #  # #   #  #        #
  #        #  #   # #  # # # #  # # # #
  #        #  #     #  #              #
  # # # #  #  #     #  # # # #  # # # #
  Copyright (c) 2025-2026 I.H.Y.A.D.

  Lines Project, Core library.
  This file is licensed under GNU Lesser General Public License v3.0 or later.
  See LICENSE for more information.
  SPDX-License-Identifier: LGPL-3.0-or-later.
*/
#pragma once

#include "lines/detail/macro.h"
#include <lines/temporal/date.hpp>
#include <optional>
#include <variant>

namespace Lines {
struct LINES_API EveryUnit {
    Temporal::Minutes interval;
};

struct LINES_API EveryWeekday {
    std::vector<Temporal::Weekday> weekdays;
};

struct LINES_API Once {
    Temporal::Date date;
};

struct LINES_API TaskRepeatRule {
    using RepeatType = std::variant<EveryUnit, EveryWeekday, Once>;
    RepeatType repeat_type;
    std::optional<Temporal::Date> end;

    LINES_NODISCARD auto next_date(const Temporal::Date &completed_at) const
        -> std::optional<Temporal::Date> {
        return std::visit(
            [&](auto &&v) -> std::optional<Temporal::Date> {
                using T = std::decay_t<decltype(v)>;
                LINES_CONSTEXPR_IF(std::is_same_v<T, EveryUnit>) {
                    const Temporal::Date date = completed_at + v.interval;
                    if (!end.has_value() || date <= end.value()) {
                        return date;
                    }
                    return std::nullopt;
                }
                LINES_CONSTEXPR_IF(std::is_same_v<T, EveryWeekday>) {
                    Temporal::Date tomorrow = completed_at + Temporal::Days{1};
                    while (std::ranges::find(v.weekdays, tomorrow.weekday()) == v.weekdays.end()) {
                        tomorrow += Temporal::Days{1};
                    }
                    if (!end.has_value() || tomorrow <= end.value()) {
                        return tomorrow;
                    }
                    return std::nullopt;
                }
                LINES_CONSTEXPR_IF(std::is_same_v<T, Once>) {
                    if (v.date <= completed_at) {
                        return std::nullopt;
                    }
                    return v.date;
                }
                return std::nullopt;
            },
            repeat_type);
    }
};
} // namespace Lines
