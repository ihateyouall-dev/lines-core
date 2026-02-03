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
#include <gtest/gtest.h>
#include <lines/tasks/task_info.hpp>

using namespace Lines;

TEST(TaskInfo, Construction) {
    TaskInfo info("title");
    EXPECT_EQ(info.title, "title");
    EXPECT_FALSE(info.description.has_value());
    EXPECT_TRUE(info.tags.empty());

    TaskInfo copy(info);
    copy.description = "desc";
    copy.tags = {"first", "seconds"};
    EXPECT_EQ(copy.title, "title");
    EXPECT_TRUE(copy.description.has_value());
    EXPECT_FALSE(copy.tags.empty());
}

TEST(TaskInfo, ThrowsOnEmptyTitle) { EXPECT_THROW(TaskInfo(""), std::invalid_argument); }
