#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"
#include "simulator/event_generators/Depolymerization.hpp"
#include "simulator/state/SingleStrandFilament.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;

class Depolymerization : public testing::Test {
protected:
    virtual void SetUp() {
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

        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(0, 1))));
        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(0, 1))));
    }

    virtual void TearDown() {
        s.filaments.clear();
        s.concentrations.clear();
    }

    State s;
};


TEST_F(Depolymerization, Basic) {
    event_generators::BarbedEndDepolymerization t_b0(0, 1);
    event_generators::BarbedEndDepolymerization t_b1(1, 2);
    event_generators::PointedEndDepolymerization t_p0(0, 3);
    event_generators::PointedEndDepolymerization t_p1(1, 4);

    StateModifications initial_modifications;
    initial_modifications.created_filaments.reserve(2);
    initial_modifications.created_filaments.emplace_back(0);
    initial_modifications.created_filaments.emplace_back(1);

    EXPECT_DOUBLE_EQ(1, t_b0.rate(&s, initial_modifications));
    EXPECT_DOUBLE_EQ(2, t_b1.rate(&s, initial_modifications));
    EXPECT_DOUBLE_EQ(3, t_p0.rate(&s, initial_modifications));
    EXPECT_DOUBLE_EQ(4, t_p1.rate(&s, initial_modifications));

    {
        auto modifications = t_p0.perform_event(&s, 0.3);
        EXPECT_EQ(7, s.filaments[0]->length());
        EXPECT_EQ(8, s.filaments[1]->length());
        EXPECT_DOUBLE_EQ(1, t_b0.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(2, t_b1.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(0, t_p0.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(8, t_p1.rate(&s, modifications));
        EXPECT_EQ(1, s.concentrations[0]->monomer_count());
        EXPECT_EQ(0, s.concentrations[1]->monomer_count());
    }

    {
        auto modifications = t_p1.perform_event(&s, 5.3);
        EXPECT_EQ(7, s.filaments[0]->length());
        EXPECT_EQ(7, s.filaments[1]->length());
        EXPECT_DOUBLE_EQ(1, t_b0.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(2, t_b1.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(0, t_p0.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(8, t_p1.rate(&s, modifications));
        EXPECT_EQ(1, s.concentrations[0]->monomer_count());
        EXPECT_EQ(1, s.concentrations[1]->monomer_count());
    }

    {
        auto modifications = t_p1.perform_event(&s, 5.3);
        EXPECT_EQ(7, s.filaments[0]->length());
        EXPECT_EQ(6, s.filaments[1]->length());
        EXPECT_DOUBLE_EQ(1, t_b0.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(2, t_b1.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(0, t_p0.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(4, t_p1.rate(&s, modifications));
        EXPECT_EQ(1, s.concentrations[0]->monomer_count());
        EXPECT_EQ(2, s.concentrations[1]->monomer_count());
    }
}
