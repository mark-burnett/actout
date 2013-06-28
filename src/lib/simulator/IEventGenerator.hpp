#pragma once

#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"


namespace simulator {


class IEventGenerator {
public:
    virtual ~IEventGenerator() {}

    virtual double rate(State const* state,
            StateModifications const& modifications) = 0;

    virtual StateModifications perform_event(State* state,
            double const& random_number) const = 0;
};


} // namespace simulator
