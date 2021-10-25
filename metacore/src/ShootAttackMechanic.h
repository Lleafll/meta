#pragma once

#include "Position.h"
#include <vector>

namespace metacore {

class ShootAttackMechanic final {
  public:
    [[nodiscard]] std::vector<Position> const& projectiles() const;
    void start();
    void tick();

  private:
    std::vector<Position> projectiles_ = {};
};

} // namespace metacore