#include "Enemies.h"

namespace metacore {

Enemies::Enemies(std::vector<Position> positions)
    : positions_{std::move(positions)}
{
}

std::vector<Position> const& Enemies::positions() const
{
    return positions_;
}

} // namespace metacore