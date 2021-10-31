#include "GameOverState.h"
#include "GameState.h"

namespace metacore {

GameState to_game_state(GameOverState const& state)
{
    return {
        state.player_position,
        state.pickup_position,
        std::nullopt,
        std::nullopt,
        state.enemy_positions,
        GameProgress::Lost};
}

} // namespace metacore
