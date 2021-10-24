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

TEST(PlayerTest, StartSlashAttack)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    EXPECT_TRUE(player.is_slashing());
}

TEST(PlayerTest, EndSlashAttackAfterMoveUp)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    player.move_up();
    EXPECT_FALSE(player.is_slashing());
}

TEST(PlayerTest, EndSlashAttackAfterMoveDown)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    player.move_down();
    EXPECT_FALSE(player.is_slashing());
}

TEST(PlayerTest, EndSlashAttackAfterMoveRight)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    player.move_right();
    EXPECT_FALSE(player.is_slashing());
}

TEST(PlayerTest, EndSlashAttackAfterMoveLeft)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    player.move_left();
    EXPECT_FALSE(player.is_slashing());
}

} // namespace
