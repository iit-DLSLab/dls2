#include "iit/robots/hyq/inverse_dynamics.h"
#include "iit/robots/hyq/inertia_properties.h"
#ifndef EIGEN_NO_DEBUG
    #include <iostream>
    static bool warning_printed = false; // Here to avoid spamming messages in the tests
#endif

using namespace std;
using namespace iit::rbd;
using namespace iit::HyQ::dyn;
using namespace iit::dog;



iit::HyQ::dyn::InverseDynamics::InverseDynamics(InertiaProperties& inertia, MotionTransforms& transforms) :
    inertiaProps( & inertia ),
    xm( & transforms ),
    LF_hipassembly_I(inertiaProps->getTensor_LF_hipassembly() ),
    LF_upperleg_I(inertiaProps->getTensor_LF_upperleg() ),
    LF_lowerleg_I(inertiaProps->getTensor_LF_lowerleg() ),
    RF_hipassembly_I(inertiaProps->getTensor_RF_hipassembly() ),
    RF_upperleg_I(inertiaProps->getTensor_RF_upperleg() ),
    RF_lowerleg_I(inertiaProps->getTensor_RF_lowerleg() ),
    LH_hipassembly_I(inertiaProps->getTensor_LH_hipassembly() ),
    LH_upperleg_I(inertiaProps->getTensor_LH_upperleg() ),
    LH_lowerleg_I(inertiaProps->getTensor_LH_lowerleg() ),
    RH_hipassembly_I(inertiaProps->getTensor_RH_hipassembly() ),
    RH_upperleg_I(inertiaProps->getTensor_RH_upperleg() ),
    RH_lowerleg_I(inertiaProps->getTensor_RH_lowerleg() )
    ,
        trunk_I( inertiaProps->getTensor_trunk() ),
        LF_lowerleg_Ic(LF_lowerleg_I),
        RF_lowerleg_Ic(RF_lowerleg_I),
        LH_lowerleg_Ic(LH_lowerleg_I),
        RH_lowerleg_Ic(RH_lowerleg_I)
{
#ifndef EIGEN_NO_DEBUG
    if(warning_printed == false)
    {
      warning_printed = true;
      std::cout << "Robot HyQ, InverseDynamics::InverseDynamics()" << std::endl;
      std::cout << "Compiled with Eigen debug active" << std::endl;
    }
#endif
    LF_hipassembly_v.setZero();
    LF_upperleg_v.setZero();
    LF_lowerleg_v.setZero();
    RF_hipassembly_v.setZero();
    RF_upperleg_v.setZero();
    RF_lowerleg_v.setZero();
    LH_hipassembly_v.setZero();
    LH_upperleg_v.setZero();
    LH_lowerleg_v.setZero();
    RH_hipassembly_v.setZero();
    RH_upperleg_v.setZero();
    RH_lowerleg_v.setZero();
}

iit::HyQ::dyn::InverseDynamics::Velocity iit::HyQ::dyn::InverseDynamics::getLinkVelocity(const JointState &q,
                                                                                         const JointState &qd,
                                                                                         const JointState &qdd,
                                                                                         const Acceleration &gravity_acceleration,
                                                                                         const Velocity &trunk_velocity,
                                                                                         const LinkIdentifiers &link_id)
{
    setJointStatus(q);
    propagateVelAcc(gravity_acceleration,trunk_velocity,qd,qdd);

    switch (link_id) {
    case LF_HIPASSEMBLY:
        return this->getVelocity_LF_hipassembly();
    case RF_HIPASSEMBLY:
        return this->getVelocity_RF_hipassembly();
    case LH_HIPASSEMBLY:
        return this->getVelocity_LH_hipassembly();
    case RH_HIPASSEMBLY:
        return this->getVelocity_RH_hipassembly();
    case LF_UPPERLEG:
        return this->getVelocity_LF_upperleg();
    case RF_UPPERLEG:
        return this->getVelocity_RF_upperleg();
    case LH_UPPERLEG:
        return this->getVelocity_LH_upperleg();
    case RH_UPPERLEG:
        return this->getVelocity_RH_upperleg();
    case LF_LOWERLEG:
        return this->getVelocity_LF_lowerleg();
    case RF_LOWERLEG:
        return this->getVelocity_RF_lowerleg();
    case LH_LOWERLEG:
        return this->getVelocity_LH_lowerleg();
    case RH_LOWERLEG:
        return this->getVelocity_RH_lowerleg();
    default:
        std::cerr<<"getLinkVelocity ERROR: spatial velocity not available for linkid : " << link_id << std::endl;
        break;
    }
}

iit::HyQ::dyn::InverseDynamics::Acceleration iit::HyQ::dyn::InverseDynamics::getLinkAcceleration(const JointState &q,
                                                                                                 const JointState &qd,
                                                                                                 const JointState &qdd,
                                                                                                 const Acceleration &gravity_acceleration,
                                                                                                 const Velocity &trunk_velocity,
                                                                                                 const LinkIdentifiers &link_id)
{
    setJointStatus(q);
    propagateVelAcc(gravity_acceleration,trunk_velocity,qd,qdd);

    switch (link_id) {
    case LF_HIPASSEMBLY:
        return this->getAcceleration_LF_hipassembly();
    case RF_HIPASSEMBLY:
        return this->getAcceleration_RF_hipassembly();
    case LH_HIPASSEMBLY:
        return this->getAcceleration_LH_hipassembly();
    case RH_HIPASSEMBLY:
        return this->getAcceleration_RH_hipassembly();
    case LF_UPPERLEG:
        return this->getAcceleration_LF_upperleg();
    case RF_UPPERLEG:
        return this->getAcceleration_RF_upperleg();
    case LH_UPPERLEG:
        return this->getAcceleration_LH_upperleg();
    case RH_UPPERLEG:
        return this->getAcceleration_RH_upperleg();
    case LF_LOWERLEG:
        return this->getAcceleration_LF_lowerleg();
    case RF_LOWERLEG:
        return this->getAcceleration_RF_lowerleg();
    case LH_LOWERLEG:
        return this->getAcceleration_LH_lowerleg();
    case RH_LOWERLEG:
        return this->getAcceleration_RH_lowerleg();
    default:
        std::cerr<<"getLinkAcceleration ERROR: spatial acceleration not available for linkid : " << link_id << std::endl;
        break;
    }
}


