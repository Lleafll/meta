#pragma once

#include "CharacterTexture.h"
#include "EnvironmentTexture.h"
#include "Position.h"
#include "Tile.h"
#include "UpgradeChoices.h"
#include <optional>
#include <vector>

namespace metacore {

enum class GameProgress { Running, Lost };

struct GameState final {
    Position player_position = {};
    std::optional<Position> upgrade_position = {};
    std::optional<UpgradeChoices> upgrade_choices = {};
    bool slash_attack = false;
    std::vector<Position> enemy_positions = {};
    GameProgress progress = GameProgress::Running;
    std::optional<std::vector<Position>> projectiles = {};
    std::vector<Tile> tiles = {};
    CharacterTexture player_texture = CharacterTexture::None;
    CharacterTexture enemies_texture = CharacterTexture::None;
    EnvironmentTexture environment_texture = EnvironmentTexture::None;

    bool operator==(GameState const&) const = default;
};

} // namespace metacore