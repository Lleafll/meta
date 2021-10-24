#pragma once

#include "DefaultState.h"
#include "InitialPickingUpState.h"
#include "InitialState.h"
#include "PickingUpState.h"
#include <variant>

namespace metacore {

struct InternalGameState final {
    std::variant<
        InitialState,
        InitialPickingUpState,
        DefaultState,
        PickingUpState>
        value;
};

} // namespace metacore