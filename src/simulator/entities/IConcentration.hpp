#pragma once

#include <inttypes.h>

namespace entities {
namespace state_components {

class IConcentration {
public:
    virtual ~IConcentration() {}

    virtual double value() const = 0;
    virtual uint64_t monomer_count() const = 0;

    virtual void add_monomer() = 0;
    virtual void remove_monomer() = 0;
};

} // namespace state_components
} // namespace entities
