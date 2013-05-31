#pragma once

#include "entities/common.hpp"

#include <string>
#include <vector>

namespace entities {

class SpeciesMap {
public:
    species_t get_species_index(std::string const& species_name);
    std::string const get_species_string(
            species_t const& species_index) const;

private:
    std::vector<std::string> species_map_;
};

} // namespace entities
