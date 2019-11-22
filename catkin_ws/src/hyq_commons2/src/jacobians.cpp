#include "iit/robots/hyq/jacobians.h"
#include <iit/commons/dog/joint_id_tricks.h>

using namespace iit::dog;

iit::HyQ::Jacobians::Jacobians
    (const dog::KinDynParams& getter_lengths)
     : 
    fr_trunk_J_LF_foot(lengths_values), 
    fr_trunk_J_RF_foot(lengths_values), 
    fr_trunk_J_LH_foot(lengths_values), 
    fr_trunk_J_RH_foot(lengths_values), 
    fr_trunk_J_LF_shin(lengths_values), 
    fr_trunk_J_RF_shin(lengths_values), 
    fr_trunk_J_LH_shin(lengths_values), 
    fr_trunk_J_RH_shin(lengths_values)
    ,
    valuesGetter_lengths(getter_lengths)
{
    updateParameters();
}

namespace iit {
namespace HyQ {
dog::FootJac Jacobians::getFootJacobianXY(const JointState & q,
                               const iit::dog::LegID& leg,
                               const double& foot_x,
                               const double& foot_y)
{

    double sin__q_HAA__ = std::sin( q(iit::dog::toJointID(leg, iit::dog::HAA)));
    double sin__q_HFE__ = std::sin( q(iit::dog::toJointID(leg, iit::dog::HFE)));
    double sin__q_KFE__ = std::sin( q(iit::dog::toJointID(leg, iit::dog::KFE)));
    double cos__q_HAA__ = std::cos( q(iit::dog::toJointID(leg, iit::dog::HAA)));
    double cos__q_HFE__ = std::cos( q(iit::dog::toJointID(leg, iit::dog::HFE)));
    double cos__q_KFE__ = std::cos( q(iit::dog::toJointID(leg, iit::dog::KFE)));

    switch(leg){
        case iit::dog::LF:
            foot_jacobian_(0,0) = 0.0;
            foot_jacobian_(0,1) = (((((( 1.0 *  foot_y) *  cos__q_HFE__) + ( foot_x *  sin__q_HFE__)) *  sin__q_KFE__) + (((( 1.0 *  foot_y) *  sin__q_HFE__) - ( foot_x *  cos__q_HFE__)) *  cos__q_KFE__)) - ( 0.35 *  cos__q_HFE__));
            foot_jacobian_(0,2) = (((( foot_y *  cos__q_HFE__) + ( foot_x *  sin__q_HFE__)) *  sin__q_KFE__) + ((( foot_y *  sin__q_HFE__) - ( foot_x *  cos__q_HFE__)) *  cos__q_KFE__));
            foot_jacobian_(1,0) = (((((((( 1.0 *  foot_y) *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_y) *  cos__q_HAA__) *  sin__q_HFE__) - (( foot_x *  cos__q_HAA__) *  cos__q_HFE__)) *  cos__q_KFE__)) - (( 0.35 *  cos__q_HAA__) *  cos__q_HFE__)) - ( 0.08 *  cos__q_HAA__));
            foot_jacobian_(1,1) = ((((((( 1.0 *  foot_x) *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_y *  sin__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_y) *  sin__q_HAA__) *  cos__q_HFE__) + (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  sin__q_HAA__) *  sin__q_HFE__));
            foot_jacobian_(1,2) = ((((( foot_x *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_y *  sin__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + (((( foot_y *  sin__q_HAA__) *  cos__q_HFE__) + (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__));
            foot_jacobian_(2,0) = (((((((- foot_y *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_x) *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_y *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  sin__q_HAA__) *  cos__q_HFE__)) + ( 0.08 *  sin__q_HAA__));
            foot_jacobian_(2,1) = ((((((( 1.0 *  foot_x) *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_y) *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  cos__q_HAA__) *  sin__q_HFE__));
            foot_jacobian_(2,2) = ((((( foot_x *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + (((( foot_y *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__));
        break;
        case iit::dog::RF:
             foot_jacobian_(0,0) = 0.0;
             foot_jacobian_(0,1) = (((((( 1.0 *  foot_y) *  cos__q_HFE__) + ( foot_x *  sin__q_HFE__)) *  sin__q_KFE__) + (((( 1.0 *  foot_y) *  sin__q_HFE__) - ( foot_x *  cos__q_HFE__)) *  cos__q_KFE__)) - ( 0.35 *  cos__q_HFE__));
             foot_jacobian_(0,2) = (((( foot_y *  cos__q_HFE__) + ( foot_x *  sin__q_HFE__)) *  sin__q_KFE__) + ((( foot_y *  sin__q_HFE__) - ( foot_x *  cos__q_HFE__)) *  cos__q_KFE__));
             foot_jacobian_(1,0) = (((((((- foot_y *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_x) *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  cos__q_HAA__) *  cos__q_HFE__)) + ( 0.08 *  cos__q_HAA__));
             foot_jacobian_(1,1) = ((((((( 1.0 *  foot_y) *  sin__q_HAA__) *  sin__q_HFE__) - (( foot_x *  sin__q_HAA__) *  cos__q_HFE__)) *  sin__q_KFE__) + ((((- foot_y *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) - (( 0.35 *  sin__q_HAA__) *  sin__q_HFE__));
             foot_jacobian_(1,2) = ((((( foot_y *  sin__q_HAA__) *  sin__q_HFE__) - (( foot_x *  sin__q_HAA__) *  cos__q_HFE__)) *  sin__q_KFE__) + ((((- foot_y *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__));
             foot_jacobian_(2,0) = (((((((- foot_y *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_x) *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_y *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  sin__q_HAA__) *  cos__q_HFE__)) + ( 0.08 *  sin__q_HAA__));
             foot_jacobian_(2,1) = ((((((( 1.0 *  foot_x) *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_y) *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  cos__q_HAA__) *  sin__q_HFE__));
             foot_jacobian_(2,2) = ((((( foot_x *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + (((( foot_y *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__));
        break;

        case iit::dog::LH:
            foot_jacobian_(0,0) = 0.0;
            foot_jacobian_(0,1) = (((((( 1.0 *  foot_y) *  cos__q_HFE__) + ( foot_x *  sin__q_HFE__)) *  sin__q_KFE__) + (((( 1.0 *  foot_y) *  sin__q_HFE__) - ( foot_x *  cos__q_HFE__)) *  cos__q_KFE__)) - ( 0.35 *  cos__q_HFE__));
            foot_jacobian_(0,2) = (((( foot_y *  cos__q_HFE__) + ( foot_x *  sin__q_HFE__)) *  sin__q_KFE__) + ((( foot_y *  sin__q_HFE__) - ( foot_x *  cos__q_HFE__)) *  cos__q_KFE__));
            foot_jacobian_(1,0) = (((((((( 1.0 *  foot_y) *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_y) *  cos__q_HAA__) *  sin__q_HFE__) - (( foot_x *  cos__q_HAA__) *  cos__q_HFE__)) *  cos__q_KFE__)) - (( 0.35 *  cos__q_HAA__) *  cos__q_HFE__)) - ( 0.08 *  cos__q_HAA__));
            foot_jacobian_(1,1) = ((((((( 1.0 *  foot_x) *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_y *  sin__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_y) *  sin__q_HAA__) *  cos__q_HFE__) + (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  sin__q_HAA__) *  sin__q_HFE__));
            foot_jacobian_(1,2) = ((((( foot_x *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_y *  sin__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + (((( foot_y *  sin__q_HAA__) *  cos__q_HFE__) + (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__));
            foot_jacobian_(2,0) = (((((((- foot_y *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_x) *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_y *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  sin__q_HAA__) *  cos__q_HFE__)) + ( 0.08 *  sin__q_HAA__));
            foot_jacobian_(2,1) = ((((((( 1.0 *  foot_x) *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_y) *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  cos__q_HAA__) *  sin__q_HFE__));
            foot_jacobian_(2,2) = ((((( foot_x *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + (((( foot_y *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__));
        break;

        case iit::dog::RH:
            foot_jacobian_(0,0) = 0.0;
            foot_jacobian_(0,1) = (((((( 1.0 *  foot_y) *  cos__q_HFE__) + ( foot_x *  sin__q_HFE__)) *  sin__q_KFE__) + (((( 1.0 *  foot_y) *  sin__q_HFE__) - ( foot_x *  cos__q_HFE__)) *  cos__q_KFE__)) - ( 0.35 *  cos__q_HFE__));
            foot_jacobian_(0,2) = (((( foot_y *  cos__q_HFE__) + ( foot_x *  sin__q_HFE__)) *  sin__q_KFE__) + ((( foot_y *  sin__q_HFE__) - ( foot_x *  cos__q_HFE__)) *  cos__q_KFE__));
            foot_jacobian_(1,0) = (((((((- foot_y *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_x) *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  cos__q_HAA__) *  cos__q_HFE__)) + ( 0.08 *  cos__q_HAA__));
            foot_jacobian_(1,1) = ((((((( 1.0 *  foot_y) *  sin__q_HAA__) *  sin__q_HFE__) - (( foot_x *  sin__q_HAA__) *  cos__q_HFE__)) *  sin__q_KFE__) + ((((- foot_y *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) - (( 0.35 *  sin__q_HAA__) *  sin__q_HFE__));
            foot_jacobian_(1,2) = ((((( foot_y *  sin__q_HAA__) *  sin__q_HFE__) - (( foot_x *  sin__q_HAA__) *  cos__q_HFE__)) *  sin__q_KFE__) + ((((- foot_y *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__));
            foot_jacobian_(2,0) = (((((((- foot_y *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_x *  sin__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_x) *  sin__q_HAA__) *  cos__q_HFE__) - (( foot_y *  sin__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  sin__q_HAA__) *  cos__q_HFE__)) + ( 0.08 *  sin__q_HAA__));
            foot_jacobian_(2,1) = ((((((( 1.0 *  foot_x) *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + ((((( 1.0 *  foot_y) *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__)) + (( 0.35 *  cos__q_HAA__) *  sin__q_HFE__));
            foot_jacobian_(2,2) = ((((( foot_x *  cos__q_HAA__) *  cos__q_HFE__) - (( foot_y *  cos__q_HAA__) *  sin__q_HFE__)) *  sin__q_KFE__) + (((( foot_y *  cos__q_HAA__) *  cos__q_HFE__) + (( foot_x *  cos__q_HAA__) *  sin__q_HFE__)) *  cos__q_KFE__));

        break;
    }
    return foot_jacobian_;
}

}
}


void iit::HyQ::Jacobians::updateParameters() {
    lengths_values.foot_x = valuesGetter_lengths.getValue_foot_x();
    lengths_values.LF_shin = valuesGetter_lengths.getValue_LF_shin();
    lengths_values.RF_shin = valuesGetter_lengths.getValue_RF_shin();
    lengths_values.LH_shin = valuesGetter_lengths.getValue_LH_shin();
    lengths_values.RH_shin = valuesGetter_lengths.getValue_RH_shin();
}


iit::HyQ::Jacobians::Type_fr_trunk_J_LF_foot::Type_fr_trunk_J_LF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = - 1.0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
}

const iit::HyQ::Jacobians::Type_fr_trunk_J_LF_foot& iit::HyQ::Jacobians::Type_fr_trunk_J_LF_foot::update(const JointState& jState) {
    static double sin__q_LF_HAA__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    
    sin__q_LF_HAA__ = std::sin( jState(LF_HAA));
    sin__q_LF_HFE__ = std::sin( jState(LF_HFE));
    sin__q_LF_KFE__ = std::sin( jState(LF_KFE));
    cos__q_LF_HAA__ = std::cos( jState(LF_HAA));
    cos__q_LF_HFE__ = std::cos( jState(LF_HFE));
    cos__q_LF_KFE__ = std::cos( jState(LF_KFE));
    
    (*this)(1,1) =  cos__q_LF_HAA__;
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(2,1) = - sin__q_LF_HAA__;
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(3,1) = (((( lengths_values -> foot_x *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( lengths_values -> foot_x *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - ( 0.35 *  cos__q_LF_HFE__));
    (*this)(3,2) = ((( lengths_values -> foot_x *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( lengths_values -> foot_x *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,0) = (((((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  cos__q_LF_HAA__));
    (*this)(4,1) = ((((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) + (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(4,2) = (((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,0) = ((((((- lengths_values -> foot_x *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + (( 0.35 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.08 *  sin__q_LF_HAA__));
    (*this)(5,1) = ((((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) + (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(5,2) = (((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    return *this;
}
iit::HyQ::Jacobians::Type_fr_trunk_J_RF_foot::Type_fr_trunk_J_RF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 1.0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
}

const iit::HyQ::Jacobians::Type_fr_trunk_J_RF_foot& iit::HyQ::Jacobians::Type_fr_trunk_J_RF_foot::update(const JointState& jState) {
    static double sin__q_RF_HAA__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    
    sin__q_RF_HAA__ = std::sin( jState(RF_HAA));
    sin__q_RF_HFE__ = std::sin( jState(RF_HFE));
    sin__q_RF_KFE__ = std::sin( jState(RF_KFE));
    cos__q_RF_HAA__ = std::cos( jState(RF_HAA));
    cos__q_RF_HFE__ = std::cos( jState(RF_HFE));
    cos__q_RF_KFE__ = std::cos( jState(RF_KFE));
    
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(3,1) = (((( lengths_values -> foot_x *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( lengths_values -> foot_x *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) - ( 0.35 *  cos__q_RF_HFE__));
    (*this)(3,2) = ((( lengths_values -> foot_x *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( lengths_values -> foot_x *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,0) = ((((((- lengths_values -> foot_x *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.08 *  cos__q_RF_HAA__));
    (*this)(4,1) = (((((- lengths_values -> foot_x *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__));
    (*this)(4,2) = ((((- lengths_values -> foot_x *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,0) = ((((((- lengths_values -> foot_x *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.08 *  sin__q_RF_HAA__));
    (*this)(5,1) = ((((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__));
    (*this)(5,2) = (((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    return *this;
}
iit::HyQ::Jacobians::Type_fr_trunk_J_LH_foot::Type_fr_trunk_J_LH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = - 1.0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
}

const iit::HyQ::Jacobians::Type_fr_trunk_J_LH_foot& iit::HyQ::Jacobians::Type_fr_trunk_J_LH_foot::update(const JointState& jState) {
    static double sin__q_LH_HAA__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    
    sin__q_LH_HAA__ = std::sin( jState(LH_HAA));
    sin__q_LH_HFE__ = std::sin( jState(LH_HFE));
    sin__q_LH_KFE__ = std::sin( jState(LH_KFE));
    cos__q_LH_HAA__ = std::cos( jState(LH_HAA));
    cos__q_LH_HFE__ = std::cos( jState(LH_HFE));
    cos__q_LH_KFE__ = std::cos( jState(LH_KFE));
    
    (*this)(1,1) =  cos__q_LH_HAA__;
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(2,1) = - sin__q_LH_HAA__;
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(3,1) = (((( lengths_values -> foot_x *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( lengths_values -> foot_x *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - ( 0.35 *  cos__q_LH_HFE__));
    (*this)(3,2) = ((( lengths_values -> foot_x *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( lengths_values -> foot_x *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,0) = (((((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  cos__q_LH_HAA__));
    (*this)(4,1) = ((((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) + (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__));
    (*this)(4,2) = (((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,0) = ((((((- lengths_values -> foot_x *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + (( 0.35 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.08 *  sin__q_LH_HAA__));
    (*this)(5,1) = ((((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) + (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__));
    (*this)(5,2) = (((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    return *this;
}
iit::HyQ::Jacobians::Type_fr_trunk_J_RH_foot::Type_fr_trunk_J_RH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 1.0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
}

const iit::HyQ::Jacobians::Type_fr_trunk_J_RH_foot& iit::HyQ::Jacobians::Type_fr_trunk_J_RH_foot::update(const JointState& jState) {
    static double sin__q_RH_HAA__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    
    sin__q_RH_HAA__ = std::sin( jState(RH_HAA));
    sin__q_RH_HFE__ = std::sin( jState(RH_HFE));
    sin__q_RH_KFE__ = std::sin( jState(RH_KFE));
    cos__q_RH_HAA__ = std::cos( jState(RH_HAA));
    cos__q_RH_HFE__ = std::cos( jState(RH_HFE));
    cos__q_RH_KFE__ = std::cos( jState(RH_KFE));
    
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(3,1) = (((( lengths_values -> foot_x *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( lengths_values -> foot_x *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) - ( 0.35 *  cos__q_RH_HFE__));
    (*this)(3,2) = ((( lengths_values -> foot_x *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( lengths_values -> foot_x *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,0) = ((((((- lengths_values -> foot_x *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.08 *  cos__q_RH_HAA__));
    (*this)(4,1) = (((((- lengths_values -> foot_x *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__));
    (*this)(4,2) = ((((- lengths_values -> foot_x *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,0) = ((((((- lengths_values -> foot_x *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.08 *  sin__q_RH_HAA__));
    (*this)(5,1) = ((((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__));
    (*this)(5,2) = (((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    return *this;
}
iit::HyQ::Jacobians::Type_fr_trunk_J_LF_shin::Type_fr_trunk_J_LF_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = - 1.0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
}

const iit::HyQ::Jacobians::Type_fr_trunk_J_LF_shin& iit::HyQ::Jacobians::Type_fr_trunk_J_LF_shin::update(const JointState& jState) {
    static double sin__q_LF_HAA__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    
    sin__q_LF_HAA__ = std::sin( jState(LF_HAA));
    sin__q_LF_HFE__ = std::sin( jState(LF_HFE));
    sin__q_LF_KFE__ = std::sin( jState(LF_KFE));
    cos__q_LF_HAA__ = std::cos( jState(LF_HAA));
    cos__q_LF_HFE__ = std::cos( jState(LF_HFE));
    cos__q_LF_KFE__ = std::cos( jState(LF_KFE));
    
    (*this)(1,1) =  cos__q_LF_HAA__;
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(2,1) = - sin__q_LF_HAA__;
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(3,1) = (((( lengths_values -> LF_shin *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( lengths_values -> LF_shin *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - ( 0.35 *  cos__q_LF_HFE__));
    (*this)(3,2) = ((( lengths_values -> LF_shin *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( lengths_values -> LF_shin *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,0) = (((((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  cos__q_LF_HAA__));
    (*this)(4,1) = ((((( lengths_values -> LF_shin *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> LF_shin *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) + (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(4,2) = (((( lengths_values -> LF_shin *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> LF_shin *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,0) = ((((((- lengths_values -> LF_shin *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> LF_shin *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + (( 0.35 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.08 *  sin__q_LF_HAA__));
    (*this)(5,1) = ((((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) + (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(5,2) = (((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    return *this;
}
iit::HyQ::Jacobians::Type_fr_trunk_J_RF_shin::Type_fr_trunk_J_RF_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 1.0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
}

const iit::HyQ::Jacobians::Type_fr_trunk_J_RF_shin& iit::HyQ::Jacobians::Type_fr_trunk_J_RF_shin::update(const JointState& jState) {
    static double sin__q_RF_HAA__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    
    sin__q_RF_HAA__ = std::sin( jState(RF_HAA));
    sin__q_RF_HFE__ = std::sin( jState(RF_HFE));
    sin__q_RF_KFE__ = std::sin( jState(RF_KFE));
    cos__q_RF_HAA__ = std::cos( jState(RF_HAA));
    cos__q_RF_HFE__ = std::cos( jState(RF_HFE));
    cos__q_RF_KFE__ = std::cos( jState(RF_KFE));
    
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(3,1) = (((( lengths_values -> RF_shin *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( lengths_values -> RF_shin *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) - ( 0.35 *  cos__q_RF_HFE__));
    (*this)(3,2) = ((( lengths_values -> RF_shin *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( lengths_values -> RF_shin *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,0) = ((((((- lengths_values -> RF_shin *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> RF_shin *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.08 *  cos__q_RF_HAA__));
    (*this)(4,1) = (((((- lengths_values -> RF_shin *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> RF_shin *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__));
    (*this)(4,2) = ((((- lengths_values -> RF_shin *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> RF_shin *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,0) = ((((((- lengths_values -> RF_shin *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> RF_shin *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.08 *  sin__q_RF_HAA__));
    (*this)(5,1) = ((((( lengths_values -> RF_shin *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> RF_shin *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__));
    (*this)(5,2) = (((( lengths_values -> RF_shin *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> RF_shin *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    return *this;
}
iit::HyQ::Jacobians::Type_fr_trunk_J_LH_shin::Type_fr_trunk_J_LH_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = - 1.0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
}

const iit::HyQ::Jacobians::Type_fr_trunk_J_LH_shin& iit::HyQ::Jacobians::Type_fr_trunk_J_LH_shin::update(const JointState& jState) {
    static double sin__q_LH_HAA__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    
    sin__q_LH_HAA__ = std::sin( jState(LH_HAA));
    sin__q_LH_HFE__ = std::sin( jState(LH_HFE));
    sin__q_LH_KFE__ = std::sin( jState(LH_KFE));
    cos__q_LH_HAA__ = std::cos( jState(LH_HAA));
    cos__q_LH_HFE__ = std::cos( jState(LH_HFE));
    cos__q_LH_KFE__ = std::cos( jState(LH_KFE));
    
    (*this)(1,1) =  cos__q_LH_HAA__;
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(2,1) = - sin__q_LH_HAA__;
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(3,1) = (((( lengths_values -> LH_shin *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( lengths_values -> LH_shin *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - ( 0.35 *  cos__q_LH_HFE__));
    (*this)(3,2) = ((( lengths_values -> LH_shin *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( lengths_values -> LH_shin *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,0) = (((((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  cos__q_LH_HAA__));
    (*this)(4,1) = ((((( lengths_values -> LH_shin *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> LH_shin *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) + (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__));
    (*this)(4,2) = (((( lengths_values -> LH_shin *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> LH_shin *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,0) = ((((((- lengths_values -> LH_shin *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> LH_shin *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + (( 0.35 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.08 *  sin__q_LH_HAA__));
    (*this)(5,1) = ((((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) + (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__));
    (*this)(5,2) = (((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    return *this;
}
iit::HyQ::Jacobians::Type_fr_trunk_J_RH_shin::Type_fr_trunk_J_RH_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 1.0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
}

const iit::HyQ::Jacobians::Type_fr_trunk_J_RH_shin& iit::HyQ::Jacobians::Type_fr_trunk_J_RH_shin::update(const JointState& jState) {
    static double sin__q_RH_HAA__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    
    sin__q_RH_HAA__ = std::sin( jState(RH_HAA));
    sin__q_RH_HFE__ = std::sin( jState(RH_HFE));
    sin__q_RH_KFE__ = std::sin( jState(RH_KFE));
    cos__q_RH_HAA__ = std::cos( jState(RH_HAA));
    cos__q_RH_HFE__ = std::cos( jState(RH_HFE));
    cos__q_RH_KFE__ = std::cos( jState(RH_KFE));
    
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(3,1) = (((( lengths_values -> RH_shin *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( lengths_values -> RH_shin *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) - ( 0.35 *  cos__q_RH_HFE__));
    (*this)(3,2) = ((( lengths_values -> RH_shin *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( lengths_values -> RH_shin *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,0) = ((((((- lengths_values -> RH_shin *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> RH_shin *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.08 *  cos__q_RH_HAA__));
    (*this)(4,1) = (((((- lengths_values -> RH_shin *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> RH_shin *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__));
    (*this)(4,2) = ((((- lengths_values -> RH_shin *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> RH_shin *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,0) = ((((((- lengths_values -> RH_shin *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> RH_shin *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.08 *  sin__q_RH_HAA__));
    (*this)(5,1) = ((((( lengths_values -> RH_shin *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> RH_shin *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__));
    (*this)(5,2) = (((( lengths_values -> RH_shin *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> RH_shin *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    return *this;
}
