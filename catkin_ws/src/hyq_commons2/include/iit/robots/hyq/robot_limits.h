#ifndef IIT_HYQ_LIMITS_H
#define IIT_HYQ_LIMITS_H

#include <iit/commons/dog/robot_limits.h>
#include "cylinder_lever_arm.h"

namespace iit {
namespace HyQ {

class Limits : public dog::LimitsBase
{
public:
    Limits();
    virtual ~Limits();

    void setMaxEffort(const dog::JointState & max_effort);

    dog::JointState getTorqueLimits(const dog::JointState & q);
private:
   dog::JointState  max_actuator_effort_;
};

} // namespace HyQ
} // namespace iit
#endif
