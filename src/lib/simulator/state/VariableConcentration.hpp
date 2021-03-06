#pragma once

#include <inttypes.h>


namespace simulator {
namespace state {


class VariableConcentration {
public:
    VariableConcentration(double const& concentration, double const& volume)
        : _number(volume * concentration), _volume(volume) {}

    double value() const { return _number / _volume; }
    void add_monomer() { ++_number;};
    void remove_monomer() { if (_number > 0) --_number;};
    uint64_t monomer_count() const { return _number; }

private:
    uint64_t _number;
    const double _volume;
};


} // namespace state
} // namespace simulator
