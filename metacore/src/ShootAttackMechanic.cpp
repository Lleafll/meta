#include "ShootAttackMechanic.h"
#include <algorithm>
#include <iterator>

namespace metacore {

namespace {

constexpr auto projectile_step_size = 50;

} // namespace

ShootAttackMechanic::ShootAttackMechanic(
    std::span<PositionAndOrientation const> const projectiles)
    : projectiles_{[projectiles]() -> std::vector<Position> {
          auto positions = std::vector<Position>{};
          std::ranges::transform(
              projectiles,
              std::back_inserter(positions),
              &PositionAndOrientation::position);
          return positions;
      }()}
{
}

std::vector<Position> const& ShootAttackMechanic::projectiles() const
{
    return projectiles_;
}

void ShootAttackMechanic::start(
    PositionAndOrientation const& position_and_orientation)
{
    projectiles_.push_back(position_and_orientation.position);
    orientations_.push_back(position_and_orientation.orientation);
}

void ShootAttackMechanic::tick()
{
    for (auto& projectile : projectiles_) {
        projectile.x += projectile_step_size;
    }
}

} // namespace metacore
