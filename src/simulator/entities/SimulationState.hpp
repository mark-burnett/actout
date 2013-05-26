#pragma once

#include "entities/IStateComponent.hpp"

#include <inttypes.h>
#include <string>
#include <vector>

namespace entities {

class SimulationState {
public:
    double time;
    double total_event_rate;
    uint64_t event_count;
    uint64_t next_component_id;

    SimulationState() : time(0), total_event_rate(0), event_count(0) {}

    std::vector<IStateComponent const*> operator[](
            std::string const& tag) const;
};

} // namespace entities
