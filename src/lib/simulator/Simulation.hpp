#pragma once

#include "simulator/IEndCondition.hpp"
#include "simulator/IEventGenerator.hpp"
#include "simulator/IMeasurement.hpp"
#include "simulator/IRNG.hpp"
#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"

#include <memory>
#include <vector>

namespace simulator {

class Simulation {
private:
    typedef std::vector<std::unique_ptr<IEventGenerator> > eg_container;

public:
    Simulation(std::vector<EndCondition> end_conditions,
            eg_container& event_generators)
        : end_conditions_(end_conditions),
        event_generators_(event_generators) {};

    void execute(State* state,
            std::vector<Measurement>& measurements,
            IRNG* rng);

    StateModifications get_initial_modifications(State const* state) const;

private:
    std::vector<double> calculate_accumulated_rates(State const* state,
            StateModifications const& modifications);
    StateModifications perform_next_event(State* state,
            std::vector<double> const& accumulated_rates, IRNG* rng) const;

private:
    std::vector<EndCondition> end_conditions_;
    eg_container& event_generators_;
};

} // namespace simulator
