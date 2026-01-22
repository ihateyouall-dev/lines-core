#pragma once

#include <cstdint>

namespace Lines {

using uint = std::uint32_t;

class TaskCompletion {
  public:
    enum class State : uint8_t { NotCompleted, Completed, Skipped };

  private:
    State _state = State::NotCompleted;

  protected:
    void set_state(State state) noexcept;

  public:
    TaskCompletion() = default;
    TaskCompletion(const TaskCompletion &) = default; // LCOV_EXCL_LINE
    TaskCompletion(TaskCompletion &&) = default;
    auto operator=(const TaskCompletion &) -> TaskCompletion & = default;
    auto operator=(TaskCompletion &&) -> TaskCompletion & = default;
    ~TaskCompletion() = default;

    void complete() noexcept;
    [[nodiscard]] auto completed() const noexcept -> bool;
    [[nodiscard]] auto finished() const noexcept -> bool;

    void reset() noexcept;
    void skip() noexcept;
    [[nodiscard]] auto skipped() const noexcept -> bool;
    [[nodiscard]] auto state() const noexcept -> State;
};
} // namespace Lines
