#include "Ping.hpp"

#include "requests.pb.h"
#include "responses.pb.h"

#include <assert.h>

namespace interaction {

google::protobuf::Message*
Ping::execute(google::protobuf::Message* request) const {
    assert(request->GetDescriptor() ==
           requests::Ping::descriptor());

    return new responses::Pong();
};

} // namespace interaction
