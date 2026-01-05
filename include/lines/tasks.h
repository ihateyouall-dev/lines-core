#pragma once

#include "detail/macro.h"
#include "temporal.h"
#include <algorithm>        // std::ranges::all_of
#include <initializer_list> // std::initializer_list
#include <optional>         // std::optional
#include <stdexcept>        // std::out_of_range, std::invalid_argument
#include <string>           // std::string
#include <utility>          // std::move
#include <vector>           // std::vector

using uint = unsigned int;

namespace Lines {
struct TaskInfo {
    TaskInfo() = default;
    TaskInfo(const TaskInfo &) = default;
    TaskInfo(TaskInfo &&) = default;
    auto operator=(const TaskInfo &) -> TaskInfo & = default;
    auto operator=(TaskInfo &&) -> TaskInfo & = default;
    explicit TaskInfo(std::string title, std::optional<std::string> desc = std::nullopt,
                      std::vector<std::string> categories = {})
        : title(std::move(title)), description(std::move(desc)), categories(std::move(categories)) {
        if (this->title.empty()) {
            throw std::invalid_argument("TaskInfo: title must not be empty");
        }
    }
    ~TaskInfo() = default;
    std::string title;
    std::optional<std::string> description;
    std::vector<std::string> categories;
};

class TaskCompletion {
  public:
    TaskCompletion() = default;
    TaskCompletion(const TaskCompletion &) = default;
    TaskCompletion(TaskCompletion &&) = default;
    auto operator=(const TaskCompletion &) -> TaskCompletion & = default;
    auto operator=(TaskCompletion &&) -> TaskCompletion & = default;
    virtual ~TaskCompletion() = default;

    virtual void complete() noexcept = 0;
    [[nodiscard]] virtual auto completed() const noexcept -> bool = 0;
    virtual void reset() noexcept = 0;

    [[nodiscard]] virtual auto clone() const -> std::unique_ptr<TaskCompletion> = 0;
};

class BinaryTaskCompletion : public TaskCompletion {
    bool _completed = false;

  public:
    BinaryTaskCompletion(const BinaryTaskCompletion &) = default;
    BinaryTaskCompletion(BinaryTaskCompletion &&) = default;
    auto operator=(const BinaryTaskCompletion &) -> BinaryTaskCompletion & = default;
    auto operator=(BinaryTaskCompletion &&) -> BinaryTaskCompletion & = default;
    ~BinaryTaskCompletion() override = default;

    void complete() noexcept override { _completed = true; }
    [[nodiscard]] auto completed() const noexcept -> bool override { return _completed; }
    void reset() noexcept override { _completed = false; }
    [[nodiscard]] auto clone() const -> std::unique_ptr<TaskCompletion> override {
        return std::make_unique<BinaryTaskCompletion>(*this);
    }
};

class ProgressTaskCompletion : public TaskCompletion {
    uint _min;
    uint _max;
    uint _current = 0;

  public:
    ProgressTaskCompletion() = delete;
    ProgressTaskCompletion(uint min, uint max) : _min(min), _max(max) {} // NOLINT
    ProgressTaskCompletion(const ProgressTaskCompletion &) = default;
    ProgressTaskCompletion(ProgressTaskCompletion &&) = default;
    auto operator=(const ProgressTaskCompletion &) -> ProgressTaskCompletion & = default;
    auto operator=(ProgressTaskCompletion &&) -> ProgressTaskCompletion & = default;
    ~ProgressTaskCompletion() override = default;

    void complete() noexcept override { _current = _max; }
    [[nodiscard]] auto completed() const noexcept -> bool override { return _current == _max; }
    void reset() noexcept override { _current = 0; }
    [[nodiscard]] auto clone() const -> std::unique_ptr<TaskCompletion> override {
        return std::make_unique<ProgressTaskCompletion>(*this);
    }

    [[nodiscard]] auto min() const noexcept -> uint { return _min; }
    [[nodiscard]] auto max() const noexcept -> uint { return _max; }
    [[nodiscard]] auto current() const noexcept -> uint { return _current; }

    void set_current(uint current) { _current = current; }
};

class TaskVisibility {
    using Pred = std::function<bool(const Temporal::Date &, const std::optional<Temporal::Date> &)>;
    Pred _predicate;
    std::optional<Temporal::Date> _starting_from;

  public:
    TaskVisibility(const TaskVisibility &) = default;
    TaskVisibility(TaskVisibility &&) = default;
    auto operator=(const TaskVisibility &) -> TaskVisibility & = default;
    auto operator=(TaskVisibility &&) -> TaskVisibility & = default;
    explicit TaskVisibility(Pred predicate,
                            const std::optional<Temporal::Date> &starting_from = std::nullopt)
        : _predicate(std::move(predicate)), _starting_from(starting_from) {}
    ~TaskVisibility() = default;

