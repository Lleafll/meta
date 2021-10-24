#pragma once

#include "Enemies.h"
#include "Player.h"
#include "UpgradeChoices.h"
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct PickingUpState final {
    PickingUpState(
        Player const& player, UpgradeChoices const& choices, Enemies enemies);

    Player player;
    UpgradeChoices choices;
    Enemies enemies;
};

GameState to_game_state(PickingUpState const& state);

} // namespace metacore