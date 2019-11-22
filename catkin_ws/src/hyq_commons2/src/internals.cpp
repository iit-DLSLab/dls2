
#include <iit/robots/hyq/internals.h>
#include <iit/robots/hyq/constants.h>


const iit::HyQ::internal::MagicNumbers iit::HyQ::internal::misc_cfg;

iit::HyQ::internal::MagicNumbers::MagicNumbers()
{
    HAAJointToMotorOffset[dog::LF] = HyQ::LF_HAA_JOINT_MOTOR_OFF;
    HAAJointToMotorOffset[dog::RF] = HyQ::LF_HAA_JOINT_MOTOR_OFF;
    HAAJointToMotorOffset[dog::LH] = HyQ::LF_HAA_JOINT_MOTOR_OFF;
    HAAJointToMotorOffset[dog::RH] = HyQ::LF_HAA_JOINT_MOTOR_OFF;

}
