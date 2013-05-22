#include "Ping.hpp"

#include "requests.pb.h"
#include "responses.pb.h"

#include <assert.h>

namespace interaction {

Ping::return_type
Ping::execute(google::protobuf::Message const* request) const {
    assert(request->GetDescriptor() == requests::Ping::descriptor());

    return Ping::return_type(new responses::Pong());
};

} // namespace interaction
