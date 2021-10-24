#pragma once

#include "DefaultState.h"
#include "PickingUpState.h"
#include <variant>

namespace metacore {

struct InternalGameState final {
    std::variant<DefaultState, PickingUpState> value;
};

} // namespace metacore