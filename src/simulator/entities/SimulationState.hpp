#pragma once

#include "entities/IStateComponent.hpp"

#include <inttypes.h>
#include <map>
#include <memory>
#include <string>
#include <tuple>

namespace entities {

class SimulationState {
public:
    typedef std::multimap<std::string const, IStateComponent*> tag_map_t;
    typedef std::multimap<uint64_t const, std::string> component_map_t;

private:
    tag_map_t tag_map;
    component_map_t component_map;
    std::map<uint64_t const, std::unique_ptr<IStateComponent> > components;
    std::map<IStateComponent const*, uint64_t> id_lookup;

public:
    double time;
    double total_event_rate;
    uint64_t event_count;
    uint64_t next_component_id;

    SimulationState() : time(0), total_event_rate(0), event_count(0) {}

    std::tuple<typename tag_map_t::iterator, typename tag_map_t::iterator>
            find_components(std::string const& tag);
    std::tuple<typename tag_map_t::const_iterator, typename tag_map_t::const_iterator>
            find_components(std::string const& tag) const;

    std::map<IStateComponent*, uint64_t const>::size_type
            num_components() const;

    uint64_t add_component(std::unique_ptr<IStateComponent>&& component,
            std::string const& tag);

    IStateComponent* get(uint64_t component_id);
    uint64_t lookup_id(IStateComponent const* component) const;

    std::unique_ptr<IStateComponent>&& pop(uint64_t component_id);
};

} // namespace entities
