#include "simulator/IEndCondition.hpp"
#include "simulator/IEventGenerator.hpp"
#include "simulator/IMeasurement.hpp"
#include "simulator/IRNG.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"

#include "simulator/end_conditions/EventCount.hpp"
#include "simulator/event_generators/Polymerization.hpp"
#include "simulator/state/FixedConcentration.hpp"
#include "simulator/state/SingleStrandFilament.hpp"

#include <gtest/gtest.h>
#include <inttypes.h>

using namespace simulator;
using namespace simulator::end_conditions;
using namespace simulator::event_generators;
using namespace simulator::state;


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


TEST_F(SimpleSimulation, get_initial_modifications) {
    auto modifications = simulation->get_initial_modifications(&state);

    ASSERT_EQ(1, modifications.created_filaments.size());
    ASSERT_EQ(1, modifications.modified_concentrations.size());
}


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
