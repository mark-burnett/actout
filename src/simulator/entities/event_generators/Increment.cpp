#include "entities/SimulationState.hpp"
#include "entities/event_generators/Increment.hpp"
#include "entities/events/Increment.hpp"

#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>

namespace entities {
namespace event_generators {

double
Increment::rate(SimulationState const* state,
        std::vector<StateModificationDescriptor> const&
            modified_state_components) {
    typename SimulationState::tag_map_t::const_iterator begin, end;
    std::tie(begin, end) = state->find_components(tag_);
    cached_rate_ = rate_ * std::distance(begin, end);

    return cached_rate_;
}

std::unique_ptr<IEvent const>
Increment::create_event(SimulationState const* state,
        double const& random_number) const {
    uint64_t index = static_cast<uint64_t>(random_number / cached_rate_);

    typename SimulationState::tag_map_t::const_iterator begin, end;
    std::tie(begin, end) = state->find_components(tag_);

    std::advance(begin, index);
    uint64_t component_id = state->lookup_id(begin->second);

    return std::unique_ptr<events::Increment const>(
            new events::Increment(component_id, amount_));
}

} // namespace event_generators
} // namespace entities
