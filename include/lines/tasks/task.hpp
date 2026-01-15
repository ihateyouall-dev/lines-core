#pragma once

#include <lines/tasks/task_completion.hpp>
#include <lines/tasks/task_info.hpp>
#include <lines/tasks/task_repeat.hpp>

namespace Lines {
class Task {
    TaskInfo _info;
    std::unique_ptr<TaskCompletion> _completion;
    TaskRepeatRule _rule;
    explicit Task(TaskInfo info, std::unique_ptr<TaskCompletion> completion, TaskRepeatRule rule);

  public:
    friend class TaskFactory;
    Task(const Task &task);
    auto operator=(const Task &task) -> Task &;
    Task(Task &&) = default;
    auto operator=(Task &&) -> Task & = default;
    ~Task() = default;

    auto completion() -> TaskCompletion &;
    [[nodiscard]] auto completion() const -> const TaskCompletion &;

    void set_title(const std::string &title);
    void set_description(const std::string &description);
    void set_tags(std::vector<std::string> tags);

    [[nodiscard]] auto title() const -> const std::string &;
    [[nodiscard]] auto description() const -> const std::optional<std::string> &;
    [[nodiscard]] auto tags() const -> const std::vector<std::string> &;
};

class TaskFactory final {
    TaskInfo _info;
    std::unique_ptr<TaskCompletion> _completion = std::make_unique<TaskCompletion>();
    TaskRepeatRule _rule =
        TaskRepeatRule(EveryUnit{duration_cast<Temporal::Minutes>(Temporal::Days{1})});

  public:
    TaskFactory() = default;
    TaskFactory(const TaskFactory &) = delete;
    TaskFactory(TaskFactory &&) = default;
    auto operator=(const TaskFactory &) = delete;
    auto operator=(TaskFactory &&) -> TaskFactory & = default;
    ~TaskFactory() = default;
    auto info(const TaskInfo &info) && -> TaskFactory;

    auto completion(const TaskCompletion &completion) && -> TaskFactory;

    auto repeat(const TaskRepeatRule &rule) && -> TaskFactory;

    auto task() && -> Task;
};

inline auto make_task() -> TaskFactory { return {}; }
} // namespace Lines
