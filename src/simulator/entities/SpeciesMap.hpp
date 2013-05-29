#pragma once

#include <string>
#include <vector>

namespace entities {

class SpeciesMap {
private:
    std::vector<std::string> species_map_;

public:
    typedef std::vector<std::string>::size_type species_t;

    species_t get_species_index(std::string const& species_name);
    std::string const get_species_string(
            species_t const& species_index) const;
};

} // namespace entities
