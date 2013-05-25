#pragma once

#include "entities/SimulationState.hpp"
#include "entities/IStateComponent.hpp"
#include <memory>
#include <vector>

namespace entities {
namespace events {

class NOP : public IEvent {
public:
    std::vector<std::unique_ptr<IStateComponent const> >
        apply(SimulationState const* state) const {
            return std::vector<std::unique_ptr<IStateComponent const> >(); }
};

} // namespace events
} // namespace entities
