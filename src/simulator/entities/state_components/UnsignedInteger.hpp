#pragma once

#include <inttypes.h>

namespace entities {
namespace state_components {

class UnsignedInteger : public IStateComponent {
public:
    uint64_t value;
    UnsignedInteger(uint64_t const &value_=0) : value(value_) {}
};

} // namespace state_components
} // namespace entities