void iit::HyQ::dyn::InverseDynamics::propagateVelAcc(
    const Acceleration& g, const Velocity& trunk_v,
    const JointState& qd, const JointState& qdd)
{

	// First pass, link 'LF_hipassembly'
	    LF_hipassembly_v = ((xm->fr_LF_hipassembly_X_fr_trunk) * trunk_v);
	    LF_hipassembly_v(iit::rbd::AZ) += qd(LF_HAA);

	    Utils::fillAsMotionCrossProductMx(LF_hipassembly_v, spareMx);

	    LF_hipassembly_a = (spareMx.col(iit::rbd::AZ) * qd(LF_HAA));
	    LF_hipassembly_a(iit::rbd::AZ) += qdd(LF_HAA);

	    LF_hipassembly_f = LF_hipassembly_I * LF_hipassembly_a + (-spareMx.transpose() * LF_hipassembly_I * LF_hipassembly_v);

	    // First pass, link 'LF_upperleg'
	    LF_upperleg_v = ((xm->fr_LF_upperleg_X_fr_LF_hipassembly) * LF_hipassembly_v);
	    LF_upperleg_v(iit::rbd::AZ) += qd(LF_HFE);

	    Utils::fillAsMotionCrossProductMx(LF_upperleg_v, spareMx);

	    LF_upperleg_a = ((xm->fr_LF_upperleg_X_fr_LF_hipassembly) * LF_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(LF_HFE));
	    LF_upperleg_a(iit::rbd::AZ) += qdd(LF_HFE);

	    LF_upperleg_f = LF_upperleg_I * LF_upperleg_a + (-spareMx.transpose() * LF_upperleg_I * LF_upperleg_v);

	    // First pass, link 'LF_lowerleg'
	    LF_lowerleg_v = ((xm->fr_LF_lowerleg_X_fr_LF_upperleg) * LF_upperleg_v);
	    LF_lowerleg_v(iit::rbd::AZ) += qd(LF_KFE);

	    Utils::fillAsMotionCrossProductMx(LF_lowerleg_v, spareMx);

	    LF_lowerleg_a = ((xm->fr_LF_lowerleg_X_fr_LF_upperleg) * LF_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(LF_KFE));
	    LF_lowerleg_a(iit::rbd::AZ) += qdd(LF_KFE);

	    LF_lowerleg_f = LF_lowerleg_I * LF_lowerleg_a + (-spareMx.transpose() * LF_lowerleg_I * LF_lowerleg_v);

	    // First pass, link 'RF_hipassembly'
	    RF_hipassembly_v = ((xm->fr_RF_hipassembly_X_fr_trunk) * trunk_v);
	    RF_hipassembly_v(iit::rbd::AZ) += qd(RF_HAA);

	    Utils::fillAsMotionCrossProductMx(RF_hipassembly_v, spareMx);

	    RF_hipassembly_a = (spareMx.col(iit::rbd::AZ) * qd(RF_HAA));
	    RF_hipassembly_a(iit::rbd::AZ) += qdd(RF_HAA);

	    RF_hipassembly_f = RF_hipassembly_I * RF_hipassembly_a + (-spareMx.transpose() * RF_hipassembly_I * RF_hipassembly_v);

	    // First pass, link 'RF_upperleg'
	    RF_upperleg_v = ((xm->fr_RF_upperleg_X_fr_RF_hipassembly) * RF_hipassembly_v);
	    RF_upperleg_v(iit::rbd::AZ) += qd(RF_HFE);

	    Utils::fillAsMotionCrossProductMx(RF_upperleg_v, spareMx);

	    RF_upperleg_a = ((xm->fr_RF_upperleg_X_fr_RF_hipassembly) * RF_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(RF_HFE));
	    RF_upperleg_a(iit::rbd::AZ) += qdd(RF_HFE);

	    RF_upperleg_f = RF_upperleg_I * RF_upperleg_a + (-spareMx.transpose() * RF_upperleg_I * RF_upperleg_v);

	    // First pass, link 'RF_lowerleg'
	    RF_lowerleg_v = ((xm->fr_RF_lowerleg_X_fr_RF_upperleg) * RF_upperleg_v);
	    RF_lowerleg_v(iit::rbd::AZ) += qd(RF_KFE);

	    Utils::fillAsMotionCrossProductMx(RF_lowerleg_v, spareMx);

	    RF_lowerleg_a = ((xm->fr_RF_lowerleg_X_fr_RF_upperleg) * RF_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(RF_KFE));
	    RF_lowerleg_a(iit::rbd::AZ) += qdd(RF_KFE);

	    RF_lowerleg_f = RF_lowerleg_I * RF_lowerleg_a + (-spareMx.transpose() * RF_lowerleg_I * RF_lowerleg_v);

	    // First pass, link 'LH_hipassembly'
	    LH_hipassembly_v = ((xm->fr_LH_hipassembly_X_fr_trunk) * trunk_v);
	    LH_hipassembly_v(iit::rbd::AZ) += qd(LH_HAA);

	    Utils::fillAsMotionCrossProductMx(LH_hipassembly_v, spareMx);

	    LH_hipassembly_a = (spareMx.col(iit::rbd::AZ) * qd(LH_HAA));
	    LH_hipassembly_a(iit::rbd::AZ) += qdd(LH_HAA);

	    LH_hipassembly_f = LH_hipassembly_I * LH_hipassembly_a + (-spareMx.transpose() * LH_hipassembly_I * LH_hipassembly_v);

	    // First pass, link 'LH_upperleg'
	    LH_upperleg_v = ((xm->fr_LH_upperleg_X_fr_LH_hipassembly) * LH_hipassembly_v);
	    LH_upperleg_v(iit::rbd::AZ) += qd(LH_HFE);

	    Utils::fillAsMotionCrossProductMx(LH_upperleg_v, spareMx);

	    LH_upperleg_a = ((xm->fr_LH_upperleg_X_fr_LH_hipassembly) * LH_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(LH_HFE));
	    LH_upperleg_a(iit::rbd::AZ) += qdd(LH_HFE);

	    LH_upperleg_f = LH_upperleg_I * LH_upperleg_a + (-spareMx.transpose() * LH_upperleg_I * LH_upperleg_v);

	    // First pass, link 'LH_lowerleg'
	    LH_lowerleg_v = ((xm->fr_LH_lowerleg_X_fr_LH_upperleg) * LH_upperleg_v);
	    LH_lowerleg_v(iit::rbd::AZ) += qd(LH_KFE);

	    Utils::fillAsMotionCrossProductMx(LH_lowerleg_v, spareMx);

	    LH_lowerleg_a = ((xm->fr_LH_lowerleg_X_fr_LH_upperleg) * LH_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(LH_KFE));
	    LH_lowerleg_a(iit::rbd::AZ) += qdd(LH_KFE);

	    LH_lowerleg_f = LH_lowerleg_I * LH_lowerleg_a + (-spareMx.transpose() * LH_lowerleg_I * LH_lowerleg_v);

	    // First pass, link 'RH_hipassembly'
	    RH_hipassembly_v = ((xm->fr_RH_hipassembly_X_fr_trunk) * trunk_v);
	    RH_hipassembly_v(iit::rbd::AZ) += qd(RH_HAA);

	    Utils::fillAsMotionCrossProductMx(RH_hipassembly_v, spareMx);

	    RH_hipassembly_a = (spareMx.col(iit::rbd::AZ) * qd(RH_HAA));
	    RH_hipassembly_a(iit::rbd::AZ) += qdd(RH_HAA);

	    RH_hipassembly_f = RH_hipassembly_I * RH_hipassembly_a + (-spareMx.transpose() * RH_hipassembly_I * RH_hipassembly_v);

	    // First pass, link 'RH_upperleg'
	    RH_upperleg_v = ((xm->fr_RH_upperleg_X_fr_RH_hipassembly) * RH_hipassembly_v);
	    RH_upperleg_v(iit::rbd::AZ) += qd(RH_HFE);

	    Utils::fillAsMotionCrossProductMx(RH_upperleg_v, spareMx);

	    RH_upperleg_a = ((xm->fr_RH_upperleg_X_fr_RH_hipassembly) * RH_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(RH_HFE));
	    RH_upperleg_a(iit::rbd::AZ) += qdd(RH_HFE);

	    RH_upperleg_f = RH_upperleg_I * RH_upperleg_a + (-spareMx.transpose() * RH_upperleg_I * RH_upperleg_v);

	    // First pass, link 'RH_lowerleg'
	    RH_lowerleg_v = ((xm->fr_RH_lowerleg_X_fr_RH_upperleg) * RH_upperleg_v);
	    RH_lowerleg_v(iit::rbd::AZ) += qd(RH_KFE);

	    Utils::fillAsMotionCrossProductMx(RH_lowerleg_v, spareMx);

	    RH_lowerleg_a = ((xm->fr_RH_lowerleg_X_fr_RH_upperleg) * RH_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(RH_KFE));
	    RH_lowerleg_a(iit::rbd::AZ) += qdd(RH_KFE);

	    RH_lowerleg_f = RH_lowerleg_I * RH_lowerleg_a + (-spareMx.transpose() * RH_lowerleg_I * RH_lowerleg_v);

}

