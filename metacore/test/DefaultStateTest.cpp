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
            UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot}}};
    auto const game_state = to_game_state(state);
    auto const expected =
        GameState{Position{12, 34}, Position{56, 78}, std::nullopt};
    EXPECT_EQ(expected, game_state);
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
            UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot}}};
    EXPECT_TRUE(to_game_state(state).slash_attack);
}

} // namespace