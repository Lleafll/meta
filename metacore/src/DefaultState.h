#pragma once

#include "Enemies.h"
#include "Pickup.h"
#include "Player.h"
#include <optional>
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct DefaultState final {
    DefaultState(
        Player const& player, Pickup const& pickup, Enemies const& enemies);

    Player player;
    Pickup pickup;
    Enemies enemies;
};

GameState to_game_state(DefaultState const& state);

} // namespace metacore