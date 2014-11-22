#include "simulator/state/VariableConcentration.hpp"

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(VariableConcentration) {
    simulator::state::VariableConcentration c(4.8, 1 / 1.2);

    BOOST_CHECK_CLOSE(4.8, c.value(), 0.00001);

    c.remove_monomer();
    BOOST_CHECK_CLOSE(3.6, c.value(), 0.00001);

    c.add_monomer();
    c.add_monomer();

    BOOST_CHECK_CLOSE(6, c.value(), 0.00001);
}
