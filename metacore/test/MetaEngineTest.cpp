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
    auto engine = MetaEngine{InternalGameState{DefaultState{
        Player{PositionD{0, 0}},
        Pickup{Position{50, 0}, {{}, {}}},
        Enemies{{}}}}};
    ASSERT_EQ((Position{50, 0}), engine.calculate_state().upgrade_position);
    engine.input_right();
    EXPECT_NE((Position{50, 0}), engine.calculate_state().upgrade_position);
}

TEST(MetaEngineTest, UpgradeSelectFirst)
{
    auto engine = MetaEngine{InternalGameState{PickingUpState{
        Player{PositionD{453, 7865}}, UpgradeChoices{{}, {}}, Enemies{{}}}}};
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
    auto engine = MetaEngine{InternalGameState{PickingUpState{
        Player{PositionD{453, 7865}}, UpgradeChoices{{}, {}}, Enemies{{}}}}};
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

TEST(MetaEngineTest, AttackStartsSlash)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    auto engine = MetaEngine{InternalGameState{
        DefaultState{player, Pickup{{}, {{}, {}}}, Enemies{{}}}}};
    ASSERT_FALSE(engine.calculate_state().slash_attack);
    engine.input_attack();
    EXPECT_TRUE(engine.calculate_state().slash_attack);
}

TEST(MetaEngineTest, ProperlyTransitionToGameOver)
{
    auto engine = MetaEngine{InternalGameState{DefaultState{
        Player{PositionD{50, 0}},
        Pickup{{500, 500}, {{}, {}}},
        Enemies{{PositionD{0, 0}}}}}};
    engine.input_left();
    auto const state = engine.calculate_state();
    auto const expected = GameState{
        {0, 0},
        Position{500, 500},
        std::nullopt,
        std::nullopt,
        {{0, 0}},
        GameProgress::Lost};
    EXPECT_EQ(expected, state);
}

TEST(MetaEngineTest, InputRestart)
{
    auto engine = MetaEngine{InternalGameState{DefaultState{
        Player{PositionD{34, 65}},
        Pickup{{5634, 654}, {{}, {}}},
        Enemies{{PositionD{654, 65}}}}}};
    engine.input_restart();
    auto const state = engine.calculate_state();
    auto expected = GameState{Position{0, 0}, state.upgrade_position};
    expected.tiles = state.tiles;
    expected.layout_bounds = LayoutBounds{-250, 250, -250, 250};
    EXPECT_EQ(expected, state);
}

TEST(MetaengineTest, DoNotKillEnemiesInSlashRangeIfNoUpgradeIsSelected)
{
    auto engine = MetaEngine{InternalGameState{DefaultState{
        Player{{0, 0}},
        Pickup{{5634, 654}, {{}, {}}},
        Enemies{{PositionD{-50, -50}}}}}};
    engine.input_attack();
    auto const enemies_count = engine.calculate_state().enemy_positions.size();
    EXPECT_EQ(1, enemies_count);
}

TEST(MetaengineTest, KillEnemiesInSlashRangeIfSlashUpgradeIsSelected)
{
    auto player = Player{PositionD{0, 0}}; // Player faces up
    player.set_attack(AttackUpgrade::Slash);
    auto engine = MetaEngine{InternalGameState{DefaultState{
        player, Pickup{{5634, 654}, {{}, {}}}, Enemies{{PositionD{50, 50}}}}}};
    engine.input_attack();
    auto const enemies_count = engine.calculate_state().enemy_positions.size();
    EXPECT_EQ(0, enemies_count);
}

TEST(MetaengineTest, DoNotKillEnemiesInSlashRangeIfShootUpgradeIsSelected)
{
    auto player = Player{PositionD{0, 0}};
    player.set_attack(AttackUpgrade::Shoot);
    auto engine = MetaEngine{InternalGameState{DefaultState{
        player,
        Pickup{{5634, 654}, {{}, {}}},
        Enemies{{PositionD{-100, -100}}}}}};
    engine.input_attack();
    auto const enemies_count = engine.calculate_state().enemy_positions.size();
    EXPECT_EQ(1, enemies_count);
}

TEST(MetaEngineTest, DoNotSlashEnemiesBehindPlayer)
{
    auto player = Player{PositionD{0, 0}}; // Player is facing up
    player.set_attack(AttackUpgrade::Slash);
    auto engine = MetaEngine{InternalGameState{DefaultState{
        player, Pickup{{5634, 654}, {{}, {}}}, Enemies{{PositionD{0, -50}}}}}};
    engine.input_attack();
    auto const enemies_count = engine.calculate_state().enemy_positions.size();
    EXPECT_EQ(1, enemies_count);
}

TEST(MetaEngineTest, InputAttackAdvancesProjectiles)
{
    auto player = Player{PositionD{0, 0}};
    player.set_attack(AttackUpgrade::Shoot);
    auto engine = MetaEngine{InternalGameState{
        DefaultState{player, Pickup{{65464, 754676}, {{}, {}}}, Enemies{{}}}}};
    {
        auto const projectiles = engine.calculate_state().projectiles;
        ASSERT_TRUE(projectiles.has_value());
        ASSERT_TRUE(projectiles->empty());
    }
    engine.input_attack();
    ASSERT_TRUE(engine.calculate_state().projectiles.has_value());
    ASSERT_EQ(1, engine.calculate_state().projectiles->size());
    auto const projectile_first_position =
        engine.calculate_state().projectiles->front();
    engine.input_attack();
    ASSERT_TRUE(engine.calculate_state().projectiles.has_value());
    ASSERT_EQ(2, engine.calculate_state().projectiles->size());
    auto const projectile_second_position =
        engine.calculate_state().projectiles->front();
    EXPECT_NE(projectile_first_position, projectile_second_position);
}

} // namespace