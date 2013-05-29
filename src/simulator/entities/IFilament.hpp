#pragma once

#include "entities/SpeciesMap.hpp"

#include <inttypes.h>


namespace entities {
namespace state {


class IFilament {
public:
    virtual ~IFilament() {}

    // query filament status
    virtual uint64_t species_count(
            SpeciesMap::species_index_t const& species) const = 0;
    virtual uint64_t boundary_count(
            SpeciesMap::species_index_t const& pointed_species,
            SpeciesMap::species_index_t const& barbed_species) const = 0;
    virtual uint64_t length() const = 0;

    // XXX Maybe we can provide an alternative marshalling mechanism?
    //     What about analysis and plotting filaments?
//    virtual std::vector<SpeciesMap::species_index_t const> get_data() const = 0;

    // query tip states
    virtual SpeciesMap::species_index_t const peek_barbed() const = 0;
    virtual SpeciesMap::species_index_t const peek_pointed() const = 0;


    // add element to either end
    virtual void append_barbed(SpeciesMap::species_index_t const& species) = 0;
    virtual void append_pointed(SpeciesMap::species_index_t const& species) = 0;

    // remove element from either end
    virtual SpeciesMap::species_index_t const pop_barbed() = 0;
    virtual SpeciesMap::species_index_t const pop_pointed() = 0;

    // change nth specific subunit to *
    virtual void update_subunit(uint64_t instance_number,
            SpeciesMap::species_index_t const& old_species,
            SpeciesMap::species_index_t const& new_species) = 0;
    // change +/- element of nth specific boundary of type * to *
    virtual void update_boundary(uint64_t instance_number,
            SpeciesMap::species_index_t const& old_pointed_species,
            SpeciesMap::species_index_t const& old_barbed_species,
            SpeciesMap::species_index_t const& new_barbed_species) = 0;
};

class DepolymerizingEmptyFilament : public std::exception {};

} // namespace state
} // namespace entities
