#include "TurnBasedClock.h"
#include "InternalGameState.h"

namespace metacore {

TurnBasedClock::TurnBasedClock(InternalGameState& state) : state_{&state}
{
}

void TurnBasedClock::input()
{
    advance(*state_);
}

} // namespace metacore
