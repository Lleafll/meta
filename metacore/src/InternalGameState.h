#pragma once

#include "DefaultState.h"
#include "GameOverState.h"
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
        PickingUpState,
        GameOverState>
        value;
};

} // namespace metacore