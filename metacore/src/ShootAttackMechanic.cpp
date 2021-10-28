#include "ShootAttackMechanic.h"
#include <algorithm>
#include <gsl/gsl_assert>
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
    Expects(positions_.size() == orientations_.size());
    for (auto i = 0ull; i < positions_.size(); ++i) {
        auto& position = positions_[i];
        switch (orientations_[i]) {
            case Orientation::Up:
                position.y += projectile_step_size;
                break;
            case Orientation::Down:
                position.y -= projectile_step_size;
                break;
            case Orientation::Right:
                position.x += projectile_step_size;
                break;
            case Orientation::Left:
                position.x -= projectile_step_size;
                break;
        }
    }
}

} // namespace metacore
