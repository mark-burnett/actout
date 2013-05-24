#pragma once

#include "messages.hpp"
#include <google/protobuf/message.h>

namespace interaction {

class IInteraction {
public:
    typedef MessagePtr return_type;

    virtual ~IInteraction() {}

    virtual return_type execute(
                google::protobuf::Message const* request) const = 0;
};

} // namespace interaction
