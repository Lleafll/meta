#include "TurnBasedClock.h"
#include "InternalGameState.h"

namespace metacore {

namespace {

constexpr auto turn_time = std::chrono::milliseconds{25};

} // namespace

TurnBasedClock::TurnBasedClock(InternalGameState& state) : state_{&state}
{
}

void TurnBasedClock::input()
{
    advance(*state_, turn_time);
}

} // namespace metacore
