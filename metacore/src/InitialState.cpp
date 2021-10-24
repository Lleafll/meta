#include "InitialState.h"
#include "GameState.h"

namespace metacore {

InitialState::InitialState(Player const& player, Pickup const& pickup)
    : player{player}, pickup{pickup}
{
}

GameState to_game_state(InitialState const& state)
{
    return {state.player.position(), state.pickup.position};
}

} // namespace metacore