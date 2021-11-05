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
    auto const expected = std::vector<PositionD>{{73, 65}};
    EXPECT_EQ(expected, projectiles);
}

TEST(ShootAttackMechanic, Tick)
{
    auto attack = ShootAttackMechanic{std::array{
        PositionDAndOrientation{PositionD{0, 0}, Orientation::Right}}};
    attack.tick();
    auto const expected = std::vector<PositionD>{{50, 0}};
    EXPECT_EQ(expected, attack.projectiles());
}

TEST(ShootAttackMechanic, TickObeysDirections)
{
    auto attack = ShootAttackMechanic{
        std::array{PositionDAndOrientation{PositionD{0, 0}, Orientation::Up}}};
    attack.tick();
    auto const expected = std::vector<PositionD>{{0, 50}};
    EXPECT_EQ(expected, attack.projectiles());
}

} // namespace