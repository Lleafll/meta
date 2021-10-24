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

} // namespace