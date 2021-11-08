#pragma once

#include "RealTimeClock.h"
#include "SuperhotClock.h"
#include "TurnBasedClock.h"
#include <variant>
namespace metacore {
class InternalGameState;
} // namespace metacore

namespace metacore {

enum class ClockUpgrade { TurnBased, RealTime, Superhot };

class Clock {
  public:
    explicit Clock(InternalGameState& state);

    void set_state(InternalGameState& state);
    void set(ClockUpgrade upgrade);
    void input();
    void stop();

  private:
    std::variant<TurnBasedClock, RealTimeClock, SuperhotClock> clock_;
    InternalGameState* state_;
};

} // namespace metacore
