cmake_minimum_required(VERSION 2.8)

project(simulator)


set(SOURCES
    IConcentration.hpp
    IEndCondition.hpp
    IEventGenerator.hpp
    IFilament.hpp
    IMeasurement.hpp
    IRNG.hpp
    Simulation.cpp
    Simulation.hpp
    SpeciesMap.cpp
    SpeciesMap.hpp
    State.hpp
    StateModifications.hpp
    common.hpp

    end_conditions/Duration.hpp
    end_conditions/EventCount.hpp

    event_generators/Depolymerization.cpp
    event_generators/Depolymerization.hpp
    event_generators/FilamentSpeciesChangeRandom.cpp
    event_generators/FilamentSpeciesChangeRandom.hpp
    event_generators/FreeSpeciesChange.hpp
    event_generators/Polymerization.cpp
    event_generators/Polymerization.hpp
    event_generators/TipSpeciesChange.cpp
    event_generators/TipSpeciesChange.hpp

    measurements/Concentration.hpp
    measurements/SingleValueBase.hpp

    rng/MersenneTwister.hpp

    state/FixedConcentration.hpp
    state/SingleStrandFilament.cpp
    state/SingleStrandFilament.hpp
    state/VariableConcentration.hpp
)


add_library(simulator ${SOURCES})
target_link_libraries(simulator)
