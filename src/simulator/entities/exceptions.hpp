#pragma once

#include <exception>

namespace entities {

class SimulationError : public std::exception {};

class BoundsError : public SimulationError {};
class BoundaryUpdateEmptyFilament : public BoundsError {};
class BoundaryUpdateSmallFilament : public BoundsError {};
class DepolymerizingEmptyFilament : public BoundsError {};
class IllegalBoundaryIndex : public BoundsError {};
class IllegalSpeciesIndex : public BoundsError {};

class IllegalFilamentUpdate : public SimulationError {};

} // namespace entities
