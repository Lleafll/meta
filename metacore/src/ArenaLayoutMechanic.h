#pragma once

#include "mdspan.h"
#include "Tile.h"
#include <array>

namespace metacore {

class ArenaLayoutMechanic final {
    static constexpr auto height = 10;
    static constexpr auto width = 10;

  public:
    constexpr ArenaLayoutMechanic()
    {
        auto const tiles_2D = tiles();
        for (auto i = 0; i < tiles_2D.extent(0); ++i) {
            tiles_2D(i, 0) = Tile::Obstacle;
            tiles_2D(i, tiles_2D.extent(1) - 1) = Tile::Obstacle;
        }
        for (auto i = 0; i < tiles_2D.extent(1); ++i) {
            tiles_2D(0, i) = Tile::Obstacle;
            tiles_2D(tiles_2D.extent(0) - 1, i) = Tile::Obstacle;
        }
    }

    [[nodiscard]] constexpr mdspan<Tile const, width, height> tiles() const
    {
        return mdspan<Tile const, width, height>{tiles_.data()};
    }

  private:
    std::array<Tile, width* height> tiles_ = {Tile::Empty};

    [[nodiscard]] constexpr mdspan<Tile, width, height> tiles()
    {
        return mdspan<Tile, width, height>{tiles_.data()};
    }
};

} // namespace metacore