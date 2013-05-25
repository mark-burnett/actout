#include "entities/IEndCondition.hpp"
#include "entities/IEvent.hpp"
#include "entities/IStateComponent.hpp"
#include "entities/Simulation.hpp"
#include "entities/SimulationState.hpp"

#include <algorithm>
#include <functional>
#include <memory>

using namespace std::placeholders;

namespace entities {

std::unique_ptr<SimulationState>
Simulation::execute(std::unique_ptr<SimulationState>& state) {
    std::vector<std::unique_ptr<IStateComponent const> >
        modified_state_components;

    while (true) {
        state->total_event_rate = calculate_total_event_rate(
                state.get(), modified_state_components);

        double dt = rng_->exponential(state->total_event_rate);

        // measure!

        if (std::any_of(end_conditions_->cbegin(), end_conditions_->cend(),
                std::bind(&IEndCondition::satisfied, _1, state.get()))) {
            break;
        }

        double r = rng_->uniform(0, state->total_event_rate);
        auto event = next_event(state.get(), r, state->total_event_rate);

        modified_state_components = event->apply(state.get());

        state->time += dt;
        state->event_count++;
    }

    return std::unique_ptr<SimulationState>(new SimulationState());
}


double
Simulation::calculate_total_event_rate(SimulationState const* state,
        std::vector<std::unique_ptr<IStateComponent const> > const&
            modified_state_components) {
    double R = 0;
    for (auto g = event_generators_->begin();
            g != event_generators_->end(); ++g) {
        R += (*g)->rate(state, modified_state_components);
    }

    return R;
}


std::unique_ptr<IEvent const>
Simulation::next_event(SimulationState const* state,
        double const& r, double const& total_event_rate) const {
    return std::unique_ptr<IEvent const>();
}

} // namespace entities
