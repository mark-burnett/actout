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
    uint64_t i = random_number / (
            rate_ * state->concentrations[species_]->value());
    append_species(state->filaments[i].get());
    state->concentrations[species_]->remove_monomer();

    std::vector<StateModificationDescriptor> r;
    r.reserve(2);
    r.emplace_back(i, StateModificationDescriptor::FILAMENT,
            StateModificationDescriptor::MODIFIED);
    r.emplace_back(species_, StateModificationDescriptor::CONCENTRATION,
            StateModificationDescriptor::MODIFIED);

    return r;
}


} // namespace event_generators
} // namespace entities
