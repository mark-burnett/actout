#pragma once

#include "entities/IFilament.hpp"
#include "entities/common.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <inttypes.h>
#include <list>
#include <vector>


namespace entities {
namespace state {

class SingleStrandFilament : public IFilament {
private:
    struct Segment {
        uint64_t number;
        species_t species;

        Segment(uint64_t const& number_,
                species_t const& species_)
            : number(number_), species(species_) {}
    };

    uint64_t length_;
    std::list<Segment> segments_;
    std::vector<uint64_t> species_counts_;
    std::vector<std::vector<uint64_t> > boundary_counts_;

    class SingleStrandFilamentIterator : public boost::iterator_facade<
                                            SingleStrandFilamentIterator,
                                            species_t const,
                                            boost::bidirectional_traversal_tag> {
    public:
        SingleStrandFilamentIterator(
                std::list<SingleStrandFilament::Segment>::const_iterator segment)
            : segment_(segment), index_(0) {}

    private:
        std::list<SingleStrandFilament::Segment>::const_iterator segment_;
        uint64_t index_;

        friend class boost::iterator_core_access;

        void increment() {
            ++index_;
            if (index_ < segment_->number) {
                return;
            } else {
                index_ = 0;
                ++segment_;
            }
        }

        species_t const& dereference() const {
            return segment_->species;
        }
        bool equal(SingleStrandFilamentIterator const& other) const {
            return segment_ == other.segment_ && index_ == other.index_;
        }
    };
    void fracture_segment(std::list<Segment>::iterator& segment,
            uint64_t const& index,
            species_t const& new_species);

    void fracture_unitary_segment(std::list<Segment>::iterator& segment,
            species_t const& new_species);

    void initialize_counts(uint64_t const& num_species);

public:
    SingleStrandFilament(uint64_t const& num_species,
        std::vector<species_t>::const_iterator begin,
        std::vector<species_t>::const_iterator end);

    SingleStrandFilament(uint64_t const& num_species,
            uint64_t const& number,
            species_t const& species);

    uint64_t const species_count(
            species_t const& species) const;
    uint64_t const boundary_count(species_t const& pointed_species,
            species_t const& barbed_species) const;
    uint64_t const boundary_count() const {
        return segments_.size() - 1;
    }
    uint64_t const length() const;

    species_t const peek_barbed() const;
    species_t const peek_pointed() const;


    void append_barbed(species_t const& species);
    void append_pointed(species_t const& species);

    species_t const pop_barbed();
    species_t const pop_pointed();

    void update_subunit(uint64_t instance_number,
            species_t const& old_species,
            species_t const& new_species);
    void update_boundary(uint64_t instance_number,
            species_t const& old_pointed_species,
            species_t const& old_barbed_species,
            species_t const& new_barbed_species);

    SingleStrandFilamentIterator begin() const;
    SingleStrandFilamentIterator end() const;
};


template<typename C>
typename C::iterator
bounded_prev(typename C::iterator& i, C& l) {
    if (i == l.begin()) {
        return l.end();
    } else {
        return std::prev(i);
    }
}


} // namespace state
} // namespace entities
