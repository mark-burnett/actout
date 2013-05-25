#pragma once

#include "entities/IStateComponent.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace entities {

class SimulationState {
public:
    double time;
    double total_event_rate;
    std::size_t event_count;

    SimulationState() : time(0), total_event_rate(0), event_count(0) {}

    std::vector<IStateComponent const*> operator[](
            std::string const& tag) const;
};

} // namespace entities
