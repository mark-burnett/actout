#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"
#include "simulator/event_generators/TipSpeciesChange.hpp"
#include "simulator/state/SingleStrandFilament.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;

struct TipHydrolysisTest {
    TipHydrolysisTest() {
        std::vector<species_t> values1;
        values1 += 0, 1, 0, 0, 2, 1, 0, 1;
        std::vector<species_t> values2;
        values2 += 1, 1, 2, 0, 2, 0, 0, 0;

        s.filaments.push_back(std::move(
                    std::unique_ptr<state::SingleStrandFilament>(
                        new state::SingleStrandFilament(3,
                            values1.begin(), values1.end()))));
        s.filaments.push_back(std::move(
                    std::unique_ptr<state::SingleStrandFilament>(
                        new state::SingleStrandFilament(3,
                            values2.begin(), values2.end()))));

        s.concentrations.emplace_back(state::VariableConcentration(0, 1));
        s.concentrations.emplace_back(state::VariableConcentration(0, 1));

        initial_modifications.created_filaments.reserve(2);
        initial_modifications.created_filaments.emplace_back(0);
        initial_modifications.created_filaments.emplace_back(1);
    }

    ~TipHydrolysisTest() {
        s.filaments.clear();
        s.concentrations.clear();

        initial_modifications.created_filaments.clear();
    }

    State s;
    StateModifications initial_modifications;
};


BOOST_FIXTURE_TEST_SUITE(TipHydro, TipHydrolysisTest)


BOOST_AUTO_TEST_CASE(BETipHydrolysis) {
    event_generators::BarbedTipSpeciesChange be_10(1, 0, 3);
    event_generators::BarbedTipSpeciesChange be_01(0, 1, 2);

    BOOST_CHECK_CLOSE(3, be_10.rate(&s, initial_modifications), 0.00001);
    BOOST_CHECK_CLOSE(2, be_01.rate(&s, initial_modifications), 0.00001);

    {
        auto modifications = be_10.perform_event(&s, 0.2);
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments.size());
        BOOST_CHECK_EQUAL(0, modifications.modified_filaments[0]);
        BOOST_CHECK_CLOSE(0, be_10.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(4, be_01.rate(&s, modifications), 0.00001);
    }

    {
        auto modifications = be_01.perform_event(&s, 2.7);
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments.size());
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments[0]);
        BOOST_CHECK_CLOSE(3, be_10.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(2, be_01.rate(&s, modifications), 0.00001);
    }

}

BOOST_AUTO_TEST_CASE(PETipHydrolysis) {
    event_generators::PointedTipSpeciesChange pe_12(1, 2, 3);

    BOOST_CHECK_CLOSE(3, pe_12.rate(&s, initial_modifications), 0.00001);
    {
        auto modifications = pe_12.perform_event(&s, 0.2);
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments.size());
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments[0]);
        BOOST_CHECK_CLOSE(0, pe_12.rate(&s, modifications), 0.00001);
    }
}

BOOST_AUTO_TEST_SUITE_END()
