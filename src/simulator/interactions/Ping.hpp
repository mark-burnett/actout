#pragma once

#include "IInteraction.hpp"

#include <google/protobuf/message.h>

namespace interaction {

class Ping: public IInteraction {
    public:
        google::protobuf::Message* execute(
                    google::protobuf::Message* request) const;
};

} // namespace interaction
