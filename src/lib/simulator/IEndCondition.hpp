#pragma once

#include "simulator/State.hpp"
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/member.hpp>


namespace mpl = boost::mpl;
using namespace boost::type_erasure;

BOOST_TYPE_ERASURE_MEMBER((has_satisfied), satisfied, 1)


namespace simulator {

typedef any<mpl::vector<
    has_satisfied<bool(State const*)>,
    copy_constructible<>
> > EndCondition;

} // namespace simulator
