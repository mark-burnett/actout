#pragma once

#include "entities/State.hpp"

namespace entities {

class IMeasurement {
public:
    virtual ~IMeasurement() {}

    virtual void perform(State const* state) = 0;
};

} // namespace entities
