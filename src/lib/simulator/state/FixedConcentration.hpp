#pragma once

#include <inttypes.h>


namespace simulator {
namespace state {


class FixedConcentration {
public:
    FixedConcentration(double new_value) : _value(new_value) {}

    double value() const {return _value;};
    void add_monomer() {};
    void remove_monomer() {};
    uint64_t monomer_count() const { return 0; }

private:
    double _value;
};


} // namespace state
} // namespace simulator
