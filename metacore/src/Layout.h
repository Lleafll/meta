#pragma once

#include "ArenaLayoutMechanic.h"
#include "DungeonLayoutMechanic.h"
#include <optional>
#include <span>
#include <variant>
namespace metacore {
struct Tile;
struct LayoutBounds;
} // namespace metacore

namespace metacore {

enum class LayoutUpgrade { Arena, Dungeon, OpenWorld };

class Layout final {
  public:
    Layout() = default;
    explicit Layout(LayoutUpgrade upgrade);

    [[nodiscard]] std::span<Tile const> tiles() const;
    [[nodiscard]] std::optional<LayoutBounds> bounds() const;
    void set_upgrade(LayoutUpgrade upgrade);

  private:
    std::variant<std::monostate, ArenaLayoutMechanic, DungeonLayoutMechanic>
        layout_ = {};
};

} // namespace metacore