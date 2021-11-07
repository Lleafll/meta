#include "Player.h"
#include <array>
#include <format>
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(PlayerTest, MoveUp)
{
    auto player = Player{{0, 0}};
    player.move_up();
    player.advance({});
    auto const position = player.position();
    EXPECT_EQ(0, position.x);
    EXPECT_GT(position.y, 0);
}

TEST(PlayerTest, MoveDown)
{
    auto player = Player{{0, 0}};
    player.move_down();
    player.advance({});
    auto const position = player.position();
    EXPECT_EQ(0, position.x);
    EXPECT_LT(position.y, 0);
}

TEST(PlayerTest, MoveRight)
{
    auto player = Player{{0, 0}};
    player.move_right();
    player.advance({});
    auto const position = player.position();
    EXPECT_GT(position.x, 0);
    EXPECT_EQ(0, position.y);
}

TEST(PlayerTest, MoveLeft)
{
    auto player = Player{{0, 0}};
    player.move_left();
    player.advance({});
    auto const position = player.position();
    EXPECT_LT(position.x, 0);
    EXPECT_EQ(0, position.y);
}

TEST(PlayerTest, MoveUpConsidersObstacles)
{
    constexpr auto tick = std::chrono::milliseconds{10};
    auto const environment = std::array{Tile{{0, 50}, TileType::Obstacle}};
    auto player = Player{{0, 0}};
    player.move_up();
    player.advance(environment, tick);
    auto const position = player.position();
    EXPECT_EQ((PositionD{0, 0}), position);
}

TEST(PlayerTest, MoveUpDoesWorkWithStairs)
{
    auto const environment = std::array{Tile{{0, 50}, TileType::Stairs}};
    auto player = Player{{0, 0}};
    player.move_up();
    player.advance(environment);
    auto const position = player.position();
    EXPECT_EQ(0, position.x);
    EXPECT_GT(position.y, 0);
}

TEST(PlayerTest, MoveDownConsidersObstacles)
{
    constexpr auto tick = std::chrono::milliseconds{10};
    auto const environment = std::array{Tile{{0, -50}, TileType::Obstacle}};
    auto player = Player{{0, 0}};
    player.move_down();
    player.advance(environment, tick);
    auto const position = player.position();
    EXPECT_EQ((PositionD{0, 0}), position);
}

TEST(PlayerTest, MoveRightConsidersObstacles)
{
    constexpr auto tick = std::chrono::milliseconds{10};
    auto const environment = std::array{Tile{{50, 0}, TileType::Obstacle}};
    auto player = Player{{0, 0}};
    player.move_right();
    player.advance(environment, tick);
    auto const position = player.position();
    EXPECT_EQ((PositionD{0, 0}), position);
}

TEST(PlayerTest, MoveLeftConsidersObstacles)
{
    constexpr auto tick = std::chrono::milliseconds{10};
    auto const environment = std::array{Tile{{-50, 0}, TileType::Obstacle}};
    auto player = Player{{0, 0}};
    player.move_left();
    player.advance(environment, tick);
    auto const position = player.position();
    EXPECT_EQ((PositionD{0, 0}), position);
}

TEST(PlayerTest, StartSlashAttack)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    player.advance({});
    EXPECT_TRUE(player.is_slashing());
}

TEST(PlayerTest, EndSlashAttackAfterMoveUp)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    player.advance({});
    player.move_up();
    player.advance({});
    EXPECT_FALSE(player.is_slashing());
}

TEST(PlayerTest, EndSlashAttackAfterMoveDown)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    player.advance({});
    player.move_down();
    player.advance({});
    EXPECT_FALSE(player.is_slashing());
}

TEST(PlayerTest, EndSlashAttackAfterMoveRight)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    player.advance({});
    player.move_right();
    player.advance({});
    EXPECT_FALSE(player.is_slashing());
}

TEST(PlayerTest, EndSlashAttackAfterMoveLeft)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    player.advance({});
    player.move_left();
    player.advance({});
    EXPECT_FALSE(player.is_slashing());
}

TEST(PlayerTest, TargetIsHitWorksWithShoot)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Shoot);
    EXPECT_FALSE(player.target_is_hit({0, 0}));
    player.attack();
    player.advance({});
    EXPECT_TRUE(player.target_is_hit({0, 0}));
}

TEST(PlayerTest, TargetIsHitWorksWithSlash)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    EXPECT_FALSE(player.target_is_hit({0, 1}));
    player.attack();
    player.advance({});
    EXPECT_TRUE(player.target_is_hit({0, 1}));
}

TEST(PlayerTest, ShootAttackObeysDirection)
{
    auto player = Player{PositionDAndOrientation{{0, 0}, Orientation::Down}};
    player.set_attack(AttackUpgrade::Shoot);
    player.attack();
    player.advance({});
    player.move_up();
    player.advance({});
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const projectile = projectiles->front();
    EXPECT_EQ(0, projectile.x);
    EXPECT_LT(projectile.y, 0);
}

TEST(PlayerTest, ShootAttackObeysDirectionFromMovingUp)
{
    auto player = Player{PositionDAndOrientation{{0, 0}, Orientation::Down}};
    player.set_attack(AttackUpgrade::Shoot);
    player.move_up();
    player.advance({});
    player.attack();
    player.advance({});
    player.move_up();
    player.advance({});
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const projectile = projectiles->front();
    EXPECT_EQ(0, projectile.x);
    EXPECT_GT(projectile.y, 0);
}

TEST(PlayerTest, ShootAttackObeysDirectionFromMovingLeft)
{
    constexpr auto tick = std::chrono::milliseconds{5};
    auto player = Player{PositionDAndOrientation{{0, 0}, Orientation::Down}};
    player.set_attack(AttackUpgrade::Shoot);
    player.move_right();
    player.advance({}, tick);
    player.attack();
    player.advance({}, tick);
    player.move_up();
    player.advance({}, tick);
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const projectile = projectiles->front();
    EXPECT_GT(projectile.x, 0);
    EXPECT_EQ(0, projectile.y);
}

TEST(PlayerTest, ShootAttackObeysDirectionFromMovingRight)
{
    auto player = Player{PositionDAndOrientation{{0, 0}, Orientation::Down}};
    player.set_attack(AttackUpgrade::Shoot);
    player.move_left();
    player.advance({});
    player.attack();
    player.advance({});
    player.move_up();
    player.advance({});
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const projectile = projectiles->front();
    EXPECT_LT(projectile.x, 0);
    EXPECT_EQ(0, projectile.y);
}

TEST(PlayerTest, ShootAttackObeysDirectionFromMovingDown)
{
    auto player = Player{PositionDAndOrientation{{0, 0}, Orientation::Up}};
    player.set_attack(AttackUpgrade::Shoot);
    player.move_down();
    player.advance({});
    player.attack();
    player.advance({});
    player.move_up();
    player.advance({});
    auto const* const projectiles = player.projectiles();
    ASSERT_TRUE(projectiles != nullptr);
    ASSERT_EQ(1, projectiles->size());
    auto const projectile = projectiles->front();
    EXPECT_EQ(0, projectile.x);
    EXPECT_LT(projectile.y, 0);
}

} // namespace