void iit::HyQ::dyn::InverseDynamics::id(
    JointState& jForces, Acceleration& trunk_a,
    const Acceleration& g, const Velocity& trunk_v,
    const JointState& qd, const JointState& qdd,
    const ExtForces& fext)
{
    trunk_Ic = trunk_I;
    LF_hipassembly_Ic = LF_hipassembly_I;
    LF_upperleg_Ic = LF_upperleg_I;
    RF_hipassembly_Ic = RF_hipassembly_I;
    RF_upperleg_Ic = RF_upperleg_I;
    LH_hipassembly_Ic = LH_hipassembly_I;
    LH_upperleg_Ic = LH_upperleg_I;
    RH_hipassembly_Ic = RH_hipassembly_I;
    RH_upperleg_Ic = RH_upperleg_I;

    propagateVelAcc(g, trunk_v, qd, qdd);
    
    // The force exerted on the floating base by the links
    Utils::fillAsMotionCrossProductMx(trunk_v, spareMx);
    trunk_f = -spareMx.transpose() * trunk_I * trunk_v;
    

    // Add the external forces:
    trunk_f -= fext[TRUNK];
    LF_hipassembly_f -= fext[LF_HIPASSEMBLY];
    LF_upperleg_f -= fext[LF_UPPERLEG];
    LF_lowerleg_f -= fext[LF_LOWERLEG];
    RF_hipassembly_f -= fext[RF_HIPASSEMBLY];
    RF_upperleg_f -= fext[RF_UPPERLEG];
    RF_lowerleg_f -= fext[RF_LOWERLEG];
    LH_hipassembly_f -= fext[LH_HIPASSEMBLY];
    LH_upperleg_f -= fext[LH_UPPERLEG];
    LH_lowerleg_f -= fext[LH_LOWERLEG];
    RH_hipassembly_f -= fext[RH_HIPASSEMBLY];
    RH_upperleg_f -= fext[RH_UPPERLEG];
    RH_lowerleg_f -= fext[RH_LOWERLEG];

    RH_upperleg_Ic = RH_upperleg_Ic + (xm->fr_RH_lowerleg_X_fr_RH_upperleg).transpose() * RH_lowerleg_Ic * (xm->fr_RH_lowerleg_X_fr_RH_upperleg);
    RH_upperleg_f = RH_upperleg_f + (xm->fr_RH_lowerleg_X_fr_RH_upperleg).transpose() * RH_lowerleg_f;
    
    RH_hipassembly_Ic = RH_hipassembly_Ic + (xm->fr_RH_upperleg_X_fr_RH_hipassembly).transpose() * RH_upperleg_Ic * (xm->fr_RH_upperleg_X_fr_RH_hipassembly);
    RH_hipassembly_f = RH_hipassembly_f + (xm->fr_RH_upperleg_X_fr_RH_hipassembly).transpose() * RH_upperleg_f;
    
    trunk_Ic = trunk_Ic + (xm->fr_RH_hipassembly_X_fr_trunk).transpose() * RH_hipassembly_Ic * (xm->fr_RH_hipassembly_X_fr_trunk);
    trunk_f = trunk_f + (xm->fr_RH_hipassembly_X_fr_trunk).transpose() * RH_hipassembly_f;
    
    LH_upperleg_Ic = LH_upperleg_Ic + (xm->fr_LH_lowerleg_X_fr_LH_upperleg).transpose() * LH_lowerleg_Ic * (xm->fr_LH_lowerleg_X_fr_LH_upperleg);
    LH_upperleg_f = LH_upperleg_f + (xm->fr_LH_lowerleg_X_fr_LH_upperleg).transpose() * LH_lowerleg_f;
    
    LH_hipassembly_Ic = LH_hipassembly_Ic + (xm->fr_LH_upperleg_X_fr_LH_hipassembly).transpose() * LH_upperleg_Ic * (xm->fr_LH_upperleg_X_fr_LH_hipassembly);
    LH_hipassembly_f = LH_hipassembly_f + (xm->fr_LH_upperleg_X_fr_LH_hipassembly).transpose() * LH_upperleg_f;
    
    trunk_Ic = trunk_Ic + (xm->fr_LH_hipassembly_X_fr_trunk).transpose() * LH_hipassembly_Ic * (xm->fr_LH_hipassembly_X_fr_trunk);
    trunk_f = trunk_f + (xm->fr_LH_hipassembly_X_fr_trunk).transpose() * LH_hipassembly_f;
    
    RF_upperleg_Ic = RF_upperleg_Ic + (xm->fr_RF_lowerleg_X_fr_RF_upperleg).transpose() * RF_lowerleg_Ic * (xm->fr_RF_lowerleg_X_fr_RF_upperleg);
    RF_upperleg_f = RF_upperleg_f + (xm->fr_RF_lowerleg_X_fr_RF_upperleg).transpose() * RF_lowerleg_f;
    
    RF_hipassembly_Ic = RF_hipassembly_Ic + (xm->fr_RF_upperleg_X_fr_RF_hipassembly).transpose() * RF_upperleg_Ic * (xm->fr_RF_upperleg_X_fr_RF_hipassembly);
    RF_hipassembly_f = RF_hipassembly_f + (xm->fr_RF_upperleg_X_fr_RF_hipassembly).transpose() * RF_upperleg_f;
    
    trunk_Ic = trunk_Ic + (xm->fr_RF_hipassembly_X_fr_trunk).transpose() * RF_hipassembly_Ic * (xm->fr_RF_hipassembly_X_fr_trunk);
    trunk_f = trunk_f + (xm->fr_RF_hipassembly_X_fr_trunk).transpose() * RF_hipassembly_f;
    
    LF_upperleg_Ic = LF_upperleg_Ic + (xm->fr_LF_lowerleg_X_fr_LF_upperleg).transpose() * LF_lowerleg_Ic * (xm->fr_LF_lowerleg_X_fr_LF_upperleg);
    LF_upperleg_f = LF_upperleg_f + (xm->fr_LF_lowerleg_X_fr_LF_upperleg).transpose() * LF_lowerleg_f;
    
    LF_hipassembly_Ic = LF_hipassembly_Ic + (xm->fr_LF_upperleg_X_fr_LF_hipassembly).transpose() * LF_upperleg_Ic * (xm->fr_LF_upperleg_X_fr_LF_hipassembly);
    LF_hipassembly_f = LF_hipassembly_f + (xm->fr_LF_upperleg_X_fr_LF_hipassembly).transpose() * LF_upperleg_f;
    
    trunk_Ic = trunk_Ic + (xm->fr_LF_hipassembly_X_fr_trunk).transpose() * LF_hipassembly_Ic * (xm->fr_LF_hipassembly_X_fr_trunk);
    trunk_f = trunk_f + (xm->fr_LF_hipassembly_X_fr_trunk).transpose() * LF_hipassembly_f;
    

    // The base acceleration due to the force due to the movement of the links
    trunk_a = - trunk_Ic.inverse() * trunk_f;
    
    LF_hipassembly_a = xm->fr_LF_hipassembly_X_fr_trunk * trunk_a;
    jForces(LF_HAA) = (LF_hipassembly_Ic.row(iit::rbd::AZ) * LF_hipassembly_a + LF_hipassembly_f(iit::rbd::AZ));
    
    LF_upperleg_a = xm->fr_LF_upperleg_X_fr_LF_hipassembly * LF_hipassembly_a;
    jForces(LF_HFE) = (LF_upperleg_Ic.row(iit::rbd::AZ) * LF_upperleg_a + LF_upperleg_f(iit::rbd::AZ));
    
    LF_lowerleg_a = xm->fr_LF_lowerleg_X_fr_LF_upperleg * LF_upperleg_a;
    jForces(LF_KFE) = (LF_lowerleg_Ic.row(iit::rbd::AZ) * LF_lowerleg_a + LF_lowerleg_f(iit::rbd::AZ));
    
    RF_hipassembly_a = xm->fr_RF_hipassembly_X_fr_trunk * trunk_a;
    jForces(RF_HAA) = (RF_hipassembly_Ic.row(iit::rbd::AZ) * RF_hipassembly_a + RF_hipassembly_f(iit::rbd::AZ));
    
    RF_upperleg_a = xm->fr_RF_upperleg_X_fr_RF_hipassembly * RF_hipassembly_a;
    jForces(RF_HFE) = (RF_upperleg_Ic.row(iit::rbd::AZ) * RF_upperleg_a + RF_upperleg_f(iit::rbd::AZ));
    
    RF_lowerleg_a = xm->fr_RF_lowerleg_X_fr_RF_upperleg * RF_upperleg_a;
    jForces(RF_KFE) = (RF_lowerleg_Ic.row(iit::rbd::AZ) * RF_lowerleg_a + RF_lowerleg_f(iit::rbd::AZ));
    
    LH_hipassembly_a = xm->fr_LH_hipassembly_X_fr_trunk * trunk_a;
    jForces(LH_HAA) = (LH_hipassembly_Ic.row(iit::rbd::AZ) * LH_hipassembly_a + LH_hipassembly_f(iit::rbd::AZ));
    
    LH_upperleg_a = xm->fr_LH_upperleg_X_fr_LH_hipassembly * LH_hipassembly_a;
    jForces(LH_HFE) = (LH_upperleg_Ic.row(iit::rbd::AZ) * LH_upperleg_a + LH_upperleg_f(iit::rbd::AZ));
    
    LH_lowerleg_a = xm->fr_LH_lowerleg_X_fr_LH_upperleg * LH_upperleg_a;
    jForces(LH_KFE) = (LH_lowerleg_Ic.row(iit::rbd::AZ) * LH_lowerleg_a + LH_lowerleg_f(iit::rbd::AZ));
    
    RH_hipassembly_a = xm->fr_RH_hipassembly_X_fr_trunk * trunk_a;
    jForces(RH_HAA) = (RH_hipassembly_Ic.row(iit::rbd::AZ) * RH_hipassembly_a + RH_hipassembly_f(iit::rbd::AZ));
    
    RH_upperleg_a = xm->fr_RH_upperleg_X_fr_RH_hipassembly * RH_hipassembly_a;
    jForces(RH_HFE) = (RH_upperleg_Ic.row(iit::rbd::AZ) * RH_upperleg_a + RH_upperleg_f(iit::rbd::AZ));
    
    RH_lowerleg_a = xm->fr_RH_lowerleg_X_fr_RH_upperleg * RH_upperleg_a;
    jForces(RH_KFE) = (RH_lowerleg_Ic.row(iit::rbd::AZ) * RH_lowerleg_a + RH_lowerleg_f(iit::rbd::AZ));
    

    trunk_a += g;
}


