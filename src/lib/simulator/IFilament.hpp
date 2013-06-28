#pragma once

#include "simulator/common.hpp"

#include <inttypes.h>


namespace simulator {
namespace state {


class IFilament {
public:
    virtual ~IFilament() {}

    // query filament status
    virtual uint64_t const species_count(
            species_t const& species) const = 0;
    virtual uint64_t const boundary_count(
            species_t const& pointed_species,
            species_t const& barbed_species) const = 0;
    virtual uint64_t const boundary_count() const = 0;
    virtual uint64_t const length() const = 0;

    // query tip states
    virtual species_t const peek_barbed() const = 0;
    virtual species_t const peek_pointed() const = 0;


    // add element to either end
    virtual void append_barbed(species_t const& species) = 0;
    virtual void append_pointed(species_t const& species) = 0;

    // remove element from either end
    virtual species_t const pop_barbed() = 0;
    virtual species_t const pop_pointed() = 0;

    // change nth specific subunit to *
    virtual void update_subunit(uint64_t instance_number,
            species_t const& old_species,
            species_t const& new_species) = 0;
    // change +/- element of nth specific boundary of type * to *
    virtual void update_boundary(uint64_t instance_number,
            species_t const& old_pointed_species,
            species_t const& old_barbed_species,
            species_t const& new_barbed_species) = 0;
};


} // namespace state
} // namespace simulator
