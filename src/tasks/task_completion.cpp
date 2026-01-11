#include <lines/tasks/task_completion.hpp>

#include <stdexcept>

void Lines::TaskCompletion::set_state(State state) noexcept { _state = state; }
void Lines::TaskCompletion::complete() noexcept { set_state(State::Completed); };
auto Lines::TaskCompletion::completed() const noexcept -> bool {
    return _state == State::Completed;
}

auto Lines::TaskCompletion::finished() const noexcept -> bool {
    return _state != State::NotCompleted;
}

void Lines::TaskCompletion::reset() noexcept { set_state(State::NotCompleted); }
void Lines::TaskCompletion::skip() noexcept { set_state(State::Skipped); }
auto Lines::TaskCompletion::skipped() const noexcept -> bool { return _state == State::Skipped; }

auto Lines::TaskCompletion::state() const noexcept -> State { return _state; }

auto Lines::TaskCompletion::clone() const -> std::unique_ptr<TaskCompletion> {
    return std::make_unique<TaskCompletion>(*this);
};
Lines::ProgressTaskCompletion::ProgressTaskCompletion(uint min, uint max) // NOLINT
    : _min(min), _max(max), _current(min) {
    if (_min > _max) {
        throw std::invalid_argument("ProgressTaskCompletion: min > max");
    }
}

void Lines::ProgressTaskCompletion::complete() noexcept {
    TaskCompletion::complete();
    _current = _max;
}

void Lines::ProgressTaskCompletion::reset() noexcept {
    TaskCompletion::reset();
    _current = _min;
}

auto Lines::ProgressTaskCompletion::clone() const -> std::unique_ptr<TaskCompletion> {
    return std::make_unique<ProgressTaskCompletion>(*this);
}

auto Lines::ProgressTaskCompletion::min() const noexcept -> uint { return _min; }

auto Lines::ProgressTaskCompletion::max() const noexcept -> uint { return _max; }

auto Lines::ProgressTaskCompletion::current() const noexcept -> uint { return _current; }

void Lines::ProgressTaskCompletion::set_current(uint current) {
    if (current > _max || current < _min) {
        throw std::out_of_range("ProgressTaskCompletion: current out of range");
    }
    if (current == _max) {
        complete();
        return;
    }
    _current = current;
}

void Lines::ProgressTaskCompletion::set_state(State state) noexcept {
    TaskCompletion::set_state(state);
    if (state == State::Completed) {
        _current = _max;
    } else if (state == State::NotCompleted) {
        reset();
    }
}
