#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"
#include "simulator/event_generators/Polymerization.hpp"
#include "simulator/state/SingleStrandFilament.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;

class Polymerization : public testing::Test {
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
                        new state::VariableConcentration(6, 1))));
        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(4, 1))));
    }

    virtual void TearDown() {
        s.filaments.clear();
        s.concentrations.clear();
    }

    State s;
};


TEST_F(Polymerization, Basic) {
    event_generators::BarbedEndPolymerization t_b0(0, 1);
    event_generators::BarbedEndPolymerization t_b1(1, 2);
    event_generators::PointedEndPolymerization t_p0(0, 3);
    event_generators::PointedEndPolymerization t_p1(1, 4);

    StateModifications const no_modifications;
    EXPECT_DOUBLE_EQ(12, t_b0.rate(&s, no_modifications));
    EXPECT_DOUBLE_EQ(16, t_b1.rate(&s, no_modifications));
    EXPECT_DOUBLE_EQ(36, t_p0.rate(&s, no_modifications));
    EXPECT_DOUBLE_EQ(32, t_p1.rate(&s, no_modifications));
    EXPECT_EQ(6, s.concentrations[0]->monomer_count());
    EXPECT_EQ(4, s.concentrations[1]->monomer_count());

    EXPECT_EQ(8, s.filaments[0]->length());

    {
        auto modifications = t_b0.perform_event(&s, 8.5);
        EXPECT_EQ(1, modifications.modified_filaments.size());
        EXPECT_EQ(1, modifications.modified_filaments[0]);
        EXPECT_EQ(1, modifications.modified_concentrations.size());
        EXPECT_EQ(0, modifications.modified_concentrations[0]);
    }

    EXPECT_EQ(8, s.filaments[0]->length());
    EXPECT_EQ(9, s.filaments[1]->length());
    EXPECT_EQ(0, s.filaments[1]->peek_barbed());
    EXPECT_DOUBLE_EQ(10, t_b0.rate(&s, no_modifications));
    EXPECT_DOUBLE_EQ(16, t_b1.rate(&s, no_modifications));
    EXPECT_DOUBLE_EQ(30, t_p0.rate(&s, no_modifications));
    EXPECT_DOUBLE_EQ(32, t_p1.rate(&s, no_modifications));
    EXPECT_EQ(5, s.concentrations[0]->monomer_count());
    EXPECT_EQ(4, s.concentrations[1]->monomer_count());

    {
        auto modifications = t_p1.perform_event(&s, 12.1);
        EXPECT_EQ(1, modifications.modified_filaments.size());
        EXPECT_EQ(0, modifications.modified_filaments[0]);
        EXPECT_EQ(1, modifications.modified_concentrations.size());
        EXPECT_EQ(1, modifications.modified_concentrations[0]);
    }

    EXPECT_EQ(9, s.filaments[0]->length());
    EXPECT_EQ(9, s.filaments[1]->length());
    EXPECT_EQ(1, s.filaments[0]->peek_pointed());
    EXPECT_DOUBLE_EQ(10, t_b0.rate(&s, no_modifications));
    EXPECT_DOUBLE_EQ(12, t_b1.rate(&s, no_modifications));
    EXPECT_DOUBLE_EQ(30, t_p0.rate(&s, no_modifications));
    EXPECT_DOUBLE_EQ(24, t_p1.rate(&s, no_modifications));
    EXPECT_EQ(5, s.concentrations[0]->monomer_count());
    EXPECT_EQ(3, s.concentrations[1]->monomer_count());

}
