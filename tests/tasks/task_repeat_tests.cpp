#include "lines/temporal/duration.hpp"
#include <gtest/gtest.h>
#include <lines/tasks/task_repeat.hpp>

using namespace Lines;

TEST(TaskVisibility, EveryUnit) {
    TaskRepeatRule rule{.repeat_type =
                            EveryUnit{duration_cast<Temporal::Minutes>(Temporal::Days{1})}};

    EXPECT_EQ(rule.next_date(Temporal::Date{Temporal::Days{1}}), Temporal::Date{Temporal::Days{2}});
}