void iit::HyQ::dyn::InverseDynamics::G_terms_fully_actuated(
    Force& baseWrench, JointState& jForces,
    const Acceleration& g)
{
    const Acceleration& trunk_a = -g;

    // Link 'LF_hipassembly'
    LF_hipassembly_a = (xm->fr_LF_hipassembly_X_fr_trunk) * trunk_a;
    LF_hipassembly_f = LF_hipassembly_I * LF_hipassembly_a;
    // Link 'LF_upperleg'
    LF_upperleg_a = (xm->fr_LF_upperleg_X_fr_LF_hipassembly) * LF_hipassembly_a;
    LF_upperleg_f = LF_upperleg_I * LF_upperleg_a;
    // Link 'LF_lowerleg'
    LF_lowerleg_a = (xm->fr_LF_lowerleg_X_fr_LF_upperleg) * LF_upperleg_a;
    LF_lowerleg_f = LF_lowerleg_I * LF_lowerleg_a;
    // Link 'RF_hipassembly'
    RF_hipassembly_a = (xm->fr_RF_hipassembly_X_fr_trunk) * trunk_a;
    RF_hipassembly_f = RF_hipassembly_I * RF_hipassembly_a;
    // Link 'RF_upperleg'
    RF_upperleg_a = (xm->fr_RF_upperleg_X_fr_RF_hipassembly) * RF_hipassembly_a;
    RF_upperleg_f = RF_upperleg_I * RF_upperleg_a;
    // Link 'RF_lowerleg'
    RF_lowerleg_a = (xm->fr_RF_lowerleg_X_fr_RF_upperleg) * RF_upperleg_a;
    RF_lowerleg_f = RF_lowerleg_I * RF_lowerleg_a;
    // Link 'LH_hipassembly'
    LH_hipassembly_a = (xm->fr_LH_hipassembly_X_fr_trunk) * trunk_a;
    LH_hipassembly_f = LH_hipassembly_I * LH_hipassembly_a;
    // Link 'LH_upperleg'
    LH_upperleg_a = (xm->fr_LH_upperleg_X_fr_LH_hipassembly) * LH_hipassembly_a;
    LH_upperleg_f = LH_upperleg_I * LH_upperleg_a;
    // Link 'LH_lowerleg'
    LH_lowerleg_a = (xm->fr_LH_lowerleg_X_fr_LH_upperleg) * LH_upperleg_a;
    LH_lowerleg_f = LH_lowerleg_I * LH_lowerleg_a;
    // Link 'RH_hipassembly'
    RH_hipassembly_a = (xm->fr_RH_hipassembly_X_fr_trunk) * trunk_a;
    RH_hipassembly_f = RH_hipassembly_I * RH_hipassembly_a;
    // Link 'RH_upperleg'
    RH_upperleg_a = (xm->fr_RH_upperleg_X_fr_RH_hipassembly) * RH_hipassembly_a;
    RH_upperleg_f = RH_upperleg_I * RH_upperleg_a;
    // Link 'RH_lowerleg'
    RH_lowerleg_a = (xm->fr_RH_lowerleg_X_fr_RH_upperleg) * RH_upperleg_a;
    RH_lowerleg_f = RH_lowerleg_I * RH_lowerleg_a;

    trunk_f = trunk_I * trunk_a;

    secondPass_fullyActuated(jForces);

    baseWrench = trunk_f;
}

