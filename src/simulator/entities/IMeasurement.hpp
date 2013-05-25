#pragma once

#include "entities/SimulationState.hpp"

namespace entities {

class IMeasurement {
public:
    virtual ~IMeasurement() {}

    virtual void perform(SimulationState const* state,
                double const& next_event_time) = 0;
};

} // namespace entities
