#pragma once

#include "entities/SimulationState.hpp"

#include <inttypes.h>
#include <memory>
#include <vector>

namespace entities {

struct StateModificationDescriptor {
    uint64_t component_id;

    enum component_type_t {CONCENTRATION, FILAMENT};
    component_type_t component_type;

    enum modification_t {CREATED, DELETED, MODIFIED};
    modification_t modification;
};

class IEvent {
public:
    virtual ~IEvent() {}

    virtual std::vector<StateModificationDescriptor>
        apply(SimulationState* state) const = 0;
};

} // namespace entities
