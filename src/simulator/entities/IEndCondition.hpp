#pragma once

namespace entities {

class IEndCondition {
    public:
        virtual ~IEndCondition() {}

        virtual bool satisfied() const = 0;
};

} // namespace entities
