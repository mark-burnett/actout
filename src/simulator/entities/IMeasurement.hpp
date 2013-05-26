#pragma once

#include "entities/SimulationState.hpp"

namespace entities {

class IMeasurement {
public:
    virtual ~IMeasurement() {}

    virtual void perform(SimulationState const* state) = 0;
};

} // namespace entities
