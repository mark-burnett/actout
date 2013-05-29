#include "entities/SpeciesMap.hpp"
#include "entities/state/SingleStrandFilament.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <boost/assign/std/vector.hpp>

// Bring operator+= into the namespace
using namespace boost::assign;

using namespace entities;
using namespace entities::state;

TEST(SegmentedFilament, IteratorConstructor) {
    std::vector<SpeciesMap::species_index_t> values;
    values += 0;
    values += 1;
    values += 0;
    values += 0;
    values += 2;
    values += 1;
    values += 0;
    values += 1;
//    = {0, 1, 0, 0, 2, 1, 0, 1};

    SingleStrandFilament f(3, values.begin(), values.end());

    EXPECT_EQ(8, f.length());
    EXPECT_EQ(4, f.species_count(0));
    EXPECT_EQ(3, f.species_count(1));
    EXPECT_EQ(1, f.species_count(2));
    EXPECT_EQ(2, f.boundary_count(0, 1));
    EXPECT_EQ(1, f.boundary_count(0, 2));
}

TEST(SegmentedFilament, NumberSpeciesConstructor) {
    SingleStrandFilament f(5, 26, 0);
    EXPECT_EQ(26, f.length());EXPECT_EQ(26, f.species_count(0));
    EXPECT_EQ( 0, f.species_count(1));
    EXPECT_EQ( 0, f.species_count(2));
    EXPECT_EQ( 0, f.species_count(3));
    EXPECT_EQ( 0, f.species_count(4));
    EXPECT_EQ(0, f.peek_barbed());
}

TEST(SegmentedFilament, Append) {
    SingleStrandFilament f(3, 20, 0);

    f.append_barbed(1);
    EXPECT_EQ(21, f.length());
    EXPECT_EQ(1, f.peek_barbed());
    EXPECT_EQ(1, f.species_count(1));

    f.append_pointed(2);
    EXPECT_EQ(22, f.length());
    EXPECT_EQ(2, f.peek_pointed());
    EXPECT_EQ(1, f.species_count(2));
}

TEST(SegmentedFilament, Remove) {
    SingleStrandFilament f(3, 20, 0);

    SpeciesMap::species_index_t s = f.pop_barbed();
    EXPECT_EQ(0, s);
    EXPECT_EQ(19, f.length());

    f.append_barbed(1);
    s = f.pop_barbed();
    EXPECT_EQ(1, s);
    EXPECT_EQ(19, f.length());

    s = f.pop_pointed();
    EXPECT_EQ(0, s);
    EXPECT_EQ(18, f.length());

    f.append_pointed(2);
    s = f.pop_pointed();
    EXPECT_EQ(2, s);
    EXPECT_EQ(18, f.length());

}

//TEST(SegmentedFilament, RemoveEmpty) {
//    filaments::SegmentedFilament f(1, 0);
//
//    SpeciesMap::species_index_t s = f.pop_barbed();
//    EXPECT_EQ(0, s);
//    EXPECT_EQ(0, f.length());
//    EXPECT_EQ(0, f.species_count(0));
//
//    try {
//        f.pop_barbed();
//        EXPECT_TRUE(false) << "Should have thrown exception.";
//    } catch (filaments::DepolymerizingEmptyFilament e) {
//    }
//    EXPECT_EQ(0, f.length());
//    EXPECT_EQ(0, f.species_count(0));
//
//    try {
//        f.pop_barbed();
//        EXPECT_TRUE(false) << "Should have thrown exception.";
//    } catch (filaments::DepolymerizingEmptyFilament e) {
//    }
//    EXPECT_EQ(0, f.length());
//    EXPECT_EQ(0, f.species_count(0));
//}

