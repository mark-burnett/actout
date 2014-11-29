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

#include <boost/assign/std/vector.hpp>
#include <boost/test/unit_test.hpp>
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

struct SimpleSimulation {
    void run_simulation() {
        simulation = std::unique_ptr<Simulation>(
                new Simulation(end_conditions, event_generators));

        simulation->execute(&state, measurements, rng.get());
    }

    SimpleSimulation() {
        std::vector<species_t> values(100);
        state.filaments.push_back(std::move(
                    std::unique_ptr<SingleStrandFilament>(
                        new SingleStrandFilament(5,
                            values.begin(), values.end()))));
        state.concentrations.emplace_back(FixedConcentration(6));
        rng = std::unique_ptr<MockRNG>(new MockRNG());
    }

    std::vector<EndCondition> end_conditions;
    std::vector<EventGenerator> event_generators;

    State state;
    std::vector<Measurement> measurements;
    std::unique_ptr<IRNG> rng;

    std::unique_ptr<Simulation> simulation;
};


BOOST_FIXTURE_TEST_SUITE(SimTest, SimpleSimulation)

BOOST_AUTO_TEST_CASE(get_initial_modifications) {
    auto modifications = simulation->get_initial_modifications(&state);

    BOOST_CHECK_EQUAL(1, modifications.created_filaments.size());
    BOOST_CHECK_EQUAL(1, modifications.modified_concentrations.size());
}


BOOST_AUTO_TEST_CASE(polymerization) {
    end_conditions.push_back(EventCount(10));

    event_generators.push_back(BarbedEndPolymerization(0, 3));

    run_simulation();

    BOOST_REQUIRE_EQUAL(11, state.time);
    BOOST_CHECK_EQUAL(110, state.filaments[0]->length());
}

BOOST_AUTO_TEST_SUITE_END()
