#include "entities/state/SingleStrandFilament.hpp"
#include "entities/common.hpp"
#include "entities/exceptions.hpp"

#include <algorithm>
#include <functional>
#include <iterator>


namespace entities {
namespace state {


void
SingleStrandFilament::initialize_counts(uint64_t const& num_species) {
    length_ = 0;

    species_counts_.reserve(num_species);
    species_counts_.resize(num_species);
    boundary_counts_.reserve(num_species);
    boundary_counts_.resize(num_species);
    for (auto& bc_row : boundary_counts_) {
        bc_row.reserve(num_species);
        bc_row.resize(num_species);
    }
}


SingleStrandFilament::SingleStrandFilament(uint64_t const& num_species,
        std::vector<species_t>::const_iterator begin,
        std::vector<species_t>::const_iterator end) {
    initialize_counts(num_species);

    while (begin != end) {
        auto boundary = std::adjacent_find(begin, end,
                std::not_equal_to<species_t const>());

        if (boundary != end) {
            ++boundary;
        }
        auto number = std::distance(begin, boundary);

        segments_.emplace_back(number, *begin);
        length_ += number;
        species_counts_[*begin] += number;

        if (boundary != end) {
            ++boundary_counts_[*begin][*boundary];
        }

        begin = boundary;
    }
}


SingleStrandFilament::SingleStrandFilament(uint64_t const& num_species,
        uint64_t const& number, species_t const& species) {
    initialize_counts(num_species);
    segments_.emplace_back(number, species);
    length_ = number;
    species_counts_[species] = number;
}


uint64_t const
SingleStrandFilament::species_count(
        species_t const& species) const {
    return species_counts_[species];
}

uint64_t const
SingleStrandFilament::boundary_count(
        species_t const& pointed_species,
        species_t const& barbed_species) const {
    return boundary_counts_[pointed_species][barbed_species];
}

uint64_t const
SingleStrandFilament::length() const {
    return length_;
}

species_t const
SingleStrandFilament::peek_barbed() const {
    return segments_.back().species;
}

species_t const
SingleStrandFilament::peek_pointed() const {
    return segments_.front().species;
}


void
SingleStrandFilament::append_barbed(
        species_t const& species) {
    if (0 == length_) {
        segments_.emplace_back(1, species);
    } else {
        if (species == peek_barbed()) {
            ++segments_.back().number;
        } else {
            ++boundary_counts_[peek_barbed()][species];
            segments_.emplace_back(1, species);
        }
    }

    ++length_;
    ++species_counts_[species];
}

void
SingleStrandFilament::append_pointed(
        species_t const& species) {
    if (0 == length_) {
        segments_.emplace_front(1, species);
    } else {
        if (species == peek_pointed()) {
            ++segments_.front().number;
        } else {
            ++boundary_counts_[species][peek_pointed()];
            segments_.emplace_front(1, species);
        }
    }

    ++length_;
    ++species_counts_[species];
}

species_t const
SingleStrandFilament::pop_barbed() {
    if (length_ > 0) {
        Segment &seg = segments_.back();
        uint64_t species = seg.species;

        if (1 == seg.number) {
            segments_.pop_back();
            if (length_ > 1) {
                --boundary_counts_[peek_barbed()][species];
            }
        } else {
            --seg.number;
        }

        --length_;
        --species_counts_[species];

        return species;
    } else {
        throw DepolymerizingEmptyFilament();
    }
}

species_t const
SingleStrandFilament::pop_pointed() {
    if (length_ > 0) {
        Segment &seg = segments_.front();
        uint64_t species = seg.species;

        if (1 == seg.number) {
            segments_.pop_front();
            if (length_ > 1) {
                --boundary_counts_[species][peek_pointed()];
            }
        } else {
            --seg.number;
        }

        --length_;
        --species_counts_[species];

        return species;
    } else {
        throw DepolymerizingEmptyFilament();
    }
}

void
SingleStrandFilament::update_subunit(uint64_t instance_number,
        species_t const& old_species,
        species_t const& new_species) {
    if (old_species == new_species) {
        throw IllegalFilamentUpdate();
    }

    if (instance_number >= species_counts_[old_species]) {
        throw IllegalSpeciesIndex();
    }

    --species_counts_[old_species];
    ++species_counts_[new_species];

    for (auto i = segments_.begin(); i != segments_.end(); ++i) {
        if (old_species == i->species) {
            if (instance_number < i->number) {
                fracture_segment(i, instance_number, new_species);
                return;
            }
            instance_number -= i->number;
        }
    }
}

void
SingleStrandFilament::update_boundary(uint64_t instance_number,
        species_t const& old_pointed_species,
        species_t const& old_barbed_species,
        species_t const& new_barbed_species) {
    if (old_barbed_species == new_barbed_species) {
        throw IllegalFilamentUpdate();
    }

    if (instance_number >=
            boundary_counts_[old_pointed_species][old_barbed_species]) {
        throw IllegalBoundaryIndex();
    }
    --species_counts_[old_barbed_species];
    ++species_counts_[new_barbed_species];

    uint64_t count = 0;

    auto pointed_segment = segments_.begin();
    auto barbed_segment = std::next(pointed_segment);

    if (segments_.end() == barbed_segment) {
        throw BoundaryUpdateSmallFilament();
    }

    while (barbed_segment != segments_.end()) {
        if (old_pointed_species == pointed_segment->species &&
                old_barbed_species == barbed_segment->species) {
            if (instance_number == count) {
                // When updating boundaries,
                // we always choose the left-most protomer.
                fracture_segment(barbed_segment, 0, new_barbed_species);
                return;
            }
            ++count;
        }
        ++pointed_segment;
        ++barbed_segment;
    }
}

void
SingleStrandFilament::fracture_segment(std::list<Segment>::iterator& segment,
        uint64_t const& index, species_t const& new_species) {
    if (1 == segment->number) {
        fracture_unitary_segment(segment, new_species);
        return;
    }

    // Non 1 length cases:
    uint64_t left = index;
    uint64_t right = segment->number - index - 1;

    // cases are left edge, middle, right edge
    // left edge:  left = 0
        // check for merge, 1 or 2 boundaries
    // right edge: right = 0
        // check for merge, 1 or 2 boundaries
    // middle: left > 0 && right > 0
        // no merge, 2 boundaries

    if (0 == left) { // left edge of segment
        auto pn = bounded_prev(segment, segments_);
        if (segments_.end() != pn) { // not near pointed end of filament
            if (new_species == pn->species) { // same, no boundary
                ++(pn->number);
                --(segment->number);
            } else { // different, add boundary
                --boundary_counts_[pn->species][segment->species];
                segments_.emplace(segment, 1, new_species);
                --(segment->number);
                ++boundary_counts_[pn->species][new_species];
                ++boundary_counts_[new_species][segment->species];
            }
        } else { // near pointed end of filament
            segments_.emplace(segment, 1, new_species);
            --(segment->number);
            ++boundary_counts_[new_species][segment->species];
        }

    } else if (0 == right) { // right edge
        auto bn = std::next(segment);
        if (segments_.end() != bn) { // not near barbed end of filament
            if (new_species == bn->species) { // same, no boundary
                ++(bn->number);
                --(segment->number);
            } else { // different, add boundary
                --boundary_counts_[segment->species][bn->species];
                segments_.emplace(bn, 1, new_species);
                segment->number = left;
                ++boundary_counts_[new_species][bn->species];
                ++boundary_counts_[segment->species][new_species];
            }
        } else { // near barbed end of filament
            segments_.emplace_back(1, new_species);
            segment->number = left;
            ++boundary_counts_[segment->species][new_species];
        }

    } else { // middle
        segments_.emplace(segment, left, segment->species);
        segments_.emplace(segment, 1, new_species);
        segment->number = right;

        ++boundary_counts_[new_species][segment->species];
        ++boundary_counts_[segment->species][new_species];
    }
}

void
SingleStrandFilament::fracture_unitary_segment(
        std::list<Segment>::iterator& segment,
        species_t const& new_species) {
    auto old_species = segment->species;
    auto pn = bounded_prev(segment, segments_);
    auto bn = std::next(segment);

    if (segments_.end() != pn) {
        --boundary_counts_[pn->species][old_species];
    }
    if (segments_.end() != bn) {
        --boundary_counts_[old_species][bn->species];
    }

    // cases are:
        // near pointed end
            // only segment
            // merge no boundary
            // no merge & boundary
        // near barbed end
            // merge no boundary
            // no merge & boundary
        // not near filament ends
            // merge left & boundary,
            // merge all
            // merge right & boundary,
            // no merge & 2 boundaries,

    if (segments_.end() == pn) { // near pointed end
        if (segments_.end() == bn) { // only segment
            segment->species = new_species;
        } else { // check for merge right
            if (new_species == bn->species) { // merge, no boundaries
                ++(bn->number);
                segments_.erase(segment);
            } else { // no merge, boundaries
                segment->species = new_species;
                ++boundary_counts_[new_species][bn->species];
            }
        }

    } else if (segments_.end() == bn) { // near barbed end
        if (new_species == pn->species) { // merge, no boundaries
            ++(pn->number);
            segments_.erase(segment);
        } else { // no merge, boundaries
            segment->species = new_species;
            ++boundary_counts_[pn->species][new_species];
        }

    } else { // away from ends
        if (new_species == pn->species) { // merge left or all, 0 or 1 boundaries
            if (new_species == bn->species) { // merge all, 0 boundaries
                pn->number += bn->number + 1;
                segments_.erase(bn);
            } else { // merge left, 1 boundary
                ++(pn->number);
                ++boundary_counts_[pn->species][bn->species];
            }
            segments_.erase(segment);

        } else if (new_species == bn->species) { // merge right, 1 boundary
            ++(bn->number);
            ++boundary_counts_[pn->species][bn->species];
            segments_.erase(segment);

        } else { // no merges, 2 boundaries
            segment->species = new_species;
            ++boundary_counts_[pn->species][new_species];
            ++boundary_counts_[new_species][bn->species];
        }
    }
}


SingleStrandFilament::SingleStrandFilamentIterator
SingleStrandFilament::begin() const {
    return SingleStrandFilamentIterator(segments_.cbegin());
}

SingleStrandFilament::SingleStrandFilamentIterator
SingleStrandFilament::end() const {
    return SingleStrandFilamentIterator(segments_.cend());
}


} // namespace state
} // namespace entities
