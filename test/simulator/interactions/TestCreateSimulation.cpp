#include "interactions/CreateSimulation.hpp"
#include "messages.hpp"
#include "requests.pb.h"
#include "responses.pb.h"

#include <gtest/gtest.h>

TEST(CreateSimulation, Basic) {
    interaction::CreateSimulation p = interaction::CreateSimulation();

    auto request = MessagePtr(new requests::CreateSimulation());
    auto response = p.execute(request.get());

    ASSERT_EQ(response->GetDescriptor(), responses::Pong::descriptor());
}
