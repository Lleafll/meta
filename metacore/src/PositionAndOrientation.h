#pragma once

#include "Orientation.h"
#include "Position.h"

namespace metacore {

struct PositionAndOrientation final {
    constexpr PositionAndOrientation() = default;
    constexpr PositionAndOrientation(
        Position const& position, Orientation const orientation)
        : position{position}, orientation{orientation}
    {
    }

    Position position = {};
    Orientation orientation = Orientation::Up;
};
} // namespace metacore