TEST(SegmentedFilament, UpdateSpecies) {
    SingleStrandFilament f(5, 20, 0);

    f.update_subunit(4, 0, 1);
    EXPECT_EQ(20, f.length());
    EXPECT_EQ(1, f.species_count(1));
    EXPECT_EQ(1, f.boundary_count(0, 1));
    EXPECT_EQ(1, f.boundary_count(1, 0));

    f.update_subunit(3, 0, 1);
    EXPECT_EQ(20, f.length());
    EXPECT_EQ(2, f.species_count(1));
    EXPECT_EQ(1, f.boundary_count(0, 1));
    EXPECT_EQ(1, f.boundary_count(1, 0));

    f.update_subunit(1, 1, 2);
    EXPECT_EQ(20, f.length());
    EXPECT_EQ(1, f.species_count(1));
    EXPECT_EQ(1, f.species_count(2));
    EXPECT_EQ(1, f.boundary_count(0, 1));
    EXPECT_EQ(1, f.boundary_count(1, 2));
    EXPECT_EQ(1, f.boundary_count(2, 0));

    f.update_subunit(0, 0, 3);
    EXPECT_EQ(20, f.length());
    EXPECT_EQ(3, f.peek_pointed());
    EXPECT_EQ(1, f.species_count(3));
}

TEST(SegmentedFilament, UpdateBoundary) {
    SingleStrandFilament f(5, 20, 0);

    f.update_subunit(5, 0, 1);
    f.update_boundary(0, 0, 1, 3);
    EXPECT_EQ(1, f.boundary_count(0, 3));
    EXPECT_EQ(1, f.boundary_count(3, 0));

    EXPECT_EQ(19, f.species_count(0));
    EXPECT_EQ(0, f.species_count(1));
    EXPECT_EQ(1, f.species_count(3));

    f.update_subunit(10, 0, 3);
    EXPECT_EQ(2, f.boundary_count(0, 3));
    f.update_boundary(1, 0, 3, 4);
    EXPECT_EQ(1, f.boundary_count(0, 3));
    EXPECT_EQ(1, f.boundary_count(3, 0));
    EXPECT_EQ(1, f.boundary_count(0, 4));
    EXPECT_EQ(1, f.boundary_count(4, 0));

    EXPECT_EQ(18, f.species_count(0));
    EXPECT_EQ(0, f.species_count(1));
    EXPECT_EQ(1, f.species_count(3));
    EXPECT_EQ(1, f.species_count(4));
}

