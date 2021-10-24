#pragma once

#include "GameState.h"
#include "Pickup.h"
#include "Position.h"

namespace metacore {

struct DefaultState final {
    Position player = {0, 0};
    Pickup pickup = {{200, 50}, {PickupUpgrade::Slash, PickupUpgrade::Shoot}};
};

constexpr GameState to_game_state(DefaultState const& state)
{
    return {state.player, state.pickup.position, std::nullopt};
}

} // namespace metacore