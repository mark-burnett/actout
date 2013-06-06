#include "entities/State.hpp"
#include "entities/StateModifications.hpp"
#include "entities/event_generators/FilamentSpeciesChangeRandom.hpp"


namespace entities {
namespace event_generators {


double
FilamentSpeciesChangeRandom::rate(State const* state,
        StateModifications const& modifications) {
    update_cache(state, modifications);
    return rate_ * cached_count_;
}


StateModifications
FilamentSpeciesChangeRandom::perform_event(State* state,
        double const& random_number) const {
    uint64_t subunit_index = static_cast<uint64_t>(random_number / rate_);

    uint64_t filament_index = 0;
    for (; filament_index < cached_counts_.size(); ++filament_index) {
        if (cached_counts_[filament_index] <= subunit_index) {
            subunit_index -= cached_counts_[filament_index];
        } else {
            break;
        }
    }

    state->filaments[filament_index]->update_subunit(subunit_index,
            old_species_, new_species_);

    StateModifications m;
    m.modified_filaments.reserve(1);
    m.modified_filaments.emplace_back(filament_index);
    return m;
}


void
FilamentSpeciesChangeRandom::update_cache(State const* state,
        StateModifications const& modifications) {
    if (modifications.has_deleted_filament) {
        auto i = cached_counts_.begin();
        std::advance(i, modifications.deleted_filament);
        cached_count_ -= *i;
        cached_counts_.erase(i);
    }

    for (auto& i : modifications.modified_filaments) {
        cached_count_ -= cached_counts_[i];
        cached_counts_[i] = state->filaments[i]->species_count(old_species_);
        cached_count_ += cached_counts_[i];
    }

    for (auto& i : modifications.created_filaments) {
        // XXX Now there's a restriction that new filaments are added to the end
        cached_counts_.emplace_back(
                state->filaments[i]->species_count(old_species_));
        cached_count_ += cached_counts_[i];
    }
}

} // namespace event_generators
} // namespace entities
