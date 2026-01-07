#pragma once

#include "detail/macro.h"
#include "temporal.h"
#include <algorithm> // std::ranges::all_of
#include <cassert>
#include <cstddef>          // std::size_t, std::ptrdiff_t
#include <functional>       // std::function
#include <initializer_list> // std::initializer_list
#include <memory>           // std::unique_ptr, std::make_unique
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
                      std::vector<std::string> tags = {})
        : title(std::move(title)), description(std::move(desc)), tags(std::move(tags)) {
        if (this->title.empty()) {
            throw std::invalid_argument("TaskInfo: title must not be empty");
        }
    }
    ~TaskInfo() = default;
    std::string title;
    std::optional<std::string> description;
    std::vector<std::string> tags;
};

class TaskCompletion {
  public:
    enum class State : uint8_t { NotCompleted, Completed, Skipped };

  private:
    State _state = State::NotCompleted;

  protected:
    virtual void set_state(State state) noexcept { _state = state; }

  public:
    TaskCompletion() = default;
    TaskCompletion(const TaskCompletion &) = default;
    TaskCompletion(TaskCompletion &&) = default;
    auto operator=(const TaskCompletion &) -> TaskCompletion & = default;
    auto operator=(TaskCompletion &&) -> TaskCompletion & = default;
    virtual ~TaskCompletion() = default;

    virtual void complete() noexcept { set_state(State::Completed); };
    [[nodiscard]] auto completed() const noexcept -> bool { return _state == State::Completed; }
    [[nodiscard]] auto finished() const noexcept -> bool { return _state != State::NotCompleted; }

    virtual void reset() noexcept { set_state(State::NotCompleted); }
    virtual void skip() noexcept { set_state(State::Skipped); }
    [[nodiscard]] auto skipped() const noexcept -> bool { return _state == State::Skipped; }
    [[nodiscard]] auto state() const noexcept -> State { return _state; }

    [[nodiscard]] virtual auto clone() const -> std::unique_ptr<TaskCompletion> {
        return std::make_unique<TaskCompletion>(*this);
    };
};

class ProgressTaskCompletion : public TaskCompletion {
    uint _min;
    uint _max;
    uint _current;

  public:
    ProgressTaskCompletion() = delete;
    ProgressTaskCompletion(uint min, uint max) : _min(min), _max(max), _current(min) { // NOLINT
        if (_min > _max) {
            throw std::invalid_argument("ProgressTaskCompletion: min > max");
        }
    }
    ProgressTaskCompletion(const ProgressTaskCompletion &) = default;
    ProgressTaskCompletion(ProgressTaskCompletion &&) = default;
    auto operator=(const ProgressTaskCompletion &) -> ProgressTaskCompletion & = default;
    auto operator=(ProgressTaskCompletion &&) -> ProgressTaskCompletion & = default;
    ~ProgressTaskCompletion() override = default;

    void complete() noexcept override {
        TaskCompletion::complete();
        _current = _max;
    }
    void reset() noexcept override {
        TaskCompletion::reset();
        _current = _min;
    }
    [[nodiscard]] auto clone() const -> std::unique_ptr<TaskCompletion> override {
        return std::make_unique<ProgressTaskCompletion>(*this);
    }

    [[nodiscard]] auto min() const noexcept -> uint { return _min; }
    [[nodiscard]] auto max() const noexcept -> uint { return _max; }
    [[nodiscard]] auto current() const noexcept -> uint { return _current; }

    void set_current(uint current) {
        if (current > _max || current < _min) {
            throw std::out_of_range("ProgressTaskCompletion: current out of range");
        }
        if (current == _max) {
            complete();
            return;
        }
        _current = current;
    }

    void set_state(State state) noexcept override {
        TaskCompletion::set_state(state);
        if (state == State::Completed) {
            _current = _max;
        } else if (state == State::NotCompleted) {
            reset();
        }
    }
};

class TaskVisibility {
    using Pred = std::function<bool(const Temporal::Date &)>;
    Pred _predicate;

  public:
    TaskVisibility() = delete;
    TaskVisibility(const TaskVisibility &) = default;
    TaskVisibility(TaskVisibility &&) = default;
    auto operator=(const TaskVisibility &) -> TaskVisibility & = default;
    auto operator=(TaskVisibility &&) -> TaskVisibility & = default;
    explicit TaskVisibility(Pred predicate) : _predicate(std::move(predicate)) {}
    ~TaskVisibility() = default;

    [[nodiscard]] auto visible(const Temporal::Date &date) const noexcept -> bool {
        return _predicate(date);
    }
};

