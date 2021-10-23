#include "MetaEngine.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(MetaEngineTest, InputRightOnFreshEngine)
{
    auto engine = MetaEngine{};
    ASSERT_EQ((Position{0, 0}), (engine.calculate_state().player_position));
    engine.input_right();
    auto const position = engine.calculate_state().player_position;
    EXPECT_LT(0, position.x);
    EXPECT_EQ(0, position.y);
}

TEST(MetaEngineTest, InputLeftOnFreshEngine)
{
    auto engine = MetaEngine{};
    ASSERT_EQ((Position{0, 0}), (engine.calculate_state().player_position));
    engine.input_left();
    auto const position = engine.calculate_state().player_position;
    EXPECT_GT(0, position.x);
    EXPECT_EQ(0, position.y);
}

TEST(MetaEngineTest, UpgradePositionIsNonZeroOnStart)
{
    auto engine = MetaEngine{};
    auto const position = engine.calculate_state().upgrade;
    EXPECT_NE(0, position.x);
    EXPECT_NE(0, position.y);
}

TEST(MetaEngineTest, UpgradeCollision)
{
    auto engine = MetaEngine{{Position{0, 0}, Position{50, 0}}};
    ASSERT_EQ((Position{50, 0}), engine.calculate_state().upgrade);
    engine.input_right();
    EXPECT_NE((Position{50, 0}), engine.calculate_state().upgrade);
}

} // namespace