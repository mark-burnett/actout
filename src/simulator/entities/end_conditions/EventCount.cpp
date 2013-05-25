#include "entities/end_conditions/EventCount.hpp"

namespace entities {
namespace end_conditions {

bool
EventCount::satisfied(SimulationState const* state) const {
    return (state->event_count >= max_events_);
}

} // namespace end_conditions
} // namespace entities
