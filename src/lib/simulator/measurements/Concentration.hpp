#pragma once

#include "simulator/State.hpp"
#include "simulator/measurements/SingleValueBase.hpp"
#include "simulator/common.hpp"


namespace simulator {
namespace measurements {


class Concentration : public SingleValueBase {
public:
    Concentration(double const& sample_interval, species_t const& species)
        : SingleValueBase(sample_interval), species_(species) {}

private:
    double const get_value(State const* state) const {
        return state->concentrations[species_].value();
    }

private:
    species_t const species_;
};


} // namespace simulator
} // namespace measurements
