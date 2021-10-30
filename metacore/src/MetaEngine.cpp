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
constexpr auto enemy_collision_distance = 50;

} // namespace

struct MetaEngine::Impl final {
    Impl() = default;
    explicit Impl(InternalGameState state) : state{std::move(state)}
    {
    }

    PickUpGenerator pick_up_generator = {};
    InternalGameState state = [this]() -> InternalGameState {
        auto const layout = Layout{LayoutUpgrade::Arena};
        return {DefaultState{
            Player{Position{0, 0}},
            pick_up_generator.generate(layout.bounds()),
            Enemies{},
            layout}};
    }();
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

void kill_enemies_that_got_hit(Player const& player, Enemies& enemies)
{
    auto const& positions = enemies.positions();
    if (positions.empty()) {
        return;
    }
    for (auto i = positions.size(); i > 0; --i) {
        if (player.target_is_hit(positions[i - 1])) {
            enemies.kill(i - 1);
        }
    }
}

void advance_and_maybe_transition(
    DefaultState& state, InternalGameState& internal_state)
{
    kill_enemies_that_got_hit(state.player, state.enemies);
    state.enemies.advance(state.player.position());
    if (check_for_enemy_collision(
            state.player.position(), state.enemies.positions())) {
        internal_state.value = GameOverState{
            state.player.position(),
            state.pickup.position,
            state.enemies.positions()};
    }
}

template<void (Player::*move_direction)(std::span<Tile const>)>
void move_and_maybe_transition(
    DefaultState& state, InternalGameState& internal_state)
{
    (state.player.*move_direction)(state.layout.tiles());
    if (state.layout.check_for_transition(
            state.player, state.enemies.positions())) {
        return;
    }
    advance_and_maybe_transition(state, internal_state);
    if (std::holds_alternative<DefaultState>(internal_state.value) &&
        check_pickup(state.player, state.pickup)) {
        internal_state.value = PickingUpState{
            state.player, state.pickup.upgrades, state.enemies, state.layout};
    }
}

} // namespace

void MetaEngine::input_right()
{
    std::visit(
        [this]<typename T>(T& state) {
            if constexpr (std::is_same_v<T, DefaultState>) {
                move_and_maybe_transition<&Player::move_right>(
                    state, impl_->state);
            }
        },
        impl_->state.value);
}

void MetaEngine::input_left()
{
    std::visit(
        overloaded{
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
            [this](DefaultState& state) {
                state.player.attack();
                advance_and_maybe_transition(state, impl_->state);
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
void set_upgrade(Player& player, Layout& layout, UpgradeChoices const& choices)
{
    auto const choice = choices.*member;
    switch (choice) {
        case PickupUpgrade::Slash:
            player.set_attack(AttackUpgrade::Slash);
            break;
        case PickupUpgrade::Shoot:
            player.set_attack(AttackUpgrade::Shoot);
            break;
        case PickupUpgrade::Dungeon:
            layout.set_upgrade(LayoutUpgrade::Dungeon);
            break;
        case PickupUpgrade::OpenWorld:
            layout.set_upgrade(LayoutUpgrade::OpenWorld);
            break;
        case PickupUpgrade::Car:
            player.texture = CharacterTexture::Car;
            break;
        case PickupUpgrade::Knight:
            player.texture = CharacterTexture::Knight;
            break;
    }
}

template<PickupUpgrade UpgradeChoices::*member>
DefaultState
apply_upgrade_and_transition(PickingUpState& state, Pickup const& next_pickup)
{
    set_upgrade<member>(state.player, state.layout, state.choices);
    return DefaultState{state.player, next_pickup, state.enemies, state.layout};
}

} // namespace

void MetaEngine::select_first_upgrade()
{
    std::visit(
        overloaded{
            [this](PickingUpState& state) {
                impl_->state.value =
                    apply_upgrade_and_transition<&UpgradeChoices::first>(
                        state,
                        impl_->pick_up_generator.generate(
                            state.layout.bounds()));
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
                        state,
                        impl_->pick_up_generator.generate(
                            state.layout.bounds()));
            },
            [](auto const&) {}},
        impl_->state.value);
}

} // namespace metacore
