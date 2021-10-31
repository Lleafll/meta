#pragma once

namespace metacore {
struct InternalGameState;
} // namespace metacore

namespace metacore {

class Clock {
  public:
    explicit Clock(InternalGameState& state);

    void input();

  private:
    InternalGameState* state_;
};

} // namespace metacore
