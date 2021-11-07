#pragma once

#include "Orientation.h"
#include "PositionDAndOrientation.h"
#include <chrono>
#include <span>
#include <vector>

namespace metacore {

class ShootAttackMechanic final {
  public:
    ShootAttackMechanic() = default;
    explicit ShootAttackMechanic(
        std::span<PositionDAndOrientation const> projectiles);

    [[nodiscard]] std::vector<PositionD> const& projectiles() const;
    void start(PositionDAndOrientation const& position_and_orientation);
    void tick(std::chrono::microseconds diff = std::chrono::microseconds{1000});

  private:
    std::vector<PositionD> positions_ = {};
    std::vector<Orientation> orientations_ = {};
};

} // namespace metacore