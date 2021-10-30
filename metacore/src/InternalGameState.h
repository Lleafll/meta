#pragma once

#include "DefaultState.h"
#include "GameOverState.h"
#include "PickingUpState.h"
#include <variant>

namespace metacore {

struct InternalGameState final {
    std::variant<DefaultState, PickingUpState, GameOverState> value;
};

} // namespace metacore