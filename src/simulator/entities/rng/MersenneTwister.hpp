#pragma once

#include "entities/IRNG.hpp"

#include <inttypes.h>
#include <random>

namespace entities {
namespace rng {


class MersenneTwister : public IRNG {
public:
    MersenneTwister() : engine_(std::random_device()()) {}
    MersenneTwister(uint64_t const& seed) : engine_(seed) {}

    double exponential(double const& denominator) {
        std::exponential_distribution<double> d(denominator);
        return d(engine_);
    }

    double uniform(double const& min=0, double const& max=1) {
        std::uniform_real_distribution<double> d(min, max);
        return d(engine_);
    }

private:
    std::mt19937_64 engine_;
};


} // namespace rng
} // namespace entities
