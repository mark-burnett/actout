# actout

[![Build Status][travis-status]](https://travis-ci.org/mark-burnett/actout)

actout is a Monte Carlo simulator for actin filament dynamics.  It is
implemented efficiently, using the Gillespie algorithm.


## Project Status

Currently under initial development.  That means migrating the core algorithm
over from my [original actin project](https://github.com/mark-burnett/filament-dynamics)
and making a few improvements to make certain features (e.g. a physical barrier
that impedes polymerization, filament severing and annealing, and possibly
filament branching) easier to implement.

After the migration of core code is complete, I will start working on a
sensible interface and command line utility to enable basic usage.  Once that
is in place, I will try to actually use the code for research, see what
additional needs arise and work from there.

One important goal for this project is to shed the significant cruft associated
with the old project, so the complex database backend will be left behind
unless and until it is deemed truly necessary.


## Installation

### Dependencies

A c++ 11 friendly compiler -- the code is currently tested with gcc 4.6, clang
3.0 and clang 3.2.

- Boost
- CMake
- Google Protocol Buffers
- Google Test

To see specific (Ubuntu) dependencies, checkout .travis.yml

### Building the project

```bash
mkdir build
cd build
cmake ..
make
make test
```


## Usage

The project is still a skeleton, so there's nothing to do yet.


[travis-status]: https://travis-ci.org/mark-burnett/actout.png?branch=master
