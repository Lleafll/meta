#include "MetaEngine.h"
#include "GameState.h"

namespace metacore {

namespace {

constexpr auto player_move_increment = 5;
constexpr auto pickup_distance = 50;
constexpr auto initial_bounds = 250;

} // namespace

struct MetaEngine::Impl final {
    Position player = {0, 0};
    Position upgrade = {200, 50};

    static Impl from(GameState const& state)
    {
        return {state.player_position, state.upgrade};
    }
};

MetaEngine::MetaEngine() : impl_{std::make_unique<Impl>()}
{
}

MetaEngine::MetaEngine(GameState const& state)
    : impl_{std::make_unique<Impl>(Impl::from(state))}
{
}

MetaEngine::~MetaEngine() = default;

GameState MetaEngine::calculate_state() const
{
    return {impl_->player, impl_->upgrade};
}

namespace {

void check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
    Position const& player, Position& upgrade)
{
    if (is_within_distance<pickup_distance>(player, upgrade)) {
        upgrade = {
            (upgrade.y + 333) % (2 * initial_bounds) - initial_bounds,
            (upgrade.x + 333) % (2 * initial_bounds) - initial_bounds};
    }
}

} // namespace

void MetaEngine::input_right()
{
    impl_->player.x += player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        impl_->player, impl_->upgrade);
}

void MetaEngine::input_left()
{
    impl_->player.x -= player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        impl_->player, impl_->upgrade);
}

void MetaEngine::input_up()
{
    impl_->player.y += player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        impl_->player, impl_->upgrade);
}

void MetaEngine::input_down()
{
    impl_->player.y -= player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        impl_->player, impl_->upgrade);
}

} // namespace metacore
