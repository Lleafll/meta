#include "MetaEngine.h"
#include "GameState.h"
#include <array>

namespace metacore {

namespace {

constexpr auto player_move_increment = 5;
constexpr auto pickup_distance = 50;
constexpr auto initial_bounds = 500;

} // namespace

MetaEngine::MetaEngine(GameState const& state) noexcept : state_{state}
{
}

GameState MetaEngine::calculate_state() const noexcept
{
    return state_;
}

namespace {

void check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
    Position const& player, Position& upgrade)
{
    if (is_within_distance<pickup_distance>(player, upgrade)) {
        upgrade = {
            (upgrade.y + 333) % initial_bounds,
            (upgrade.x + 333) % initial_bounds};
    }
}

} // namespace

void MetaEngine::input_right() noexcept
{
    state_.player_position.x += player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        state_.player_position, state_.upgrade);
}

} // namespace metacore
