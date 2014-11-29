#include "simulator/State.hpp"
#include "simulator/common.hpp"
#include "simulator/measurements/FilamentLengthMean.hpp"
#include "simulator/state/SingleStrandFilament.hpp"

#include <boost/assign/std/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <inttypes.h>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;


BOOST_AUTO_TEST_CASE(Basic) {
    std::vector<species_t> initial_values(10, 0);

    State s;
    s.filaments.push_back(std::move(
                std::unique_ptr<state::SingleStrandFilament>(
                    new state::SingleStrandFilament(1,
                        initial_values.begin(), initial_values.end()))));
    s.filaments.push_back(std::move(
                std::unique_ptr<state::SingleStrandFilament>(
                    new state::SingleStrandFilament(1,
                        initial_values.begin(), initial_values.end()))));

    measurements::FilamentLengthMean m(1);

    for (uint64_t i = 0; i < 11; ++i) {
        s.time = i;
        m.perform(&s);
        s.filaments[0]->append_barbed(0);
    }

    std::vector<double> expected_values;
    expected_values += 10, 10.5, 11, 11.5, 12, 12.5, 13, 13.5, 14, 14.5, 15;
    {
        auto samples = m.get_samples();
        BOOST_CHECK_EQUAL_COLLECTIONS(
                expected_values.begin(), expected_values.end(),
                samples.begin(), samples.end());
    }

    for (uint64_t i = 0; i < 6; ++i) {
        s.time = 10 + i;
        m.perform(&s);
        s.filaments[1]->pop_barbed();
    }

    expected_values += 15, 14.5, 14, 13.5, 13;
    {
        auto samples = m.get_samples();
        BOOST_CHECK_EQUAL_COLLECTIONS(
                expected_values.begin(), expected_values.end(),
                samples.begin(), samples.end());
    }
}
