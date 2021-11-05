#include "PickingUpState.h"
#include "GameState.h"
#include <algorithm>
#include <utility>

namespace metacore {

PickingUpState::PickingUpState(
    Player player,
    UpgradeChoices const& choices,
    Enemies enemies,
    Layout layout)
    : player{std::move(player)},
      choices{choices},
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

GameState to_game_state(PickingUpState const& state)
{
    auto const& player = state.player;
    auto const& enemies = state.enemies;
    auto const& layout = state.layout;
    auto const tiles = layout.tiles();
    auto const projectiles = player.projectiles();
    return {
        to_position(player.position()),
        std::nullopt,
        state.choices,
        player.is_slashing() ? std::optional{player.orientation()}
                             : std::optional<Orientation>{},
        to_positions(enemies.positions()),
        GameProgress::Running,
        projectiles == nullptr ? std::optional<std::vector<Position>>{}
                               : to_positions(*projectiles),
        {tiles.begin(), tiles.end()},
        player.texture,
        enemies.texture,
        layout.texture,
        layout.bounds().value_or(LayoutBounds{})};
}

} // namespace metacore