#include "DungeonLayoutMechanic.h"

namespace metacore {

namespace {

template<int tile_size, LayoutBounds bounds>
void add_bounds(std::vector<Tile>& tiles)
{
    auto const& [left, right, bottom, top] = bounds;
    for (auto i = left; i < right; i += tile_size) {
        tiles.push_back(Tile{{i, bottom}, TileType::Obstacle});
        tiles.push_back(Tile{{i, top - tile_size}, TileType::Obstacle});
    }
    for (auto i = bottom + tile_size; i < top - tile_size; i += tile_size) {
        tiles.push_back(Tile{{left, i}, TileType::Obstacle});
        tiles.push_back(Tile{{right - tile_size, i}, TileType::Obstacle});
    }
}

template<int tile_size, LayoutBounds bounds>
void add_tiles(std::mt19937& random_engine, std::vector<Tile>& tiles)
{
    auto distribution = std::uniform_int_distribution{0, 10};
    for (auto x = bounds.left; x < bounds.right; x += tile_size) {
        for (auto y = bounds.bottom; y < bounds.top; y += tile_size) {
            if (distribution(random_engine) == 10) {
                tiles.push_back(Tile{{x, y}, TileType::Obstacle});
            }
        }
    }
}

template<int tile_size, LayoutBounds bounds>
Position get_random_stairs_position(std::mt19937& random_engine)
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
    add_bounds<tile_size, bounds_>(tiles_);
    tiles_.push_back(
        {get_random_stairs_position<tile_size, bounds_>(random_engine_),
         TileType::Stairs});
}

std::vector<Tile> const& DungeonLayoutMechanic::tiles() const
{
    return tiles_;
}

bool DungeonLayoutMechanic::check_for_transition(
    Position const& player_position)
{
    auto& stairs_tile = tiles_.back();
    if (is_within_distance<tile_size>(player_position, stairs_tile.position)) {
        tiles_ = {};
        add_bounds<tile_size, bounds_>(tiles_);
        add_tiles<tile_size, bounds_>(random_engine_, tiles_);
        tiles_.push_back(
            {get_random_stairs_position<tile_size, bounds_>(random_engine_),
             TileType::Stairs});
        return true;
    }
    return false;
}

} // namespace metacore