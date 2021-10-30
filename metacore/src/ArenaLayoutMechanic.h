#pragma once

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
        for (auto i = -width / 2; i < width / 2; ++i) {
            tiles_[index] = Tile{{i * tile_size, 0}, TileType::Obstacle};
            ++index;
            tiles_[index] = Tile{
                {i * tile_size, (height / 2 - 1) * tile_size},
                TileType::Obstacle};
            ++index;
        }
        for (auto i = -height / 2 - 1; i < height / 2 - 1; ++i) {
            tiles_[index] = Tile{{0, i * tile_size}, TileType::Obstacle};
            ++index;
            tiles_[index] = Tile{
                {(width / 2 - 1) * tile_size, i * tile_size},
                TileType::Obstacle};
            ++index;
        }
    }

    [[nodiscard]] constexpr std::array<Tile, tile_count> const& tiles() const
    {
        return tiles_;
    }

  private:
    std::array<Tile, tile_count> tiles_ = {};
};

} // namespace metacore