#include "DefaultState.h"
#include "GameState.h"

namespace metacore {

DefaultState::DefaultState(Player const& player, Pickup const& pickup)
    : player{player}, pickup{pickup}
{
}

GameState to_game_state(DefaultState const& state)
{
    return {state.player.position(), state.pickup.position, std::nullopt};
}

} // namespace metacore