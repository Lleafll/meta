#pragma once

#include "Position.h"

namespace metacore {

struct GameState final {
    Position player_position = {};
    Position upgrade = {};
};

} // namespace metacore