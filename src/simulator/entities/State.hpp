#pragma once

#include "entities/IConcentration.hpp"
#include "entities/IFilament.hpp"
#include "entities/SpeciesMap.hpp"

#include <inttypes.h>
#include <memory>
#include <vector>


using entities::state::IConcentration;
using entities::state::IFilament;


namespace entities {


struct State {
    double time;
    double total_event_rate;
    uint64_t event_count;

    SpeciesMap species_map;

    std::vector<std::unique_ptr<IFilament> > filaments;
    std::vector<std::unique_ptr<IConcentration> > concentrations;
};


} // namespace entities
