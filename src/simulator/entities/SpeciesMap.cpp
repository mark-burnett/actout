#include "entities/SpeciesMap.hpp"

#include <algorithm>


namespace entities {


SpeciesMap::species_index_t
SpeciesMap::get_species_index(std::string const& species_string) {
    auto i = std::find(species_map_.begin(),
            species_map_.end(), species_string);
    if (i == species_map_.end()) {
        species_map_.push_back(species_string);
        return species_map_.size() - 1;

    } else {
        return std::distance(species_map_.begin(), i);
    }
}


std::string const
SpeciesMap::get_species_string(species_index_t& species_index) const {
    return species_map_[species_index];
}


} // namespace entities
