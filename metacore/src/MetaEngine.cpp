#include "MetaEngine.h"
#include "DefaultState.h"
#include "GameState.h"
#include "InternalGameState.h"
#include "overloaded.h"
#include <variant>

namespace metacore {

namespace {

constexpr auto pickup_distance = 50;

} // namespace

struct MetaEngine::Impl final {
    InternalGameState state = {DefaultState{
        Player{Position{0, 0}},
        Pickup{
            Position{200, 50},
            UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot}}}};
};

MetaEngine::MetaEngine() : impl_{std::make_unique<Impl>()}
{
}

MetaEngine::MetaEngine(InternalGameState const& state)
    : impl_{std::make_unique<Impl>(Impl{state})}
{
}

MetaEngine::~MetaEngine() = default;

GameState MetaEngine::calculate_state() const
{
    return std::visit(
        [](auto const& state) -> GameState { return to_game_state(state); },
        impl_->state.value);
}

namespace {

void check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
    Player const& player,
    Pickup const& pickup,
    std::variant<DefaultState, PickingUpState>& state)
{
    if (is_within_distance<pickup_distance>(
            player.position(), pickup.position)) {
        state = PickingUpState{player, pickup.upgrades};
    }
}

} // namespace

void MetaEngine::input_right()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                state.player.move_right();
                check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
                    state.player, state.pickup, impl_->state.value);
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::input_left()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                state.player.move_left();
                check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
                    state.player, state.pickup, impl_->state.value);
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::input_up()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                state.player.move_up();
                check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
                    state.player, state.pickup, impl_->state.value);
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::input_down()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                state.player.move_down();
                check_if_upgrade_is_hit_and_reset_upgrade_accordingly(
                    state.player, state.pickup, impl_->state.value);
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::input_attack()
{
    std::visit(
        overloaded{
            [](DefaultState& state) { state.player.attack(); },
            [](auto const&) {}},
        impl_->state.value);
}

namespace {

template<PickupUpgrade UpgradeChoices::*member>
DefaultState apply_upgrade_and_transition(PickingUpState& state)
{
    auto const choice = state.choices.*member;
    switch (choice) {
        case PickupUpgrade::Slash:
            state.player.set_attack(AttackUpgrade::Slash);
            break;
        case PickupUpgrade::Shoot:
            break;
    }
    return DefaultState{
        state.player, Pickup{Position{0, 0}, UpgradeChoices{{}, {}}}};
}

} // namespace

void MetaEngine::select_first_upgrade()
{
    std::visit(
        overloaded{
            [this](PickingUpState& state) {
                impl_->state.value =
                    apply_upgrade_and_transition<&UpgradeChoices::first>(state);
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::select_second_upgrade()
{
    std::visit(
        overloaded{
            [this](PickingUpState& state) {
                impl_->state.value =
                    apply_upgrade_and_transition<&UpgradeChoices::second>(
                        state);
            },
            [](auto const&) {}},
        impl_->state.value);
}

} // namespace metacore
