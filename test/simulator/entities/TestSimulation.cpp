#include "entities/IEndCondition.hpp"
#include "entities/IEvent.hpp"
#include "entities/IEventGenerator.hpp"
#include "entities/IMeasurement.hpp"
#include "entities/IRNG.hpp"
#include "entities/Simulation.hpp"
#include "entities/SimulationState.hpp"

#include "entities/end_conditions/EventCount.hpp"

#include <gtest/gtest.h>

using namespace entities;


class MockRNG : public IRNG {
    double exponential(double const& denominator) { return 1.0; }
    double uniform(double const& min=0, double const& max=1) { return (min + max) / 2; }
};


TEST(Simulation, EmptySimulation) {
    auto rng = std::unique_ptr<IRNG>(new MockRNG);

    std::vector<std::unique_ptr<IEndCondition const> > ecs;
    auto ec = std::unique_ptr<IEndCondition const>(
            new end_conditions::EventCount(0));
    ecs.push_back(std::move(ec));
    std::vector<std::unique_ptr<IEventGenerator> > event_generators;

    auto state = std::unique_ptr<SimulationState>(new SimulationState());
    std::vector<std::unique_ptr<IMeasurement> > measurements;

    auto s = Simulation(ecs, event_generators);

    s.execute(state.get(), measurements, rng.get());

    ASSERT_EQ(state->event_count, 0);
    ASSERT_EQ(state->time, 0);
}
