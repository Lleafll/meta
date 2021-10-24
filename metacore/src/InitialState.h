#pragma once

#include "Pickup.h"
#include "Player.h"
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct InitialState final {
    InitialState(Player const& player, Pickup const& pickup);

    Player player;
    Pickup pickup;
};

GameState to_game_state(InitialState const& state);

} // namespace metacore
