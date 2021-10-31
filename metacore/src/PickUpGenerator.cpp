#include "PickUpGenerator.h"
#include "Pickup.h"

namespace metacore {

namespace {

constexpr auto spawn_edge = 250;

UpgradeChoices next_choices(int& state)
{
    ++state;
    switch (state) {
        case 1:
            return UpgradeChoices{PickupUpgrade::Car, PickupUpgrade::Knight};
        case 2:
            return UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot};
        case 3:
            return UpgradeChoices{PickupUpgrade::Alien, PickupUpgrade::Robot};
        case 4:
            return UpgradeChoices{
                PickupUpgrade::Dungeon, PickupUpgrade::OpenWorld};
        case 5:
            return UpgradeChoices{
                PickupUpgrade::Castle, PickupUpgrade::SpaceStation};
        default:
            return UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot};
    }
}

} // namespace

Pickup PickUpGenerator::generate(std::optional<LayoutBounds> const& bounds)
{
    auto const [left, right, bottom, top] = bounds.value_or(
        LayoutBounds{-spawn_edge, spawn_edge, -spawn_edge, spawn_edge});
    auto const distribution_width =
        std::uniform_int_distribution{left, right - 50};
    auto const distribution_height =
        std::uniform_int_distribution{bottom, top - 50};
    return Pickup{
        Position{
            distribution_width(random_engine_),
            distribution_height(random_engine_)},
        next_choices(state)};
}

} // namespace metacore