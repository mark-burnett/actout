#pragma once

#include "entities/SimulationState.hpp"
#include "entities/IStateComponent.hpp"

#include <inttypes.h>
#include <memory>
#include <vector>

namespace entities {

struct StateModificationDescriptor {
    uint64_t component_id;
    // XXX make this an enum (created, deleted, modified);
    bool deleted;
};

class IEvent {
public:
    virtual ~IEvent() {}

    virtual std::vector<StateModificationDescriptor>
        apply(SimulationState* state) const = 0;
};

} // namespace entities
