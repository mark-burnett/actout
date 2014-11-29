#pragma once

#include "simulator/State.hpp"
#include "simulator/StateModifications.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/member.hpp>

namespace mpl = boost::mpl;
using namespace boost::type_erasure;


BOOST_TYPE_ERASURE_MEMBER((has_rate), rate, 2)
BOOST_TYPE_ERASURE_MEMBER((has_perform_event), perform_event, 2)


namespace simulator {

typedef any<mpl::vector<
    has_rate<double(State const*, StateModifications const&)>,
    has_perform_event<StateModifications(State*, double), _self const>,
    copy_constructible<>
> > EventGenerator;

} // namespace simulator
