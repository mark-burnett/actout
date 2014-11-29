#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"
#include "simulator/event_generators/Polymerization.hpp"
#include "simulator/state/SingleStrandFilament.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include <memory>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;

struct Polymerization {
    Polymerization() {
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

        s.concentrations.emplace_back(state::VariableConcentration(6, 1));
        s.concentrations.emplace_back(state::VariableConcentration(4, 1));
    }

    ~Polymerization() {
        s.filaments.clear();
        s.concentrations.clear();
    }

    State s;
};


BOOST_FIXTURE_TEST_SUITE(Poly, Polymerization)

BOOST_AUTO_TEST_CASE(Basic) {
    event_generators::BarbedEndPolymerization t_b0(0, 1);
    event_generators::BarbedEndPolymerization t_b1(1, 2);
    event_generators::PointedEndPolymerization t_p0(0, 3);
    event_generators::PointedEndPolymerization t_p1(1, 4);

    StateModifications const no_modifications;
    BOOST_CHECK_CLOSE(12, t_b0.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_CLOSE(16, t_b1.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_CLOSE(36, t_p0.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_CLOSE(32, t_p1.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_EQUAL(6, s.concentrations[0].monomer_count());
    BOOST_CHECK_EQUAL(4, s.concentrations[1].monomer_count());

    BOOST_CHECK_EQUAL(8, s.filaments[0]->length());

    {
        auto modifications = t_b0.perform_event(&s, 8.5);
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments.size());
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments[0]);
        BOOST_CHECK_EQUAL(1, modifications.modified_concentrations.size());
        BOOST_CHECK_EQUAL(0, modifications.modified_concentrations[0]);
    }

    BOOST_CHECK_EQUAL(8, s.filaments[0]->length());
    BOOST_CHECK_EQUAL(9, s.filaments[1]->length());
    BOOST_CHECK_EQUAL(0, s.filaments[1]->peek_barbed());
    BOOST_CHECK_CLOSE(10, t_b0.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_CLOSE(16, t_b1.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_CLOSE(30, t_p0.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_CLOSE(32, t_p1.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_EQUAL(5, s.concentrations[0].monomer_count());
    BOOST_CHECK_EQUAL(4, s.concentrations[1].monomer_count());

    {
        auto modifications = t_p1.perform_event(&s, 12.1);
        BOOST_CHECK_EQUAL(1, modifications.modified_filaments.size());
        BOOST_CHECK_EQUAL(0, modifications.modified_filaments[0]);
        BOOST_CHECK_EQUAL(1, modifications.modified_concentrations.size());
        BOOST_CHECK_EQUAL(1, modifications.modified_concentrations[0]);
    }

    BOOST_CHECK_EQUAL(9, s.filaments[0]->length());
    BOOST_CHECK_EQUAL(9, s.filaments[1]->length());
    BOOST_CHECK_EQUAL(1, s.filaments[0]->peek_pointed());
    BOOST_CHECK_CLOSE(10, t_b0.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_CLOSE(12, t_b1.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_CLOSE(30, t_p0.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_CLOSE(24, t_p1.rate(&s, no_modifications), 0.00001);
    BOOST_CHECK_EQUAL(5, s.concentrations[0].monomer_count());
    BOOST_CHECK_EQUAL(3, s.concentrations[1].monomer_count());

}

BOOST_AUTO_TEST_SUITE_END()
