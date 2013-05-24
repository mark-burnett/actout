#include "CreateSimulation.hpp"

#include "requests.pb.h"
#include "responses.pb.h"

#include <assert.h>

namespace interaction {

CreateSimulation::return_type
CreateSimulation::execute(google::protobuf::Message const* request) const {
    assert(
        request->GetDescriptor() == requests::CreateSimulation::descriptor());

    return CreateSimulation::return_type(new responses::Pong());
};

} // namespace interaction
