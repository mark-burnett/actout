#pragma once

#include "IInteraction.hpp"

#include <google/protobuf/message.h>

namespace interaction {

class Ping: public IInteraction {
    public:
        return_type execute(
                    google::protobuf::Message const* request) const;
};

} // namespace interaction
