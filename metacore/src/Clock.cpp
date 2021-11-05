#include "Clock.h"

namespace metacore {

Clock::Clock(InternalGameState& state)
    : clock_{TurnBasedClock{state}}, state_{&state}
{
}

void Clock::input()
{
    std::visit([](TurnBasedClock& clock) { clock.input(); }, clock_);
}

} // namespace metacore
