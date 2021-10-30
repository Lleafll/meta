#include "PickUpGenerator.h"
#include "Pickup.h"

namespace metacore {

namespace {

constexpr auto spawn_edge = 250;

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
        UpgradeChoices{PickupUpgrade::Slash, PickupUpgrade::Shoot}};
}

} // namespace metacore