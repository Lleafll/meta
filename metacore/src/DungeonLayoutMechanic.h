#pragma once

#include "LayoutBounds.h"
#include "PositionD.h"
#include "Tile.h"
#include <random>
#include <vector>

namespace metacore {

class DungeonLayoutMechanic final {
    static constexpr auto height = 12;
    static constexpr auto width = 12;

  public:
    static LayoutBounds const& bounds();

    DungeonLayoutMechanic();

    [[nodiscard]] std::vector<Tile> const& tiles() const;
    bool check_for_transition(PositionD const& player_position);

  private:
    static constexpr auto tile_size = 50;
    static constexpr LayoutBounds bounds_ = {
        -(width * tile_size) / 2,
        (width * tile_size) / 2,
        -(height* tile_size) / 2,
        (height * tile_size) / 2};

    std::vector<Tile> tiles_ = {};
    std::mt19937 random_engine_ = std::mt19937{std::random_device{}()};
};

} // namespace metacore