#pragma once

#include <cstddef>

namespace entities {

class SimulationState {
public:
    double time;
    double total_event_rate;
    std::size_t event_count;
};

} // namespace entities