void iit::HyQ::dyn::InverseDynamics::C_terms_fully_actuated(
    Force& baseWrench, JointState& jForces,
    const Velocity& trunk_v, const JointState& qd)
{
    // Link 'LF_hipassembly'
    LF_hipassembly_v = ((xm->fr_LF_hipassembly_X_fr_trunk) * trunk_v);
    LF_hipassembly_v(iit::rbd::AZ) += qd(LF_HAA);
    Utils::fillAsMotionCrossProductMx(LF_hipassembly_v, spareMx);
    LF_hipassembly_a = (spareMx.col(iit::rbd::AZ) * qd(LF_HAA));
    LF_hipassembly_f = LF_hipassembly_I * LF_hipassembly_a + (-spareMx.transpose() * LF_hipassembly_I * LF_hipassembly_v);
    
    // Link 'LF_upperleg'
    LF_upperleg_v = ((xm->fr_LF_upperleg_X_fr_LF_hipassembly) * LF_hipassembly_v);
    LF_upperleg_v(iit::rbd::AZ) += qd(LF_HFE);
    Utils::fillAsMotionCrossProductMx(LF_upperleg_v, spareMx);
    LF_upperleg_a = ((xm->fr_LF_upperleg_X_fr_LF_hipassembly) * LF_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(LF_HFE));
    LF_upperleg_f = LF_upperleg_I * LF_upperleg_a + (-spareMx.transpose() * LF_upperleg_I * LF_upperleg_v);
    
    // Link 'LF_lowerleg'
    LF_lowerleg_v = ((xm->fr_LF_lowerleg_X_fr_LF_upperleg) * LF_upperleg_v);
    LF_lowerleg_v(iit::rbd::AZ) += qd(LF_KFE);
    Utils::fillAsMotionCrossProductMx(LF_lowerleg_v, spareMx);
    LF_lowerleg_a = ((xm->fr_LF_lowerleg_X_fr_LF_upperleg) * LF_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(LF_KFE));
    LF_lowerleg_f = LF_lowerleg_I * LF_lowerleg_a + (-spareMx.transpose() * LF_lowerleg_I * LF_lowerleg_v);
    
    // Link 'RF_hipassembly'
    RF_hipassembly_v = ((xm->fr_RF_hipassembly_X_fr_trunk) * trunk_v);
    RF_hipassembly_v(iit::rbd::AZ) += qd(RF_HAA);
    Utils::fillAsMotionCrossProductMx(RF_hipassembly_v, spareMx);
    RF_hipassembly_a = (spareMx.col(iit::rbd::AZ) * qd(RF_HAA));
    RF_hipassembly_f = RF_hipassembly_I * RF_hipassembly_a + (-spareMx.transpose() * RF_hipassembly_I * RF_hipassembly_v);
    
    // Link 'RF_upperleg'
    RF_upperleg_v = ((xm->fr_RF_upperleg_X_fr_RF_hipassembly) * RF_hipassembly_v);
    RF_upperleg_v(iit::rbd::AZ) += qd(RF_HFE);
    Utils::fillAsMotionCrossProductMx(RF_upperleg_v, spareMx);
    RF_upperleg_a = ((xm->fr_RF_upperleg_X_fr_RF_hipassembly) * RF_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(RF_HFE));
    RF_upperleg_f = RF_upperleg_I * RF_upperleg_a + (-spareMx.transpose() * RF_upperleg_I * RF_upperleg_v);
    
    // Link 'RF_lowerleg'
    RF_lowerleg_v = ((xm->fr_RF_lowerleg_X_fr_RF_upperleg) * RF_upperleg_v);
    RF_lowerleg_v(iit::rbd::AZ) += qd(RF_KFE);
    Utils::fillAsMotionCrossProductMx(RF_lowerleg_v, spareMx);
    RF_lowerleg_a = ((xm->fr_RF_lowerleg_X_fr_RF_upperleg) * RF_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(RF_KFE));
    RF_lowerleg_f = RF_lowerleg_I * RF_lowerleg_a + (-spareMx.transpose() * RF_lowerleg_I * RF_lowerleg_v);
    
    // Link 'LH_hipassembly'
    LH_hipassembly_v = ((xm->fr_LH_hipassembly_X_fr_trunk) * trunk_v);
    LH_hipassembly_v(iit::rbd::AZ) += qd(LH_HAA);
    Utils::fillAsMotionCrossProductMx(LH_hipassembly_v, spareMx);
    LH_hipassembly_a = (spareMx.col(iit::rbd::AZ) * qd(LH_HAA));
    LH_hipassembly_f = LH_hipassembly_I * LH_hipassembly_a + (-spareMx.transpose() * LH_hipassembly_I * LH_hipassembly_v);
    
    // Link 'LH_upperleg'
    LH_upperleg_v = ((xm->fr_LH_upperleg_X_fr_LH_hipassembly) * LH_hipassembly_v);
    LH_upperleg_v(iit::rbd::AZ) += qd(LH_HFE);
    Utils::fillAsMotionCrossProductMx(LH_upperleg_v, spareMx);
    LH_upperleg_a = ((xm->fr_LH_upperleg_X_fr_LH_hipassembly) * LH_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(LH_HFE));
    LH_upperleg_f = LH_upperleg_I * LH_upperleg_a + (-spareMx.transpose() * LH_upperleg_I * LH_upperleg_v);
    
    // Link 'LH_lowerleg'
    LH_lowerleg_v = ((xm->fr_LH_lowerleg_X_fr_LH_upperleg) * LH_upperleg_v);
    LH_lowerleg_v(iit::rbd::AZ) += qd(LH_KFE);
    Utils::fillAsMotionCrossProductMx(LH_lowerleg_v, spareMx);
    LH_lowerleg_a = ((xm->fr_LH_lowerleg_X_fr_LH_upperleg) * LH_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(LH_KFE));
    LH_lowerleg_f = LH_lowerleg_I * LH_lowerleg_a + (-spareMx.transpose() * LH_lowerleg_I * LH_lowerleg_v);
    
    // Link 'RH_hipassembly'
    RH_hipassembly_v = ((xm->fr_RH_hipassembly_X_fr_trunk) * trunk_v);
    RH_hipassembly_v(iit::rbd::AZ) += qd(RH_HAA);
    Utils::fillAsMotionCrossProductMx(RH_hipassembly_v, spareMx);
    RH_hipassembly_a = (spareMx.col(iit::rbd::AZ) * qd(RH_HAA));
    RH_hipassembly_f = RH_hipassembly_I * RH_hipassembly_a + (-spareMx.transpose() * RH_hipassembly_I * RH_hipassembly_v);
    
    // Link 'RH_upperleg'
    RH_upperleg_v = ((xm->fr_RH_upperleg_X_fr_RH_hipassembly) * RH_hipassembly_v);
    RH_upperleg_v(iit::rbd::AZ) += qd(RH_HFE);
    Utils::fillAsMotionCrossProductMx(RH_upperleg_v, spareMx);
    RH_upperleg_a = ((xm->fr_RH_upperleg_X_fr_RH_hipassembly) * RH_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(RH_HFE));
    RH_upperleg_f = RH_upperleg_I * RH_upperleg_a + (-spareMx.transpose() * RH_upperleg_I * RH_upperleg_v);
    
    // Link 'RH_lowerleg'
    RH_lowerleg_v = ((xm->fr_RH_lowerleg_X_fr_RH_upperleg) * RH_upperleg_v);
    RH_lowerleg_v(iit::rbd::AZ) += qd(RH_KFE);
    Utils::fillAsMotionCrossProductMx(RH_lowerleg_v, spareMx);
    RH_lowerleg_a = ((xm->fr_RH_lowerleg_X_fr_RH_upperleg) * RH_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(RH_KFE));
    RH_lowerleg_f = RH_lowerleg_I * RH_lowerleg_a + (-spareMx.transpose() * RH_lowerleg_I * RH_lowerleg_v);
    

    Utils::fillAsForceCrossProductMx(trunk_v, spareMx);
    trunk_f = spareMx * trunk_I * trunk_v;

    secondPass_fullyActuated(jForces);

    baseWrench = trunk_f;
}

