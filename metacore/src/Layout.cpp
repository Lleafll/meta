#include "Layout.h"
#include "overloaded.h"

namespace metacore {

Layout::Layout(LayoutUpgrade const upgrade) : layout_{ArenaLayoutMechanic{}}
{
}

std::span<Tile const> Layout::tiles() const
{
    return std::visit(
        overloaded{
            [](std::monostate const&) -> std::span<Tile const> { return {}; },
            [](ArenaLayoutMechanic const& layout) -> std::span<Tile const> {
                return layout.tiles();
            }},
        layout_);
}

std::optional<LayoutBounds> Layout::bounds() const
{
    return std::visit(
        overloaded{
            [](std::monostate const&) -> std::optional<LayoutBounds> {
                return {};
            },
            [](ArenaLayoutMechanic const&) -> std::optional<LayoutBounds> {
                return ArenaLayoutMechanic::bounds();
            }},
        layout_);
};

} // namespace metacore