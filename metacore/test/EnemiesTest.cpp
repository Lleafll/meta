#include "Enemies.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(EnemiesTest, AdvanceMovesEnemies)
{
    auto const player_position = PositionD{50, 50};
    auto enemies = Enemies{{{0, 0}, {200, 500}}};
    enemies.advance(player_position, std::chrono::milliseconds{1000});
    auto const& positions = enemies.positions();
    EXPECT_EQ(2, positions.size());
    auto const& enemy1 = positions[0];
    EXPECT_EQ(0, enemy1.x);
    EXPECT_GT(enemy1.y, 0);
    auto const& enemy2 = positions[1];
    EXPECT_EQ(200, enemy2.x);
    EXPECT_LT(enemy2.y, 500);
}

TEST(EnemiesTest, AutomaticallySpawnAdditionalEnemies)
{
    auto enemies = Enemies{};
    for (auto i = 0; i < 4; ++i) {
        enemies.advance(PositionD{0, 0});
    }
    EXPECT_GT(enemies.positions().size(), 0);
}

} // namespace