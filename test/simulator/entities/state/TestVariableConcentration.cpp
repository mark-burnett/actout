#include "entities/state/VariableConcentration.hpp"

#include <gtest/gtest.h>


TEST(Concentrations, VariableConcentration) {
    entities::state::VariableConcentration c(4.8, 1 / 1.2);

    EXPECT_DOUBLE_EQ(4.8, c.value());

    c.remove_monomer();
    EXPECT_DOUBLE_EQ(3.6, c.value());

    c.add_monomer();
    c.add_monomer();

    EXPECT_DOUBLE_EQ(6, c.value());
}
