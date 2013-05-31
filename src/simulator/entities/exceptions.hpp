#pragma once

#include <exception>

class BoundsError : public std::exception {};
class DepolymerizingEmptyFilament : public BoundsError {};
class IllegalSpeciesIndex : public BoundsError {};
class IllegalBoundaryIndex : public BoundsError {};
class BoundaryUpdateSmallFilament : public BoundsError {};
class BoundaryUpdateEmptyFilament : public BoundsError {};
