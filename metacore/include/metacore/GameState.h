#pragma once

#include "PickupUpgrade.h"
#include "Position.h"
#include <optional>
#include <vector>

namespace metacore {

struct GameState final {
    Position player_position = {};
    Position upgrade = {};
    std::optional<std::vector<PickupUpgrade>> upgrade_choices = {};
};

} // namespace metacore