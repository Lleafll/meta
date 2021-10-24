#pragma once

#include "GameState.h"
#include "Position.h"
#include "UpgradeChoices.h"

namespace metacore {

struct PickingUpState final {
    constexpr PickingUpState(
        Position const& player, UpgradeChoices const& choices)
        : player{player}, choices{choices}
    {
    }

    Position player;
    UpgradeChoices choices;
};

constexpr GameState to_game_state(PickingUpState const& state)
{
    return {state.player, std::nullopt, state.choices};
}

} // namespace metacore