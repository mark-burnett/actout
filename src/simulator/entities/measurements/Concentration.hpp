#pragma once

#include "entities/State.hpp"
#include "entities/measurements/SingleValueBase.hpp"
#include "entities/common.hpp"


namespace entities {
namespace measurements {


class Concentration : public SingleValueBase {
public:
    Concentration(double const& sample_interval, species_t const& species)
        : SingleValueBase(sample_interval), species_(species) {}

private:
    double const get_value(State const* state) const {
        return state->concentrations[species_]->value();
    }

private:
    species_t const species_;
};


} // namespace entities
} // namespace measurements
