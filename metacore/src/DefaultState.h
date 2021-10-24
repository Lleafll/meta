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
    DefaultState(Player const& player, Pickup const& pickup);

    Player player;
    Pickup pickup;
    std::optional<Enemies> enemies;
};

GameState to_game_state(DefaultState const& state);

} // namespace metacore