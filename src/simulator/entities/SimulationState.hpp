#pragma once

#include "entities/IConcentration.hpp"
#include "entities/IFilament.hpp"

#include <inttypes.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

using entities::state_components::IConcentration;
using entities::state_components::IFilament;

namespace entities {

struct SimulationState {
    double time;
    double total_event_rate;
    uint64_t event_count;

    typedef std::vector<std::unique_ptr<IFilament> > filaments_t;
    filaments_t filaments;

    typedef std::map<std::string, std::unique_ptr<IConcentration> >
            concentrations_t;
    concentrations_t concentrations;
};

} // namespace entities
