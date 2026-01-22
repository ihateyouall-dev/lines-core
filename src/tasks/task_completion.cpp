#include <lines/tasks/task_completion.hpp>

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
