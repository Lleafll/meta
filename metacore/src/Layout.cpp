#include "Layout.h"

namespace metacore {

std::span<Tile const> Layout::tiles() const
{
    return layout_.tiles();
}

} // namespace metacore