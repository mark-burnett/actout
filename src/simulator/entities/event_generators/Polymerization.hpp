#pragma once

#include "entities/IEventGenerator.hpp"
#include "entities/IFilament.hpp"
#include "entities/SpeciesMap.hpp"
#include "entities/State.hpp"

namespace entities {
namespace event_generators {

class FixedRatePolymerizationBase : public IEventGenerator {
public:
    FixedRatePolymerizationBase(SpeciesMap::species_t const& species,
            double const& rate) : species_(species), rate_(rate) {}

    double rate(State const* state,
            std::vector<StateModificationDescriptor> const&
                modified_state_components);

    std::vector<StateModificationDescriptor> perform_event(
            State* state, double const& random_number) const;

protected:
    SpeciesMap::species_t const species_;
    double const rate_;

    virtual void append_species(IFilament* filament) const = 0;
};


class BarbedEndPolymerization : public FixedRatePolymerizationBase {
public:
    BarbedEndPolymerization(SpeciesMap::species_t const& species,
            double const& rate) : FixedRatePolymerizationBase(species, rate) {}

protected:
    void append_species(IFilament* filament) const {
        filament->append_barbed(species_);
    }
};


class PointedEndPolymerization : public FixedRatePolymerizationBase {
public:
    PointedEndPolymerization(SpeciesMap::species_t const& species,
            double const& rate) : FixedRatePolymerizationBase(species, rate) {}

protected:
    void append_species(IFilament* filament) const {
        filament->append_pointed(species_);
    }
};


} // namespace event_generators
} // namespace entities
