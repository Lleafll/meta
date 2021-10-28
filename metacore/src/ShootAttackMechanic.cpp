#include "ShootAttackMechanic.h"
#include <algorithm>
#include <iterator>

namespace metacore {

namespace {

constexpr auto projectile_step_size = 50;

} // namespace

ShootAttackMechanic::ShootAttackMechanic(
    std::span<PositionAndOrientation const> const projectiles)
    : positions_{[projectiles]() -> std::vector<Position> {
          auto positions = std::vector<Position>{};
          std::ranges::transform(
              projectiles,
              std::back_inserter(positions),
              &PositionAndOrientation::position);
          return positions;
      }()},
      orientations_{[projectiles]() -> std::vector<Orientation> {
          auto orientations = std::vector<Orientation>{};
          std::ranges::transform(
              projectiles,
              std::back_inserter(orientations),
              &PositionAndOrientation::orientation);
          return orientations;
      }()}
{
}

std::vector<Position> const& ShootAttackMechanic::projectiles() const
{
    return positions_;
}

void ShootAttackMechanic::start(
    PositionAndOrientation const& position_and_orientation)
{
    positions_.push_back(position_and_orientation.position);
    orientations_.push_back(position_and_orientation.orientation);
}

void ShootAttackMechanic::tick()
{
    for (auto& projectile : positions_) {
        projectile.x += projectile_step_size;
    }
}

} // namespace metacore
