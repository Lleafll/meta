#pragma once

#include "Enemies.h"
#include "Layout.h"
#include "Pickup.h"
#include "Player.h"
#include <optional>
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct DefaultState final {
    DefaultState(
        Player player,
        Pickup const& pickup,
        Enemies enemies,
        Layout layout = {});

    Player player;
    Pickup pickup;
    Enemies enemies;
    Layout layout;
};

GameState to_game_state(DefaultState const& state);

} // namespace metacore