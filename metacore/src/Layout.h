#pragma once

#include "ArenaLayoutMechanic.h"
#include "Tile.h"
#include <span>

namespace metacore {

class Layout final {
  public:
    [[nodiscard]] std::span<Tile const> tiles() const;

  private:
    ArenaLayoutMechanic layout_;
};

} // namespace metacore