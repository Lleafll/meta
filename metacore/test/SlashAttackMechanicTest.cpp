#include "SlashAttackMechanic.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(SlashAttackMechanicTest, IsInitiallyNotActive)
{
    auto const mechanic = SlashAttackMechanic{};
    EXPECT_FALSE(mechanic.is_active());
}

TEST(SlashAttackMechanicTest, IsActiveAfterStarting)
{
    auto mechanic = SlashAttackMechanic{};
    mechanic.start({});
    EXPECT_TRUE(mechanic.is_active());
}

TEST(SlashAttackMechanicTest, IsInactiveAfterTick)
{
    auto mechanic = SlashAttackMechanic{};
    mechanic.start({});
    mechanic.tick(std::chrono::microseconds{5'000'000});
    EXPECT_FALSE(mechanic.is_active());
}

} // namespace