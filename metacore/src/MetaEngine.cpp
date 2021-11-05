#include "MetaEngine.h"
#include "Clock.h"
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

constexpr auto pickup_distance = 50.0;
constexpr auto enemy_collision_distance = 20.0;

} // namespace

struct MetaEngine::Impl final {
    PickUpGenerator pick_up_generator = {};

  private:
    InternalGameState state_ = [this]() -> InternalGameState {
        auto const layout = Layout{LayoutUpgrade::Arena};
        return {DefaultState{
            Player{PositionD{0, 0}},
            pick_up_generator.generate(layout.bounds()),
            Enemies{},
            layout}};
    }();

  public:
    Clock clock = Clock{state_};

    Impl() = default;
    explicit Impl(InternalGameState state)
        : state_{std::move(state)}, clock{state_}
    {
    }

    [[nodiscard]] InternalGameState& state()
    {
        return state_;
    }

    void state(InternalGameState const& state)
    {
        state_ = state;
        clock.set_state(state_);
    }
};

MetaEngine::MetaEngine() : impl_{std::make_unique<Impl>()}
{
}

MetaEngine::MetaEngine(InternalGameState const& state)
    : impl_{std::make_unique<Impl>(state)}
{
}

MetaEngine::~MetaEngine() = default;

GameState MetaEngine::calculate_state() const
{
    return std::visit(
        [](auto const& state) -> GameState { return to_game_state(state); },
        impl_->state().value);
}

namespace {

bool check_for_enemy_collision(
    PositionD const& player, std::vector<PositionD> const& enemies)
{
    return std::ranges::any_of(
        enemies, [&player](PositionD const& enemy) -> bool {
            return is_within_distance<enemy_collision_distance>(player, enemy);
        });
}

bool check_pickup(Player const& player, Pickup const& pickup)
{
    return is_within_distance<pickup_distance>(
        player.position(), to_position_d(pickup.position));
}

std::vector<Position> to_positions(std::vector<PositionD> const& positions_d)
{
    auto positions = std::vector<Position>{};
    std::ranges::transform(
        positions_d, std::back_inserter(positions), to_position);
    return positions;
}

void advance_and_maybe_transition(DefaultState& state, MetaEngine::Impl& impl)
{
    impl.clock.input();
    if (check_for_enemy_collision(
            state.player.position(), state.enemies.positions())) {
        auto const& player = state.player;
        auto const& enemies = state.enemies;
        auto const& layout = state.layout;
        auto const tiles = layout.tiles();
        impl.state({GameOverState{
            to_position(player.position()),
            state.pickup.position,
            to_positions(enemies.positions()),
            {tiles.begin(), tiles.end()},
            player.texture,
            enemies.texture,
            layout.texture,
            layout.bounds().value_or(LayoutBounds{})}});
    }
}

template<void (Player::*move_direction)()>
void move_and_maybe_transition(DefaultState& state, MetaEngine::Impl& impl)
{
    (state.player.*move_direction)();
    advance_and_maybe_transition(state, impl);
    if (std::holds_alternative<DefaultState>(impl.state().value) &&
        check_pickup(state.player, state.pickup)) {
        impl.state({PickingUpState{
            state.player, state.pickup.upgrades, state.enemies, state.layout}});
    }
}

} // namespace

void MetaEngine::input_right()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                move_and_maybe_transition<&Player::move_right>(state, *impl_);
            },
            [](auto const&) {}},
        impl_->state().value);
}

void MetaEngine::input_left()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                move_and_maybe_transition<&Player::move_left>(state, *impl_);
            },
            [](auto const&) {}},
        impl_->state().value);
}

void MetaEngine::input_up()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                move_and_maybe_transition<&Player::move_up>(state, *impl_);
            },
            [](auto const&) {}},
        impl_->state().value);
}

void MetaEngine::input_down()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                move_and_maybe_transition<&Player::move_down>(state, *impl_);
            },
            [](auto const&) {}},
        impl_->state().value);
}

void MetaEngine::input_attack()
{
    std::visit(
        overloaded{
            [this](DefaultState& state) {
                state.player.attack();
                advance_and_maybe_transition(state, *impl_);
            },
            [](auto const&) {}},
        impl_->state().value);
}

void MetaEngine::input_restart()
{
    impl_ = std::make_unique<Impl>();
}

namespace {

template<PickupUpgrade UpgradeChoices::*member>
void set_upgrade(
    Player& player,
    Enemies& enemies,
    Layout& layout,
    Clock& clock,
    UpgradeChoices const& choices)
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
        case PickupUpgrade::Alien:
            enemies.texture = CharacterTexture::Alien;
            break;
        case PickupUpgrade::Robot:
            enemies.texture = CharacterTexture::Robot;
            break;
        case PickupUpgrade::SpaceStation:
            layout.texture = EnvironmentTexture::SpaceStation;
            break;
        case PickupUpgrade::Castle:
            layout.texture = EnvironmentTexture::Castle;
            break;
        case PickupUpgrade::RealTime:
            clock.set(ClockUpgrade::RealTime);
            break;
        case PickupUpgrade::Superhot:
            break;
    }
}

template<PickupUpgrade UpgradeChoices::*member>
DefaultState apply_upgrade_and_transition(
    PickingUpState& state, Pickup const& next_pickup, Clock& clock)
{
    set_upgrade<member>(
        state.player, state.enemies, state.layout, clock, state.choices);
    return DefaultState{state.player, next_pickup, state.enemies, state.layout};
}

} // namespace

void MetaEngine::select_first_upgrade()
{
    std::visit(
        overloaded{
            [this](PickingUpState& state) {
                impl_->state(
                    {apply_upgrade_and_transition<&UpgradeChoices::first>(
                        state,
                        impl_->pick_up_generator.generate(
                            state.layout.bounds()),
                        impl_->clock)});
            },
            [](auto const&) {}},
        impl_->state().value);
}

void MetaEngine::select_second_upgrade()
{
    std::visit(
        overloaded{
            [this](PickingUpState& state) {
                impl_->state(
                    {apply_upgrade_and_transition<&UpgradeChoices::second>(
                        state,
                        impl_->pick_up_generator.generate(
                            state.layout.bounds()),
                        impl_->clock)});
            },
            [](auto const&) {}},
        impl_->state().value);
}

} // namespace metacore
