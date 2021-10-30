#pragma once

#include "ArenaLayoutMechanic.h"
#include "Tile.h"
#include <span>
#include <variant>

namespace metacore {

enum class LayoutUpgrade { Arena };

class Layout final {
  public:
    Layout() = default;
    explicit Layout(LayoutUpgrade upgrade);

    [[nodiscard]] std::span<Tile const> tiles() const;

  private:
    std::variant<std::monostate, ArenaLayoutMechanic> layout_ = {};
};

} // namespace metacore