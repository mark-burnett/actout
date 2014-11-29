#include "simulator/State.hpp"
#include "simulator/measurements/Concentration.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <inttypes.h>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;


BOOST_AUTO_TEST_CASE(Basic) {
    State s;
    s.concentrations.emplace_back(state::VariableConcentration(2, 1));

    measurements::Concentration m(1, 0);
    for (uint64_t i = 0; i < 11; ++i) {
        s.time = 0.5 * i;
        m.perform(&s);
        s.concentrations[0].add_monomer();
    }

    std::vector<double> expected_values;
    expected_values += 2, 4, 6, 8, 10, 12;
    {
        auto samples = m.get_samples();
        BOOST_CHECK_EQUAL_COLLECTIONS(
                expected_values.begin(), expected_values.end(),
                samples.begin(), samples.end());
    }

    for (uint64_t i = 0; i < 6; ++i) {
        s.time = 5 + i;
        m.perform(&s);
        s.concentrations[0].remove_monomer();
    }

    expected_values += 12, 11, 10, 9, 8;
    {
        auto samples = m.get_samples();
        BOOST_CHECK_EQUAL_COLLECTIONS(
                expected_values.begin(), expected_values.end(),
                samples.begin(), samples.end());
    }
}
