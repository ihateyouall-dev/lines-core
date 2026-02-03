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
#include <lines/tasks/task_completion.hpp>

using namespace Lines;

TEST(TaskCompletion, States) {
    TaskCompletion c;
    c.complete();
    EXPECT_TRUE(c.completed());
    EXPECT_FALSE(c.skipped());
    EXPECT_TRUE(c.finished());
    EXPECT_EQ(c.state(), TaskCompletion::State::Completed);
    c.skip();
    EXPECT_FALSE(c.completed());
    EXPECT_TRUE(c.skipped());
    EXPECT_TRUE(c.finished());
    EXPECT_EQ(c.state(), TaskCompletion::State::Skipped);
    c.reset();
    EXPECT_FALSE(c.finished());
    EXPECT_FALSE(c.completed());
    EXPECT_FALSE(c.skipped());
    EXPECT_EQ(c.state(), TaskCompletion::State::NotCompleted);
}
