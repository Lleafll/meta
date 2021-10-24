#include "DefaultState.h"
#include "GameState.h"
#include <utility>

namespace metacore {

DefaultState::DefaultState(
    Player const& player, Pickup const& pickup, Enemies enemies)
    : player{player}, pickup{pickup}, enemies{std::move(enemies)}
{
}

GameState to_game_state(DefaultState const& state)
{
    auto const& player = state.player;
    return {
        player.position(),
        state.pickup.position,
        std::nullopt,
        player.is_slashing(),
        state.enemies.positions()};
}

} // namespace metacore