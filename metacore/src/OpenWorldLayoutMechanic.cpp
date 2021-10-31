#include "OpenWorldLayoutMechanic.h"
#include "LayoutBounds.h"
#include <optional>

namespace metacore {

namespace {

constexpr auto height_count = 12;
constexpr auto width_count = 16;
constexpr auto tile_size = 50;
constexpr auto height = height_count * tile_size;
constexpr auto width = width_count * tile_size;
constexpr LayoutBounds layout_bounds = {
    -width / 2, width / 2, -height / 2, height / 2};
constexpr auto tile_fill_rate = 0.4;

} // namespace

LayoutBounds const& OpenWorldLayoutMechanic::bounds()
{
    return layout_bounds;
}

namespace {

int simple_hash(int const x, int const y, int const z)
{
    auto seed = ((x >> 16) ^ y) * 0x45d9f3b;
    seed = ((seed >> 16) ^ z) * 0x45d9f3b;
    return seed;
}

std::vector<Tile>
build_tiles(int const world_seed, int const world_x, int const world_y)
{
    auto tiles = std::vector<Tile>{};
    auto const tile_seed = simple_hash(world_seed, world_x, world_y);
    constexpr auto cutoff = static_cast<int>(
        std::numeric_limits<int>::max() * (1 - tile_fill_rate));
    for (auto x = layout_bounds.left; x < layout_bounds.right; x += tile_size) {
        for (auto y = layout_bounds.bottom; y < layout_bounds.top;
             y += tile_size) {
            auto const hash = simple_hash(tile_seed, x, y);
            if (hash > cutoff) {
                tiles.push_back(Tile{{x, y}, TileType::Obstacle});
            }
        }
    }
    return tiles;
}

} // namespace

OpenWorldLayoutMechanic::OpenWorldLayoutMechanic()
    : seed{547839}, tiles_{build_tiles(seed, world_x, world_y)}
{
}

std::vector<Tile> const& OpenWorldLayoutMechanic::tiles() const
{
    return tiles_;
}

namespace {

std::optional<Orientation>
check_if_out_of_bounds(Position const& position, int& x, int& y)
{
    if (position.x < layout_bounds.left) {
        --x;
        return Orientation::Left;
    }
    if (position.x > layout_bounds.right) {
        ++x;
        return Orientation::Right;
    }
    if (position.y < layout_bounds.bottom) {
        --y;
        return Orientation::Down;
    }
    if (position.y > layout_bounds.right) {
        ++y;
        return Orientation::Up;
    }
    return std::nullopt;
}

void move_from_bound(Position& position, Orientation const bound)
{
    switch (bound) {
        case Orientation::Left:
            position.x += width;
            break;
        case Orientation::Right:
            position.x -= width;
            break;
        case Orientation::Down:
            position.y += height;
            break;
        case Orientation::Up:
            position.y -= height;
            break;
    }
}

} // namespace

bool OpenWorldLayoutMechanic::check_for_transition(
    Player& player, std::span<Position> const enemies)
{
    auto const bound =
        check_if_out_of_bounds(player.position(), world_x, world_y);
    if (bound.has_value()) {
        move_from_bound(player.position(), *bound);
        for (auto& enemy : enemies) {
            move_from_bound(enemy, *bound);
        }
        tiles_ = build_tiles(seed, world_x, world_y);
        return true;
    }
    return false;
}

} // namespace metacore