namespace Visibility {
inline auto always() {
    return TaskVisibility([](auto) { return true; });
}

inline auto never() {
    return TaskVisibility([](auto) { return false; });
}

inline auto from(const Temporal::Date &from) {
    return TaskVisibility([from](const Temporal::Date &date) { return date >= from; });
}

inline auto until(const Temporal::Date &until) {
    return TaskVisibility([until](const Temporal::Date &date) { return date <= until; });
}

inline auto between(const Temporal::Date &from, const Temporal::Date &until) {
    if (from > until) {
        throw std::invalid_argument("Visibility::between: from > until");
    }

    return TaskVisibility(
        [from, until](const Temporal::Date &date) { return date >= from && date <= until; });
}

inline auto weekdays() {
    return TaskVisibility([](const Temporal::Date &date) {
        auto weekday = date.weekday();
        return weekday >= Temporal::Weekday::Monday && weekday <= Temporal::Weekday::Friday;
    });
}

inline auto weekends() {
    return TaskVisibility([](const Temporal::Date &date) {
        auto weekday = date.weekday();
        return weekday == Temporal::Weekday::Saturday || weekday == Temporal::Weekday::Sunday;
    });
}

inline auto once(const Temporal::Date &target) {
    return TaskVisibility([target](const Temporal::Date &date) { return date == target; });
}

inline auto every_month_day(const Temporal::Day &day) {
    if (unsigned(day) == 0 || unsigned(day) > 31) { // NOLINT
        throw std::invalid_argument("Visibility::every_month_day: invalid day");
    }

    return TaskVisibility([day](const Temporal::Date &date) { return date.day() == day; });
}

inline auto every_nth_day(uint n, const Temporal::Date &start) {
    if (n == 0) {
        throw std::invalid_argument("Visibility::every_nth_day: n must not be 0");
    }
    return TaskVisibility([n, start](const Temporal::Date &date) {
        if (date < start) {
            return false;
        }
        return (date - start).count() % n == 0;
    });
}

inline auto every_weekday(Temporal::Weekday weekday) {
    return TaskVisibility(
        [weekday](const Temporal::Date &date) { return date.weekday() == weekday; });
}
}; // namespace Visibility

class Task {
    TaskInfo _info;
    std::unique_ptr<TaskCompletion> _completion;
    TaskVisibility _visibility;
    explicit Task(TaskInfo info, std::unique_ptr<TaskCompletion> completion,
                  TaskVisibility visibility)
        : _info(std::move(info)), _completion(std::move(completion)),
          _visibility(std::move(visibility)) {
        if (_info.title.empty()) {
            throw std::invalid_argument("Task: title must not be empty");
        }
        if (!_completion) {
            throw std::invalid_argument("Task: completion must not be null");
        }
    }

  public:
    friend class TaskFactory;
    Task(const Task &task) : _info(task._info), _visibility(task._visibility) {
        _completion = task._completion->clone();
    }
    auto operator=(const Task &task) -> Task & {
        if (this == &task) {
            return *this;
        }
        _completion = task._completion->clone();

        _info = task._info;
        _visibility = task._visibility;
        return *this;
    };
    Task(Task &&) = default;
    auto operator=(Task &&) -> Task & = default;
    ~Task() = default;

    auto completion() -> TaskCompletion & { return *_completion; }
    [[nodiscard]] auto completion() const -> const TaskCompletion & { return *_completion; }

    void set_title(const std::string &title) {
        if (title.empty()) {
            throw std::invalid_argument("Task: title must not be empty");
        }
        _info.title = title;
    }
    void set_description(const std::string &description) { _info.description = description; }
    void set_tags(std::vector<std::string> tags) { _info.tags = std::move(tags); }

    [[nodiscard]] auto title() const -> const std::string & { return _info.title; }
    [[nodiscard]] auto description() const -> const std::optional<std::string> & {
        return _info.description;
    }
    [[nodiscard]] auto tags() const -> const std::vector<std::string> & { return _info.tags; }

    [[nodiscard]] auto visible(const Temporal::Date &date) const noexcept -> bool {
        return _visibility.visible(date);
    }
};

class TaskFactory final {
    TaskInfo _info;
    std::unique_ptr<TaskCompletion> _completion = std::make_unique<TaskCompletion>();
    TaskVisibility _visibility = Visibility::always();

  public:
    TaskFactory() = default;
    TaskFactory(const TaskFactory &) = delete;
    TaskFactory(TaskFactory &&) = default;
    auto operator=(const TaskFactory &) = delete;
    auto operator=(TaskFactory &&) -> TaskFactory & = default;
    ~TaskFactory() = default;
    auto info(const TaskInfo &info) && -> TaskFactory {
        _info = info;
        return std::move(*this);
    }

    auto completion(const TaskCompletion &completion) && -> TaskFactory {
        _completion = completion.clone();
        return std::move(*this);
    }

    auto visibility(const TaskVisibility &visibility) && -> TaskFactory {
        _visibility = visibility;
        return std::move(*this);
    }

    auto task() && {
        return Task(std::move(_info), std::move(_completion), std::move(_visibility));
    }
};

inline auto make_task() -> TaskFactory { return {}; }

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
    explicit TaskList(std::vector<Task> tasks) : _tasks(std::move(tasks)) {}
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
