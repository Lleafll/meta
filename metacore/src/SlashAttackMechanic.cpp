#include "SlashAttackMechanic.h"

namespace metacore {

bool SlashAttackMechanic::is_active() const
{
    return is_active_;
}

void SlashAttackMechanic::start()
{
    is_active_ = true;
}

void SlashAttackMechanic::tick()
{
    is_active_ = false;
}

} // namespace metacore