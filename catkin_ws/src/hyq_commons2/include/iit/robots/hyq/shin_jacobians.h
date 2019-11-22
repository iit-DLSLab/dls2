#ifndef IIT_HYQ_SHIN_JACOBIANS_H_
#define IIT_HYQ_SHIN_JACOBIANS_H_

#include <iit/commons/dog/shin_jacobians.h>
#include <iit/commons/dog/leg_data_map.h>

#include <iit/commons/dog/declarations.h>
#include <iit/robots/hyq/jacobians.h>

#include <iit/rbd/utils.h>

namespace iit {
namespace HyQ {



class ShinJacobians : public dog::ShinJacobians
{
typedef dog::JointState JointState;


public:
    ShinJacobians(dog::KinDynParams& param_getter) :
        param_getter_(param_getter), jacobians_(param_getter_)
    {
    }
    ~ShinJacobians() {}

    dog::FootJac getShinJacobian(const JointState& q,
                                  const double& contact_point,
                                  const dog::LegID& leg) {
        switch(leg){
        case dog::LF:
            param_getter_.setValue_LF_shin(contact_point);
            jacobians_.updateParameters();
            return jacobians_.fr_trunk_J_LF_shin(q).block<3,3>(rbd::LX,0);
        case dog::RF:
            param_getter_.setValue_RF_shin(contact_point);
            jacobians_.updateParameters();
            return jacobians_.fr_trunk_J_RF_shin(q).block<3,3>(rbd::LX,0);
        case dog::LH:
            param_getter_.setValue_LH_shin(contact_point);
            jacobians_.updateParameters();
            return jacobians_.fr_trunk_J_LH_shin(q).block<3,3>(rbd::LX,0);
        case dog::RH:
            param_getter_.setValue_RH_shin(contact_point);
            jacobians_.updateParameters();
            return jacobians_.fr_trunk_J_RH_shin(q).block<3,3>(rbd::LX,0);
        }
        return dog::FootJac::Identity();
    }

private:
    dog::KinDynParams& param_getter_;
    HyQ::Jacobians jacobians_;
};


}
}


#endif
