#pragma once

#include "GameState.h"

namespace metacore {

class MetaEngine final {
  public:
    [[nodiscard]] GameState calculate_state() const noexcept;
    void input_right() noexcept;

  private:
    GameState state_;
};

}; // namespace metacore