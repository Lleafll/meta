#include "MetaEngine.h"
#include "GameState.h"
#include "InternalGameState.h"
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

TEST(MetaEngineTest, InputUpOnFreshEngine)
{
    auto engine = MetaEngine{};
    ASSERT_EQ((Position{0, 0}), (engine.calculate_state().player_position));
    engine.input_up();
    auto const position = engine.calculate_state().player_position;
    EXPECT_EQ(0, position.x);
    EXPECT_LT(0, position.y);
}

TEST(MetaEngineTest, InputDownOnFreshEngine)
{
    auto engine = MetaEngine{};
    ASSERT_EQ((Position{0, 0}), (engine.calculate_state().player_position));
    engine.input_down();
    auto const position = engine.calculate_state().player_position;
    EXPECT_EQ(0, position.x);
    EXPECT_GT(0, position.y);
}

TEST(MetaEngineTest, UpgradePositionIsNonZeroOnStart)
{
    auto engine = MetaEngine{};
    auto const position = engine.calculate_state().upgrade_position;
    ASSERT_TRUE(position.has_value());
    EXPECT_NE(0, position->x);
    EXPECT_NE(0, position->y);
}

TEST(MetaEngineTest, UpgradeCollision)
{
    auto engine = MetaEngine{
        {DefaultState{Position{0, 0}, Pickup{Position{50, 0}, {{}, {}}}}}};
    ASSERT_EQ((Position{50, 0}), engine.calculate_state().upgrade_position);
    engine.input_right();
    EXPECT_NE((Position{50, 0}), engine.calculate_state().upgrade_position);
}

TEST(MetaEngineTest, UpgradeSelectFirst)
{
    auto engine = MetaEngine{
        {PickingUpState{Position{453, 7865}, UpgradeChoices{{}, {}}}}};
    {
        auto const original_state = engine.calculate_state();
        ASSERT_TRUE(original_state.upgrade_choices.has_value());
        ASSERT_FALSE(original_state.upgrade_position.has_value());
    }
    engine.select_first_upgrade();
    auto const state = engine.calculate_state();
    EXPECT_FALSE(state.upgrade_choices.has_value());
    EXPECT_TRUE(state.upgrade_position.has_value());
}

TEST(MetaEngineTest, UpgradeSelectSecond)
{
    auto engine = MetaEngine{
        {PickingUpState{Position{453, 7865}, UpgradeChoices{{}, {}}}}};
    {
        auto const original_state = engine.calculate_state();
        ASSERT_TRUE(original_state.upgrade_choices.has_value());
        ASSERT_FALSE(original_state.upgrade_position.has_value());
    }
    engine.select_second_upgrade();
    auto const state = engine.calculate_state();
    EXPECT_FALSE(state.upgrade_choices.has_value());
    EXPECT_TRUE(state.upgrade_position.has_value());
}

} // namespace