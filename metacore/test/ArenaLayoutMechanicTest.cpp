#include "ArenaLayoutMechanic.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(ArenaLayoutMechanicTest, ObstaclesAtBoundaries)
{
    auto const arena = ArenaLayoutMechanic{};
    auto const tiles = arena.tiles();
    auto const width = tiles.extent(0);
    auto const height = tiles.extent(1);
    EXPECT_EQ(Tile::Obstacle, tiles(0, 0));
    EXPECT_EQ(Tile::Obstacle, tiles(0, height / 2));
    EXPECT_EQ(Tile::Obstacle, tiles(0, height - 1));
    EXPECT_EQ(Tile::Obstacle, tiles(width - 1, 0));
    EXPECT_EQ(Tile::Obstacle, tiles(width / 2, 0));
    EXPECT_EQ(Tile::Obstacle, tiles(width - 1, height - 1));
}

} // namespace
