#pragma once

#include "Position.h"
#include <vector>

namespace metacore {

class ShootAttackMechanic final {
  public:
    ShootAttackMechanic() = default;
    explicit ShootAttackMechanic(std::vector<Position> projectiles);

    [[nodiscard]] std::vector<Position> const& projectiles() const;
    void start(Position const& position);
    void tick();

  private:
    std::vector<Position> projectiles_ = {};
};

} // namespace metacore