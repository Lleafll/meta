#pragma once

#include "PositionAndOrientation.h"

namespace metacore {

class SlashAttackMechanic final {
  public:
    [[nodiscard]] bool is_active() const;
    void start(PositionAndOrientation const& where);
    void tick();
    [[nodiscard]] bool target_is_hit(Position const& target) const;

  private:
    bool is_active_ = false;
    PositionAndOrientation where_ = {};
};

} // namespace metacore