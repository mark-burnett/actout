#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"
#include "simulator/event_generators/Depolymerization.hpp"
#include "simulator/state/SingleStrandFilament.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;

struct Depolymerization {
    Depolymerization() {
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

        s.concentrations.emplace_back(
                        state::VariableConcentration(0, 1));
        s.concentrations.emplace_back(
                        state::VariableConcentration(0, 1));
    }

    ~Depolymerization() {
        s.filaments.clear();
        s.concentrations.clear();
    }

    State s;
};


BOOST_FIXTURE_TEST_SUITE(Depoly, Depolymerization)

BOOST_AUTO_TEST_CASE(Basic) {
    event_generators::BarbedEndDepolymerization t_b0(0, 1);
    event_generators::BarbedEndDepolymerization t_b1(1, 2);
    event_generators::PointedEndDepolymerization t_p0(0, 3);
    event_generators::PointedEndDepolymerization t_p1(1, 4);

    StateModifications initial_modifications;
    initial_modifications.created_filaments.reserve(2);
    initial_modifications.created_filaments.emplace_back(0);
    initial_modifications.created_filaments.emplace_back(1);

    BOOST_CHECK_CLOSE(1, t_b0.rate(&s, initial_modifications), 0.00001);
    BOOST_CHECK_CLOSE(2, t_b1.rate(&s, initial_modifications), 0.00001);
    BOOST_CHECK_CLOSE(3, t_p0.rate(&s, initial_modifications), 0.00001);
    BOOST_CHECK_CLOSE(4, t_p1.rate(&s, initial_modifications), 0.00001);

    {
        auto modifications = t_p0.perform_event(&s, 0.3);
        BOOST_CHECK_EQUAL(7, s.filaments[0]->length());
        BOOST_CHECK_EQUAL(8, s.filaments[1]->length());
        BOOST_CHECK_CLOSE(1, t_b0.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(2, t_b1.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(0, t_p0.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(8, t_p1.rate(&s, modifications), 0.00001);
        BOOST_CHECK_EQUAL(1, s.concentrations[0].monomer_count());
        BOOST_CHECK_EQUAL(0, s.concentrations[1].monomer_count());
    }

    {
        auto modifications = t_p1.perform_event(&s, 5.3);
        BOOST_CHECK_EQUAL(7, s.filaments[0]->length());
        BOOST_CHECK_EQUAL(7, s.filaments[1]->length());
        BOOST_CHECK_CLOSE(1, t_b0.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(2, t_b1.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(0, t_p0.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(8, t_p1.rate(&s, modifications), 0.00001);
        BOOST_CHECK_EQUAL(1, s.concentrations[0].monomer_count());
        BOOST_CHECK_EQUAL(1, s.concentrations[1].monomer_count());
    }

    {
        auto modifications = t_p1.perform_event(&s, 5.3);
        BOOST_CHECK_EQUAL(7, s.filaments[0]->length());
        BOOST_CHECK_EQUAL(6, s.filaments[1]->length());
        BOOST_CHECK_CLOSE(1, t_b0.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(2, t_b1.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(0, t_p0.rate(&s, modifications), 0.00001);
        BOOST_CHECK_CLOSE(4, t_p1.rate(&s, modifications), 0.00001);
        BOOST_CHECK_EQUAL(1, s.concentrations[0].monomer_count());
        BOOST_CHECK_EQUAL(2, s.concentrations[1].monomer_count());
    }
}

BOOST_AUTO_TEST_SUITE_END()
