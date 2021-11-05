#pragma once

#include "Orientation.h"
#include "PositionD.h"

namespace metacore {

struct PositionDAndOrientation final {
    constexpr PositionDAndOrientation() = default;
    constexpr PositionDAndOrientation(
        PositionD const& position, Orientation const orientation)
        : position{position}, orientation{orientation}
    {
    }

    PositionD position = {};
    Orientation orientation = Orientation::Up;
};
} // namespace metacore