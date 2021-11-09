#include "InternalGameState.h"
#include "GameState.h"

#include <utility>

namespace metacore {

namespace {

constexpr auto pickup_distance = 50.0;
constexpr auto enemy_collision_distance = 20.0;

} // namespace

InternalGameState::InternalGameState()
    : pickup_generator_{},
      state_{[this]() -> DefaultState {
          auto const layout = Layout{LayoutUpgrade::Arena};
          return {
              Player{PositionD{0, 0}},
              pickup_generator_.generate(layout.bounds()),
              Enemies{},
              layout};
      }()},
      clock_{*this}
{
}

InternalGameState::InternalGameState(
    std::variant<DefaultState, PickingUpState, GameOverState> state)
    : state_{std::move(state)}, clock_{*this}
{
}

InternalGameState::InternalGameState(InternalGameState&& other) noexcept
    : pickup_generator_{other.pickup_generator_},
      state_{std::move(other.state_)},
      clock_{std::move(other.clock_)}
{
    clock_.set_state(*this);
}

InternalGameState&
InternalGameState::operator=(InternalGameState&& other) noexcept
{
    pickup_generator_ = other.pickup_generator_;
    state_ = std::move(other.state_);
    clock_ = std::move(other.clock_);
    clock_.set_state(*this);
    return *this;
}

namespace {
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

bool check_for_enemy_collision(
    PositionD const& player, std::vector<PositionD> const& enemies)
{
    return std::ranges::any_of(
        enemies, [&player](PositionD const& enemy) -> bool {
            return is_within_distance(enemy_collision_distance, player, enemy);
        });
}

std::vector<Position> to_positions(std::vector<PositionD> const& positions_d)
{
    auto positions = std::vector<Position>{};
    std::ranges::transform(
        positions_d, std::back_inserter(positions), to_position);
    return positions;
}

GameOverState transition_to_game_over(DefaultState const& state)
{
    auto const& player = state.player;
    auto const& enemies = state.enemies;
    auto const& layout = state.layout;
    auto const tiles = layout.tiles();
    return {
        to_position(player.position()),
        state.pickup.position,
        to_positions(enemies.positions()),
        {tiles.begin(), tiles.end()},
        player.texture,
        enemies.texture,
        layout.texture,
        layout.bounds().value_or(LayoutBounds{})};
}

PickingUpState transition_to_picking_up(DefaultState const& state)
{
    return {state.player, state.pickup.upgrades, state.enemies, state.layout};
}

bool check_pickup(Player const& player, Pickup const& pickup)
{
    return is_within_distance(
        pickup_distance, player.position(), to_position_d(pickup.position));
}

struct StateAdvancer final {
    using ReturnType = std::optional<
        std::variant<DefaultState, PickingUpState, GameOverState>>;

    std::chrono::microseconds diff;

    ReturnType operator()(DefaultState& state) const
    {
        state.player.advance(state.layout.tiles(), diff);
        state.layout.check_for_transition(
            state.player, state.enemies.positions());
        kill_enemies_that_got_hit(state.player, state.enemies);
        state.enemies.advance(state.player.position(), diff);
        if (check_for_enemy_collision(
                state.player.position(), state.enemies.positions())) {
            return transition_to_game_over(state);
        }
        if (check_pickup(state.player, state.pickup)) {
            return transition_to_picking_up(state);
        }
        return std::nullopt;
    }

    ReturnType operator()(PickingUpState&) const
    {
        return {};
    }

    ReturnType operator()(GameOverState&) const
    {
        return {};
    }
};

} // namespace

void InternalGameState::advance(std::chrono::microseconds const diff)
{
    std::visit(StateAdvancer{diff}, state_);
}

GameState InternalGameState::to_game_state() const
{
    return std::visit(
        [](auto const& state) -> GameState {
            return metacore::to_game_state(state);
        },
        state_);
}

void InternalGameState::move_player(Orientation const orientation)
{
    if (auto* const state = std::get_if<DefaultState>(&state_);
        state != nullptr) {
        auto& player = state->player;
        switch (orientation) {
            case Orientation::Up:
                player.move_up();
                break;
            case Orientation::Down:
                player.move_down();
                break;
            case Orientation::Left:
                player.move_left();
                break;
            case Orientation::Right:
                player.move_right();
                break;
        }
        clock_.input();
    }
}

void InternalGameState::attack_player()
{
}

void InternalGameState::stop_player()
{
    if (auto* const state = std::get_if<DefaultState>(&state_);
        state != nullptr) {
        state->player.stop();
        clock_.stop();
    }
}

namespace {
void set_upgrade(
    Player& player,
    Enemies& enemies,
    Layout& layout,
    Clock& clock,
    PickupUpgrade const upgrade)
{
    switch (upgrade) {
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
            clock.set(ClockUpgrade::Superhot);
            break;
    }
}

DefaultState
transition_to_default(PickingUpState& state, PickUpGenerator& generator)
{
    return {
        state.player,
        generator.generate(state.layout.bounds()),
        state.enemies,
        state.layout};
}

} // namespace

void InternalGameState::select_upgrade(UpgradeSelection const selection)
{
    if (auto* const state = std::get_if<PickingUpState>(&state_);
        state != nullptr) {
        auto const& choices = state->choices;
        auto const choice = selection == UpgradeSelection::First
            ? choices.first
            : choices.second;
        set_upgrade(
            state->player, state->enemies, state->layout, clock_, choice);
        state_ = transition_to_default(*state, pickup_generator_);
    }
}

} // namespace metacore