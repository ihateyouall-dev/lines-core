#pragma once

#include <algorithm> // std::ranges::all_of
#include <chrono>    // std::chrono::system_clock
#include <optional>  // std::optional
#include <stdexcept> // std::out_of_range
#include <string>  // std::string
#include <utility> // std::move
#include <vector>  // std::vector

using TimePoint = std::chrono::system_clock::time_point;
using namespace std::chrono_literals;
using namespace std::chrono;
using uint = unsigned int;

struct Progress {
    Progress(uint current,          // NOLINT(bugprone-easily-swappable-parameters)
             uint maximal) noexcept // NOLINT(bugprone-easily-swappable-parameters)
        : current(current), maximal(maximal) {}
    Progress() = default;
    uint current = 0;
    uint maximal = 1;

    [[nodiscard]] auto completed() const noexcept -> bool { return current >= maximal; }
};

class Task {
    std::string _title;
    std::string _description;
    std::string _category;
    bool _completed = false;
    std::optional<Progress> _progress;

  public:
    Task(const Task &) = default;
    Task(Task &&) = default;
    auto operator=(const Task &) -> Task & = default;
    auto operator=(Task &&) -> Task & = default;
    ~Task() = default;
    Task(std::string title, std::string description, std::string category)
        : _title(std::move(title)), _description(std::move(description)),
          _category(std::move(category)) {}

    void complete() { _completed = true; }

    void reset() { _completed = false; }

    [[nodiscard]] auto completed() const -> bool { return _completed; }
};

class TaskList {
    std::vector<Task> _tasks;

  public:
    TaskList(const TaskList &) = default;
    TaskList(TaskList &&) = default;
    auto operator=(const TaskList &) -> TaskList & = default;
    auto operator=(TaskList &&) -> TaskList & = default;
    explicit TaskList(std::vector<Task> tasks) : _tasks(std::move(tasks)) {}
    TaskList() = default;
    ~TaskList() = default;

    [[nodiscard]] auto completed() const -> bool {
        return std::ranges::all_of(_tasks, &Task::completed);
    }

    auto operator[](const std::size_t index) -> Task & { return _tasks[index]; }
    auto operator[](const std::size_t index) const -> const Task & { return _tasks[index]; }
    auto at(const std::size_t index) -> Task & { return _tasks.at(index); }
    [[nodiscard]] auto at(const std::size_t index) const -> const Task & {
        return _tasks.at(index);
    }

    auto begin() noexcept { return _tasks.begin(); }
    auto end() noexcept { return _tasks.end(); }
    [[nodiscard]] auto begin() const noexcept { return _tasks.begin(); }
    [[nodiscard]] auto end() const noexcept { return _tasks.end(); }

    void reset() {
        for (auto &task : _tasks) {
            task.reset();
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

class TasksSystem {
    TaskList _daily_tasks;

  public:
    TasksSystem() = default;

    TasksSystem(const TasksSystem &) = default;
    TasksSystem(TasksSystem &&) = default;
    auto operator=(const TasksSystem &) -> TasksSystem & = default;
    auto operator=(TasksSystem &&) -> TasksSystem & = default;
    ~TasksSystem() = default;

    void add_task(const Task &task) { _daily_tasks.add(task); }
    void add_task(Task &&task) { _daily_tasks.add(std::move(task)); }

    auto get_daily_task(uint index) -> Task& { return _daily_tasks.at(index); }
    [[nodiscard]] auto get_daily_task(uint index) const -> const Task& { return _daily_tasks.at(index); } 

    void swap_tasks(uint index1, uint index2) { _daily_tasks.swap(index1, index2); }

    [[nodiscard]] auto daily_tasks() const -> const TaskList& { return _daily_tasks; }
};
