#include "PickingUpState.h"
#include "GameState.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(PickingUpStateTest, ToGameState)
{
    auto const state = PickingUpState{
        Player{Position{12, 34}},
        UpgradeChoices{PickupUpgrade::Shoot, PickupUpgrade::Slash},
        Enemies{{}}};
    auto const game_state = to_game_state(state);
    auto const expected = GameState{
        Position{12, 34},
        std::nullopt,
        UpgradeChoices{PickupUpgrade::Shoot, PickupUpgrade::Slash}};
    EXPECT_EQ(expected, game_state);
}

TEST(PickingUpStateTest, ToGameStateConsidersEnemies)
{
    auto const state = PickingUpState{
        Player{Position{}},
        UpgradeChoices{PickupUpgrade::Shoot, PickupUpgrade::Slash},
        Enemies{{Position{435, 765}}}};
    auto const enemy_positions = to_game_state(state).enemy_positions;
    auto const expected = std::vector<Position>{{435, 765}};
    EXPECT_EQ(expected, enemy_positions);
}

TEST(PickingUpStateTest, ToGameStateConsidersLayout)
{
    auto const state = PickingUpState{
        Player{},
        UpgradeChoices{PickupUpgrade::Shoot, PickupUpgrade::Slash},
        Enemies{},
        Layout{LayoutUpgrade::Arena}};
    auto const tiles = to_game_state(state).tiles;
    EXPECT_EQ(36, tiles.size());
}

} // namespace