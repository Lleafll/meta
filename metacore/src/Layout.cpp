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
            [](OpenWorldLayoutMechanic const& layout) -> std::span<Tile const> {
                return layout.tiles();
            },
            [](auto const& layout) -> std::span<Tile const> {
                return layout.tiles();
            },
        },
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
            },
            [](DungeonLayoutMechanic const&) -> std::optional<LayoutBounds> {
                return DungeonLayoutMechanic::bounds();
            },
            [](OpenWorldLayoutMechanic const&) -> std::optional<LayoutBounds> {
                return OpenWorldLayoutMechanic::bounds();
            },
        },
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
            layout_ = OpenWorldLayoutMechanic{};
            break;
    }
}

bool Layout::check_for_transition(
    Player& player, std::span<PositionD> const enemies)
{
    return std::visit(
        overloaded{
            [&player](DungeonLayoutMechanic& layout) -> bool {
                return layout.check_for_transition(player.position());
            },
            [&player, enemies](OpenWorldLayoutMechanic& layout) -> bool {
                return layout.check_for_transition(player, enemies);
            },
            [](auto const&) -> bool { return false; }},
        layout_);
}

} // namespace metacore