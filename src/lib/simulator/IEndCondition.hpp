#pragma once

#include "simulator/State.hpp"

namespace simulator {

class IEndCondition {
public:
    virtual ~IEndCondition() {}

    virtual bool satisfied(State const* state) const = 0;
};

} // namespace simulator
