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
#include "lines/temporal/date.hpp"
#include "lines/temporal/datetime.hpp"
#include "lines/temporal/timepoint.hpp"

#include <optional>
#include <variant>

namespace Lines {
namespace TaskRepeat {
struct LINES_API EveryUnit {
    Temporal::Seconds interval;
    std::string unit_str;
};

struct LINES_API EveryWeekday {
    std::vector<Temporal::Weekday> weekdays;
};
} // namespace TaskRepeat

struct LINES_API TaskRepeatRule {
    using RepeatType = std::variant<TaskRepeat::EveryUnit, TaskRepeat::EveryWeekday>;
    RepeatType repeat_type;
    std::optional<Temporal::TimePoint> end;

    LINES_NODISCARD auto next_deadline(const Temporal::TimePoint &completed_at) const
        -> std::optional<Temporal::TimePoint> {
        return std::visit(
            [&](auto &&v) -> std::optional<Temporal::TimePoint> {
                using T = std::decay_t<decltype(v)>;
                LINES_CONSTEXPR_IF(std::is_same_v<T, TaskRepeat::EveryUnit>) {
                    const Temporal::TimePoint res = completed_at + v.interval;
                    if (!end || res <= *end) {
                        return res;
                    }
                    return std::nullopt;
                }
                LINES_CONSTEXPR_IF(std::is_same_v<T, TaskRepeat::EveryWeekday>) {
                    Temporal::TimePoint res = completed_at + Temporal::Days{1};
                    while (
                        std::ranges::find(v.weekdays, Temporal::DateTime(res).date().weekday()) ==
                        v.weekdays.end()) {
                        res += Temporal::Days{1};
                    }
                    if (!end || res <= *end) {
                        return res;
                    }
                    return std::nullopt;
                }
                return std::nullopt;
            },
            repeat_type);
    }
};
} // namespace Lines
