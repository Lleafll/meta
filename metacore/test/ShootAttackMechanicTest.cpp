#include "ShootAttackMechanic.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(ShootAttackMechanic, Start)
{
    auto attack = ShootAttackMechanic{};
    ASSERT_TRUE(attack.projectiles().empty());
    attack.start(Position{23, 65});
    auto const& projectiles = attack.projectiles();
    auto const expected = std::vector<Position>{{23, 65}};
    EXPECT_EQ(expected, projectiles);
}

TEST(ShootAttackMechanic, Tick)
{
    auto attack = ShootAttackMechanic{{Position{0, 0}}};
    attack.tick();
    auto const expected = std::vector<Position>{{50, 0}};
    EXPECT_EQ(expected, attack.projectiles());
}

} // namespace