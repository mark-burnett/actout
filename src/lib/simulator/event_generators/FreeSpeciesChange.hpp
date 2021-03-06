#pragma once

#include "simulator/IEventGenerator.hpp"
#include "simulator/IFilament.hpp"
#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"


namespace simulator {
namespace event_generators {


class FreeSpeciesChange {
public:
    FreeSpeciesChange(species_t const& old_species,
            std::vector<species_t> const& new_species, double const& rate)
        : old_species_(old_species), new_species_(new_species), rate_(rate) {}

    double rate(State const* state, StateModifications const& modifications) {
        return state->concentrations[old_species_].value() * rate_;
    }

    StateModifications perform_event(State* state, double random_number) const {
        StateModifications m;
        m.modified_concentrations.reserve(1 + new_species_.size());

        state->concentrations[old_species_].remove_monomer();
        m.modified_concentrations.emplace_back(old_species_);

        for (auto& ns : new_species_) {
            state->concentrations[ns].add_monomer();
            m.modified_concentrations.emplace_back(ns);
        }

        return m;
    }

private:
    species_t const old_species_;
    std::vector<species_t> new_species_;
    double const rate_;
};


} // namespace event_generators
} // namespace simulator
