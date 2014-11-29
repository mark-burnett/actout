#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"
#include "simulator/event_generators/FilamentSpeciesChangeRandom.hpp"
#include "simulator/state/SingleStrandFilament.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;

struct TestFilamentSpeciesChangeRandom {
    TestFilamentSpeciesChangeRandom() {
        std::vector<species_t> values1;
        values1 += 0, 1, 0, 0, 2, 1, 0, 1;
        std::vector<species_t> values2;
        values2 += 1, 1, 2, 0, 2, 0, 0, 1;


        s.filaments.push_back(std::move(
                    std::unique_ptr<state::SingleStrandFilament>(
                        new state::SingleStrandFilament(5,
                            values1.begin(), values1.end()))));
        s.filaments.push_back(std::move(
                    std::unique_ptr<state::SingleStrandFilament>(
                        new state::SingleStrandFilament(5,
                            values2.begin(), values2.end()))));

        s.concentrations.emplace_back(state::VariableConcentration(0, 1));
        s.concentrations.emplace_back(state::VariableConcentration(0, 1));
        s.concentrations.emplace_back(state::VariableConcentration(0, 1));
        s.concentrations.emplace_back(state::VariableConcentration(0, 1));
    }

    ~TestFilamentSpeciesChangeRandom() {
        s.filaments.clear();
        s.concentrations.clear();
    }

    State s;
};


BOOST_FIXTURE_TEST_SUITE(FilSpecChange, TestFilamentSpeciesChangeRandom)

BOOST_AUTO_TEST_CASE(InitialRate) {
    event_generators::FilamentSpeciesChangeRandom t_0(0, 4, 3);
    event_generators::FilamentSpeciesChangeRandom t_1(1, 4, 2);
    event_generators::FilamentSpeciesChangeRandom t_2(2, 4, 4);
    event_generators::FilamentSpeciesChangeRandom t_3(3, 4, 9);

    StateModifications initial_modifications;
    initial_modifications.created_filaments.reserve(2);
    initial_modifications.created_filaments.emplace_back(0);
    initial_modifications.created_filaments.emplace_back(1);

    BOOST_CHECK_CLOSE(21, t_0.rate(&s, initial_modifications), 0.00001);
    BOOST_CHECK_CLOSE(12, t_1.rate(&s, initial_modifications), 0.00001);
    BOOST_CHECK_CLOSE(12, t_2.rate(&s, initial_modifications), 0.00001);
    BOOST_CHECK_CLOSE( 0, t_3.rate(&s, initial_modifications), 0.00001);
}

BOOST_AUTO_TEST_CASE(Perform) {
    event_generators::FilamentSpeciesChangeRandom t_0(0, 4, 3);
    event_generators::FilamentSpeciesChangeRandom t_1(1, 4, 2);
    event_generators::FilamentSpeciesChangeRandom t_2(2, 4, 4);

    StateModifications initial_modifications;
    initial_modifications.created_filaments.reserve(2);
    initial_modifications.created_filaments.emplace_back(0);
    initial_modifications.created_filaments.emplace_back(1);

    // Must call rate() with initial modifications before perform
    t_0.rate(&s, initial_modifications);
    t_1.rate(&s, initial_modifications);
    t_2.rate(&s, initial_modifications);

    {
        auto modifications = t_1.perform_event(&s, 7);
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments.size());
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments[0]);
        BOOST_CHECK_CLOSE(21, t_0.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(10, t_1.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(12, t_2.rate(&s, modifications), 0.00001);
    }

    BOOST_CHECK_EQUAL(4, s.filaments[1]->peek_pointed());
}

BOOST_AUTO_TEST_SUITE_END()
