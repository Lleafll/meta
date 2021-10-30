#pragma once

#include "LayoutBounds.h"
#include "Tile.h"
#include <array>
#include <random>

namespace metacore {

class DungeonLayoutMechanic final {
    static constexpr auto height = 12;
    static constexpr auto width = 12;
    static constexpr auto tile_count = 2 * height + 2 * width - 4 + 1;

  public:
    DungeonLayoutMechanic();

    [[nodiscard]] std::array<Tile, tile_count> const& tiles() const;
    [[nodiscard]] static LayoutBounds const& bounds();
    bool check_for_transition(Position const& player_position);

  private:
    static constexpr auto tile_size = 50;
    static constexpr LayoutBounds bounds_ = {
        -width / 2 * tile_size,
        width / 2 * tile_size,
        -height / 2 * tile_size,
        height / 2 * tile_size};

    std::array<Tile, tile_count> tiles_ = {};
    std::mt19937 random_engine_ = std::mt19937{std::random_device{}()};
};

} // namespace metacore