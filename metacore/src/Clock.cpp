#include "Clock.h"
#include "overloaded.h"
#include <gsl/gsl_assert>

namespace metacore {

Clock::Clock(InternalGameState& state)
    : clock_{TurnBasedClock{state}}, state_{&state}
{
}

void Clock::set_state(InternalGameState& state)
{
    state_ = &state;
}

void Clock::set(ClockUpgrade const upgrade)
{
    Expects(state_ != nullptr);
    switch (upgrade) {
        case ClockUpgrade::TurnBased:
            clock_ = TurnBasedClock{*state_};
            break;
        case ClockUpgrade::RealTime:
            clock_ = RealTimeClock{*state_};
            break;
        case ClockUpgrade::Superhot:
            clock_ = SuperhotClock{*state_};
            break;
    }
}

void Clock::input()
{
    std::visit(
        overloaded{
            [](TurnBasedClock& clock) { clock.input(); },
            [](SuperhotClock& clock) { clock.input_start(); },
            [](auto const&) {}},
        clock_);
}

void Clock::stop()
{
    std::visit(
        overloaded{
            [](SuperhotClock& clock) { clock.input_stop(); },
            [](auto const&) {}},
        clock_);
}

} // namespace metacore
