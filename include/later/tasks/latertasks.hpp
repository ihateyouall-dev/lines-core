#pragma once

#include "../macro/latermacro.hpp"
#include "../temporal/latertemporal.h"
#include <algorithm>        // std::ranges::all_of
#include <initializer_list> // std::initializer_list
#include <optional>         // std::optional
#include <stdexcept>        // std::out_of_range, std::invalid_argument
#include <string>           // std::string
#include <utility>          // std::move
#include <vector>           // std::vector

using uint = unsigned int;

namespace Later {
class Task {
    std::string _title;
    std::optional<std::string> _description = std::nullopt;
    std::optional<std::string> _category = std::nullopt;
    bool _completed = false;

  public:
    Task(std::string title, std::string description, std::string category)
        : _title(std::move(title)) { // NOLINT

        if (description == "") {
            _description = std::nullopt;
        } else {
            _description = std::move(description);
        }
        if (category == "") {
            _category = std::nullopt;
        } else {
            _category = std::move(category);
        }
    }
    Task() = default;
    Task(const Task &) = default;
    Task(Task &&) = default;
    auto operator=(const Task &) -> Task & = default;
    auto operator=(Task &&) -> Task & = default;
    ~Task() = default;

    void complete() { _completed = true; }

    void reset() { _completed = false; }

    [[nodiscard]] auto completed() const -> bool { return _completed; }

    [[nodiscard]] auto title() const -> std::string { return _title; }
    [[nodiscard]] auto description() const -> std::optional<std::string> { return _description; }
    [[nodiscard]] auto category() const -> std::optional<std::string> { return _category; }
    void set_title(const std::string &title) { _title = title; }
    void set_description(const std::string &description) {
        if (description == "") {
            _description = std::nullopt;
        } else {
            _description = description;
        }
    }
    void set_category(const std::string &category) {
        if (category == "") {
            _category = std::nullopt;
        } else {
            _category = category;
        }
    }
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
        return std::ranges::all_of(_tasks, &Task::completed);
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
} // namespace Later
