#pragma once

#include "Clock.h"
#include "DefaultState.h"
#include "GameOverState.h"
#include "PickingUpState.h"
#include "PickUpGenerator.h"
#include <variant>

namespace metacore {

enum class UpgradeSelection { First, Second };

class InternalGameState final {
  public:
    InternalGameState();
    explicit InternalGameState(
        std::variant<DefaultState, PickingUpState, GameOverState> state);

    ~InternalGameState() = default;
    InternalGameState(InternalGameState const&) = delete;
    InternalGameState(InternalGameState&& other) noexcept;
    InternalGameState& operator=(InternalGameState const&) = delete;
    InternalGameState& operator=(InternalGameState&& other) noexcept;

    void advance(std::chrono::microseconds diff);
    [[nodiscard]] GameState to_game_state() const;
    void move_player(Orientation orientation);
    void attack_player();
    void stop_player();
    void select_upgrade(UpgradeSelection selection);

  private:
    PickUpGenerator pickup_generator_ = {};
    std::variant<DefaultState, PickingUpState, GameOverState> state_;
    Clock clock_;
};

} // namespace metacore