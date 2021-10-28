#include "Player.h"
#include <format>
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

TEST(PlayerTest, TargetIsHitWorksWithShoot)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Shoot);
    EXPECT_FALSE(player.target_is_hit({0, 0}));
    player.attack();
    EXPECT_TRUE(player.target_is_hit({0, 0}));
}

TEST(PlayerTest, TargetIsHitWorksWithSlash)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    EXPECT_FALSE(player.target_is_hit({0, 0}));
    player.attack();
    EXPECT_TRUE(player.target_is_hit({0, 0}));
}

TEST(PlayerTest, ShootAttackObeysDirection)
{
    auto player = Player{PositionAndOrientation{{0, 0}, Orientation::Down}};
    player.set_attack(AttackUpgrade::Shoot);
    player.attack();
    player.move_up();
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const projectile = projectiles->front();
    auto const expected = Position{0, -50};
    EXPECT_EQ(expected, projectile)
        << std::format("{},{}", projectile.x, projectile.y);
}

TEST(PlayerTest, ShootAttackObeysDirectionFromMovingUp)
{
    auto player = Player{PositionAndOrientation{{0, 0}, Orientation::Down}};
    player.set_attack(AttackUpgrade::Shoot);
    player.move_up();
    player.attack();
    player.move_up();
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const projectile = projectiles->front();
    auto const expected = Position{0, 50 + 50};
    EXPECT_EQ(expected, projectile)
        << std::format("{},{}", projectile.x, projectile.y);
}

TEST(PlayerTest, ShootAttackObeysDirectionFromMovingLeft)
{
    auto player = Player{PositionAndOrientation{{0, 0}, Orientation::Down}};
    player.set_attack(AttackUpgrade::Shoot);
    player.move_right();
    player.attack();
    player.move_up();
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const projectile = projectiles->front();
    auto const expected = Position{50 + 50, 0};
    EXPECT_EQ(expected, projectile)
        << std::format("{},{}", projectile.x, projectile.y);
}

TEST(PlayerTest, ShootAttackObeysDirectionFromMovingRight)
{
    auto player = Player{PositionAndOrientation{{0, 0}, Orientation::Down}};
    player.set_attack(AttackUpgrade::Shoot);
    player.move_left();
    player.attack();
    player.move_up();
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const projectile = projectiles->front();
    auto const expected = Position{-50 - 50, 0};
    EXPECT_EQ(expected, projectile)
        << std::format("{},{}", projectile.x, projectile.y);
}

TEST(PlayerTest, ShootAttackObeysDirectionFromMovingDown)
{
    auto player = Player{PositionAndOrientation{{0, 0}, Orientation::Up}};
    player.set_attack(AttackUpgrade::Shoot);
    player.move_down();
    player.attack();
    player.move_up();
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const expected = Position{0, -50 - 50};
    EXPECT_EQ(expected, projectiles->front());
}

} // namespace
