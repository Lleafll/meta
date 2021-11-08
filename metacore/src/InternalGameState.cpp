#include "InternalGameState.h"
#include "GameState.h"
#include "overloaded.h"

namespace metacore {

namespace {

constexpr auto pickup_distance = 50.0;
constexpr auto enemy_collision_distance = 20.0;

} // namespace

InternalGameState::InternalGameState()
    : state_{[this]() -> DefaultState {
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

struct StateAdvancer final {
    std::chrono::microseconds diff;

    void operator()(DefaultState& state) const
    {
        state.player.advance(state.layout.tiles(), diff);
        state.layout.check_for_transition(
            state.player, state.enemies.positions());
        kill_enemies_that_got_hit(state.player, state.enemies);
        state.enemies.advance(state.player.position(), diff);
    }

    void operator()(PickingUpState&) const
    {
    }

    void operator()(GameOverState&) const
    {
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

} // namespace metacore