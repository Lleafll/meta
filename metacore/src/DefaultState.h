#pragma once

#include "Pickup.h"
#include "Player.h"
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct DefaultState final {
    DefaultState(Player const& player, Pickup const& pickup);

    Player player;
    Pickup pickup;
};

GameState to_game_state(DefaultState const& state);

} // namespace metacore