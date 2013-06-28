#include "simulator/state/SingleStrandFilament.hpp"
#include "simulator/common.hpp"
#include "simulator/exceptions.hpp"

#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <iterator>
#include <map>
#include <memory>
#include <vector>


// Bring operator+= into the namespace
using namespace boost::assign;

using namespace simulator;
using namespace simulator::state;

class SingleStrandFilamentTest : public ::testing::Test {
protected:
    std::unique_ptr<SingleStrandFilament> filament;

    void initialize_filament(uint64_t const& num_species,
            std::vector<species_t> const& values) {
        filament = std::unique_ptr<SingleStrandFilament>(new
                SingleStrandFilament(num_species,
                    values.begin(), values.end()));
    }

    std::vector<species_t> initialize_filament(uint64_t const& num_species,
            species_t const& species, uint64_t const& count) {
        filament = std::unique_ptr<SingleStrandFilament>(new
                SingleStrandFilament(num_species, count, species));

        std::vector<species_t> values;
        for (uint64_t i = 0; i < count; ++i) {
            values += species;
        }

        return values;
    }


    void validate_filament(std::vector<species_t> const& expected_values) {
        EXPECT_EQ(expected_values.size(), filament->length());

        validate_tips(expected_values);
        validate_filament_values(expected_values);
        validate_species_counts(expected_values);
        validate_boundary_counts(expected_values);
    }

    void validate_tips(std::vector<species_t> const& expected_values) {
        if (!expected_values.empty()) {
            EXPECT_EQ(expected_values.back(), filament->peek_barbed());
            EXPECT_EQ(expected_values.front(), filament->peek_pointed());
        }
    }

    void validate_filament_values(
            std::vector<species_t> const& expected_values) {
        auto iter = filament->begin();
        for (uint64_t i = 0; i < expected_values.size(); ++i) {
            EXPECT_EQ(expected_values[i], *iter)
                << "Element " << i << " differs.";
            ++iter;
        }
    }

    void validate_species_counts(
            std::vector<species_t> const& expected_values) {
        std::map<species_t, uint64_t> expected_species_counts;
        for (auto const& i : expected_values) {
            auto e = expected_species_counts.find(i);
            if (e == expected_species_counts.end()) {
                expected_species_counts[i] = 0;
            }

            ++expected_species_counts[i];
        }

        for (auto const& i : expected_species_counts) {
            EXPECT_EQ(i.second, filament->species_count(i.first))
                << "Species count for " << i.first << " differs.";
        }
    }

    void validate_boundary_counts(
            std::vector<species_t> const& expected_values) {
        std::map<species_t, std::map<species_t, uint64_t> >
            expected_boundary_counts;
        for (auto pn = expected_values.begin();
                pn != expected_values.end(); ++pn) {
            auto bn = std::next(pn);
            if (bn != expected_values.end()) {
                if (*pn != *bn) {
                    auto inner_map = expected_boundary_counts[*pn].find(*bn);
                    if (inner_map == expected_boundary_counts[*pn].end()) {
                        expected_boundary_counts[*pn][*bn] = 0;
                    }

                    ++expected_boundary_counts[*pn][*bn];
                }
            }
        }

        uint64_t total_boundary_counts = 0;
        for (auto const& pn : expected_boundary_counts) {
            for (auto const& bn : pn.second) {
                EXPECT_EQ(bn.second,
                        filament->boundary_count(pn.first, bn.first));
                total_boundary_counts += bn.second;
            }
        }

        EXPECT_EQ(total_boundary_counts, filament->boundary_count());
    }
};

