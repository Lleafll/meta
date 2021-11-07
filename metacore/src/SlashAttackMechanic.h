#pragma once

#include "PositionDAndOrientation.h"
#include <chrono>

namespace metacore {

class SlashAttackMechanic final {
  public:
    [[nodiscard]] bool is_active() const;
    void start(PositionDAndOrientation const& where);
    void tick(std::chrono::microseconds diff);
    [[nodiscard]] bool target_is_hit(PositionD const& target) const;

  private:
    std::chrono::microseconds time_passed_ =
        std::chrono::microseconds{1'000'000};
    PositionDAndOrientation where_ = {};
};

} // namespace metacore