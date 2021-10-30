#pragma once

#include "Enemies.h"
#include "Layout.h"
#include "Player.h"
#include "UpgradeChoices.h"
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct PickingUpState final {
    PickingUpState(
        Player player,
        UpgradeChoices const& choices,
        Enemies enemies,
        Layout const& layout = {});

    Player player;
    UpgradeChoices choices;
    Enemies enemies;
    Layout layout;
};

GameState to_game_state(PickingUpState const& state);

} // namespace metacore