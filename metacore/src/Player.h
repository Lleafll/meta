#pragma once

#include "PlayerAttackMechanic.h"
#include "Position.h"

namespace metacore {

class Player final {
  public:
    explicit Player(Position const& position);

    [[nodiscard]] Position const& position() const;
    void move_up();
    void move_down();
    void move_right();
    void move_left();

  private:
    Position position_;
    PlayerAttackMechanic attack_ = {};
};

} // namespace metacore