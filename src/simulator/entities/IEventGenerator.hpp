#pragma once

#include "entities/State.hpp"
#include "entities/StateModifications.hpp"


namespace entities {


class IEventGenerator {
public:
    virtual ~IEventGenerator() {}

    virtual double rate(State const* state,
            StateModifications const& modifications) = 0;

    virtual StateModifications perform_event(State* state,
            double const& random_number) const = 0;
};


} // namespace entities
