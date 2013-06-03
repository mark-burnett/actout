#pragma once

#include "entities/State.hpp"
#include "entities/measurements/SingleValueBase.hpp"

#include <inttypes.h>


namespace entities {
namespace measurements {


class FilamentLengthMean : public SingleValueBase {
public:
    FilamentLengthMean(double const& sample_interval)
        : SingleValueBase(sample_interval) {}

private:
    double const get_value(State const* state) const {
        uint64_t length = 0;
        for (auto& filament : state->filaments) {
            length += filament->length();
        }

        return length / static_cast<double>(state->filaments.size());
    }

};


} // namespace entities
} // namespace measurements
