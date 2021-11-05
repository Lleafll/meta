#pragma once

#include "RealTimeClock.h"
#include "TurnBasedClock.h"
#include <variant>
namespace metacore {
struct InternalGameState;
} // namespace metacore

namespace metacore {

enum class ClockUpgrade { TurnBased, RealTime };

class Clock {
  public:
    explicit Clock(InternalGameState& state);

    void set_state(InternalGameState& state);
    void set(ClockUpgrade upgrade);
    void input();

  private:
    std::variant<TurnBasedClock, RealTimeClock> clock_;
    InternalGameState* state_;
};

} // namespace metacore
