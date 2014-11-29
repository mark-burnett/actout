#pragma once

#include "simulator/State.hpp"

#include <inttypes.h>

namespace simulator {
namespace end_conditions {

class Duration {
private:
    double const end_time_;

public:
    Duration(double const& end_time)
        : end_time_(end_time) {}

    bool satisfied(State const* state) {
        return state->time > end_time_;
    }
};

} // namespace end_conditions
} // namespace simulator
