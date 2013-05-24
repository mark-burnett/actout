#pragma once

#include "entities/IEndCondition.hpp"
#include "entities/IEventGenerator.hpp"
#include "entities/IRNG.hpp"
#include "entities/SimulationState.hpp"
#include <memory>
#include <vector>

namespace entities {

class Simulation {
    private:
        typedef std::vector<
            std::unique_ptr<IEndCondition const> > ec_container;
        typedef std::vector<
            std::unique_ptr<IEventGenerator const> > eg_container;

        std::unique_ptr<IRNG>& rng_;
        std::unique_ptr<ec_container>& end_conditions_;
        std::unique_ptr<eg_container>& event_generators_;

    public:
        Simulation(std::unique_ptr<IRNG>&& rng,
                std::unique_ptr<ec_container>&& end_conditions,
                std::unique_ptr<eg_container>&& event_generators)
            : rng_(rng),
            end_conditions_(end_conditions),
            event_generators_(event_generators) {};

        std::unique_ptr<SimulationState const> execute(
                SimulationState const* state) const;
};

} // namespace entities
