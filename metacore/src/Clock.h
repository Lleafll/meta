#pragma once

#include "TurnBasedClock.h"
#include <variant>
namespace metacore {
struct InternalGameState;
} // namespace metacore

namespace metacore {

enum class ClockUpgrade { TurnBased };

class Clock {
  public:
    explicit Clock(InternalGameState& state);

    void input();

  private:
    std::variant<TurnBasedClock> clock_;
    InternalGameState* state_;
};

} // namespace metacore
