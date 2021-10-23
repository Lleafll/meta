#pragma once

#include "GameState.h"

namespace metacore {

class MetaEngine final {
  public:
    MetaEngine() = default;
    explicit MetaEngine(GameState const& state);

    [[nodiscard]] GameState calculate_state() const;
    void input_right();
    void input_left();
    void input_up();
    void input_down();

  private:
    GameState state_ = {{}, {200, 50}};
};

}; // namespace metacore