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
        constexpr auto width_begin = -width / 2;
        constexpr auto width_end = width / 2;
        constexpr auto height_begin = -height / 2;
        constexpr auto height_end = height / 2;
        for (auto i = width_begin; i < width_end; ++i) {
            tiles_[index] = Tile{
                {i * tile_size, height_begin * tile_size}, TileType::Obstacle};
            ++index;
            tiles_[index] = Tile{
                {i * tile_size, (height_end - 1) * tile_size},
                TileType::Obstacle};
            ++index;
        }
        for (auto i = height_begin + 1; i < height_end - 1; ++i) {
            tiles_[index] = Tile{
                {width_begin * tile_size, i * tile_size}, TileType::Obstacle};
            ++index;
            tiles_[index] = Tile{
                {(width_end - 1) * tile_size, i * tile_size},
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