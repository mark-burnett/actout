#pragma once

#include "entities/IEndCondition.hpp"
#include "entities/IEventGenerator.hpp"
#include "entities/IMeasurement.hpp"
#include "entities/IRNG.hpp"
#include "entities/State.hpp"

#include <memory>
#include <vector>

namespace entities {

class Simulation {
private:
    typedef std::vector<std::unique_ptr<IEndCondition const> > ec_container;
    typedef std::vector<std::unique_ptr<IEventGenerator> > eg_container;

public:
    Simulation(ec_container const& end_conditions,
            eg_container& event_generators)
        : end_conditions_(end_conditions),
        event_generators_(event_generators) {};

    void execute(State* state,
            std::vector<std::unique_ptr<IMeasurement> >& measurements,
            IRNG* rng);

private:
    std::vector<double> calculate_accumulated_rates(
            State const* state,
            std::vector<StateModificationDescriptor> const&
                modified_state_components);
    std::vector<StateModificationDescriptor> perform_next_event(
            State* state, std::vector<double> const& accumulated_rates,
            IRNG* rng) const;

private:
    ec_container const& end_conditions_;
    eg_container& event_generators_;
};

} // namespace entities
