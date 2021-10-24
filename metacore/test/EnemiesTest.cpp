#include "Enemies.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(EnemiesTest, AdvanceMovesEnemies)
{
    auto const player_position = Position{50, 50};
    auto enemies = Enemies{{{0, 0}, {200, 500}}};
    enemies.advance(player_position);
    auto const expected = std::vector<Position>{{0, 25}, {200, 475}};
    EXPECT_EQ(expected, enemies.positions());
}

TEST(EnemiesTest, AutomaticallySpawnAdditionalEnemies)
{
    auto enemies = Enemies{};
    for (auto i = 0; i < 4; ++i) {
        enemies.advance(Position{0, 0});
    }
    EXPECT_EQ(1, enemies.positions().size());
}

} // namespace