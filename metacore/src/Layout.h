#pragma once

#include "ArenaLayoutMechanic.h"
#include <optional>
#include <span>
#include <variant>
namespace metacore {
struct Tile;
struct LayoutBounds;
} // namespace metacore

namespace metacore {

enum class LayoutUpgrade { Arena };

class Layout final {
  public:
    Layout() = default;
    explicit Layout(LayoutUpgrade upgrade);

    [[nodiscard]] std::span<Tile const> tiles() const;
    [[nodiscard]] std::optional<LayoutBounds> bounds() const;

  private:
    std::variant<std::monostate, ArenaLayoutMechanic> layout_ = {};
};

} // namespace metacore