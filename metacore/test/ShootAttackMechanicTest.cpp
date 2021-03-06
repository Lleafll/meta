#include "ShootAttackMechanic.h"
#include <array>
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(ShootAttackMechanic, Start)
{
    auto attack = ShootAttackMechanic{};
    ASSERT_TRUE(attack.projectiles().empty());
    attack.start({PositionD{23, 65}, Orientation::Right});
    auto const& projectiles = attack.projectiles();
    EXPECT_EQ(1, projectiles.size());
    auto const& projectile = projectiles.front();
    EXPECT_GT(projectile.x, 23);
    EXPECT_EQ(65, projectile.y);
}

TEST(ShootAttackMechanic, Tick)
{
    auto attack = ShootAttackMechanic{std::array{
        PositionDAndOrientation{PositionD{0, 0}, Orientation::Right}}};
    attack.tick();
    auto const& projectiles = attack.projectiles();
    EXPECT_EQ(1, projectiles.size());
    auto const& projectile = projectiles.front();
    EXPECT_GT(projectile.x, 0);
    EXPECT_EQ(0, projectile.y);
}

TEST(ShootAttackMechanic, TickObeysDirections)
{
    auto attack = ShootAttackMechanic{
        std::array{PositionDAndOrientation{PositionD{0, 0}, Orientation::Up}}};
    attack.tick();
    auto const& projectiles = attack.projectiles();
    EXPECT_EQ(1, projectiles.size());
    auto const& projectile = projectiles.front();
    EXPECT_EQ(0, projectile.x);
    EXPECT_GT(projectile.y, 0);
}

} // namespace