#pragma once

#include "Player.h"
#include <span>

namespace metacore {

class OpenWorldLayoutMechanic final {
  public:
    bool check_for_transition(Player& player, std::span<Position> enemies);
};

} // namespace metacore