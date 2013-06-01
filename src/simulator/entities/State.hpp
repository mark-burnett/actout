#pragma once

#include "entities/IConcentration.hpp"
#include "entities/IFilament.hpp"
#include "entities/SpeciesMap.hpp"
#include "entities/common.hpp"

#include <inttypes.h>
#include <memory>
#include <vector>


using entities::state::IConcentration;
using entities::state::IFilament;


namespace entities {


struct State {
public:
    State() : time(0), total_event_rate(0), event_count(0) {}

    double time;
    double total_event_rate;
    uint64_t event_count;

    SpeciesMap species_map;

    std::vector<std::unique_ptr<IFilament> > filaments;
    std::vector<std::unique_ptr<IConcentration> > concentrations;
};


} // namespace entities
