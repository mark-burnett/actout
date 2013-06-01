#include <gtest/gtest.h>
#include <google/protobuf/message.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    auto rv = RUN_ALL_TESTS();

    google::protobuf::ShutdownProtobufLibrary();

    return rv;
}
