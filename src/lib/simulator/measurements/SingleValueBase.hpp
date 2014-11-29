#pragma once

#include "simulator/IMeasurement.hpp"
#include "simulator/State.hpp"

#include <cmath>
#include <inttypes.h>
#include <vector>


namespace simulator {
namespace measurements {


class SingleValueBase : public IMeasurement {
public:
    SingleValueBase(double const& sample_interval)
        : sample_interval_(sample_interval) {}

    void perform(State const* state) {
        uint64_t samples_to_record = static_cast<uint64_t>(
            std::ceil((state->time - sample_interval_ * samples.size()
                    + 0.5 * sample_interval_)
                / sample_interval_)
        );

        if (samples_to_record > 0) {
            samples.insert(samples.end(), samples_to_record, get_value(state));
        }
    }

    std::vector<double> get_samples() const {
        return samples;
    }

private:
    virtual double const get_value(State const* state) const = 0;

public:  // XXX There should probably be a read-only accessor for this
    std::vector<double> samples;

private:
    double const sample_interval_;
};


} // namespace simulator
} // namespace measurements
