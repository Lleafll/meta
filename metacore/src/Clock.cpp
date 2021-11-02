#include "Clock.h"
#include "InternalGameState.h"

namespace metacore {

Clock::Clock(InternalGameState& state) : state_{&state}
{
}

void Clock::input()
{
    advance(*state_);
}

} // namespace metacore
