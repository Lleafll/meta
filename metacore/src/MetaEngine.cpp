#include "MetaEngine.h"
#include "DefaultState.h"
#include "GameState.h"
#include "overloaded.h"
#include "PickingUpState.h"
#include <variant>

namespace metacore {

namespace {

constexpr auto player_move_increment = 5;
constexpr auto pickup_distance = 50;

} // namespace

struct MetaEngine::Impl final {
    std::variant<DefaultState, PickingUpState> state;

    static Impl from(GameState const& state)
    {
        return {DefaultState{
            state.player_position,
            {state.upgrade_position.value_or(Position{0, 0}),
             {PickupUpgrade::Slash, PickupUpgrade::Shoot}}}};
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
    return std::visit(
        [](auto const& state) -> GameState { return to_game_state(state); },
        impl_->state);
}

namespace {

void check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
    Position const& player,
    Pickup const& pickup,
    std::variant<DefaultState, PickingUpState>& state)
{
    if (is_within_distance<pickup_distance>(player, pickup.position)) {
        state = PickingUpState{player, pickup.upgrades};
    }
}

} // namespace

void MetaEngine::input_right()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                state.player.x += player_move_increment;
                check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
                    state.player, state.pickup, impl_->state);
            },
            [](auto const&) {}},
        impl_->state);
}

void MetaEngine::input_left()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                state.player.x -= player_move_increment;
                check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
                    state.player, state.pickup, impl_->state);
            },
            [](auto const&) {}},
        impl_->state);
}

void MetaEngine::input_up()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                state.player.y += player_move_increment;
                check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
                    state.player, state.pickup, impl_->state);
            },
            [](auto const&) {}},
        impl_->state);
}

void MetaEngine::input_down()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                state.player.y -= player_move_increment;
                check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
                    state.player, state.pickup, impl_->state);
            },
            [](auto const&) {}},
        impl_->state);
}

} // namespace metacore
