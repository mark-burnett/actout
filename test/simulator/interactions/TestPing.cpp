#include "interactions/Ping.hpp"
#include "messages.hpp"
#include "requests.pb.h"
#include "responses.pb.h"

#include <gtest/gtest.h>

TEST(Ping, Basic) {
    interaction::Ping p = interaction::Ping();

    auto request = MessagePtr(new requests::Ping());
    auto response = p.execute(request.get());

    ASSERT_EQ(response->GetDescriptor(), responses::Pong::descriptor());
}
