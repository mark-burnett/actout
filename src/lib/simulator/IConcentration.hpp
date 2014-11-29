#pragma once

#include <inttypes.h>

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/member.hpp>

namespace mpl = boost::mpl;
using namespace boost::type_erasure;


BOOST_TYPE_ERASURE_MEMBER((has_value), value, 0)
BOOST_TYPE_ERASURE_MEMBER((has_monomer_count), monomer_count, 0)
BOOST_TYPE_ERASURE_MEMBER((has_add_monomer), add_monomer, 0)
BOOST_TYPE_ERASURE_MEMBER((has_remove_monomer), remove_monomer, 0)


namespace simulator {
namespace state {

typedef any<mpl::vector<
    has_value<double(), _self const>,
    has_monomer_count<uint64_t(), _self const>,
    has_add_monomer<void()>,
    has_remove_monomer<void()>,
    copy_constructible<>
> > Concentration;

} // namespace state
} // namespace simulator
