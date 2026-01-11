#include <lines/tasks/task.hpp>

Lines::Task::Task(TaskInfo info, std::unique_ptr<TaskCompletion> completion,
                  std::unique_ptr<ITaskVisibilityRule> visibility)
    : _info(std::move(info)), _completion(std::move(completion)),
      _visibility(std::move(visibility)) {
    if (_info.title.empty()) {
        throw std::invalid_argument("Task: title must not be empty");
    }
    if (!_completion) {
        throw std::invalid_argument("Task: completion must not be null");
    }
}

Lines::Task::Task(const Task &task) : _info(task._info), _visibility(task._visibility->clone()) {
    _completion = task._completion->clone();
}

auto Lines::Task::operator=(const Task &task) -> Task & {
    if (this == &task) {
        return *this;
    }
    _completion = task._completion->clone();

    _info = task._info;
    _visibility = task._visibility->clone();
    return *this;
};

auto Lines::Task::completion() -> TaskCompletion & { return *_completion; }

auto Lines::Task::completion() const -> const TaskCompletion & { return *_completion; }

void Lines::Task::set_title(const std::string &title) {
    if (title.empty()) {
        throw std::invalid_argument("Task: title must not be empty");
    }
    _info.title = title;
}

void Lines::Task::set_description(const std::string &description) {
    _info.description = description;
}

void Lines::Task::set_tags(std::vector<std::string> tags) { _info.tags = std::move(tags); }

auto Lines::Task::title() const -> const std::string & { return _info.title; }

auto Lines::Task::description() const -> const std::optional<std::string> & {
    return _info.description;
}

auto Lines::Task::tags() const -> const std::vector<std::string> & { return _info.tags; }

auto Lines::Task::visibility() const noexcept -> const ITaskVisibilityRule & {
    return *_visibility;
}

auto Lines::Task::visible(const Temporal::Date &date) const -> bool { return visibility()(date); }

auto Lines::TaskFactory::info(const TaskInfo &info) && -> TaskFactory {
    _info = info;
    return std::move(*this);
}

auto Lines::TaskFactory::completion(const TaskCompletion &completion) && -> TaskFactory {
    _completion = completion.clone();
    return std::move(*this);
}

auto Lines::TaskFactory::visibility(const ITaskVisibilityRule &visibility) && -> TaskFactory {
    _visibility = visibility.clone();
    return std::move(*this);
}

auto Lines::TaskFactory::task() && -> Task {
    return Task(std::move(_info), std::move(_completion), std::move(_visibility));
}
