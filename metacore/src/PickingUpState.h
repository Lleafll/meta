#pragma once

#include "Player.h"
#include "UpgradeChoices.h"
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct PickingUpState final {
    PickingUpState(Player const& player, UpgradeChoices const& choices);

    Player player;
    UpgradeChoices choices;
};

GameState to_game_state(PickingUpState const& state);

} // namespace metacore