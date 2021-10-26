#include "MetaEngine.h"
#include "DefaultState.h"
#include "GameState.h"
#include "InternalGameState.h"
#include "overloaded.h"
#include "PickUpGenerator.h"
#include <algorithm>
#include <utility>
#include <variant>

namespace metacore {

namespace {

constexpr auto pickup_distance = 50;
constexpr auto slash_radius = 100;
constexpr auto enemy_collision_distance = 50;

} // namespace

struct MetaEngine::Impl final {
    Impl() = default;
    explicit Impl(InternalGameState state) : state{std::move(state)}
    {
    }

    PickUpGenerator pick_up_generator = {};
    InternalGameState state = {
        InitialState{Player{Position{0, 0}}, pick_up_generator.generate()}};
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

bool check_for_enemy_collision(
    Position const& player, std::vector<Position> const& enemies)
{
    return std::ranges::any_of(
        enemies, [&player](Position const& enemy) -> bool {
            return is_within_distance<enemy_collision_distance>(player, enemy);
        });
}

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
    if (check_for_enemy_collision(
            state.player.position(), state.enemies.positions())) {
        internal_state.value = GameOverState{
            state.player.position(),
            state.pickup.position,
            state.enemies.positions()};
    } else if (check_pickup(state.player, state.pickup)) {
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

namespace {

void check_if_enemies_got_hit(Position const& player, Enemies& enemies)
{
    auto const& positions = enemies.positions();
    if (positions.empty()) {
        return;
    }
    for (auto i = positions.size(); i > 0; --i) {
        if (is_within_distance<slash_radius>(player, positions[i - 1])) {
            enemies.kill(i - 1);
        }
    }
}

} // namespace

void MetaEngine::input_attack()
{
    std::visit(
        overloaded{
            [](DefaultState& state) {
                state.player.attack();
                check_if_enemies_got_hit(
                    state.player.position(), state.enemies);
                state.enemies.advance(state.player.position());
            },
            [](auto const&) {}},
        impl_->state.value);
}

void MetaEngine::input_restart()
{
    impl_ = std::make_unique<Impl>();
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
            player.set_attack(AttackUpgrade::Shoot);
            break;
    }
}

template<PickupUpgrade UpgradeChoices::*member>
DefaultState apply_upgrade_and_transition(
    InitialPickingUpState& state, Pickup const& next_pickup)
{
    set_upgrade<member>(state.player, state.choices);
    return DefaultState{
        state.player, next_pickup, Enemies{{Position{-100, -100}}}};
}

template<PickupUpgrade UpgradeChoices::*member>
DefaultState
apply_upgrade_and_transition(PickingUpState& state, Pickup const& next_pickup)
{
    set_upgrade<member>(state.player, state.choices);
    return DefaultState{state.player, next_pickup, state.enemies};
}

} // namespace

void MetaEngine::select_first_upgrade()
{
    std::visit(
        overloaded{
            [this](InitialPickingUpState& state) {
                impl_->state.value =
                    apply_upgrade_and_transition<&UpgradeChoices::first>(
                        state, impl_->pick_up_generator.generate());
            },
            [this](PickingUpState& state) {
                impl_->state.value =
                    apply_upgrade_and_transition<&UpgradeChoices::first>(
                        state, impl_->pick_up_generator.generate());
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
                        state, impl_->pick_up_generator.generate());
            },
            [this](PickingUpState& state) {
                impl_->state.value =
                    apply_upgrade_and_transition<&UpgradeChoices::second>(
                        state, impl_->pick_up_generator.generate());
            },
            [](auto const&) {}},
        impl_->state.value);
}

} // namespace metacore
