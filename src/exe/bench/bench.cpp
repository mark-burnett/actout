#include "simulator/IEndCondition.hpp"
#include "simulator/IEventGenerator.hpp"
#include "simulator/IMeasurement.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/State.hpp"
#include "simulator/end_conditions/Duration.hpp"
#include "simulator/event_generators/Depolymerization.hpp"
#include "simulator/event_generators/FilamentSpeciesChangeRandom.hpp"
#include "simulator/event_generators/FreeSpeciesChange.hpp"
#include "simulator/event_generators/Polymerization.hpp"
#include "simulator/event_generators/TipSpeciesChange.hpp"
#include "simulator/measurements/Concentration.hpp"
#include "simulator/measurements/FilamentLengthMean.hpp"
#include "simulator/rng/MersenneTwister.hpp"
#include "simulator/state/SingleStrandFilament.hpp"
#include "simulator/state/VariableConcentration.hpp"

#include <inttypes.h>
#include <memory>

#include <iostream>


const uint64_t number_of_filaments = 1;

const double fnc = 0.00179;
const double seed_concentration = 6;

const double initial_concentration = 30;

const double duration = 2500;
const double sample_period = 1;

const double hydrolysis_rate = 0.3;
// const double dissociation_cooperativity = 1e10;
// const double dissociation_rate = 9.1e-10;
// const double dissociation_cooperativity = 1e2;
// const double dissociation_rate = 1.45e-4;

const double dissociation_cooperativity = 1;
const double dissociation_rate = 1.62e-3;
// const double vectorial_rate = 9.1;

const double b_atp_assoc_rate = 11.6;
const double b_adppi_assoc_rate = 3.4;
const double b_adp_assoc_rate = 2.9;

const double b_atp_dissoc_rate = 1.4;
const double b_adppi_dissoc_rate = 0.2;
const double b_adp_dissoc_rate = 5.4;

const double p_atp_assoc_rate = 1.3;
const double p_adppi_assoc_rate = 0.11;
const double p_adp_assoc_rate = 0.14;

const double p_atp_dissoc_rate = 0.8;
const double p_adppi_dissoc_rate = 0.02;
const double p_adp_dissoc_rate = 0.25;

const double b_tip_pi_dissoc_rate = 2;
const double p_tip_pi_dissoc_rate = 2; // could try vectorial rate

const double sol_pi_dissoc_rate = 10000;

const double pi_association_rate = 2e-6;


using namespace simulator;

using namespace end_conditions;
using namespace event_generators;
using namespace measurements;
using namespace rng;
using namespace state;

int main(int argc, char** argv) {
    double const volume = number_of_filaments / fnc;
    uint64_t const initial_filament_length = static_cast<uint64_t>(
            seed_concentration / fnc);

    // stuff passed to constructor
    std::vector<EndCondition> end_conditions;
    std::vector<std::unique_ptr<IEventGenerator> > event_generators;

    end_conditions.push_back(Duration(duration));


    // Random hydrolysis
    event_generators.push_back(std::move(
                std::unique_ptr<FilamentSpeciesChangeRandom>(
                    new FilamentSpeciesChangeRandom(0, 1, hydrolysis_rate))));

    // XXX temporary random dissociation
    event_generators.push_back(std::move(
                std::unique_ptr<FilamentSpeciesChangeRandom>(
                    new FilamentSpeciesChangeRandom(1, 2, dissociation_rate))));

    event_generators.push_back(std::move(
                std::unique_ptr<PointedTipSpeciesChange>(
                    new PointedTipSpeciesChange(1, 2, p_tip_pi_dissoc_rate))));
    event_generators.push_back(std::move(
                std::unique_ptr<BarbedTipSpeciesChange>(
                    new BarbedTipSpeciesChange(1, 2, b_tip_pi_dissoc_rate))));

    {
        std::vector<species_t> v;
        v.reserve(2);
        v.push_back(2);
        v.push_back(3);
        event_generators.push_back(std::move(
                    std::unique_ptr<FreeSpeciesChange>(
                        new FreeSpeciesChange(1, v, sol_pi_dissoc_rate))));
    }


    // poly/depoly transitions
    event_generators.push_back(std::move(
                std::unique_ptr<BarbedEndPolymerization>(
                    new BarbedEndPolymerization(0, b_atp_assoc_rate))));
    event_generators.push_back(std::move(
                std::unique_ptr<BarbedEndDepolymerization>(
                    new BarbedEndDepolymerization(0, b_atp_dissoc_rate))));

    event_generators.push_back(std::move(
                std::unique_ptr<BarbedEndPolymerization>(
                    new BarbedEndPolymerization(1, b_adppi_assoc_rate))));
    event_generators.push_back(std::move(
                std::unique_ptr<BarbedEndDepolymerization>(
                    new BarbedEndDepolymerization(1, b_adppi_dissoc_rate))));

    event_generators.push_back(std::move(
                std::unique_ptr<BarbedEndPolymerization>(
                    new BarbedEndPolymerization(2, b_adp_assoc_rate))));
    event_generators.push_back(std::move(
                std::unique_ptr<BarbedEndDepolymerization>(
                    new BarbedEndDepolymerization(2, b_adp_dissoc_rate))));

    event_generators.push_back(std::move(
                std::unique_ptr<PointedEndPolymerization>(
                    new PointedEndPolymerization(0, p_atp_assoc_rate))));
    event_generators.push_back(std::move(
                std::unique_ptr<PointedEndDepolymerization>(
                    new PointedEndDepolymerization(0, p_atp_dissoc_rate))));

    event_generators.push_back(std::move(
                std::unique_ptr<PointedEndPolymerization>(
                    new PointedEndPolymerization(1, p_adppi_assoc_rate))));
    event_generators.push_back(std::move(
                std::unique_ptr<PointedEndDepolymerization>(
                    new PointedEndDepolymerization(1, p_adppi_dissoc_rate))));

    event_generators.push_back(std::move(
                std::unique_ptr<PointedEndPolymerization>(
                    new PointedEndPolymerization(2, p_adp_assoc_rate))));
    event_generators.push_back(std::move(
                std::unique_ptr<PointedEndDepolymerization>(
                    new PointedEndDepolymerization(2, p_adp_dissoc_rate))));


    // stuff passed to execute
    State state;
    std::vector<Measurement> measurements;
    MersenneTwister rng;


    // setup state
    for (uint64_t i = 0; i < number_of_filaments; ++i) {
        state.filaments.push_back(std::move(
                    std::unique_ptr<SingleStrandFilament>(
                        new SingleStrandFilament(
                            4, initial_filament_length, 2))));
    }

    // 0 == ATP
    state.concentrations.push_back(std::move(
                    std::unique_ptr<VariableConcentration>(
                        new VariableConcentration(
                            initial_concentration, volume))));
    // 1 == ADP-Pi
    state.concentrations.push_back(std::move(
                    std::unique_ptr<VariableConcentration>(
                        new VariableConcentration(0, volume))));
    // 2 == ADP
    state.concentrations.push_back(std::move(
                    std::unique_ptr<VariableConcentration>(
                        new VariableConcentration(0, volume))));
    // 3 == Pi
    state.concentrations.push_back(std::move(
                    std::unique_ptr<VariableConcentration>(
                        new VariableConcentration(0, volume))));

    measurements.emplace_back(FilamentLengthMean(sample_period));
    // Pi concentration
    measurements.emplace_back(Concentration(sample_period, 3));

    Simulation simulation(end_conditions, event_generators);
    simulation.execute(&state, measurements, &rng);


    return 0;
}
