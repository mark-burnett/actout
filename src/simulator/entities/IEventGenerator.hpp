#pragma once

#include "entities/IEvent.hpp"
#include "entities/State.hpp"
#include <memory>

namespace entities {

class IEventGenerator {
public:
    virtual ~IEventGenerator() {}

    virtual double rate(State const* state,
            std::vector<StateModificationDescriptor> const&
                modified_state_components) = 0;

    virtual std::unique_ptr<IEvent const> create_event(
            State const* state,
            double const& random_number) const = 0;
};

} // namespace entities
