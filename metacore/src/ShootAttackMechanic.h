#pragma once

#include "Orientation.h"
#include "Position.h"
#include "PositionAndOrientation.h"
#include <span>
#include <vector>

namespace metacore {

class ShootAttackMechanic final {
  public:
    ShootAttackMechanic() = default;
    explicit ShootAttackMechanic(
        std::span<PositionAndOrientation const> projectiles);

    [[nodiscard]] std::vector<Position> const& projectiles() const;
    void start(PositionAndOrientation const& position_and_orientation);
    void tick();

  private:
    std::vector<Position> projectiles_ = {};
    std::vector<Orientation> orientations_ = {};
};

} // namespace metacore