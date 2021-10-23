#pragma once

#include "GameState.h"

namespace metacore {

class MetaEngine final {
  public:
    MetaEngine() noexcept = default;
    explicit MetaEngine(GameState const& state) noexcept;

    [[nodiscard]] GameState calculate_state() const noexcept;
    void input_right() noexcept;

  private:
    GameState state_ = {{}, {200, 50}};
};

}; // namespace metacore