void iit::HyQ::dyn::InverseDynamics::id_fully_actuated(
        Force& baseWrench, JointState& jForces,
        const Acceleration& g, const Velocity& trunk_v, const Acceleration& baseAccel,
        const JointState& qd, const JointState& qdd, const ExtForces& fext)
{
    Acceleration trunk_a = baseAccel -g;

    // First pass, link 'LF_hipassembly'
    LF_hipassembly_v = ((xm->fr_LF_hipassembly_X_fr_trunk) * trunk_v);
    LF_hipassembly_v(iit::rbd::AZ) += qd(LF_HAA);
    
    Utils::fillAsMotionCrossProductMx(LF_hipassembly_v, spareMx);
    
    LF_hipassembly_a = ((xm->fr_LF_hipassembly_X_fr_trunk) * trunk_a) + (spareMx.col(iit::rbd::AZ) * qd(LF_HAA));
    LF_hipassembly_a(iit::rbd::AZ) += qdd(LF_HAA);
    
    LF_hipassembly_f = LF_hipassembly_I * LF_hipassembly_a + (-spareMx.transpose() * LF_hipassembly_I * LF_hipassembly_v);
    
    // First pass, link 'LF_upperleg'
    LF_upperleg_v = ((xm->fr_LF_upperleg_X_fr_LF_hipassembly) * LF_hipassembly_v);
    LF_upperleg_v(iit::rbd::AZ) += qd(LF_HFE);
    
    Utils::fillAsMotionCrossProductMx(LF_upperleg_v, spareMx);
    
    LF_upperleg_a = ((xm->fr_LF_upperleg_X_fr_LF_hipassembly) * LF_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(LF_HFE));
    LF_upperleg_a(iit::rbd::AZ) += qdd(LF_HFE);
    
    LF_upperleg_f = LF_upperleg_I * LF_upperleg_a + (-spareMx.transpose() * LF_upperleg_I * LF_upperleg_v);
    
    // First pass, link 'LF_lowerleg'
    LF_lowerleg_v = ((xm->fr_LF_lowerleg_X_fr_LF_upperleg) * LF_upperleg_v);
    LF_lowerleg_v(iit::rbd::AZ) += qd(LF_KFE);
    
    Utils::fillAsMotionCrossProductMx(LF_lowerleg_v, spareMx);
    
    LF_lowerleg_a = ((xm->fr_LF_lowerleg_X_fr_LF_upperleg) * LF_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(LF_KFE));
    LF_lowerleg_a(iit::rbd::AZ) += qdd(LF_KFE);
    
    LF_lowerleg_f = LF_lowerleg_I * LF_lowerleg_a + (-spareMx.transpose() * LF_lowerleg_I * LF_lowerleg_v);
    
    // First pass, link 'RF_hipassembly'
    RF_hipassembly_v = ((xm->fr_RF_hipassembly_X_fr_trunk) * trunk_v);
    RF_hipassembly_v(iit::rbd::AZ) += qd(RF_HAA);
    
    Utils::fillAsMotionCrossProductMx(RF_hipassembly_v, spareMx);
    
    RF_hipassembly_a = ((xm->fr_RF_hipassembly_X_fr_trunk) * trunk_a) + (spareMx.col(iit::rbd::AZ) * qd(RF_HAA));
    RF_hipassembly_a(iit::rbd::AZ) += qdd(RF_HAA);
    
    RF_hipassembly_f = RF_hipassembly_I * RF_hipassembly_a + (-spareMx.transpose() * RF_hipassembly_I * RF_hipassembly_v);
    
    // First pass, link 'RF_upperleg'
    RF_upperleg_v = ((xm->fr_RF_upperleg_X_fr_RF_hipassembly) * RF_hipassembly_v);
    RF_upperleg_v(iit::rbd::AZ) += qd(RF_HFE);
    
    Utils::fillAsMotionCrossProductMx(RF_upperleg_v, spareMx);
    
    RF_upperleg_a = ((xm->fr_RF_upperleg_X_fr_RF_hipassembly) * RF_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(RF_HFE));
    RF_upperleg_a(iit::rbd::AZ) += qdd(RF_HFE);
    
    RF_upperleg_f = RF_upperleg_I * RF_upperleg_a + (-spareMx.transpose() * RF_upperleg_I * RF_upperleg_v);
    
    // First pass, link 'RF_lowerleg'
    RF_lowerleg_v = ((xm->fr_RF_lowerleg_X_fr_RF_upperleg) * RF_upperleg_v);
    RF_lowerleg_v(iit::rbd::AZ) += qd(RF_KFE);
    
    Utils::fillAsMotionCrossProductMx(RF_lowerleg_v, spareMx);
    
    RF_lowerleg_a = ((xm->fr_RF_lowerleg_X_fr_RF_upperleg) * RF_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(RF_KFE));
    RF_lowerleg_a(iit::rbd::AZ) += qdd(RF_KFE);
    
    RF_lowerleg_f = RF_lowerleg_I * RF_lowerleg_a + (-spareMx.transpose() * RF_lowerleg_I * RF_lowerleg_v);
    
    // First pass, link 'LH_hipassembly'
    LH_hipassembly_v = ((xm->fr_LH_hipassembly_X_fr_trunk) * trunk_v);
    LH_hipassembly_v(iit::rbd::AZ) += qd(LH_HAA);
    
    Utils::fillAsMotionCrossProductMx(LH_hipassembly_v, spareMx);
    
    LH_hipassembly_a = ((xm->fr_LH_hipassembly_X_fr_trunk) * trunk_a) + (spareMx.col(iit::rbd::AZ) * qd(LH_HAA));
    LH_hipassembly_a(iit::rbd::AZ) += qdd(LH_HAA);
    
    LH_hipassembly_f = LH_hipassembly_I * LH_hipassembly_a + (-spareMx.transpose() * LH_hipassembly_I * LH_hipassembly_v);
    
    // First pass, link 'LH_upperleg'
    LH_upperleg_v = ((xm->fr_LH_upperleg_X_fr_LH_hipassembly) * LH_hipassembly_v);
    LH_upperleg_v(iit::rbd::AZ) += qd(LH_HFE);
    
    Utils::fillAsMotionCrossProductMx(LH_upperleg_v, spareMx);
    
    LH_upperleg_a = ((xm->fr_LH_upperleg_X_fr_LH_hipassembly) * LH_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(LH_HFE));
    LH_upperleg_a(iit::rbd::AZ) += qdd(LH_HFE);
    
    LH_upperleg_f = LH_upperleg_I * LH_upperleg_a + (-spareMx.transpose() * LH_upperleg_I * LH_upperleg_v);
    
    // First pass, link 'LH_lowerleg'
    LH_lowerleg_v = ((xm->fr_LH_lowerleg_X_fr_LH_upperleg) * LH_upperleg_v);
    LH_lowerleg_v(iit::rbd::AZ) += qd(LH_KFE);
    
    Utils::fillAsMotionCrossProductMx(LH_lowerleg_v, spareMx);
    
    LH_lowerleg_a = ((xm->fr_LH_lowerleg_X_fr_LH_upperleg) * LH_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(LH_KFE));
    LH_lowerleg_a(iit::rbd::AZ) += qdd(LH_KFE);
    
    LH_lowerleg_f = LH_lowerleg_I * LH_lowerleg_a + (-spareMx.transpose() * LH_lowerleg_I * LH_lowerleg_v);
    
    // First pass, link 'RH_hipassembly'
    RH_hipassembly_v = ((xm->fr_RH_hipassembly_X_fr_trunk) * trunk_v);
    RH_hipassembly_v(iit::rbd::AZ) += qd(RH_HAA);
    
    Utils::fillAsMotionCrossProductMx(RH_hipassembly_v, spareMx);
    
    RH_hipassembly_a = ((xm->fr_RH_hipassembly_X_fr_trunk) * trunk_a) + (spareMx.col(iit::rbd::AZ) * qd(RH_HAA));
    RH_hipassembly_a(iit::rbd::AZ) += qdd(RH_HAA);
    
    RH_hipassembly_f = RH_hipassembly_I * RH_hipassembly_a + (-spareMx.transpose() * RH_hipassembly_I * RH_hipassembly_v);
    
    // First pass, link 'RH_upperleg'
    RH_upperleg_v = ((xm->fr_RH_upperleg_X_fr_RH_hipassembly) * RH_hipassembly_v);
    RH_upperleg_v(iit::rbd::AZ) += qd(RH_HFE);
    
    Utils::fillAsMotionCrossProductMx(RH_upperleg_v, spareMx);
    
    RH_upperleg_a = ((xm->fr_RH_upperleg_X_fr_RH_hipassembly) * RH_hipassembly_a) + (spareMx.col(iit::rbd::AZ) * qd(RH_HFE));
    RH_upperleg_a(iit::rbd::AZ) += qdd(RH_HFE);
    
    RH_upperleg_f = RH_upperleg_I * RH_upperleg_a + (-spareMx.transpose() * RH_upperleg_I * RH_upperleg_v);
    
    // First pass, link 'RH_lowerleg'
    RH_lowerleg_v = ((xm->fr_RH_lowerleg_X_fr_RH_upperleg) * RH_upperleg_v);
    RH_lowerleg_v(iit::rbd::AZ) += qd(RH_KFE);
    
    Utils::fillAsMotionCrossProductMx(RH_lowerleg_v, spareMx);
    
    RH_lowerleg_a = ((xm->fr_RH_lowerleg_X_fr_RH_upperleg) * RH_upperleg_a) + (spareMx.col(iit::rbd::AZ) * qd(RH_KFE));
    RH_lowerleg_a(iit::rbd::AZ) += qdd(RH_KFE);
    
    RH_lowerleg_f = RH_lowerleg_I * RH_lowerleg_a + (-spareMx.transpose() * RH_lowerleg_I * RH_lowerleg_v);
    

    // The base
    Utils::fillAsForceCrossProductMx(trunk_v, spareMx);
    trunk_f = trunk_I * trunk_a + spareMx * trunk_I * trunk_v;

    // Add the external forces:
    trunk_f -= fext[TRUNK];
    LF_hipassembly_f -= fext[LF_HIPASSEMBLY];
    LF_upperleg_f -= fext[LF_UPPERLEG];
    LF_lowerleg_f -= fext[LF_LOWERLEG];
    RF_hipassembly_f -= fext[RF_HIPASSEMBLY];
    RF_upperleg_f -= fext[RF_UPPERLEG];
    RF_lowerleg_f -= fext[RF_LOWERLEG];
    LH_hipassembly_f -= fext[LH_HIPASSEMBLY];
    LH_upperleg_f -= fext[LH_UPPERLEG];
    LH_lowerleg_f -= fext[LH_LOWERLEG];
    RH_hipassembly_f -= fext[RH_HIPASSEMBLY];
    RH_upperleg_f -= fext[RH_UPPERLEG];
    RH_lowerleg_f -= fext[RH_LOWERLEG];

    secondPass_fullyActuated(jForces);

    baseWrench = trunk_f;
}


