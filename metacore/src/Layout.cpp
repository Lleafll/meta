#include "Layout.h"
#include "overloaded.h"

namespace metacore {

Layout::Layout(LayoutUpgrade const upgrade)
{
    set_upgrade(upgrade);
}

std::span<Tile const> Layout::tiles() const
{
    return std::visit(
        overloaded{
            [](std::monostate const&) -> std::span<Tile const> { return {}; },
            [](auto const& layout) -> std::span<Tile const> {
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
            [](auto const&) -> std::optional<LayoutBounds> {
                return ArenaLayoutMechanic::bounds();
            }},
        layout_);
}

void Layout::set_upgrade(LayoutUpgrade const upgrade)
{
    switch (upgrade) {
        case LayoutUpgrade::Arena:
            layout_ = ArenaLayoutMechanic{};
            break;
        case LayoutUpgrade::Dungeon:
            layout_ = DungeonLayoutMechanic{};
            break;
        case LayoutUpgrade::OpenWorld:
            // TODO
            break;
    }
}

bool Layout::check_for_transition(Position const& player_position)
{
    return std::visit(
        overloaded{
            [&player_position](DungeonLayoutMechanic& layout) -> bool {
                return layout.check_for_transition(player_position);
            },
            [](auto const&) -> bool { return false; }},
        layout_);
}

} // namespace metacore