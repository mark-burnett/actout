#include "entities/IEndCondition.hpp"
#include "entities/IEvent.hpp"
#include "entities/IStateComponent.hpp"
#include "entities/Simulation.hpp"
#include "entities/SimulationState.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>

using namespace std::placeholders;

namespace entities {

std::unique_ptr<SimulationState const>
Simulation::execute(SimulationState const* initial_state) {
    std::size_t event_count = 0;
    double time = 0;

    auto state = std::unique_ptr<SimulationState>(
            new SimulationState(*initial_state));

    reset();

    std::unique_ptr<IEvent const> previous_event;
    std::vector<std::unique_ptr<IStateComponent const> >
        modified_state_components;

    while (true) {
        double total_event_rate = calculate_total_event_rate(state.get(),
                time, modified_state_components);

        double dt = rng_->exponential(total_event_rate);

        // measure!

        if (std::any_of(end_conditions_->cbegin(), end_conditions_->cend(),
                std::bind(&IEndCondition::satisfied,
                    _1, state.get(), time, event_count))) {
            break;
        }

        double r = rng_->uniform(0, total_event_rate);
        auto event = next_event(state.get(), r, total_event_rate);

        modified_state_components = event->apply(state.get());

        time += dt;
        event_count++;
    }

    return std::unique_ptr<SimulationState const>(new SimulationState());
}


double
Simulation::calculate_total_event_rate(SimulationState const* state, double const& time,
        std::vector<std::unique_ptr<IStateComponent const> > const&
            modified_state_components) const {
    return double();
}


std::unique_ptr<IEvent const>
Simulation::next_event(SimulationState const* state,
        double const& r, double const& total_event_rate) const {
    return std::unique_ptr<IEvent const>();
}


void
Simulation::reset() {
    rng_->reset();
    for (auto i = event_generators_->begin();
            i != event_generators_->end(); ++i) {
        i->reset();
    }
}

} // namespace entities
