#include "entities/IEndCondition.hpp"
#include "entities/IEventGenerator.hpp"
#include "entities/IMeasurement.hpp"
#include "entities/IRNG.hpp"
#include "entities/Simulation.hpp"
#include "entities/State.hpp"

#include "entities/end_conditions/EventCount.hpp"

#include <gtest/gtest.h>
#include <inttypes.h>

using namespace entities;


class MockRNG : public IRNG {
public:
    uint64_t exponential_call_count;
    uint64_t uniform_call_count;

    MockRNG()
        : exponential_call_count(0),
        uniform_call_count(0) {}

    double exponential(double const& denominator) {
        ++exponential_call_count;
        return 1;
    }
    double uniform(double const& min=0, double const& max=1) {
        ++uniform_call_count;
        return (min + max) / 2;
    }
};


TEST(Dummy, dumb) {
    ASSERT_EQ(0, 0);
}
