#include "entities/State.hpp"
#include "entities/StateModifications.hpp"
#include "entities/common.hpp"
#include "entities/event_generators/FilamentSpeciesChangeRandom.hpp"
#include "entities/state/SingleStrandFilament.hpp"
#include "entities/state/VariableConcentration.hpp"

#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <vector>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace entities;

class TestFilamentSpeciesChangeRandom : public testing::Test {
protected:
    void SetUp() {
        std::vector<species_t> values1;
        values1 += 0, 1, 0, 0, 2, 1, 0, 1;
        std::vector<species_t> values2;
        values2 += 1, 1, 2, 0, 2, 0, 0, 1;


        s.filaments.push_back(std::move(
                    std::unique_ptr<state::SingleStrandFilament>(
                        new state::SingleStrandFilament(5,
                            values1.begin(), values1.end()))));
        s.filaments.push_back(std::move(
                    std::unique_ptr<state::SingleStrandFilament>(
                        new state::SingleStrandFilament(5,
                            values2.begin(), values2.end()))));

        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(0, 1))));
        s.concentrations.push_back(std::move(
                    std::unique_ptr<state::VariableConcentration>(
                        new state::VariableConcentration(0, 1))));
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

protected:
    State s;
};

TEST_F(TestFilamentSpeciesChangeRandom, InitialRate) {
    event_generators::FilamentSpeciesChangeRandom t_0(0, 4, 3);
    event_generators::FilamentSpeciesChangeRandom t_1(1, 4, 2);
    event_generators::FilamentSpeciesChangeRandom t_2(2, 4, 4);
    event_generators::FilamentSpeciesChangeRandom t_3(3, 4, 9);

    StateModifications initial_modifications;
    initial_modifications.created_filaments.reserve(2);
    initial_modifications.created_filaments.emplace_back(0);
    initial_modifications.created_filaments.emplace_back(1);

    EXPECT_DOUBLE_EQ(21, t_0.rate(&s, initial_modifications));
    EXPECT_DOUBLE_EQ(12, t_1.rate(&s, initial_modifications));
    EXPECT_DOUBLE_EQ(12, t_2.rate(&s, initial_modifications));
    EXPECT_DOUBLE_EQ( 0, t_3.rate(&s, initial_modifications));
}

TEST_F(TestFilamentSpeciesChangeRandom, Perform) {
    event_generators::FilamentSpeciesChangeRandom t_0(0, 4, 3);
    event_generators::FilamentSpeciesChangeRandom t_1(1, 4, 2);
    event_generators::FilamentSpeciesChangeRandom t_2(2, 4, 4);

    StateModifications initial_modifications;
    initial_modifications.created_filaments.reserve(2);
    initial_modifications.created_filaments.emplace_back(0);
    initial_modifications.created_filaments.emplace_back(1);

    // Must call rate() with initial modifications before perform
    t_0.rate(&s, initial_modifications);
    t_1.rate(&s, initial_modifications);
    t_2.rate(&s, initial_modifications);

    {
        auto modifications = t_1.perform_event(&s, 7);
        EXPECT_EQ(1, modifications.modified_filaments.size());
        EXPECT_EQ(1, modifications.modified_filaments[0]);
        EXPECT_DOUBLE_EQ(21, t_0.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(10, t_1.rate(&s, modifications));
        EXPECT_DOUBLE_EQ(12, t_2.rate(&s, modifications));
    }

    EXPECT_EQ(4, s.filaments[1]->peek_pointed());
}