void iit::HyQ::dyn::InverseDynamics::secondPass_fullyActuated(JointState& jForces)
{
    // Link 'RH_lowerleg'
    jForces(RH_KFE) = RH_lowerleg_f(iit::rbd::AZ);
    RH_upperleg_f = RH_upperleg_f + (xm->fr_RH_lowerleg_X_fr_RH_upperleg).transpose() * RH_lowerleg_f;
    // Link 'RH_upperleg'
    jForces(RH_HFE) = RH_upperleg_f(iit::rbd::AZ);
    RH_hipassembly_f = RH_hipassembly_f + (xm->fr_RH_upperleg_X_fr_RH_hipassembly).transpose() * RH_upperleg_f;
    // Link 'RH_hipassembly'
    jForces(RH_HAA) = RH_hipassembly_f(iit::rbd::AZ);
    trunk_f = trunk_f + (xm->fr_RH_hipassembly_X_fr_trunk).transpose() * RH_hipassembly_f;
    // Link 'LH_lowerleg'
    jForces(LH_KFE) = LH_lowerleg_f(iit::rbd::AZ);
    LH_upperleg_f = LH_upperleg_f + (xm->fr_LH_lowerleg_X_fr_LH_upperleg).transpose() * LH_lowerleg_f;
    // Link 'LH_upperleg'
    jForces(LH_HFE) = LH_upperleg_f(iit::rbd::AZ);
    LH_hipassembly_f = LH_hipassembly_f + (xm->fr_LH_upperleg_X_fr_LH_hipassembly).transpose() * LH_upperleg_f;
    // Link 'LH_hipassembly'
    jForces(LH_HAA) = LH_hipassembly_f(iit::rbd::AZ);
    trunk_f = trunk_f + (xm->fr_LH_hipassembly_X_fr_trunk).transpose() * LH_hipassembly_f;
    // Link 'RF_lowerleg'
    jForces(RF_KFE) = RF_lowerleg_f(iit::rbd::AZ);
    RF_upperleg_f = RF_upperleg_f + (xm->fr_RF_lowerleg_X_fr_RF_upperleg).transpose() * RF_lowerleg_f;
    // Link 'RF_upperleg'
    jForces(RF_HFE) = RF_upperleg_f(iit::rbd::AZ);
    RF_hipassembly_f = RF_hipassembly_f + (xm->fr_RF_upperleg_X_fr_RF_hipassembly).transpose() * RF_upperleg_f;
    // Link 'RF_hipassembly'
    jForces(RF_HAA) = RF_hipassembly_f(iit::rbd::AZ);
    trunk_f = trunk_f + (xm->fr_RF_hipassembly_X_fr_trunk).transpose() * RF_hipassembly_f;
    // Link 'LF_lowerleg'
    jForces(LF_KFE) = LF_lowerleg_f(iit::rbd::AZ);
    LF_upperleg_f = LF_upperleg_f + (xm->fr_LF_lowerleg_X_fr_LF_upperleg).transpose() * LF_lowerleg_f;
    // Link 'LF_upperleg'
    jForces(LF_HFE) = LF_upperleg_f(iit::rbd::AZ);
    LF_hipassembly_f = LF_hipassembly_f + (xm->fr_LF_upperleg_X_fr_LF_hipassembly).transpose() * LF_upperleg_f;
    // Link 'LF_hipassembly'
    jForces(LF_HAA) = LF_hipassembly_f(iit::rbd::AZ);
    trunk_f = trunk_f + (xm->fr_LF_hipassembly_X_fr_trunk).transpose() * LF_hipassembly_f;
}
