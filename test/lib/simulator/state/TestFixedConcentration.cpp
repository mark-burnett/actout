#include "simulator/state/FixedConcentration.hpp"

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(FixedConcentration) {
    simulator::state::FixedConcentration c(3.1);

    BOOST_CHECK_CLOSE(3.1, c.value(), 0.00001);

    c.remove_monomer();
    BOOST_CHECK_CLOSE(3.1, c.value(), 0.00001);

    c.add_monomer();
    BOOST_CHECK_CLOSE(3.1, c.value(), 0.00001);
}
