#pragma once

#include "ArenaLayoutMechanic.h"
#include "mdspan.h"
#include "Tile.h"
#include <span>

namespace metacore {

class Layout final {
  public:
    [[nodiscard]] mdspan<Tile> tiles() const;

  private:
    ArenaLayoutMechanic layout_;
};

} // namespace metacore