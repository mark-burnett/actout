#pragma once

#include "entities/IEndCondition.hpp"
#include "entities/SimulationState.hpp"

#include <inttypes.h>

namespace entities {
namespace end_conditions {

class EventCount : public IEndCondition {
private:
    uint64_t max_events_;

public:
    EventCount(uint64_t max_events)
        : max_events_(max_events) {}

    bool satisfied(SimulationState const* state) const;
};

} // namespace end_conditions
} // namespace entities
