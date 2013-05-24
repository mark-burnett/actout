#include "entities/Simulation.hpp"
#include "entities/SimulationState.hpp"
#include <memory>

namespace entities {

std::unique_ptr<SimulationState const>
Simulation::execute(SimulationState const* state) const {
    return std::unique_ptr<SimulationState const>(new SimulationState());
}

} // namespace entities
