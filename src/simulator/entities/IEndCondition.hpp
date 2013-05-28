#pragma once

#include "entities/State.hpp"

namespace entities {

class IEndCondition {
public:
    virtual ~IEndCondition() {}

    virtual bool satisfied(State const* state) const = 0;
};

} // namespace entities
