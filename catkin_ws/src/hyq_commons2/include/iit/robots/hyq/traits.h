#ifndef IIT_ROBOGEN__HYQ_TRAITS_H_
#define IIT_ROBOGEN__HYQ_TRAITS_H_

#include "transforms.h"
#include "inverse_dynamics.h"
#include "forward_dynamics.h"
#include "jsim.h"

namespace iit {
namespace HyQ {

struct Traits {
    typedef typename HyQ::HomogeneousTransforms HomogeneousTransforms;
    typedef typename HyQ::MotionTransforms MotionTransforms;
    typedef typename HyQ::ForceTransforms ForceTransforms;

    typedef typename HyQ::dyn::ForwardDynamics FwdDynEngine;
    typedef typename HyQ::dyn::InverseDynamics InvDynEngine;
    typedef typename HyQ::dyn::JSIM JSIM;

    static const bool floating_base = true;
};

}
}

#endif
