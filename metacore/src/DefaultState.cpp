#include "DefaultState.h"
#include "GameState.h"

namespace metacore {

DefaultState::DefaultState(
    Player const& player, Pickup const& pickup, Enemies const& enemies)
    : player{player}, pickup{pickup}, enemies{enemies}
{
}

GameState to_game_state(DefaultState const& state)
{
    auto const& player = state.player;
    return {
        player.position(),
        state.pickup.position,
        std::nullopt,
        player.is_slashing()};
}

} // namespace metacore