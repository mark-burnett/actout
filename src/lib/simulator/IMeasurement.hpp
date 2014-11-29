#pragma once

#include "simulator/State.hpp"
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/member.hpp>
#include <vector>


namespace mpl = boost::mpl;
using namespace boost::type_erasure;

BOOST_TYPE_ERASURE_MEMBER((has_perform), perform, 1)
BOOST_TYPE_ERASURE_MEMBER((has_get_samples), get_samples, 0)


namespace simulator {

typedef any<mpl::vector<
    has_perform<void(State const*)>,
    has_get_samples<std::vector<double>()>,
    copy_constructible<>
> > Measurement;

} // namespace simulator
