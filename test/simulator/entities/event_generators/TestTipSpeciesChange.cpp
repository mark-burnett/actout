#include "entities/State.hpp"
#include "entities/StateModifications.hpp"
#include "entities/common.hpp"
#include "entities/event_generators/TipSpeciesChange.hpp"
#include "entities/state/SingleStrandFilament.hpp"
#include "entities/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace entities;

class TipHydrolysisTest : public testing::Test {
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

        initial_modifications.created_filaments.reserve(2);
        initial_modifications.created_filaments.emplace_back(0);
        initial_modifications.created_filaments.emplace_back(1);
    }

    virtual void TearDown() {
        s.filaments.clear();
        s.concentrations.clear();

        initial_modifications.created_filaments.clear();
    }

    State s;
    StateModifications initial_modifications;
};



TEST_F(TipHydrolysisTest, BETipHydrolysis) {
    event_generators::BarbedTipSpeciesChange be_10(1, 0, 3);
    event_generators::BarbedTipSpeciesChange be_01(0, 1, 2);

    EXPECT_DOUBLE_EQ(3, be_10.rate(&s, initial_modifications));
    EXPECT_DOUBLE_EQ(2, be_01.rate(&s, initial_modifications));

    {
        auto modifications = be_10.perform_event(&s, 0.2);
        EXPECT_EQ(1, modifications.modified_filaments.size());
        EXPECT_EQ(0, modifications.modified_filaments[0]);
        EXPECT_DOUBLE_EQ(0, be_10.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(4, be_01.rate(&s, modifications));
    }

    {
        auto modifications = be_01.perform_event(&s, 2.7);
        EXPECT_EQ(1, modifications.modified_filaments.size());
        EXPECT_EQ(1, modifications.modified_filaments[0]);
        EXPECT_DOUBLE_EQ(3, be_10.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(2, be_01.rate(&s, modifications));
    }

}

TEST_F(TipHydrolysisTest, PETipHydrolysis) {
    event_generators::PointedTipSpeciesChange pe_12(1, 2, 3);

    EXPECT_DOUBLE_EQ(3, pe_12.rate(&s, initial_modifications));
    {
        auto modifications = pe_12.perform_event(&s, 0.2);
        EXPECT_EQ(1, modifications.modified_filaments.size());
        EXPECT_EQ(1, modifications.modified_filaments[0]);
        EXPECT_DOUBLE_EQ(0, pe_12.rate(&s, modifications));
    }
}
