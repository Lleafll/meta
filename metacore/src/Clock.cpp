#include "Clock.h"
#include "InternalGameState.h"
#include "overloaded.h"

namespace metacore {

Clock::Clock(InternalGameState& state) : state_{&state}
{
}

namespace {

void kill_enemies_that_got_hit(Player const& player, Enemies& enemies)
{
    auto const& positions = enemies.positions();
    if (positions.empty()) {
        return;
    }
    for (auto i = positions.size(); i > 0; --i) {
        if (player.target_is_hit(positions[i - 1])) {
            enemies.kill(i - 1);
        }
    }
}

} // namespace

void Clock::input()
{
    std::visit(
        overloaded{
            [](DefaultState& state) {
                state.player.advance();
                state.layout.check_for_transition(
                    state.player, state.enemies.positions());
                kill_enemies_that_got_hit(state.player, state.enemies);
                state.enemies.advance(state.player.position());
            },
            [](auto const&) {}},
        state_->value);
}

} // namespace metacore
