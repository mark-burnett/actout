#pragma once

#include "entities/IEndCondition.hpp"
#include "entities/IEvent.hpp"
#include "entities/IEventGenerator.hpp"
#include "entities/IMeasurement.hpp"
#include "entities/IRNG.hpp"
#include "entities/SimulationState.hpp"

#include <memory>
#include <vector>

namespace entities {

struct SimulationResult {
    std::unique_ptr<SimulationState> state;
    std::vector<std::unique_ptr<IMeasurement> > measurements;
};

class Simulation {
private:
    typedef std::vector<std::unique_ptr<IEndCondition const> > ec_container;
    typedef std::vector<std::unique_ptr<IEventGenerator> > eg_container;

    std::unique_ptr<IRNG>& rng_;
    ec_container const& end_conditions_;
    eg_container& event_generators_;

public:
    Simulation(std::unique_ptr<IRNG>&& rng,
            ec_container const&& end_conditions,
            eg_container&& event_generators)
        : rng_(rng),
        end_conditions_(end_conditions),
        event_generators_(event_generators) {};

    SimulationResult execute(std::unique_ptr<SimulationState>& state,
            std::vector<std::unique_ptr<IMeasurement> >& measurements);

private:
    std::vector<double> calculate_accumulated_rates(
            SimulationState const* state,
            std::vector<std::unique_ptr<IStateComponent const> > const&
                modified_state_components);
    std::unique_ptr<IEvent const> next_event(SimulationState const* state,
            std::vector<double> const& event_rates) const;
};

} // namespace entities
