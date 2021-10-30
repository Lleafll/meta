#pragma once

#include "Position.h"

namespace metacore {

enum class TileType { Obstacle };

struct Tile final {
    Position position = {};
    TileType type = TileType::Obstacle;

    bool operator==(Tile const&) const = default;
};

} // namespace metacore