#pragma once

#include "entities/common.hpp"

#include <inttypes.h>
#include <memory>
#include <vector>


namespace entities {


class StateModifications {
public:
    StateModifications() : has_deleted_filament(false) {}

    bool has_deleted_filament;
    uint64_t deleted_filament;

    std::vector<uint64_t> created_filaments;
    std::vector<uint64_t> modified_filaments;

    std::vector<uint64_t> modified_concentrations;
};


} // namespace entities
