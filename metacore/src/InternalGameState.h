#pragma once

#include "Clock.h"
#include "DefaultState.h"
#include "GameOverState.h"
#include "PickingUpState.h"
#include "PickUpGenerator.h"
#include <variant>

namespace metacore {

class InternalGameState final {
  public:
    InternalGameState();

    void advance(std::chrono::microseconds diff);
    [[nodiscard]] GameState to_game_state() const;

    ~InternalGameState() = default;
    InternalGameState(InternalGameState const&) = delete;
    InternalGameState(InternalGameState&&) = delete;
    InternalGameState operator=(InternalGameState const&) = delete;
    InternalGameState operator=(InternalGameState&&) = delete;

  private:
    PickUpGenerator pickup_generator_ = {};
    std::variant<DefaultState, PickingUpState, GameOverState> state_;
    Clock clock_;
};

} // namespace metacore