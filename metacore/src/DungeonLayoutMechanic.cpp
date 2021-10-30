#include "DungeonLayoutMechanic.h"

namespace metacore {

namespace {

Position get_random_stairs_position(
    std::mt19937& random_engine,
    LayoutBounds const& bounds,
    int const tile_size)
{
    auto const& [left, right, bottom, top] = bounds;
    auto const distribution_width =
        std::uniform_int_distribution{left, right - tile_size};
    auto const distribution_height =
        std::uniform_int_distribution{bottom, top - tile_size};
    return Position{
        distribution_width(random_engine), distribution_height(random_engine)};
}

} // namespace

DungeonLayoutMechanic::DungeonLayoutMechanic()
{
    auto index = std::size_t{0};
    auto const& [left, right, bottom, top] = bounds_;
    for (auto i = left; i < right; i += tile_size) {
        tiles_[index] = Tile{{i, bottom}, TileType::Obstacle};
        ++index;
        tiles_[index] = Tile{{i, top - tile_size}, TileType::Obstacle};
        ++index;
    }
    for (auto i = bottom + tile_size; i < top - tile_size; i += tile_size) {
        tiles_[index] = Tile{{left, i}, TileType::Obstacle};
        ++index;
        tiles_[index] = Tile{{right - tile_size, i}, TileType::Obstacle};
        ++index;
    }
    tiles_.back() = {
        get_random_stairs_position(random_engine_, bounds_, tile_size),
        TileType::Stairs};
}

auto DungeonLayoutMechanic::tiles() const -> std::array<Tile, tile_count> const&
{
    return tiles_;
}

LayoutBounds const& DungeonLayoutMechanic::bounds()
{
    return bounds_;
}

bool DungeonLayoutMechanic::check_for_transition(
    Position const& player_position)
{
    auto& stairs_tile = tiles_.back();
    if (is_within_distance<tile_size>(player_position, stairs_tile.position)) {
        stairs_tile = {
            get_random_stairs_position(random_engine_, bounds_, tile_size),
            TileType::Stairs};
        return true;
    }
    return false;
}

} // namespace metacore