TEST_F(SingleStrandFilamentTest, IteratorConstructor) {
    std::vector<species_t> values;
    values += 0, 1, 0, 0, 2, 1, 0, 1;
    initialize_filament(3, values);

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, NumberSpeciesConstructor) {
    uint64_t const size = 26;
    species_t const species = 0;

    auto values = initialize_filament(1, species, size);

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, append_barbed) {
    uint64_t const size = 5;
    species_t const species = 0;

    auto values = initialize_filament(3, species, size);

    filament->append_barbed(1);
    values += 1;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, append_pointed) {
    uint64_t const size = 5;
    species_t const species = 0;

    auto values = initialize_filament(2, species, size);

    filament->append_pointed(1);
    values.insert(values.begin(), 1);
    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, pop_barbed) {
    uint64_t const size = 5;
    species_t const species = 0;

    auto values = initialize_filament(2, species, size);

    values.pop_back();
    {
        auto s = filament->pop_barbed();
        EXPECT_EQ(species, s);
    }
    validate_filament(values);

    {
        filament->append_barbed(1);
        auto s = filament->pop_barbed();
        EXPECT_EQ(1, s);
    }
    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, pop_pointed) {
    uint64_t const size = 5;
    species_t const species = 0;

    auto values = initialize_filament(2, species, size);

    values.erase(values.begin());

    {
        auto s = filament->pop_pointed();
        EXPECT_EQ(0, s);
    }
    validate_filament(values);

    {
        filament->append_pointed(1);
        auto s = filament->pop_pointed();
        EXPECT_EQ(1, s);
    }
    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, pop_barbed_Empty) {
    auto values = initialize_filament(2, 0, 0);

    EXPECT_THROW(filament->pop_barbed(), DepolymerizingEmptyFilament);
    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, pop_pointed_Empty) {
    auto values = initialize_filament(2, 0, 0);

    EXPECT_THROW(filament->pop_pointed(), DepolymerizingEmptyFilament);
    validate_filament(values);
}


TEST_F(SingleStrandFilamentTest, update_subunit_error) {
    uint64_t const size = 5;
    species_t const old_species = 0;
    species_t const new_species = 0;

    auto values = initialize_filament(2, old_species, size);

    EXPECT_THROW(filament->update_subunit(3, old_species, new_species),
            IllegalFilamentUpdate);

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, update_subunit) {
    uint64_t const size = 5;
    species_t const old_species = 0;
    species_t const new_species = 1;

    auto values = initialize_filament(2, old_species, size);

    filament->update_subunit(3, old_species, new_species);
    values[3] = new_species;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, update_subunit_twice_same_position) {
    uint64_t const size = 5;
    species_t const old_species = 0;
    species_t const intermediate_species = 1;
    species_t const final_species = 2;

    auto values = initialize_filament(3, old_species, size);

    filament->update_subunit(3, old_species, intermediate_species);
    filament->update_subunit(0, intermediate_species, final_species);
    values[3] = final_species;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, update_subunit_twice_pointed_to_barbed) {
    uint64_t const size = 7;
    species_t const old_species = 0;
    species_t const new_species = 1;

    auto values = initialize_filament(2, old_species, size);

    filament->update_subunit(3, old_species, new_species);
    filament->update_subunit(3, old_species, new_species);
    values[3] = new_species;
    values[4] = new_species;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, update_subunit_twice_barbed_to_pointed) {
    uint64_t const size = 7;
    species_t const old_species = 0;
    species_t const new_species = 1;

    auto values = initialize_filament(2, old_species, size);

    filament->update_subunit(4, old_species, new_species);
    filament->update_subunit(3, old_species, new_species);
    values[3] = new_species;
    values[4] = new_species;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, update_subunit_thrice) {
    uint64_t const size = 7;
    species_t const old_species = 0;
    species_t const new_species = 1;

    auto values = initialize_filament(2, old_species, size);

    filament->update_subunit(2, old_species, new_species);
    filament->update_subunit(2, old_species, new_species);
    filament->update_subunit(2, old_species, new_species);
    values[2] = new_species;
    values[3] = new_species;
    values[4] = new_species;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, update_boundary_error) {
    std::vector<species_t> values;
    values += 0, 1, 0, 0, 2, 1, 0, 1;

    initialize_filament(3, values);

    EXPECT_THROW(filament->update_boundary(1, 0, 1, 1),
            IllegalFilamentUpdate);

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, update_boundary) {
    std::vector<species_t> values;
    values += 0, 1, 0, 0, 2, 1, 0, 1;

    initialize_filament(3, values);

    filament->update_boundary(1, 0, 1, 2);
    values[7] = 2;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, update_boundary_twice) {
    std::vector<species_t> values;
    values += 0, 1, 0, 0, 2, 1, 0, 1;

    initialize_filament(3, values);

    filament->update_boundary(1, 0, 1, 2);
    values[7] = 2;

    filament->update_boundary(0, 1, 0, 2);
    values[2] = 2;

    validate_filament(values);
}

