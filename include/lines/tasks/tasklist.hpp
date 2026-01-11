#pragma once

#include <cassert>
#include <cstddef> // std::size_t, std::ptrdiff_t
#include <lines/tasks/task.hpp>
#include <lines/temporal/date.hpp>
#include <stdexcept> // std::out_of_range, std::invalid_argument

namespace Lines {
class TaskList {
    std::vector<Task> _tasks;

  public:
    TaskList(const TaskList &) = default;
    TaskList(TaskList &&) = default;
    TaskList(std::initializer_list<Task> tasks) : _tasks(tasks) {}
    auto operator=(const TaskList &) -> TaskList & = default;
    auto operator=(TaskList &&) -> TaskList & = default;
    auto operator=(std::initializer_list<Task> tasks) -> TaskList & {
        _tasks = tasks;
        return *this;
    }
    explicit TaskList(const std::vector<Task> &tasks) : _tasks(tasks) {}
    TaskList() = default;
    ~TaskList() = default;

    [[nodiscard]] auto completed() const -> bool {
        return std::ranges::all_of(_tasks,
                                   [](const auto &task) { return task.completion().completed(); });
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
        _tasks.erase(_tasks.begin() + static_cast<std::ptrdiff_t>(index));
    }

    auto begin() noexcept { return _tasks.begin(); }
    auto end() noexcept { return _tasks.end(); }
    [[nodiscard]] auto begin() const noexcept { return _tasks.begin(); }
    [[nodiscard]] auto end() const noexcept { return _tasks.end(); }

    void reset() {
        for (auto &task : _tasks) {
            task.completion().reset();
        }
    }

    void add(const Task &task) { _tasks.emplace_back(task); }
    void add(Task &&task) { _tasks.emplace_back(std::move(task)); }

    void swap(std::size_t index1, std::size_t index2) noexcept(false) {
        if (index1 >= _tasks.size() || index2 >= _tasks.size()) {
            throw std::out_of_range("TaskList::swap: index out of range");
        }

        std::swap(_tasks[index1], _tasks[index2]);
    }

    [[nodiscard]] auto size() const noexcept -> std::size_t { return _tasks.size(); }

    [[nodiscard]] auto empty() const noexcept -> bool { return _tasks.empty(); }

    [[nodiscard]] auto visible_at(const Temporal::Date &date) const -> TaskList {
        TaskList result;
        for (const auto &task : _tasks) {
            if (task.visible(date)) {
                result.add(task);
            }
        }
        return result;
    }
};
} // namespace Lines
