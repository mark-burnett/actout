#include "entities/event_generators/Depolymerization.hpp"

#include <iterator>
#include <vector>


namespace entities {
namespace event_generators {

double
FixedRateDepolymerization::rate(State const* state,
        StateModifications const& modifications) {
    update_cache(state, modifications);
    return matching_filament_cache.size() * rate_;
}


StateModifications
FixedRateDepolymerization::perform_event(State* state,
        double const& random_number) const {
    uint64_t relative_index = static_cast<uint64_t>(random_number / rate_);
    auto iter = matching_filament_cache.begin();
    std::advance(iter, relative_index);

    pop_filament_tip(state->filaments[*iter].get());

    state->concentrations[species_]->add_monomer();

    StateModifications rv;
    rv.modified_filaments.reserve(1);
    rv.modified_filaments.emplace_back(*iter);
    rv.modified_concentrations.reserve(1);
    rv.modified_concentrations.emplace_back(species_);

    return rv;
}


void
FixedRateDepolymerization::update_cache(State const* state,
        StateModifications const& modifications) {
    update_cache_deleted_filament(modifications);
    update_cache_created_filaments(state, modifications);
    update_cache_modified_filaments(state, modifications);
}


void
FixedRateDepolymerization::update_cache_deleted_filament(
        StateModifications const& modifications) {
    if (modifications.has_deleted_filament) {
        auto i = matching_filament_cache.lower_bound(
                modifications.deleted_filament);
        if (modifications.deleted_filament == *i) {
            i = matching_filament_cache.erase(i);
        }

        for (; i != matching_filament_cache.end(); ++i) {
            matching_filament_cache.insert(i, *i - 1);
            matching_filament_cache.erase(i);
        }
    }
}


void
FixedRateDepolymerization::update_cache_created_filaments(State const* state,
        StateModifications const& modifications) {
    for (auto& filament_id : modifications.created_filaments) {
        if (species_ == peek_filament(state->filaments[filament_id].get())) {
            // New filaments should almost always be at the end
            matching_filament_cache.insert(matching_filament_cache.end(),
                    filament_id);
        }
    }
}


void
FixedRateDepolymerization::update_cache_modified_filaments(State const* state,
        StateModifications const& modifications) {
    for (auto& filament_id : modifications.modified_filaments) {
        if (species_ == peek_filament(state->filaments[filament_id].get())) {
            matching_filament_cache.insert(filament_id);
        } else {
            matching_filament_cache.erase(filament_id);
        }
    }
}


} // namespace event_generators
} // namespace entities
