#include "ShootAttackMechanic.h"
#include <algorithm>
#include <gsl/gsl_assert>
#include <iterator>

namespace metacore {

namespace {

constexpr auto projectile_speed = 200.0 / 1'000'000.0; // per microsecond

} // namespace

ShootAttackMechanic::ShootAttackMechanic(
    std::span<PositionDAndOrientation const> const projectiles)
    : positions_{[projectiles]() -> std::vector<PositionD> {
          auto positions = std::vector<PositionD>{};
          std::ranges::transform(
              projectiles,
              std::back_inserter(positions),
              &PositionDAndOrientation::position);
          return positions;
      }()},
      orientations_{[projectiles]() -> std::vector<Orientation> {
          auto orientations = std::vector<Orientation>{};
          std::ranges::transform(
              projectiles,
              std::back_inserter(orientations),
              &PositionDAndOrientation::orientation);
          return orientations;
      }()}
{
}

std::vector<PositionD> const& ShootAttackMechanic::projectiles() const
{
    return positions_;
}

void ShootAttackMechanic::start(
    PositionDAndOrientation const& position_and_orientation)
{
    auto position = position_and_orientation.position;
    switch (position_and_orientation.orientation) {
        case Orientation::Up:
            position.y += projectile_speed;
            break;
        case Orientation::Down:
            position.y -= projectile_speed;
            break;
        case Orientation::Left:
            position.x -= projectile_speed;
            break;
        case Orientation::Right:
            position.x += projectile_speed;
            break;
    }
    positions_.push_back(position);
    orientations_.push_back(position_and_orientation.orientation);
}

void ShootAttackMechanic::tick(std::chrono::microseconds const diff)
{
    Expects(positions_.size() == orientations_.size());
    auto const step = projectile_speed * gsl::narrow_cast<double>(diff.count());
    for (auto i = 0ull; i < positions_.size(); ++i) {
        auto& position = positions_[i];
        switch (orientations_[i]) {
            case Orientation::Up:
                position.y += step;
                break;
            case Orientation::Down:
                position.y -= step;
                break;
            case Orientation::Right:
                position.x += step;
                break;
            case Orientation::Left:
                position.x -= step;
                break;
        }
    }
}

} // namespace metacore
