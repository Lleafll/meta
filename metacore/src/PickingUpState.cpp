#include "PickingUpState.h"
#include "GameState.h"
#include <utility>

namespace metacore {

PickingUpState::PickingUpState(
    Player player,
    UpgradeChoices const& choices,
    Enemies enemies,
    Layout const& layout)
    : player{std::move(player)},
      choices{choices},
      enemies{std::move(enemies)},
      layout{layout}
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