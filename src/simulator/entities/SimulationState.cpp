#include "entities/SimulationState.hpp"

#include <string>

namespace entities {

std::tuple<typename SimulationState::tag_map_t::iterator,
           typename SimulationState::tag_map_t::iterator>
SimulationState::find_components(std::string const& tag) {
    auto begin = tag_map.find(tag);
    auto end = tag_map.end();

    return std::make_tuple(begin, end);
}

std::tuple<typename SimulationState::tag_map_t::const_iterator,
           typename SimulationState::tag_map_t::const_iterator>
SimulationState::find_components(std::string const& tag) const {
    auto begin = tag_map.find(tag);
    auto end = tag_map.end();

    return std::make_tuple(begin, end);
}

std::map<IStateComponent*, uint64_t const>::size_type
SimulationState::num_components() const {
    return components.size();
}

uint64_t
SimulationState::add_component(std::unique_ptr<IStateComponent>&& component,
        std::string const& tag) {
    id_lookup[component.get()] = next_component_id;
    tag_map.insert(
            typename tag_map_t::value_type(tag, component.get()));
    component_map.insert(
            typename component_map_t::value_type(
                next_component_id, tag));
    components[next_component_id] = std::move(component);

    return next_component_id++;
}

IStateComponent*
SimulationState::get(uint64_t component_id) {
    return components[component_id].get();
}

uint64_t
SimulationState::lookup_id(IStateComponent const* component) const {
    auto i = id_lookup.find(component);
    return i->second;
}

std::unique_ptr<IStateComponent>&&
SimulationState::pop(uint64_t component_id) {
    auto c = std::move(components[component_id]);
    components.erase(component_id);

    return std::move(c);
}

} // namespace entities
