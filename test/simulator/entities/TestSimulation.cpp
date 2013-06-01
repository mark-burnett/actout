#include "entities/IEndCondition.hpp"
#include "entities/IEventGenerator.hpp"
#include "entities/IMeasurement.hpp"
#include "entities/IRNG.hpp"
#include "entities/Simulation.hpp"
#include "entities/State.hpp"

#include "entities/end_conditions/EventCount.hpp"
#include "entities/event_generators/Polymerization.hpp"
#include "entities/state/FixedConcentration.hpp"
#include "entities/state/SingleStrandFilament.hpp"

#include <gtest/gtest.h>
#include <inttypes.h>

using namespace entities;
using namespace entities::end_conditions;
using namespace entities::event_generators;
using namespace entities::state;


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

class SimpleSimulation : public ::testing::Test {
public:
    void run_simulation() {
        simulation->execute(&state, measurements, rng.get());
    }

protected:
    virtual void SetUp() {
        std::vector<species_t> values(100);
        state.filaments.push_back(std::move(
                    std::unique_ptr<SingleStrandFilament>(
                        new SingleStrandFilament(5,
                            values.begin(), values.end()))));
        state.concentrations.push_back(std::move(
                    std::unique_ptr<FixedConcentration>(
                        new FixedConcentration(6))));

        simulation = std::unique_ptr<Simulation>(
                new Simulation(end_conditions, event_generators));

        rng = std::unique_ptr<MockRNG>(new MockRNG());
    }

protected:
    std::vector<std::unique_ptr<IEndCondition const> > end_conditions;
    std::vector<std::unique_ptr<IEventGenerator> > event_generators;

    State state;
    std::vector<std::unique_ptr<IMeasurement> > measurements;
    std::unique_ptr<IRNG> rng;

    std::unique_ptr<Simulation> simulation;
};

TEST_F(SimpleSimulation, polymerization) {
    end_conditions.push_back(std::move(
                std::unique_ptr<EventCount>(
                    new EventCount(10))));

    event_generators.push_back(std::move(
                std::unique_ptr<BarbedEndPolymerization>(
                    new BarbedEndPolymerization(0, 3))));

    run_simulation();

    ASSERT_EQ(11, state.time);
    ASSERT_EQ(110, state.filaments[0]->length());
}
