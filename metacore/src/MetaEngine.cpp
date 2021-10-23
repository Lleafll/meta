#include "MetaEngine.h"
#include "GameState.h"

namespace metacore {

namespace {

constexpr auto player_move_increment = 5;
constexpr auto pickup_distance = 50;
constexpr auto initial_bounds = 500;

} // namespace

MetaEngine::MetaEngine(GameState const& state) : state_{state}
{
}

GameState MetaEngine::calculate_state() const
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

void MetaEngine::input_right()
{
    state_.player_position.x += player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        state_.player_position, state_.upgrade);
}

void MetaEngine::input_left()
{
    state_.player_position.x -= player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        state_.player_position, state_.upgrade);
}

void MetaEngine::input_up()
{
    state_.player_position.y += player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        state_.player_position, state_.upgrade);
}

void MetaEngine::input_down()
{
    state_.player_position.y -= player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        state_.player_position, state_.upgrade);
}

} // namespace metacore
