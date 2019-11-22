#include "iit/robots/hyq/robot_limits.h"

namespace iit {
namespace HyQ {


Limits::Limits()
{
    max_actuator_effort_ <<120.0, 3600.0, 3600.0, 120.0, 3600.0, 3600.0,120.0, 3600.0,3600.0, 120.0, 3600.0,3600.0; //N

}

Limits::~Limits(){}

void Limits::setMaxEffort(const dog::JointState & max_effort)
{
    max_actuator_effort_ = max_effort;
}

dog::JointState Limits::getTorqueLimits(const iit::dog::JointState & q)
{

    dog::JointState tau_max;
    tau_max.setZero();

    iit::dog::JointIdentifiers j_id;
    for (int i = 0; i<iit::dog::jointsCount; i++)
    {
        j_id = iit::dog::JointIdentifiers(i);
        double cylLength = iit::HyQ::getCylinderLength( j_id, q(j_id) );
        double cylLever  = HyQ::getCylinderLeverArm( j_id, cylLength );
        if (cylLever == 0.0) {
            tau_max(j_id) = max_actuator_effort_(j_id);
        }else{
            tau_max(j_id) = max_actuator_effort_(j_id) * cylLever;}
    }
    return tau_max;
}

}
}


