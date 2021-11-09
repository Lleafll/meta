#include "TurnBasedClock.h"
#include "InternalGameState.h"

namespace metacore {

namespace {

constexpr auto turn_time =
    std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::milliseconds{250});

} // namespace

TurnBasedClock::TurnBasedClock(InternalGameState& state) : state_{&state}
{
}

void TurnBasedClock::set_state(InternalGameState& state)
{
    state_ = &state;
}

void TurnBasedClock::input()
{
    state_->advance(turn_time);
}

} // namespace metacore
