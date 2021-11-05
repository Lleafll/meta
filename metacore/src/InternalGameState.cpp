#include "InternalGameState.h"
#include "overloaded.h"

namespace metacore {

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

void advance(InternalGameState& state, std::chrono::milliseconds const diff)
{
    std::visit(
        overloaded{
            [diff](DefaultState& state) {
                state.player.advance(state.layout.tiles(), diff);
                state.layout.check_for_transition(
                    state.player, state.enemies.positions());
                kill_enemies_that_got_hit(state.player, state.enemies);
                state.enemies.advance(state.player.position(), diff);
            },
            [](auto const&) {}},
        state.value);
}

} // namespace metacore