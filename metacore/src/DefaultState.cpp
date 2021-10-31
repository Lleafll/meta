#include "DefaultState.h"
#include "GameState.h"
#include <utility>

namespace metacore {

DefaultState::DefaultState(
    Player player, Pickup const& pickup, Enemies enemies, Layout const& layout)
    : player{std::move(player)},
      pickup{pickup},
      enemies{std::move(enemies)},
      layout{layout}
{
}

GameState to_game_state(DefaultState const& state)
{
    auto const& player = state.player;
    auto const& enemies = state.enemies;
    auto const& layout = state.layout;
    auto const* const projectiles = player.projectiles();
    auto const tiles = layout.tiles();
    return {
        player.position(),
        state.pickup.position,
        std::nullopt,
        player.is_slashing() ? std::optional{player.orientation()}
                             : std::optional<Orientation>{},
        enemies.positions(),
        GameProgress::Running,
        projectiles == nullptr ? std::optional<std::vector<Position>>{}
                               : std::optional{*projectiles},
        {tiles.begin(), tiles.end()},
        player.texture,
        enemies.texture,
        layout.texture};
}

} // namespace metacore