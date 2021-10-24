#pragma once

#include "Position.h"
#include <vector>
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct GameOverState final {
    Position player_position;
    Position pickup_position;
    std::vector<Position> enemy_positions;
};

GameState to_game_state(GameOverState const& state);

} // namespace metacore