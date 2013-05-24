#pragma once

#include "entities/SimulationState.hpp"
#include <cstddef>

namespace entities {

class IEndCondition {
    public:
        virtual ~IEndCondition() {}

        virtual bool satisfied(SimulationState const* state,
                double const& time, std::size_t const& event_count) const = 0;
};

} // namespace entities
