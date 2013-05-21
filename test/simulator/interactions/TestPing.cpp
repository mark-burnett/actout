#include "interactions/Ping.hpp"
#include "requests.pb.h"
#include "responses.pb.h"

#include <gtest/gtest.h>

TEST(Ping, Basic) {
    interaction::Ping p = interaction::Ping();

    auto request = new requests::Ping();
    auto response = p.execute(request);

    ASSERT_EQ(response->GetDescriptor(), responses::Pong::descriptor());

    delete request;
    delete response;
}
