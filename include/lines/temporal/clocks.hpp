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
#include "lines/temporal/timepoint.hpp"
#include "lines/temporal/timestamp.hpp"
#include "lines/temporal/timezone.hpp"

#include <chrono>

namespace Lines::Temporal {
struct LINES_API UTCClock final {
    LINES_API static auto now() LINES_NOEXCEPT->TimePoint {
        auto now = std::chrono::system_clock::now();
        return TimePoint(Seconds{now.time_since_epoch()});
    }

    LINES_API static auto since_midnight() -> Timestamp {
        return Timestamp(now().time_since_epoch());
    }

    LINES_API static auto today() LINES_NOEXCEPT->Date {
        auto days = floor<Days>(now().time_since_epoch());
        return Date(Days{days});
    }
};

struct LINES_API LocalClock final {
    LINES_API static auto now() LINES_NOEXCEPT->TimePoint {
        auto now = UTCClock::now();
        return ZonedTime(now, current_zone()).get_local_time();
    }

    LINES_API static auto since_midnight() -> Timestamp {
        return Timestamp(now().time_since_epoch());
    }

    LINES_API static auto today() LINES_NOEXCEPT->Date {
        auto days = floor<Days>(now().time_since_epoch());
        return Date(days);
    }

    LINES_API LINES_NODISCARD static auto current_zone() -> TimeZone {
        std::time_t now = std::time(nullptr);
        std::tm local{};

#if defined(LINES_WINDOWSNT)
        localtime_s(&local, &now);
        long offset = _timezone;
        if (local.tm_isdst) {
            offset -= 3600;
        }
        return TimeZone(Seconds{-offset});
#else
        localtime_r(&now, &local);
        return TimeZone(Seconds{local.tm_gmtoff});
#endif
    }
};
} // namespace Lines::Temporal
