#pragma once

namespace entities {

class IRNG {
    public:
        virtual ~IRNG() {}

        virtual double next() = 0;
        virtual void reset() = 0;
};

} // namespace entities
