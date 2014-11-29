#pragma once

#include "simulator/IEventGenerator.hpp"
#include "simulator/IFilament.hpp"
#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"
#include "simulator/common.hpp"


namespace simulator {
namespace event_generators {


class FixedRatePolymerizationBase {
public:
    FixedRatePolymerizationBase(species_t const& species,
            double const& rate) : species_(species), rate_(rate) {}
    virtual ~FixedRatePolymerizationBase() {}

    double rate(State const* state, StateModifications const& modifications);

    StateModifications perform_event(State* state, double random_number) const;

protected:
    species_t const species_;

private:
    virtual void append_species(IFilament* filament) const = 0;
    double const rate_;
};


class BarbedEndPolymerization : public FixedRatePolymerizationBase {
public:
    BarbedEndPolymerization(species_t const& species,
            double const& rate) : FixedRatePolymerizationBase(species, rate) {}

private:
    void append_species(IFilament* filament) const {
        filament->append_barbed(species_);
    }
};


class PointedEndPolymerization : public FixedRatePolymerizationBase {
public:
    PointedEndPolymerization(species_t const& species,
            double const& rate) : FixedRatePolymerizationBase(species, rate) {}

private:
    void append_species(IFilament* filament) const {
        filament->append_pointed(species_);
    }
};


} // namespace event_generators
} // namespace simulator
