#pragma once

#include <string>
#include <vector>

namespace entities {

class SpeciesMap {
private:
    std::vector<std::string> species_map_;

public:
    typedef std::vector<std::string>::size_type const species_index_t;

    species_index_t get_species_index(std::string const& species_string);
    std::string const get_species_string(species_index_t& species_index) const;
};

} // namespace entities
