#pragma once

#include "Position.h"
#include "UpgradeChoices.h"
#include <optional>

namespace metacore {

struct GameState final {
    Position player_position = {};
    std::optional<Position> upgrade_position = {};
    std::optional<UpgradeChoices> upgrade_choices = {};

    friend bool operator==(GameState const&, GameState const&) = default;
};

} // namespace metacore