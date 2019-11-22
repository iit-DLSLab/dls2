#ifndef IIT_HYQ_FORWARD_KINEMATICS_H_
#define IIT_HYQ_FORWARD_KINEMATICS_H_

#include <iit/commons/dog/leg_data_map.h>
#include <iit/commons/dog/forward_kinematics.h>

#include <iit/commons/dog/declarations.h>
#include <iit/robots/hyq/transforms.h>

#include <iit/rbd/utils.h>

namespace iit {
namespace HyQ {



class ForwardKinematics : public dog::ForwardKinematics
{
public:
    ForwardKinematics(dog::KinDynParams& param_getter) :
        param_getter_(param_getter), transforms_(param_getter_)
    {
        myGetters[dog::LF] = & ForwardKinematics::getFootPosLF;
        myGetters[dog::RF] = & ForwardKinematics::getFootPosRF;
        myGetters[dog::LH] = & ForwardKinematics::getFootPosLH;
        myGetters[dog::RH] = & ForwardKinematics::getFootPosRH;
    }
    ~ForwardKinematics() {}

    dog::Vector3d getFootPosLF(const dog::JointState& q) {
        transforms_.updateParameters();
        return rbd::Utils::positionVector( transforms_.fr_trunk_X_LF_foot(q) );
    }

    dog::Vector3d getFootPosRF(const dog::JointState& q) {
        transforms_.updateParameters();
        return rbd::Utils::positionVector( transforms_.fr_trunk_X_RF_foot(q) );
    }

    dog::Vector3d getFootPosLH(const dog::JointState& q)  {
        transforms_.updateParameters();
        return rbd::Utils::positionVector( transforms_.fr_trunk_X_LH_foot(q) );
    }

    dog::Vector3d getFootPosRH(const dog::JointState& q)  {
        transforms_.updateParameters();
        return rbd::Utils::positionVector( transforms_.fr_trunk_X_RH_foot(q) );
    }

    dog::Vector3d getFootPos(const dog::JointState& q, const dog::LegID& leg) {
        return ((*this).*(myGetters[leg]))(q);
    }

    dog::Matrix3d getFootOrientation(const dog::JointState &q, const dog::LegID &leg){
        switch(leg){
        case dog::LF:
            return rbd::Utils::rotationMx(transforms_.fr_trunk_X_LF_foot(q));
        case dog::RF:
            return rbd::Utils::rotationMx(transforms_.fr_trunk_X_RF_foot(q));
        case dog::LH:
            return rbd::Utils::rotationMx(transforms_.fr_trunk_X_LH_foot(q));
        case dog::RH:
            return rbd::Utils::rotationMx(transforms_.fr_trunk_X_RH_foot(q));
        }
        return dog::Matrix3d::Identity();
    }
    dog::Vector3d getShinPos(const dog::JointState& q,
                             const double& contact_pos,
                             const dog::LegID& leg)
    {
        switch(leg){
        case dog::LF:
            param_getter_.setValue_LF_shin(contact_pos);
            transforms_.updateParameters();
            return rbd::Utils::positionVector(transforms_.fr_trunk_X_LF_shin(q));
        case dog::RF:
            param_getter_.setValue_RF_shin(contact_pos);
            transforms_.updateParameters();
            return rbd::Utils::positionVector(transforms_.fr_trunk_X_RF_shin(q));
        case dog::LH:
            param_getter_.setValue_LH_shin(contact_pos);
            transforms_.updateParameters();
            return rbd::Utils::positionVector(transforms_.fr_trunk_X_LH_shin(q));
        case dog::RH:
            param_getter_.setValue_RH_shin(contact_pos);
            transforms_.updateParameters();
            return rbd::Utils::positionVector(transforms_.fr_trunk_X_RH_shin(q));
        }
        return dog::Vector3d::Zero();
    }

private:
    dog::KinDynParams& param_getter_;
    HyQ::HomogeneousTransforms transforms_;

    typedef  dog::Vector3d(ForwardKinematics::*getter)(const dog::JointState&);

    dog::LegDataMap< getter > myGetters;

};


}
}


#endif
