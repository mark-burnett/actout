#pragma once

#include "simulator/State.hpp"

#include <inttypes.h>

namespace simulator {
namespace end_conditions {

class EventCount {
private:
    uint64_t max_events_;

public:
    EventCount(uint64_t const& max_events)
        : max_events_(max_events) {}

    bool satisfied(State const* state) {
        return (state->event_count >= max_events_);
    }
};

} // namespace end_conditions
} // namespace simulator
