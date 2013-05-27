#pragma once

#include "entities/SimulationState.hpp"
#include "entities/IEvent.hpp"
#include "entities/IStateComponent.hpp"

#include "entities/state_components/UnsignedInteger.hpp"

#include <memory>
#include <vector>


namespace entities {
namespace events {

class Increment : public IEvent {
private:
    uint64_t component_id_;
    uint64_t amount_;

public:
    Increment(uint64_t component_id, uint64_t amount)
        : component_id_(component_id), amount_(amount) {}
    std::vector<StateModificationDescriptor>
        apply(SimulationState* state) const {
            auto c = static_cast<state_components::UnsignedInteger*>(
                    state->get(component_id_));
            c->value += amount_;

            std::vector<StateModificationDescriptor> v;
            auto smd = StateModificationDescriptor();
            smd.component_id = component_id_;
            smd.deleted = false;
            v.push_back(smd);
            return v;
        }
};

} // namespace events
} // namespace entities
