#pragma once

#include "Position.h"
#include "UpgradeChoices.h"

namespace metacore {

struct Pickup final {
    constexpr Pickup(Position const position, UpgradeChoices const& upgrades)
        : position{position}, upgrades{upgrades}
    {
    }

    Position position;
    UpgradeChoices upgrades;
};

} // namespace metacore
