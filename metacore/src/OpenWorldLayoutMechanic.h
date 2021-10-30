#pragma once

#include "Player.h"

namespace metacore {

class OpenWorldLayoutMechanic final {
  public:
    bool check_for_transition(Player& player);
};

} // namespace metacore