#pragma once

#include <cstdint>
#include <memory>

namespace Lines {

using uint = std::uint32_t;

class TaskCompletion {
  public:
    enum class State : uint8_t { NotCompleted, Completed, Skipped };

  private:
    State _state = State::NotCompleted;

  protected:
    virtual void set_state(State state) noexcept;

  public:
    TaskCompletion() = default;
    TaskCompletion(const TaskCompletion &) = default; // LCOV_EXCL_LINE
    TaskCompletion(TaskCompletion &&) = default;
    auto operator=(const TaskCompletion &) -> TaskCompletion & = default;
    auto operator=(TaskCompletion &&) -> TaskCompletion & = default;
    virtual ~TaskCompletion() = default;

    virtual void complete() noexcept;
    [[nodiscard]] auto completed() const noexcept -> bool;
    [[nodiscard]] auto finished() const noexcept -> bool;

    virtual void reset() noexcept;
    virtual void skip() noexcept;
    [[nodiscard]] auto skipped() const noexcept -> bool;
    [[nodiscard]] auto state() const noexcept -> State;

    [[nodiscard]] virtual auto clone() const -> std::unique_ptr<TaskCompletion>;
};

class ProgressTaskCompletion : public TaskCompletion {
    uint _min;
    uint _max;
    uint _current;

  public:
    ProgressTaskCompletion() = delete;
    ProgressTaskCompletion(uint min, uint max);
    ProgressTaskCompletion(const ProgressTaskCompletion &) = default; // LCOV_EXCL_LINE
    ProgressTaskCompletion(ProgressTaskCompletion &&) = default;
    auto operator=(const ProgressTaskCompletion &) -> ProgressTaskCompletion & = default;
    auto operator=(ProgressTaskCompletion &&) -> ProgressTaskCompletion & = default;
    ~ProgressTaskCompletion() override = default; // LCOV_EXCL_LINE

    void complete() noexcept override;
    void reset() noexcept override;
    [[nodiscard]] auto clone() const -> std::unique_ptr<TaskCompletion> override;

    [[nodiscard]] auto min() const noexcept -> uint;
    [[nodiscard]] auto max() const noexcept -> uint;
    [[nodiscard]] auto current() const noexcept -> uint;

    void set_current(uint current);

    void set_state(State state) noexcept override;
};
} // namespace Lines
