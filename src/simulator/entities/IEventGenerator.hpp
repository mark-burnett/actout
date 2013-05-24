#pragma once

#include "entities/IEvent.hpp"
#include "entities/SimulationState.hpp"
#include <memory>

namespace entities {

class IEventGenerator {
public:
    virtual ~IEventGenerator() {}

    virtual double rate(SimulationState const* state, double const& time,
                IEvent const* previous_event) = 0;

    virtual std::unique_ptr<IEvent const> create_event(
                SimulationState const* state, double const& time,
                double const& random_number) const = 0;
};

} // namespace entities
