#include "DefaultState.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(DefaultStateTest, ToGameState)
{
    auto const state = DefaultState{
        Position{12, 34},
        Pickup{
            Position{56, 78},
            UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot}}};
    auto const game_state = to_game_state(state);
    auto const expected =
        GameState{Position{12, 34}, Position{56, 78}, std::nullopt};
    EXPECT_EQ(expected, game_state);
}

} // namespace