#pragma once

#include "Player.h"
#include <span>
#include <vector>
namespace metacore {
struct LayoutBounds;
} // namespace metacore

namespace metacore {

class OpenWorldLayoutMechanic final {
  public:
    static LayoutBounds const& bounds();

    OpenWorldLayoutMechanic();

    [[nodiscard]] std::vector<Tile> const& tiles() const;
    bool check_for_transition(Player& player, std::span<Position> enemies);

  private:
    int seed;
    int world_x = 0;
    int world_y = 0;
    std::vector<Tile> tiles_;
};

} // namespace metacore