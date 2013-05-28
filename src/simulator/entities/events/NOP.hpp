#pragma once

#include "entities/State.hpp"
#include "entities/IEvent.hpp"

#include <memory>
#include <vector>

namespace entities {
namespace events {

class NOP : public IEvent {
public:
    std::vector<StateModificationDescriptor>
        apply(State* state) const {
            return std::vector<StateModificationDescriptor>(); }
};

} // namespace events
} // namespace entities