    [[nodiscard]] auto visible(const Temporal::Date &date) const noexcept -> bool {
        return _predicate(date, _starting_from);
    }
};

namespace Visibility {
inline auto always() {
    return TaskVisibility([](auto, auto) { return true; }, std::nullopt);
}
}; // namespace Visibility

class Task {
    TaskInfo _info;
    std::unique_ptr<TaskCompletion> _completion;
    TaskVisibility _visibility;

  public:
    Task(const Task &task) : _info(task._info), _visibility(task._visibility) {
        _completion = task._completion->clone();
    }
    auto operator=(const Task &task) -> Task & {
        if (this == &task) {
            return *this;
        }

        _info = task._info;
        _completion = task._completion->clone();
        _visibility = task._visibility;
        return *this;
    };
    Task(TaskInfo info, std::unique_ptr<TaskCompletion> completion, TaskVisibility visibility)
        : _info(std::move(info)), _completion(std::move(completion)),
          _visibility(std::move(visibility)) {}
    Task(Task &&) = default;
    auto operator=(Task &&) -> Task & = default;
    ~Task() = default;
};

class TaskList {
    std::vector<Task> _tasks;

  public:
    TaskList(const TaskList &) = default;
    TaskList(TaskList &&) = default;
    TaskList(std::initializer_list<Task> &tasks) : _tasks(tasks) {}
    auto operator=(const TaskList &) -> TaskList & = default;
    auto operator=(TaskList &&) -> TaskList & = default;
    auto operator=(const std::initializer_list<Task> &tasks) -> TaskList & {
        _tasks = tasks;
        return *this;
    }
    explicit TaskList(std::vector<Task> tasks) : _tasks(std::move(tasks)) {}
    TaskList() = default;
    ~TaskList() = default;

    [[nodiscard]] auto completed() const -> bool {
        return std::ranges::all_of(_tasks, [](const auto &) { return true; });
    }

    auto operator[](const std::size_t index) -> Task & { return _tasks[index]; }
    auto operator[](const std::size_t index) const -> const Task & { return _tasks[index]; }
    auto at(const std::size_t index) -> Task & { return _tasks.at(index); }
    [[nodiscard]] auto at(const std::size_t index) const -> const Task & {
        return _tasks.at(index);
    }

    void erase(const std::size_t index) {
        if (index >= _tasks.size()) {
            throw std::out_of_range("TaskList::erase: index out of range");
        }
        auto iterator = _tasks.begin();
        std::advance(iterator, index);
        _tasks.erase(iterator);
    }

    auto begin() noexcept { return _tasks.begin(); }
    auto end() noexcept { return _tasks.end(); }
    [[nodiscard]] auto begin() const noexcept { return _tasks.begin(); }
    [[nodiscard]] auto end() const noexcept { return _tasks.end(); }

    void reset() {
        for (auto &task : _tasks) {
            ;
        }
    }

    void add(const Task &task) { _tasks.emplace_back(task); }
    void add(Task &&task) { _tasks.emplace_back(std::move(task)); }

    void swap(uint index1, uint index2) noexcept(false) {
        if (index1 >= _tasks.size() || index2 >= _tasks.size()) {
            throw std::out_of_range("TaskList::swap: index out of range");
        }

        std::swap(_tasks[index1], _tasks[index2]);
    }

    [[nodiscard]] auto size() const noexcept -> std::size_t { return _tasks.size(); }

    [[nodiscard]] auto empty() const noexcept -> bool { return _tasks.empty(); }
};

class Tasks {
    TaskList _daily_tasks;

  public:
    Tasks() = default;

    explicit Tasks(TaskList daily_tasks) : _daily_tasks(std::move(daily_tasks)) {}
    Tasks(const Tasks &) = default;
    Tasks(Tasks &&) = default;
    auto operator=(const Tasks &) -> Tasks & = default;
    auto operator=(Tasks &&) -> Tasks & = default;
    ~Tasks() = default;

    void add_task(const Task &task) { _daily_tasks.add(task); }
    void add_task(Task &&task) { _daily_tasks.add(std::move(task)); }

    void delete_task(const std::size_t index) { _daily_tasks.erase(index); }

    auto get_daily_task(uint index) -> Task * { return &_daily_tasks.at(index); }
    [[nodiscard]] auto get_daily_task(uint index) const -> const Task & {
        return _daily_tasks.at(index);
    }

    void swap_tasks(uint index1, uint index2) { _daily_tasks.swap(index1, index2); }

    [[nodiscard]] auto daily_tasks() const -> const TaskList & { return _daily_tasks; }

    void update() noexcept { _daily_tasks.reset(); }
};
} // namespace Lines
