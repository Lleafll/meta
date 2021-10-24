#pragma once

#include "Player.h"
#include "UpgradeChoices.h"
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct InitialPickingUpState final {
    InitialPickingUpState(Player const& player, UpgradeChoices const& choices);

    Player player;
    UpgradeChoices choices;
};

GameState to_game_state(InitialPickingUpState const& state);

} // namespace metacore