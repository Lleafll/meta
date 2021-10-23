#include "MetaEngine.h"
#include "GameState.h"

namespace metacore {

namespace {

constexpr auto player_move_increment = 5;

} // namespace

GameState MetaEngine::calculate_state() const noexcept
{
    return state_;
}

void MetaEngine::input_right() noexcept
{
    state_.player_position.x += player_move_increment;
}

} // namespace metacore
