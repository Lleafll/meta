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
    InternalGameState state = {InitialState{
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

bool check_pickup(Player const& player, Pickup const& pickup)
{
    return is_within_distance<pickup_distance>(
        player.position(), pickup.position);
}

template<void (Player::*move_direction)()>
void move_and_maybe_transition(
    InitialState& state, InternalGameState& internal_state)
{
    (state.player.*move_direction)();
    if (check_pickup(state.player, state.pickup)) {
        internal_state.value =
            InitialPickingUpState{state.player, state.pickup.upgrades};
    }
}

template<void (Player::*move_direction)()>
void move_and_maybe_transition(
    DefaultState& state, InternalGameState& internal_state)
{
    (state.player.*move_direction)();
    state.enemies.advance(state.player.position());
    if (check_pickup(state.player, state.pickup)) {
        internal_state.value =
            PickingUpState{state.player, state.pickup.upgrades, state.enemies};
    }
}

} // namespace

void MetaEngine::input_right()
{
    std::visit(
        overloaded{
            [this](InitialState& state) {
                move_and_maybe_transition<&Player::move_right>(
                    state, impl_->state);
            },
            [this](DefaultState& state) {
                move_and_maybe_transition<&Player::move_right>(
                    state, impl_->state);
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::input_left()
{
    std::visit(
        overloaded{
            [this](InitialState& state) {
                move_and_maybe_transition<&Player::move_left>(
                    state, impl_->state);
            },
            [this](DefaultState& state) {
                move_and_maybe_transition<&Player::move_left>(
                    state, impl_->state);
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::input_up()
{
    std::visit(
        overloaded{
            [this](InitialState& state) {
                move_and_maybe_transition<&Player::move_up>(
                    state, impl_->state);
            },
            [this](DefaultState& state) {
                move_and_maybe_transition<&Player::move_up>(
                    state, impl_->state);
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::input_down()
{
    std::visit(
        overloaded{
            [this](InitialState& state) {
                move_and_maybe_transition<&Player::move_down>(
                    state, impl_->state);
            },
            [this](DefaultState& state) {
                move_and_maybe_transition<&Player::move_down>(
                    state, impl_->state);
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::input_attack()
{
    std::visit(
        overloaded{
            [](DefaultState& state) {
                state.player.attack();
                state.enemies.advance(state.player.position());
            },
            [](auto const&) {}},
        impl_->state.value);
}

namespace {

template<PickupUpgrade UpgradeChoices::*member>
void set_upgrade(Player& player, UpgradeChoices const& choices)
{
    auto const choice = choices.*member;
    switch (choice) {
        case PickupUpgrade::Slash:
            player.set_attack(AttackUpgrade::Slash);
            break;
        case PickupUpgrade::Shoot:
            break;
    }
}

template<PickupUpgrade UpgradeChoices::*member>
DefaultState apply_upgrade_and_transition(InitialPickingUpState& state)
{
    set_upgrade<member>(state.player, state.choices);
    return DefaultState{
        state.player,
        Pickup{Position{0, 0}, UpgradeChoices{{}, {}}},
        Enemies{{Position{-100, -100}}}};
}

template<PickupUpgrade UpgradeChoices::*member>
DefaultState apply_upgrade_and_transition(PickingUpState& state)
{
    set_upgrade<member>(state.player, state.choices);
    return DefaultState{
        state.player,
        Pickup{Position{0, 0}, UpgradeChoices{{}, {}}},
        state.enemies};
}

} // namespace

void MetaEngine::select_first_upgrade()
{
    std::visit(
        overloaded{
            [this](InitialPickingUpState& state) {
                impl_->state.value =
                    apply_upgrade_and_transition<&UpgradeChoices::first>(state);
            },
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
            [this](InitialPickingUpState& state) {
                impl_->state.value =
                    apply_upgrade_and_transition<&UpgradeChoices::second>(
                        state);
            },
            [this](PickingUpState& state) {
                impl_->state.value =
                    apply_upgrade_and_transition<&UpgradeChoices::second>(
                        state);
            },
            [](auto const&) {}},
        impl_->state.value);
}

} // namespace metacore
