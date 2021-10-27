#pragma once

#include "Position.h"
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

    friend bool operator==(GameState const&, GameState const&) = default;
};

} // namespace metacore