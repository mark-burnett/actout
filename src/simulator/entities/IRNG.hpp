#pragma once

namespace entities {

class IRNG {
    public:
        virtual ~IRNG() {}

        virtual double exponential(double const& denominator) = 0;
        virtual double uniform(double const& min=0, double const& max=1) = 0;

        virtual void reset() = 0;
};

} // namespace entities
