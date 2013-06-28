#pragma once

#include "simulator/State.hpp"

namespace simulator {

class IMeasurement {
public:
    virtual ~IMeasurement() {}

    virtual void perform(State const* state) = 0;
};

} // namespace simulator
