#pragma once

#include "entities/SimulationState.hpp"
#include "entities/IStateComponent.hpp"
#include <memory>
#include <vector>

namespace entities {

class IEvent {
public:
    virtual ~IEvent() {}

    virtual std::vector<std::unique_ptr<IStateComponent const> >
        apply(SimulationState const* state) const = 0;
};

} // namespace entities
