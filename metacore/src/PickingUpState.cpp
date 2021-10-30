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
    auto const tiles = state.layout.tiles();
    auto const projectiles = state.player.projectiles();
    return {
        state.player.position(),
        std::nullopt,
        state.choices,
        state.player.is_slashing(),
        state.enemies.positions(),
        GameProgress::Running,
        projectiles == nullptr ? std::optional<std::vector<Position>>{}
                               : *projectiles,
        {tiles.begin(), tiles.end()}};
}

} // namespace metacore