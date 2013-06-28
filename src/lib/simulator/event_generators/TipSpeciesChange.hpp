#pragma once

#include "simulator/IEventGenerator.hpp"
#include "simulator/IFilament.hpp"
#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"

#include <set>

namespace simulator {
namespace event_generators {

class FixedRateTipSpeciesChange : public IEventGenerator {
public:
    FixedRateTipSpeciesChange(species_t const& old_species,
            species_t const& new_species, double const& rate)
        : old_species_(old_species), new_species_(new_species), rate_(rate) {}

    double rate(State const* state, StateModifications const& modifications);

    StateModifications perform_event(State* state,
            double const& random_number) const;

protected:
    species_t const old_species_;
    species_t const new_species_;

private:
    virtual species_t peek_filament(IFilament const* filament) const = 0;
    virtual void update_filament_tip(IFilament* filament) const = 0;

    void update_cache(State const* state,
            StateModifications const& modifications);
    void update_cache_deleted_filament(StateModifications const& modifications);
    void update_cache_created_filaments(State const* state,
            StateModifications const& modifications);
    void update_cache_modified_filaments(State const* state,
            StateModifications const& modifications);

    double const rate_;
    std::set<uint64_t> matching_filament_cache;
};


class BarbedTipSpeciesChange : public FixedRateTipSpeciesChange {
public:
    BarbedTipSpeciesChange(species_t const& old_species,
            species_t const& new_species, double const& rate)
        : FixedRateTipSpeciesChange(old_species, new_species, rate) {}
private:
    species_t peek_filament(IFilament const* filament) const {
        return filament->peek_barbed();
    }

    void update_filament_tip(IFilament* filament) const {
        filament->pop_barbed();
        filament->append_barbed(new_species_);
    }
};


class PointedTipSpeciesChange : public FixedRateTipSpeciesChange {
public:
    PointedTipSpeciesChange(species_t const& old_species,
            species_t const& new_species, double const& rate)
        : FixedRateTipSpeciesChange(old_species, new_species, rate) {}
private:
    species_t peek_filament(IFilament const* filament) const {
        return filament->peek_pointed();
    }

    void update_filament_tip(IFilament* filament) const {
        filament->pop_pointed();
        filament->append_pointed(new_species_);
    }
};


} // namespace event_generators
} // namespace simulator
