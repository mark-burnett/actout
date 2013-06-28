#pragma once

#include "simulator/IConcentration.hpp"
#include "simulator/IFilament.hpp"
#include "simulator/SpeciesMap.hpp"
#include "simulator/common.hpp"

#include <inttypes.h>
#include <memory>
#include <vector>


using simulator::state::IConcentration;
using simulator::state::IFilament;


namespace simulator {


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


} // namespace simulator