// Specific SegmentedFilament cases to test
//TEST(SegmentedFilament, FractureSingleCasePointedEndMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 0, 2, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 1, 1, 0, 0, 2, 1, 0, 1;
//
//    SingleStrandFilament f(3, values.begin(), values.end());
//    f.update_subunit(0, 0, 1);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.begin(), f.end());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(1, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    EXPECT_EQ(1, f.boundary_count(0, 2));
//    EXPECT_EQ(1, f.boundary_count(2, 1));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureSingleCasePointedEndNoMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 0, 2, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 2, 1, 0, 0, 2, 1, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(0, 0, 2);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(1, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    EXPECT_EQ(1, f.boundary_count(0, 2));
//    EXPECT_EQ(2, f.boundary_count(2, 1));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureSingleCaseBarbedEndMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 0, 2, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 0, 2, 1, 0, 0;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(2, 1, 0);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(1, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    EXPECT_EQ(1, f.boundary_count(0, 2));
//    EXPECT_EQ(1, f.boundary_count(2, 1));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureSingleCaseBarbedEndNoMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 0, 2, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 0, 2, 1, 0, 2;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(2, 1, 2);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(1, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    EXPECT_EQ(2, f.boundary_count(0, 2));
//    EXPECT_EQ(1, f.boundary_count(2, 1));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureSingleCaseNoEndLeftMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 0, 2, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 0, 0, 1, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(0, 2, 0);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(3, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    EXPECT_EQ(0, f.boundary_count(0, 2));
//    EXPECT_EQ(0, f.boundary_count(2, 1));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureSingleCaseNoEndRightMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 0, 2, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 0, 1, 1, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(0, 2, 1);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(3, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    EXPECT_EQ(0, f.boundary_count(0, 2));
//    EXPECT_EQ(0, f.boundary_count(2, 1));
//
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureSingleCaseNoEndNoMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 0, 2, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 0, 3, 1, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(0, 2, 3);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(2, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    EXPECT_EQ(0, f.boundary_count(0, 2));
//    EXPECT_EQ(0, f.boundary_count(2, 1));
//
//    EXPECT_EQ(1, f.boundary_count(0, 3));
//    EXPECT_EQ(1, f.boundary_count(3, 1));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureSingleCaseNoEndMergeAll) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 0, 2, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 0, 0, 0, 2, 1, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(0, 1, 0);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(1, f.boundary_count(0, 1));
//    EXPECT_EQ(1, f.boundary_count(1, 0));
//
//    EXPECT_EQ(1, f.boundary_count(0, 2));
//    EXPECT_EQ(1, f.boundary_count(2, 1));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//// Non 1 length segment fracture cases:
//// left segment edge, not at pointed end, with merge
//// left segment edge, not at pointed end, no merge
//// left segment edge, at pointed end, no merge
//// right segment edge, not at barbed end, with merge
//// right segment edge, not at barbed end, no merge
//// right segment edge, at barbed end, no merge
//// middle of segment, ??? - could be more explicit

//TEST(SegmentedFilament, FractureMultipleCaseLeftEdgeNoEndMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 1, 1, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 0, 1, 1, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(1, 1, 0);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(3, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureMultipleCaseLeftEdgeNoEndNoMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 1, 1, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 2, 1, 1, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(1, 1, 2);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(2, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    EXPECT_EQ(1, f.boundary_count(0, 2));
//    EXPECT_EQ(1, f.boundary_count(2, 1));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureMultipleCaseLeftEdgePointedEnd) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 0, 1, 1, 0, 1, 0, 0;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 1, 0, 1, 1, 0, 1, 0, 0;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(0, 0, 1);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(2, f.boundary_count(0, 1));
//    EXPECT_EQ(3, f.boundary_count(1, 0));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureMultipleCaseRightEdgeNoEndMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 1, 1, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 1, 1, 0, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(3, 1, 0);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(3, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureMultipleCaseRightEdgeNoEndNoMerge) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 1, 1, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 1, 1, 2, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(3, 1, 2);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(3, f.boundary_count(0, 1));
//    EXPECT_EQ(1, f.boundary_count(1, 0));
//
//    EXPECT_EQ(0, f.boundary_count(0, 2));
//    EXPECT_EQ(0, f.boundary_count(2, 1));
//
//    EXPECT_EQ(1, f.boundary_count(1, 2));
//    EXPECT_EQ(1, f.boundary_count(2, 0));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureMultipleCaseRightEdgeBarbedEnd) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 0, 1, 1, 0, 1, 0, 0;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 0, 1, 1, 0, 1, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(4, 0, 1);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(3, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}

//TEST(SegmentedFilament, FractureMultipleCaseMiddle) {
//    std::vector<SpeciesMap::species_index_t> values;
//    values += 0, 1, 0, 1, 1, 1, 0, 1;
//
//    std::vector<SpeciesMap::species_index_t> expected_result;
//    expected_result += 0, 1, 0, 1, 2, 1, 0, 1;
//
//    filaments::SegmentedFilament f(values.begin(), values.end());
//    f.update_subunit(2, 1, 2);
//    std::vector<SpeciesMap::species_index_t> actual_result(f.get_states());
//
//    EXPECT_EQ(8, f.length());
//    EXPECT_EQ(8, actual_result.size());
//
//    EXPECT_EQ(3, f.boundary_count(0, 1));
//    EXPECT_EQ(2, f.boundary_count(1, 0));
//
//    EXPECT_EQ(0, f.boundary_count(0, 2));
//    EXPECT_EQ(0, f.boundary_count(2, 0));
//
//    EXPECT_EQ(1, f.boundary_count(1, 2));
//    EXPECT_EQ(1, f.boundary_count(2, 1));
//
//    for (size_t i = 0; i < expected_result.size(); ++i) {
//        EXPECT_EQ(expected_result[i], actual_result[i])
//            << "i = " << i << std::endl;
//    }
//}
