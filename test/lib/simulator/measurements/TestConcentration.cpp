#include "simulator/State.hpp"
#include "simulator/measurements/Concentration.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <inttypes.h>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;


TEST(ConcentrationMeasurment, Basic) {
    State s;
    s.concentrations.push_back(std::move(
                std::unique_ptr<state::VariableConcentration>(
                    new state::VariableConcentration(2, 1))));

    measurements::Concentration m(1, 0);
    for (uint64_t i = 0; i < 11; ++i) {
        s.time = 0.5 * i;
        m.perform(&s);
        s.concentrations[0]->add_monomer();
    }

    std::vector<double> expected_values;
    expected_values += 2, 4, 6, 8, 10, 12;
    ASSERT_EQ(expected_values, m.samples);

    for (uint64_t i = 0; i < 6; ++i) {
        s.time = 5 + i;
        m.perform(&s);
        s.concentrations[0]->remove_monomer();
    }

    expected_values += 12, 11, 10, 9, 8;
    ASSERT_EQ(expected_values, m.samples);
}
