#pragma once

#include "entities/IEvent.hpp"
#include "entities/IEventGenerator.hpp"
#include "entities/SimulationState.hpp"

#include <memory>

namespace entities {
namespace event_generators {

class Increment : public IEventGenerator {
private:
    std::string const tag_;
    double rate_;
    uint64_t amount_;

    double cached_rate_;

public:
    Increment(std::string const& tag, double const& rate,
            uint64_t const& amount) : tag_(tag), rate_(rate), amount_(amount) {}

    double rate(SimulationState const* state,
            std::vector<StateModificationDescriptor> const&
                modified_state_components);

    std::unique_ptr<IEvent const> create_event(SimulationState const* state,
            double const& random_number) const;
};

} // namespace event_generators
} // namespace entities
