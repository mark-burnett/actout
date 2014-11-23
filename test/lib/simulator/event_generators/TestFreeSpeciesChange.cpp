#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"
#include "simulator/event_generators/FreeSpeciesChange.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include <memory>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;

struct TestFreeSpeciesChange {
    TestFreeSpeciesChange() {
        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(6, 1))));
        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(0, 1))));
        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(0, 1))));
    }

    ~TestFreeSpeciesChange() {
        s.concentrations.clear();
    }

    State s;
};


BOOST_FIXTURE_TEST_SUITE(FreeSpecChange, TestFreeSpeciesChange)

BOOST_AUTO_TEST_CASE(Single) {
    std::vector<species_t> new_species(1, 1);
    event_generators::FreeSpeciesChange eg(0, new_species, 1);

    StateModifications const no_modifications;

    BOOST_CHECK_CLOSE(6, eg.rate(&s, no_modifications), 0.00001);
    {
        auto sm = eg.perform_event(&s, 3);

        BOOST_CHECK_EQUAL(0, sm.modified_filaments.size());
        BOOST_CHECK_EQUAL(2, sm.modified_concentrations.size());
        BOOST_CHECK_EQUAL(0, sm.modified_concentrations[0]);
        BOOST_CHECK_EQUAL(1, sm.modified_concentrations[1]);
    }
    BOOST_CHECK_CLOSE(5, eg.rate(&s, no_modifications), 0.00001);
}

BOOST_AUTO_TEST_CASE(Byproduct) {
    std::vector<species_t> new_species;
    new_species += 1, 2;
    event_generators::FreeSpeciesChange eg(0, new_species, 1);

    StateModifications const no_modifications;

    BOOST_CHECK_CLOSE(6, eg.rate(&s, no_modifications), 0.00001);
    {
        auto sm = eg.perform_event(&s, 3);

        BOOST_CHECK_EQUAL(0, sm.modified_filaments.size());
        BOOST_CHECK_EQUAL(3, sm.modified_concentrations.size());
        BOOST_CHECK_EQUAL(0, sm.modified_concentrations[0]);
        BOOST_CHECK_EQUAL(1, sm.modified_concentrations[1]);
        BOOST_CHECK_EQUAL(2, sm.modified_concentrations[2]);
    }
    BOOST_CHECK_CLOSE(5, eg.rate(&s, no_modifications), 0.00001);
}

BOOST_AUTO_TEST_SUITE_END()
