#pragma once

#include <lines/tasks/task_completion.hpp>
#include <lines/tasks/task_info.hpp>
#include <lines/tasks/task_repeat.hpp>

namespace Lines {
class Task {
    TaskInfo _info;
    TaskCompletion _completion;
    TaskRepeatRule _rule;

  public:
    explicit Task(TaskInfo info, TaskRepeatRule rule = {});
    Task(const Task &task) = default;
    auto operator=(const Task &task) -> Task & = default;
    Task(Task &&) = default;
    auto operator=(Task &&) -> Task & = default;
    ~Task() = default;

    auto completion() -> TaskCompletion &;
    [[nodiscard]] auto completion() const -> const TaskCompletion &;

    void set_title(const std::string &title);
    void set_description(const std::string &description);
    void set_tags(std::vector<std::string> tags);
    void set_repeat_rule(const TaskRepeatRule &rule);

    [[nodiscard]] auto title() const -> const std::string &;
    [[nodiscard]] auto description() const -> const std::optional<std::string> &;
    [[nodiscard]] auto tags() const -> const std::vector<std::string> &;
    [[nodiscard]] auto next_date(const Temporal::Date &completed_at) const
        -> std::optional<Temporal::Date>;
};
} // namespace Lines
