#include "MetaEngine.h"
#include "GameState.h"

namespace metacore {

GameState MetaEngine::calculate_state() const noexcept
{
    return state_;
}

void MetaEngine::input_right() noexcept
{
    state_.player_position.x += 1.0;
}

} // namespace metacore
