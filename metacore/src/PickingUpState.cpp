#include "PickingUpState.h"
#include "GameState.h"

namespace metacore {

PickingUpState::PickingUpState(
    Player const& player, UpgradeChoices const& choices)
    : player{player}, choices{choices}
{
}

GameState to_game_state(PickingUpState const& state)
{
    return {state.player.position(), std::nullopt, state.choices};
}

} // namespace metacore