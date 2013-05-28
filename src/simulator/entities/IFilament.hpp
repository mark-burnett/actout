#pragma once

#include "entities/SpeciesMap.hpp"

#include <inttypes.h>


namespace entities {
namespace state_components {


class IFilament {
public:
    virtual ~IFilament() {}

    // query filament status
    virtual uint64_t species_count(
            SpeciesMap::species_index_t& species) const = 0;
    virtual uint64_t boundary_count(SpeciesMap::species_index_t& pointed_state,
            SpeciesMap::species_index_t& barbed_state) const = 0;
    virtual uint64_t length() const = 0;

    // XXX Maybe we can provide an alternative marshalling mechanism?
    //     What about analysis and plotting filaments?
//    virtual std::vector<SpeciesMap::species_index_t const> get_data() const = 0;

    // query tip states
    virtual SpeciesMap::species_index_t peek_barbed() const = 0;
    virtual SpeciesMap::species_index_t peek_pointed() const = 0;


    // add element to either end
    virtual void append_barbed(SpeciesMap::species_index_t& species) = 0;
    virtual void append_pointed(SpeciesMap::species_index_t& species) = 0;

    // remove element from either end
    virtual SpeciesMap::species_index_t pop_barbed() = 0;
    virtual SpeciesMap::species_index_t pop_pointed() = 0;

    // change nth specific state to *
    virtual void update_state(uint64_t instance_number,
            SpeciesMap::species_index_t& old_species,
            SpeciesMap::species_index_t& new_species) = 0;
    // change +/- element of nth specific boundary of type * to *
    virtual void update_boundary(uint64_t instance_number,
            SpeciesMap::species_index_t& old_pointed_species,
            SpeciesMap::species_index_t& old_barbed_species,
            SpeciesMap::species_index_t& new_barbed_species) = 0;
};


} // namespace state_components
} // namespace entities
