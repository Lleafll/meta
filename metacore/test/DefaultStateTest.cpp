#include "DefaultState.h"
#include "GameState.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(DefaultStateTest, ToGameState)
{
    auto const state = DefaultState{
        Player{Position{12, 34}},
        Pickup{
            Position{56, 78},
            UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot}},
        Enemies{{}}};
    auto const game_state = to_game_state(state);
    auto const expected =
        GameState{Position{12, 34}, Position{56, 78}, std::nullopt};
    EXPECT_EQ(expected, game_state);
}

TEST(DefaultStateTest, ToGameStateConsidersEnemies)
{
    auto const state = DefaultState{
        Player{Position{}},
        Pickup{
            Position{},
            UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot}},
        Enemies{{Position{4353, 786}}}};
    auto const enemy_positions = to_game_state(state).enemy_positions;
    auto const expected = std::vector<Position>{{4353, 786}};
    EXPECT_EQ(expected, enemy_positions);
}

TEST(DefaultStateTest, SlashingIsTrueWhenPlayerIsAttacking)
{
    auto player = Player{};
    player.set_attack(AttackUpgrade::Slash);
    player.attack();
    auto const state = DefaultState{
        player,
        Pickup{
            Position{0, 0},
            UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot}},
        Enemies{{}}};
    EXPECT_TRUE(to_game_state(state).slash_attack);
}

TEST(DefaultStateTest, ToGameStateConsidersProjectiles)
{
    auto player = Player{Position{324, 765}};
    {
        player.set_attack(AttackUpgrade::Shoot);
        player.attack();
    }
    auto const state = DefaultState{player, Pickup{{}, {{}, {}}}, Enemies{}};
    auto const game_state_projectiles = to_game_state(state).projectiles;
    auto const expected = std::vector{Position{324, 815}};
    EXPECT_EQ(expected, game_state_projectiles);
}

TEST(DefaultStateTest, ToGameStateConsidersLayout)
{
    auto const state = DefaultState{
        Player{},
        Pickup{{}, {{}, {}}},
        Enemies{},
        Layout{LayoutUpgrade::Arena}};
    auto const tiles = to_game_state(state).tiles;
    EXPECT_EQ(36, tiles.size());
}

} // namespace