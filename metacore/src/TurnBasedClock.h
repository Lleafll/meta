#pragma once

namespace metacore {
class InternalGameState;
} // namespace metacore

namespace metacore {

class TurnBasedClock {
  public:
    explicit TurnBasedClock(InternalGameState& state);

    void input();

  private:
    InternalGameState* state_;
};

} // namespace metacore
