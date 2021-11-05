#pragma once

#include "PositionDAndOrientation.h"

namespace metacore {

class SlashAttackMechanic final {
  public:
    [[nodiscard]] bool is_active() const;
    void start(PositionDAndOrientation const& where);
    void tick();
    [[nodiscard]] bool target_is_hit(PositionD const& target) const;

  private:
    bool is_active_ = false;
    PositionDAndOrientation where_ = {};
};

} // namespace metacore