#include "InitialPickingUpState.h"
#include <GameState.h>
#include <gtest/gtest.h>

using namespace metacore;

namespace {

TEST(InitialPickingUpStateTest, ToGameState)
{
    auto const state = InitialPickingUpState{
        Player{Position{654, 786}},
        UpgradeChoices{PickupUpgrade::Shoot, PickupUpgrade::Slash}};
    auto const game_state = to_game_state(state);
    auto const expected = GameState{
        Position{654, 786},
        std::nullopt,
        UpgradeChoices{PickupUpgrade::Shoot, PickupUpgrade::Slash}};
    EXPECT_EQ(expected, game_state);
}

} // namespace