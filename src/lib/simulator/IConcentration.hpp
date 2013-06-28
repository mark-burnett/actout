#pragma once

#include <inttypes.h>

namespace simulator {
namespace state {

class IConcentration {
public:
    virtual ~IConcentration() {}

    virtual double value() const = 0;
    virtual uint64_t monomer_count() const = 0;

    virtual void add_monomer() = 0;
    virtual void remove_monomer() = 0;
};

} // namespace state
} // namespace simulator
