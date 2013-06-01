#include "entities/event_generators/Polymerization.hpp"

namespace entities {
namespace event_generators {



double
FixedRatePolymerizationBase::rate(State const* state,
        StateModifications const& modifications) {
    return rate_ * state->concentrations[species_]->value()
        * state->filaments.size();
}


StateModifications
FixedRatePolymerizationBase::perform_event(State* state,
        double const& random_number) const {
    uint64_t i = random_number / (
            rate_ * state->concentrations[species_]->value());
    append_species(state->filaments[i].get());
    state->concentrations[species_]->remove_monomer();

    StateModifications modifications;

    modifications.modified_filaments.reserve(1);
    modifications.modified_filaments.emplace_back(i);
    modifications.modified_concentrations.reserve(1);
    modifications.modified_concentrations.emplace_back(species_);

    return modifications;
}


} // namespace event_generators
} // namespace entities
