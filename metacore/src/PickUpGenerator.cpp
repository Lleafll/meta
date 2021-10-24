#include "PickUpGenerator.h"
#include "Pickup.h"

namespace metacore {

namespace {

constexpr auto spawn_edge = 250;

} // namespace

Pickup PickUpGenerator::generate()
{
    auto const distribution =
        std::uniform_int_distribution{-spawn_edge, spawn_edge};
    return Pickup{
        Position{distribution(random_engine_), distribution(random_engine_)},
        UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot}};
}

} // namespace metacore