#pragma once

#include "entities/SimulationState.hpp"
#include <cstddef>

namespace entities {

class IEndCondition {
public:
    virtual ~IEndCondition() {}

    virtual bool satisfied(SimulationState const* state) const = 0;
};

} // namespace entities
