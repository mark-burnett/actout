#pragma once

#include <inttypes.h>
#include <string>

namespace entities {
namespace state_components {

class IFilament {
public:
    virtual ~IFilament() {}

    // query filament status
    virtual uint64_t species_count(std::string const& species) const = 0;
    virtual uint64_t boundary_count(std::string const& pointed_state,
            std::string const& barbed_state) const = 0;
    virtual uint64_t length() const = 0;

    // XXX Maybe we can provide an alternative marshalling mechanism?
    //     What about analysis and plotting filaments?
//    virtual std::vector<std::string const> get_data() const = 0;

    // query tip states
    virtual std::string const peek_barbed() const = 0;
    virtual std::string const peek_pointed() const = 0;


    // add element to either end
    virtual void append_barbed(std::string const& species) = 0;
    virtual void append_pointed(std::string const& species) = 0;

    // remove element from either end
    virtual std::string const pop_barbed() = 0;
    virtual std::string const pop_pointed() = 0;

    // change nth specific state to *
    virtual void update_state(uint64_t instance_number,
            const std::string& old_species, const std::string& new_species) = 0;
    // change +/- element of nth specific boundary of type * to *
    virtual void update_boundary(uint64_t instance_number,
            const std::string& old_pointed_species,
            const std::string& old_barbed_species,
            const std::string& new_barbed_species) = 0;
};

} // namespace state_components
} // namespace entities
