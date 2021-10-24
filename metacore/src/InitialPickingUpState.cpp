#include "InitialPickingUpState.h"
#include "GameState.h"

namespace metacore {

InitialPickingUpState::InitialPickingUpState(
    Player const& player, UpgradeChoices const& choices)
    : player{player}, choices{choices}
{
}

GameState to_game_state(InitialPickingUpState const& state)
{
    return {state.player.position(), std::nullopt, state.choices};
}

} // namespace metacore
