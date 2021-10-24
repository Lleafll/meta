#include "PickingUpState.h"
#include "GameState.h"
#include <utility>

namespace metacore {

PickingUpState::PickingUpState(
    Player const& player, UpgradeChoices const& choices, Enemies enemies)
    : player{player}, choices{choices}, enemies{std::move(enemies)}
{
}

GameState to_game_state(PickingUpState const& state)
{
    return {
        state.player.position(),
        std::nullopt,
        state.choices,
        state.player.is_slashing(),
        state.enemies.positions()};
}

} // namespace metacore