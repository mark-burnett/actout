#include "entities/IEndCondition.hpp"
#include "entities/IEvent.hpp"
#include "entities/IMeasurement.hpp"
#include "entities/IStateComponent.hpp"
#include "entities/Simulation.hpp"
#include "entities/SimulationState.hpp"

#include <algorithm>
#include <functional>
#include <memory>

using namespace std::placeholders;

namespace entities {

SimulationResult
Simulation::execute(std::unique_ptr<SimulationState>& state,
        std::vector<std::unique_ptr<IMeasurement> >& measurements) {

    std::vector<std::unique_ptr<IStateComponent const> >
        modified_state_components;

    while (true) {
        auto accumulated_rates = calculate_accumulated_rates(
                state.get(), modified_state_components);
        state->total_event_rate = accumulated_rates.back();

        double next_event_time = state->time
            + rng_->exponential(state->total_event_rate);

        for (auto m = measurements.begin(); m != measurements.end(); ++m) {
            (*m)->perform(state.get(), next_event_time);
        }

        if (std::any_of(end_conditions_.cbegin(), end_conditions_.cend(),
                std::bind(&IEndCondition::satisfied, _1, state.get()))) {
            break;
        }

        auto event = next_event(state.get(), accumulated_rates);

        modified_state_components = event->apply(state.get());

        state->time = next_event_time;
        state->event_count++;
    }

    SimulationResult sr;
    std::swap(sr.state, state);
    std::swap(sr.measurements, measurements);

    return sr;
}


std::vector<double>
Simulation::calculate_accumulated_rates(SimulationState const* state,
            std::vector<std::unique_ptr<IStateComponent const> > const&
                modified_state_components) {
    std::vector<double> accumulated_rates;
    accumulated_rates.reserve(event_generators_.size());

    double accumulated_rate = 0;
    for (auto i = event_generators_.begin();
            i < event_generators_.end(); ++i) {
         accumulated_rate += (*i)->rate(state, modified_state_components);
         accumulated_rates.push_back(accumulated_rate);
    }

    return accumulated_rates;
}


std::unique_ptr<IEvent const>
Simulation::next_event(SimulationState const* state,
        std::vector<double> const& accumulated_rates) const {
    double r = rng_->uniform(0, state->total_event_rate);
    auto i = std::lower_bound(accumulated_rates.cbegin(), accumulated_rates.cend(), r);

    return event_generators_[i - accumulated_rates.cbegin()]->create_event(
            state, *i - r);
}

} // namespace entities
