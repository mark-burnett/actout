#pragma once

#include "IInteraction.hpp"

#include <google/protobuf/message.h>

namespace interaction {

class CreateSimulation: public IInteraction {
    public:
        return_type execute(
                    google::protobuf::Message const* request) const;
};

} // namespace interaction
