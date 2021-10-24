#include "PickingUpState.h"
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(PickingUpStateTest, ToGameState)
{
    auto const state = PickingUpState{
        Position{12, 34},
        UpgradeChoices{PickupUpgrade::Shoot, PickupUpgrade::Slash}};
    auto const game_state = to_game_state(state);
    auto const expected = GameState{
        Position{12, 34},
        std::nullopt,
        UpgradeChoices{PickupUpgrade::Shoot, PickupUpgrade::Slash}};
    EXPECT_EQ(expected, game_state);
}

} // namespace