// -----------------------------------------------------------------------------
// Test all execution paths for fracture_unitary_segment
//
// Cases (9 total):
//      only subunit (no change to boundaries)
//      away from filament ends (remove 2 old boundaries)
//          no merge (2 new boundaries)
//          merge left (1 new boundary)
//          merge right (1 new boundary)
//          merge both (no new boundaries)
//      near filament X end (remove 1 old boundary)
//          merge (no new boundaries)
//          no merge (1 new boundary)
// -----------------------------------------------------------------------------

TEST_F(SingleStrandFilamentTest, fracture_unitary_only_segment) {
    auto values = initialize_filament(3, 0, 1);

    filament->update_subunit(0, 0, 1);
    values[0] = 1;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_unitary_away_no_merge) {
    std::vector<species_t> values;
    values += 0, 1, 0;

    initialize_filament(3, values);

    filament->update_subunit(0, 1, 2);
    values[1] = 2;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_unitary_away_merge_left) {
    std::vector<species_t> values;
    values += 2, 1, 0;

    initialize_filament(3, values);

    filament->update_subunit(0, 1, 2);
    values[1] = 2;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_unitary_away_merge_right) {
    std::vector<species_t> values;
    values += 0, 1, 2;

    initialize_filament(3, values);

    filament->update_subunit(0, 1, 2);
    values[1] = 2;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_unitary_away_merge_both) {
    std::vector<species_t> values;
    values += 2, 1, 2;

    initialize_filament(3, values);

    filament->update_subunit(0, 1, 2);
    values[1] = 2;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_unitary_barbed_no_merge) {
    std::vector<species_t> values;
    values += 0, 1;

    initialize_filament(3, values);

    filament->update_subunit(0, 1, 2);
    values[1] = 2;

    validate_filament(values);
}


TEST_F(SingleStrandFilamentTest, fracture_unitary_barbed_merge) {
    std::vector<species_t> values;
    values += 2, 1;

    initialize_filament(3, values);

    filament->update_subunit(0, 1, 2);
    values[1] = 2;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_unitary_pointed_no_merge) {
    std::vector<species_t> values;
    values += 1, 0;

    initialize_filament(3, values);

    filament->update_subunit(0, 1, 2);
    values[0] = 2;

    validate_filament(values);
}


TEST_F(SingleStrandFilamentTest, fracture_unitary_pointed_merge) {
    std::vector<species_t> values;
    values += 1, 2;

    initialize_filament(3, values);

    filament->update_subunit(0, 1, 2);
    values[0] = 2;

    validate_filament(values);
}

// -----------------------------------------------------------------------------
// Test all non-unitary execution paths for fracture_segment
//
// Cases (7 total):
//      middle of segment (2 new boundaries)
//      X edge of segment
//          away from filament end X
//              matches X neighbor (no new boundaries)
//              not match X neighbor (2 new boundaries, remove old boundary)
//          near filament X end (1 new boundary)
// -----------------------------------------------------------------------------

TEST_F(SingleStrandFilamentTest, fracture_middle) {
    std::vector<species_t> values;
    values += 0, 0, 0;

    initialize_filament(2, values);

    filament->update_subunit(1, 0, 1);
    values[1] = 1;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_right_away_match) {
    std::vector<species_t> values;
    values += 0, 0, 1;

    initialize_filament(2, values);

    filament->update_subunit(1, 0, 1);
    values[1] = 1;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_right_away_not_match) {
    std::vector<species_t> values;
    values += 0, 0, 1;

    initialize_filament(3, values);

    filament->update_subunit(1, 0, 2);
    values[1] = 2;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_left_away_match) {
    std::vector<species_t> values;
    values += 1, 0, 0;

    initialize_filament(2, values);

    filament->update_subunit(0, 0, 1);
    values[1] = 1;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_left_away_not_match) {
    std::vector<species_t> values;
    values += 1, 0, 0;

    initialize_filament(3, values);

    filament->update_subunit(0, 0, 2);
    values[1] = 2;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_right_near) {
    std::vector<species_t> values;
    values += 0, 0;

    initialize_filament(2, values);

    filament->update_subunit(1, 0, 1);
    values[1] = 1;

    validate_filament(values);
}

TEST_F(SingleStrandFilamentTest, fracture_left_near) {
    std::vector<species_t> values;
    values += 0, 0;

    initialize_filament(2, values);

    filament->update_subunit(0, 0, 1);
    values[0] = 1;

    validate_filament(values);
}
