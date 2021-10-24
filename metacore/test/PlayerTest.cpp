#include "Player.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(PlayerTest, MoveUp)
{
    auto player = Player{{0, 0}};
    player.move_up();
    auto const position = player.position();
    EXPECT_EQ(0, position.x);
    EXPECT_GT(position.y, 0);
}

TEST(PlayerTest, MoveDown)
{
    auto player = Player{{0, 0}};
    player.move_down();
    auto const position = player.position();
    EXPECT_EQ(0, position.x);
    EXPECT_LT(position.y, 0);
}

TEST(PlayerTest, MoveRight)
{
    auto player = Player{{0, 0}};
    player.move_right();
    auto const position = player.position();
    EXPECT_GT(position.x, 0);
    EXPECT_EQ(0, position.y);
}

TEST(PlayerTest, MoveLeft)
{
    auto player = Player{{0, 0}};
    player.move_left();
    auto const position = player.position();
    EXPECT_LT(position.x, 0);
    EXPECT_EQ(0, position.y);
}

} // namespace
