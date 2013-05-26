#include "entities/IEndCondition.hpp"
#include "entities/IEvent.hpp"
#include "entities/IEventGenerator.hpp"
#include "entities/IMeasurement.hpp"
#include "entities/IRNG.hpp"
#include "entities/Simulation.hpp"
#include "entities/SimulationState.hpp"

#include "entities/end_conditions/EventCount.hpp"
#include "entities/events/NOP.hpp"

#include <gtest/gtest.h>
#include <inttypes.h>

using namespace entities;


class MockRNG : public IRNG {
public:
    uint64_t exponential_call_count;
    uint64_t uniform_call_count;

    MockRNG()
        : exponential_call_count(0),
        uniform_call_count(0) {}

    double exponential(double const& denominator) {
        ++exponential_call_count;
        return 1;
    }
    double uniform(double const& min=0, double const& max=1) {
        ++uniform_call_count;
        return (min + max) / 2;
    }
};

class MockEventGenerator : public IEventGenerator {
public:
    uint64_t max_events_;
    uint64_t event_count;
    MockEventGenerator(uint64_t max_events)
        : max_events_(max_events), event_count(0) {}

    double rate(SimulationState const* state,
            std::vector<StateModificationDescriptor> const&
                modified_state_components) {
        if (event_count < max_events_) {
            ++event_count;
            return 1;
        } else {
            return 0;
        }
    }

    std::unique_ptr<IEvent const> create_event(SimulationState const* state,
            double const& random_number) const {
        return std::unique_ptr<events::NOP const>(new events::NOP());
    }
};

TEST(Simulation, MinimalSimulation) {
    auto rng = std::unique_ptr<MockRNG>(new MockRNG);

    std::vector<std::unique_ptr<IEndCondition const> > ecs;
    std::vector<std::unique_ptr<IEventGenerator> > event_generators;

    auto state = std::unique_ptr<SimulationState>(new SimulationState());
    std::vector<std::unique_ptr<IMeasurement> > measurements;

    auto s = Simulation(ecs, event_generators);

    s.execute(state.get(), measurements, rng.get());

    ASSERT_EQ(0, state->event_count);
    ASSERT_EQ(1, state->time);
    ASSERT_EQ(0, state->total_event_rate);

    ASSERT_EQ(1, rng->exponential_call_count);
    ASSERT_EQ(0, rng->uniform_call_count);
}

TEST(Simulation, SingleEventSimulation) {
    auto rng = std::unique_ptr<MockRNG>(new MockRNG);

    std::vector<std::unique_ptr<IEndCondition const> > ecs;
    std::vector<std::unique_ptr<IEventGenerator> > event_generators;
    auto eg = std::unique_ptr<MockEventGenerator>(new MockEventGenerator(1));
    auto eg_ptr = eg.get();
    event_generators.push_back(std::move(eg));

    auto state = std::unique_ptr<SimulationState>(new SimulationState());
    std::vector<std::unique_ptr<IMeasurement> > measurements;

    auto s = Simulation(ecs, event_generators);

    s.execute(state.get(), measurements, rng.get());

    ASSERT_EQ(1, state->event_count);
    ASSERT_EQ(2, state->time);
    ASSERT_EQ(0, state->total_event_rate);

    ASSERT_EQ(2, rng->exponential_call_count);
    ASSERT_EQ(1, rng->uniform_call_count);

    ASSERT_EQ(1, eg_ptr->event_count);
}
