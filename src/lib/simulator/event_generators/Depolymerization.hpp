#pragma once

#include "simulator/IEventGenerator.hpp"
#include "simulator/IFilament.hpp"
#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"

#include <set>

namespace simulator {
namespace event_generators {

class FixedRateDepolymerization : public IEventGenerator {
public:
    FixedRateDepolymerization(species_t const& species,
            double const& rate) : species_(species), rate_(rate) {}

    double rate(State const* state, StateModifications const& modifications);

    StateModifications perform_event(State* state,
            double const& random_number) const;

protected:
    species_t const species_;

private:
    virtual species_t peek_filament(IFilament const* filament) const = 0;
    virtual species_t pop_filament_tip(IFilament* filament) const = 0;

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


class BarbedEndDepolymerization : public FixedRateDepolymerization {
public:
    BarbedEndDepolymerization(species_t const& species,
            double const& rate) : FixedRateDepolymerization(species, rate) {}
private:
    species_t peek_filament(IFilament const* filament) const {
        return filament->peek_barbed();
    }

    species_t pop_filament_tip(IFilament* filament) const {
        return filament->pop_barbed();
    }
};


class PointedEndDepolymerization : public FixedRateDepolymerization {
public:
    PointedEndDepolymerization(species_t const& species,
            double const& rate) : FixedRateDepolymerization(species, rate) {}
private:
    species_t peek_filament(IFilament const* filament) const {
        return filament->peek_pointed();
    }

    species_t pop_filament_tip(IFilament* filament) const {
        return filament->pop_pointed();
    }
};


} // namespace event_generators
} // namespace simulator
