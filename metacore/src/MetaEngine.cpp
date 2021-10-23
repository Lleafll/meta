#include "MetaEngine.h"
#include "GameState.h"
#include "Pickup.h"

namespace metacore {

namespace {

constexpr auto player_move_increment = 5;
constexpr auto pickup_distance = 50;
constexpr auto initial_bounds = 250;

} // namespace

struct MetaEngine::Impl final {
    Position player = {0, 0};
    Pickup pickup = {{200, 50}, {PickupUpgrade::Slash, PickupUpgrade::Shoot}};

    static Impl from(GameState const& state)
    {
        return {
            state.player_position,
            {state.upgrade, {PickupUpgrade::Slash, PickupUpgrade::Shoot}}};
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
    return {impl_->player, impl_->pickup.position};
}

namespace {

void check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
    Position const& player, Pickup& pickup)
{
    auto const& position = pickup.position;
    if (is_within_distance<pickup_distance>(player, position)) {
        pickup = {
            {(position.y + 333) % (2 * initial_bounds) - initial_bounds,
             (position.x + 333) % (2 * initial_bounds) - initial_bounds},
            {PickupUpgrade::Shoot, PickupUpgrade::Slash}};
    }
}

} // namespace

void MetaEngine::input_right()
{
    impl_->player.x += player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        impl_->player, impl_->pickup);
}

void MetaEngine::input_left()
{
    impl_->player.x -= player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        impl_->player, impl_->pickup);
}

void MetaEngine::input_up()
{
    impl_->player.y += player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        impl_->player, impl_->pickup);
}

void MetaEngine::input_down()
{
    impl_->player.y -= player_move_increment;
    check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
        impl_->player, impl_->pickup);
}

} // namespace metacore
