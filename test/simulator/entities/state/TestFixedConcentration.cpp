#include "entities/state/FixedConcentration.hpp"

#include <gtest/gtest.h>

TEST(Concentrations, FixedConcentration) {
    entities::state::FixedConcentration c(3.1);

    EXPECT_DOUBLE_EQ(3.1, c.value());

    c.remove_monomer();
    EXPECT_DOUBLE_EQ(3.1, c.value());

    c.add_monomer();
    EXPECT_DOUBLE_EQ(3.1, c.value());
}
