#pragma once

#include <google/protobuf/message.h>

namespace interaction {

class IInteraction {
    public:
        virtual google::protobuf::Message* execute(
                    google::protobuf::Message* request) const = 0;
};

} // namespace interaction
