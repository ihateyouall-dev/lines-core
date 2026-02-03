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

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace Lines {
struct TaskInfo {
    TaskInfo() = default;
    TaskInfo(const TaskInfo &) = default; // LCOV_EXCL_LINE
    TaskInfo(TaskInfo &&) = default;
    auto operator=(const TaskInfo &) -> TaskInfo & = default;
    auto operator=(TaskInfo &&) -> TaskInfo & = default;
    explicit TaskInfo(std::string title, std::optional<std::string> desc = std::nullopt,
                      std::vector<std::string> tags = {})
        : title(std::move(title)), description(std::move(desc)), tags(std::move(tags)) {
        if (this->title.empty()) {
            throw std::invalid_argument("TaskInfo: title must not be empty"); // LCOV_EXCL_LINE
        }
    }
    ~TaskInfo() = default;
    std::string title;
    std::optional<std::string> description;
    std::vector<std::string> tags;
};
} // namespace Lines
