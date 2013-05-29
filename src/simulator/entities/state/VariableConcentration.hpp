#pragma once

#include "entities/IConcentration.hpp"

#include <inttypes.h>


namespace entities {
namespace state {


class VariableConcentration : public IConcentration {
public:
    VariableConcentration(double concentration, double fnc,
            uint64_t number_of_filaments=1,
            double scale_concentration=1,
            double subtract_fraction=0) :
        _number((scale_concentration - subtract_fraction)
                * concentration * number_of_filaments / fnc),
        _volume(number_of_filaments / fnc) {}

    double value() const { return _number / _volume; }
    void add_monomer() { ++_number;};
    void remove_monomer() { if (_number > 0) --_number;};
    uint64_t monomer_count() const { return _number; }

private:
    uint64_t _number;
    const double _volume;
};


} // namespace state
} // namespace entities
