#include "entities/State.hpp"
#include "entities/StateModifications.hpp"
#include "entities/common.hpp"
#include "entities/event_generators/FreeSpeciesChange.hpp"
#include "entities/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace entities;

class TestFreeSpeciesChange : public testing::Test {
protected:
    virtual void SetUp() {
        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(6, 1))));
        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(0, 1))));
        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(0, 1))));
    }

    virtual void TearDown() {
        s.concentrations.clear();
    }

    State s;
};


TEST_F(TestFreeSpeciesChange, Single) {
    std::vector<species_t> new_species(1, 1);
    event_generators::FreeSpeciesChange eg(0, new_species, 1);

    StateModifications const no_modifications;

    EXPECT_DOUBLE_EQ(6, eg.rate(&s, no_modifications));
    {
        auto sm = eg.perform_event(&s, 3);

        EXPECT_EQ(0, sm.modified_filaments.size());
        EXPECT_EQ(2, sm.modified_concentrations.size());
        EXPECT_EQ(0, sm.modified_concentrations[0]);
        EXPECT_EQ(1, sm.modified_concentrations[1]);
    }
    EXPECT_DOUBLE_EQ(5, eg.rate(&s, no_modifications));
}

TEST_F(TestFreeSpeciesChange, Byproduct) {
    std::vector<species_t> new_species;
    new_species += 1, 2;
    event_generators::FreeSpeciesChange eg(0, new_species, 1);

    StateModifications const no_modifications;

    EXPECT_DOUBLE_EQ(6, eg.rate(&s, no_modifications));
    {
        auto sm = eg.perform_event(&s, 3);

        EXPECT_EQ(0, sm.modified_filaments.size());
        EXPECT_EQ(3, sm.modified_concentrations.size());
        EXPECT_EQ(0, sm.modified_concentrations[0]);
        EXPECT_EQ(1, sm.modified_concentrations[1]);
        EXPECT_EQ(2, sm.modified_concentrations[2]);
    }
    EXPECT_DOUBLE_EQ(5, eg.rate(&s, no_modifications));
}
