#include "DefaultState.h"
#include "GameState.h"
#include <utility>

namespace metacore {

DefaultState::DefaultState(
    Player player, Pickup const& pickup, Enemies enemies, Layout layout)
    : player{std::move(player)},
      pickup{pickup},
      enemies{std::move(enemies)},
      layout{std::move(layout)}
{
}

namespace {

std::vector<Position> to_positions(std::vector<PositionD> const& positions_d)
{
    auto positions = std::vector<Position>{};
    std::ranges::transform(
        positions_d, std::back_inserter(positions), to_position);
    return positions;
}

} // namespace

GameState to_game_state(DefaultState const& state)
{
    auto const& player = state.player;
    auto const& enemies = state.enemies;
    auto const& layout = state.layout;
    auto const* const projectiles = player.projectiles();
    auto const tiles = layout.tiles();
    return {
        to_position(player.position()),
        state.pickup.position,
        std::nullopt,
        player.is_slashing() ? std::optional{player.orientation()}
                             : std::optional<Orientation>{},
        to_positions(enemies.positions()),
        GameProgress::Running,
        projectiles == nullptr
            ? std::optional<std::vector<Position>>{}
            : std::optional<std::vector<Position>>{to_positions(*projectiles)},
        {tiles.begin(), tiles.end()},
        player.texture,
        enemies.texture,
        layout.texture,
        layout.bounds().value_or(LayoutBounds{})};
}

} // namespace metacore