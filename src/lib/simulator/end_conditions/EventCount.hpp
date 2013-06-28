#pragma once

#include "simulator/IEndCondition.hpp"
#include "simulator/State.hpp"

#include <inttypes.h>

namespace simulator {
namespace end_conditions {

class EventCount : public IEndCondition {
private:
    uint64_t max_events_;

public:
    EventCount(uint64_t const& max_events)
        : max_events_(max_events) {}

    bool satisfied(State const* state) const {
        return (state->event_count >= max_events_);
    }
};

} // namespace end_conditions
} // namespace simulator
