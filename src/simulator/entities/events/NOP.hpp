#pragma once

#include "entities/SimulationState.hpp"
#include "entities/IEvent.hpp"
#include "entities/IStateComponent.hpp"

#include <memory>
#include <vector>

namespace entities {
namespace events {

class NOP : public IEvent {
public:
    std::vector<StateModificationDescriptor>
        apply(SimulationState* state) const {
            return std::vector<StateModificationDescriptor>(); }
};

} // namespace events
} // namespace entities
