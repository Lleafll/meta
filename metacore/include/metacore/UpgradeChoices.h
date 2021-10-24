#pragma once

#include "PickupUpgrade.h"

namespace metacore {

struct UpgradeChoices final {
    constexpr UpgradeChoices(
        PickupUpgrade const first, PickupUpgrade const second)
        : first{first}, second{second}
    {
    }

    PickupUpgrade first;
    PickupUpgrade second;

    friend bool operator==(UpgradeChoices const&, UpgradeChoices const&) = default;
};

} // namespace metacore