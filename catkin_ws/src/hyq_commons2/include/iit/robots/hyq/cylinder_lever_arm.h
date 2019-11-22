/*
 *  Created on: July 7, 2014
 *      Author: Marco Frigerio
 */

#ifndef IIT_HYQ_CYLINDERLEVERARM_H_
#define IIT_HYQ_CYLINDERLEVERARM_H_

#include <iit/commons/dog/declarations.h>
#include <iit/commons/dog/joint_data_map.h>
#include <iit/robots/hyq/constants.h>

#include <math.h>


namespace iit {
namespace HyQ {

namespace internal {

class CylinderLeverArm
{
public:
    static const CylinderLeverArm& getInstance();

    double getLeverArm(dog::JointIdentifiers joint, double cyl) const;
    double getCylinderLength(dog::JointIdentifiers joint, double theta) const;
    double getJointStatus(dog::JointIdentifiers joint, double cyl) const;

private:
    CylinderLeverArm();
    static CylinderLeverArm instance;

    typedef  double(CylinderLeverArm::*FuncPtr) (double)const;
    typedef  double(CylinderLeverArm::*FuncPtr2)(dog::JointIdentifiers,double)const;
    dog::JointDataMap<FuncPtr > lengthCalculators;
    dog::JointDataMap<FuncPtr > leverCalculators;
    dog::JointDataMap<FuncPtr2> jointStatusCalculators;

    dog::JointDataMap<int> length_sign_flip;


    double haa_placeholder(double foo) const { return 0; }
    double haa_placeholder(dog::JointIdentifiers joint, double foo) const { return 0; }
    double hfe_cyl_length(double th) const;
    double kfe_cyl_length(double th) const;
    double hfe_lever(double cyl) const;
    double kfe_lever(double cyl) const;
    double hfe_joint_status(dog::JointIdentifiers joint, double cyl) const;
    double kfe_joint_status(dog::JointIdentifiers joint, double cyl) const;

};


inline const CylinderLeverArm& CylinderLeverArm::getInstance()
{
    return instance;
}


#define CALL_MEMBER_FN(ptrToMember) ((*this).*(ptrToMember))


inline double CylinderLeverArm::getCylinderLength(dog::JointIdentifiers joint, double theta) const
{
    return CALL_MEMBER_FN(lengthCalculators[joint])( length_sign_flip[joint]*theta );
}

inline double CylinderLeverArm::getLeverArm(dog::JointIdentifiers joint, double cylinderLength) const
{
    return CALL_MEMBER_FN(leverCalculators[joint])( cylinderLength );
}

inline double CylinderLeverArm::getJointStatus(dog::JointIdentifiers joint, double cylinderLength) const
{
    return CALL_MEMBER_FN(jointStatusCalculators[joint])(joint, cylinderLength );
}

#undef CALL_MEMBER_FN


inline double CylinderLeverArm::hfe_cyl_length(double th) const
{
    static const double _2a1b1 = 2*a1*b1;
    return sqrt(a1_sqr + b1_sqr - _2a1b1*cos( (M_PI/2.0 + th + eps11) ) );
}

inline double CylinderLeverArm::kfe_cyl_length(double th) const
{
    static const double _2a2b2 = 2*a2*b2;
    return sqrt(a2_sqr + b2_sqr - _2a2b2*cos( (M_PI - th - (eps21 + eps22)) ) );
}

inline double CylinderLeverArm::hfe_lever(double c) const
{
    return a1*sin( acos( (a1_sqr+c*c-b1_sqr) / (2*a1*c) ) );
}

inline double CylinderLeverArm::kfe_lever(double c) const
{
    return a2*sin( acos( (a2_sqr+c*c-b2_sqr) / (2*a2*c) ) );
}

inline double CylinderLeverArm::hfe_joint_status(dog::JointIdentifiers joint, double cyl_length) const
{
    double carg = acos((cyl_length * cyl_length - a1_sqr - b1_sqr) / (- 2 * a1 * b1));
    return + (carg - eps11 - M_PI/2.0) * length_sign_flip[joint];
}

inline double CylinderLeverArm::kfe_joint_status(dog::JointIdentifiers joint, double cyl_length) const
{
    double carg = acos((cyl_length * cyl_length - a2_sqr - b2_sqr) / (- 2 * a2 * b2));
    return -(carg + eps21 + eps22 - M_PI) * length_sign_flip[joint];
}


}


/**
 * Computes the length of the cylinder lever arm for the given joint and joint
 * angle.
 * \param joint any flexion-extension joint of HyQ; any of the HAA joints
 *        will cause 0 to be returned
 * \param cylinderLength the current total length of the cylinder
 */
inline double getCylinderLeverArm(dog::JointIdentifiers joint, double cylinderLength)
{
    return internal::CylinderLeverArm::getInstance().getLeverArm(joint, cylinderLength);
}

/**
 * Computes the total length of the cylinder (in meters) for the given joint
 * and joint angle.
 * \param joint any flexion-extension joint of HyQ; any of the HAA joints
 *        will cause 0 to be returned
 * \param theta the joint status, according to the robot convention
 */
inline double getCylinderLength(dog::JointIdentifiers joint, double theta)
{
    return internal::CylinderLeverArm::getInstance().getCylinderLength(joint, theta);
}

/**
 * Computes the position status of the given joint, corresponding to the given
 * cylinder length.
 *
 * \param joint any flexion-extension joint of HyQ; any of the HAA joints
 *        will cause 0 to be returned
 * \param cylinderLength the current total length of the cylinder
 */
inline double getJointStatus(dog::JointIdentifiers joint, double cylinderLength)
{
    return internal::CylinderLeverArm::getInstance().getJointStatus(joint, cylinderLength);
}


}
}
#endif
