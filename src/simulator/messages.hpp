#pragma once

#include <google/protobuf/message.h>
#include <memory>


typedef std::unique_ptr<google::protobuf::Message const> MessagePtr;
