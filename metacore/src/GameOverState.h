#pragma once

#include "Enemies.h"
#include "Layout.h"
#include "Pickup.h"
#include "Player.h"
#include "Position.h"
#include <vector>
namespace metacore {
struct GameState;
} // namespace metacore

namespace metacore {

struct GameOverState final {
    Position player_position;
    Position pickup_position;
    std::vector<Position> enemy_positions;
    std::vector<Tile> tiles = {};
    CharacterTexture player_texture = CharacterTexture::None;
    CharacterTexture enemies_texture = CharacterTexture::None;
    EnvironmentTexture environment_texture = EnvironmentTexture::None;
};

GameState to_game_state(GameOverState const& state);

} // namespace metacore