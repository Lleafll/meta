#pragma once

#include "LayoutBounds.h"
#include "Tile.h"
#include <array>

namespace metacore {

class ArenaLayoutMechanic final {
    static constexpr auto height = 10;
    static constexpr auto width = 10;
    static constexpr auto tile_count = 2 * height + 2 * width - 4;
    static constexpr auto tile_size = 50;

  public:
    constexpr ArenaLayoutMechanic()
    {
        auto index = std::size_t{0};
        auto const& [left, right, bottom, top] = bounds_;
        for (auto i = left; i < right; i += tile_size) {
            tiles_[index] = Tile{{i, bottom}, TileType::Obstacle};
            ++index;
            tiles_[index] = Tile{{i, top - tile_size}, TileType::Obstacle};
            ++index;
        }
        for (auto i = bottom + tile_size; i < top - tile_size; i += tile_size) {
            tiles_[index] = Tile{{left, i}, TileType::Obstacle};
            ++index;
            tiles_[index] = Tile{{right - tile_size, i}, TileType::Obstacle};
            ++index;
        }
    }

    [[nodiscard]] constexpr std::array<Tile, tile_count> const& tiles() const
    {
        return tiles_;
    }

    [[nodiscard]] static constexpr LayoutBounds const& bounds()
    {
        return bounds_;
    }

  private:
    static constexpr LayoutBounds bounds_ = {
        -width / 2 * tile_size,
        width / 2 * tile_size,
        -height / 2 * tile_size,
        height / 2 * tile_size};
    std::array<Tile, tile_count> tiles_ = {};
};

} // namespace metacore