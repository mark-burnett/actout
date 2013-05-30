#include "entities/event_generators/Polymerization.hpp"


namespace entities {
namespace event_generators {


double
FixedRatePolymerizationBase::rate(State const* state,
        std::vector<StateModificationDescriptor> const&
                modified_state_components) {
    return rate_ * state->concentrations[species_]->value()
        * state->filaments.size();
}

std::vector<StateModificationDescriptor>
FixedRatePolymerizationBase::perform_event(State* state,
        double const& random_number) const {
    uint64_t i = random_number / rate_;
    append_species(state->filaments[i].get());
    std::vector<StateModificationDescriptor> r;
    r.reserve(1);
    r.emplace_back(i, StateModificationDescriptor::FILAMENT,
            StateModificationDescriptor::MODIFIED);
    return r;
}


} // namespace event_generators
} // namespace entities
