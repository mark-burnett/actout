#pragma once

#include "entities/common.hpp"

#include <inttypes.h>
#include <memory>
#include <vector>


namespace entities {


class StateModifications {
public:
    StateModifications() : has_deleted_filaments(false) {}

    std::vector<int64_t> created_filaments;
    std::vector<int64_t> deleted_filaments;
    std::vector<int64_t> modified_filaments;

    std::vector<int64_t> modified_concentrations;
};


} // namespace entities
