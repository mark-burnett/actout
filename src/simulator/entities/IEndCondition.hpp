#pragma once

#include "entities/SimulationState.hpp"

namespace entities {

class IEndCondition {
public:
    virtual ~IEndCondition() {}

    virtual bool satisfied(SimulationState const* state) const = 0;
};

} // namespace entities
