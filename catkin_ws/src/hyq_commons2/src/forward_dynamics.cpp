#include "iit/robots/hyq/forward_dynamics.h"

using namespace iit::rbd;
using namespace iit::dog;

// Initialization of static-const data
const iit::HyQ::dyn::ForwardDynamics::ExtForces
    iit::HyQ::dyn::ForwardDynamics::zeroExtForces(Force::Zero());

iit::HyQ::dyn::ForwardDynamics::ForwardDynamics(InertiaProperties& inertia, MotionTransforms& transforms) :
    inertiaProps( & inertia ),
    motionTransforms( & transforms )
{
    LF_hipassembly_v.setZero();
    LF_hipassembly_c.setZero();
    LF_upperleg_v.setZero();
    LF_upperleg_c.setZero();
    LF_lowerleg_v.setZero();
    LF_lowerleg_c.setZero();
    RF_hipassembly_v.setZero();
    RF_hipassembly_c.setZero();
    RF_upperleg_v.setZero();
    RF_upperleg_c.setZero();
    RF_lowerleg_v.setZero();
    RF_lowerleg_c.setZero();
    LH_hipassembly_v.setZero();
    LH_hipassembly_c.setZero();
    LH_upperleg_v.setZero();
    LH_upperleg_c.setZero();
    LH_lowerleg_v.setZero();
    LH_lowerleg_c.setZero();
    RH_hipassembly_v.setZero();
    RH_hipassembly_c.setZero();
    RH_upperleg_v.setZero();
    RH_upperleg_c.setZero();
    RH_lowerleg_v.setZero();
    RH_lowerleg_c.setZero();

}

void iit::HyQ::dyn::ForwardDynamics::fd(
    JointState& qdd,
    Acceleration& trunk_a,
    const Velocity& trunk_v,
    const Acceleration& g,
    const JointState& qd,
    const JointState& tau,
    const ExtForces& fext/* = zeroExtForces */)
{
    
    trunk_AI = inertiaProps->getTensor_trunk();
    trunk_p = - fext[TRUNK];
    LF_hipassembly_AI = inertiaProps->getTensor_LF_hipassembly();
    LF_hipassembly_p = - fext[LF_HIPASSEMBLY];
    LF_upperleg_AI = inertiaProps->getTensor_LF_upperleg();
    LF_upperleg_p = - fext[LF_UPPERLEG];
    LF_lowerleg_AI = inertiaProps->getTensor_LF_lowerleg();
    LF_lowerleg_p = - fext[LF_LOWERLEG];
    RF_hipassembly_AI = inertiaProps->getTensor_RF_hipassembly();
    RF_hipassembly_p = - fext[RF_HIPASSEMBLY];
    RF_upperleg_AI = inertiaProps->getTensor_RF_upperleg();
    RF_upperleg_p = - fext[RF_UPPERLEG];
    RF_lowerleg_AI = inertiaProps->getTensor_RF_lowerleg();
    RF_lowerleg_p = - fext[RF_LOWERLEG];
    LH_hipassembly_AI = inertiaProps->getTensor_LH_hipassembly();
    LH_hipassembly_p = - fext[LH_HIPASSEMBLY];
    LH_upperleg_AI = inertiaProps->getTensor_LH_upperleg();
    LH_upperleg_p = - fext[LH_UPPERLEG];
    LH_lowerleg_AI = inertiaProps->getTensor_LH_lowerleg();
    LH_lowerleg_p = - fext[LH_LOWERLEG];
    RH_hipassembly_AI = inertiaProps->getTensor_RH_hipassembly();
    RH_hipassembly_p = - fext[RH_HIPASSEMBLY];
    RH_upperleg_AI = inertiaProps->getTensor_RH_upperleg();
    RH_upperleg_p = - fext[RH_UPPERLEG];
    RH_lowerleg_AI = inertiaProps->getTensor_RH_lowerleg();
    RH_lowerleg_p = - fext[RH_LOWERLEG];
    // ---------------------- FIRST PASS ---------------------- //
    
    // + Link LF_hipassembly
    //  - The spatial velocity:
    LF_hipassembly_v = ((motionTransforms -> fr_LF_hipassembly_X_fr_trunk) * trunk_v);
    LF_hipassembly_v(iit::rbd::AZ) += qd(LF_HAA);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(LF_hipassembly_v, spareMx);
    LF_hipassembly_c = (spareMx.col(iit::rbd::AZ) * qd(LF_HAA));
    
    //  - The bias force term:
    LF_hipassembly_p += -spareMx.transpose() * LF_hipassembly_AI * LF_hipassembly_v;
    
    // + Link LF_upperleg
    //  - The spatial velocity:
    LF_upperleg_v = ((motionTransforms -> fr_LF_upperleg_X_fr_LF_hipassembly) * LF_hipassembly_v);
    LF_upperleg_v(iit::rbd::AZ) += qd(LF_HFE);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(LF_upperleg_v, spareMx);
    LF_upperleg_c = (spareMx.col(iit::rbd::AZ) * qd(LF_HFE));
    
    //  - The bias force term:
    LF_upperleg_p += -spareMx.transpose() * LF_upperleg_AI * LF_upperleg_v;
    
    // + Link LF_lowerleg
    //  - The spatial velocity:
    LF_lowerleg_v = ((motionTransforms -> fr_LF_lowerleg_X_fr_LF_upperleg) * LF_upperleg_v);
    LF_lowerleg_v(iit::rbd::AZ) += qd(LF_KFE);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(LF_lowerleg_v, spareMx);
    LF_lowerleg_c = (spareMx.col(iit::rbd::AZ) * qd(LF_KFE));
    
    //  - The bias force term:
    LF_lowerleg_p += -spareMx.transpose() * LF_lowerleg_AI * LF_lowerleg_v;
    
    // + Link RF_hipassembly
    //  - The spatial velocity:
    RF_hipassembly_v = ((motionTransforms -> fr_RF_hipassembly_X_fr_trunk) * trunk_v);
    RF_hipassembly_v(iit::rbd::AZ) += qd(RF_HAA);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(RF_hipassembly_v, spareMx);
    RF_hipassembly_c = (spareMx.col(iit::rbd::AZ) * qd(RF_HAA));
    
    //  - The bias force term:
    RF_hipassembly_p += -spareMx.transpose() * RF_hipassembly_AI * RF_hipassembly_v;
    
    // + Link RF_upperleg
    //  - The spatial velocity:
    RF_upperleg_v = ((motionTransforms -> fr_RF_upperleg_X_fr_RF_hipassembly) * RF_hipassembly_v);
    RF_upperleg_v(iit::rbd::AZ) += qd(RF_HFE);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(RF_upperleg_v, spareMx);
    RF_upperleg_c = (spareMx.col(iit::rbd::AZ) * qd(RF_HFE));
    
    //  - The bias force term:
    RF_upperleg_p += -spareMx.transpose() * RF_upperleg_AI * RF_upperleg_v;
    
    // + Link RF_lowerleg
    //  - The spatial velocity:
    RF_lowerleg_v = ((motionTransforms -> fr_RF_lowerleg_X_fr_RF_upperleg) * RF_upperleg_v);
    RF_lowerleg_v(iit::rbd::AZ) += qd(RF_KFE);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(RF_lowerleg_v, spareMx);
    RF_lowerleg_c = (spareMx.col(iit::rbd::AZ) * qd(RF_KFE));
    
    //  - The bias force term:
    RF_lowerleg_p += -spareMx.transpose() * RF_lowerleg_AI * RF_lowerleg_v;
    
    // + Link LH_hipassembly
    //  - The spatial velocity:
    LH_hipassembly_v = ((motionTransforms -> fr_LH_hipassembly_X_fr_trunk) * trunk_v);
    LH_hipassembly_v(iit::rbd::AZ) += qd(LH_HAA);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(LH_hipassembly_v, spareMx);
    LH_hipassembly_c = (spareMx.col(iit::rbd::AZ) * qd(LH_HAA));
    
    //  - The bias force term:
    LH_hipassembly_p += -spareMx.transpose() * LH_hipassembly_AI * LH_hipassembly_v;
    
    // + Link LH_upperleg
    //  - The spatial velocity:
    LH_upperleg_v = ((motionTransforms -> fr_LH_upperleg_X_fr_LH_hipassembly) * LH_hipassembly_v);
    LH_upperleg_v(iit::rbd::AZ) += qd(LH_HFE);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(LH_upperleg_v, spareMx);
    LH_upperleg_c = (spareMx.col(iit::rbd::AZ) * qd(LH_HFE));
    
    //  - The bias force term:
    LH_upperleg_p += -spareMx.transpose() * LH_upperleg_AI * LH_upperleg_v;
    
    // + Link LH_lowerleg
    //  - The spatial velocity:
    LH_lowerleg_v = ((motionTransforms -> fr_LH_lowerleg_X_fr_LH_upperleg) * LH_upperleg_v);
    LH_lowerleg_v(iit::rbd::AZ) += qd(LH_KFE);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(LH_lowerleg_v, spareMx);
    LH_lowerleg_c = (spareMx.col(iit::rbd::AZ) * qd(LH_KFE));
    
    //  - The bias force term:
    LH_lowerleg_p += -spareMx.transpose() * LH_lowerleg_AI * LH_lowerleg_v;
    
    // + Link RH_hipassembly
    //  - The spatial velocity:
    RH_hipassembly_v = ((motionTransforms -> fr_RH_hipassembly_X_fr_trunk) * trunk_v);
    RH_hipassembly_v(iit::rbd::AZ) += qd(RH_HAA);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(RH_hipassembly_v, spareMx);
    RH_hipassembly_c = (spareMx.col(iit::rbd::AZ) * qd(RH_HAA));
    
    //  - The bias force term:
    RH_hipassembly_p += -spareMx.transpose() * RH_hipassembly_AI * RH_hipassembly_v;
    
    // + Link RH_upperleg
    //  - The spatial velocity:
    RH_upperleg_v = ((motionTransforms -> fr_RH_upperleg_X_fr_RH_hipassembly) * RH_hipassembly_v);
    RH_upperleg_v(iit::rbd::AZ) += qd(RH_HFE);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(RH_upperleg_v, spareMx);
    RH_upperleg_c = (spareMx.col(iit::rbd::AZ) * qd(RH_HFE));
    
    //  - The bias force term:
    RH_upperleg_p += -spareMx.transpose() * RH_upperleg_AI * RH_upperleg_v;
    
    // + Link RH_lowerleg
    //  - The spatial velocity:
    RH_lowerleg_v = ((motionTransforms -> fr_RH_lowerleg_X_fr_RH_upperleg) * RH_upperleg_v);
    RH_lowerleg_v(iit::rbd::AZ) += qd(RH_KFE);
    
    //  - The velocity-product acceleration term:
    Utils::fillAsMotionCrossProductMx(RH_lowerleg_v, spareMx);
    RH_lowerleg_c = (spareMx.col(iit::rbd::AZ) * qd(RH_KFE));
    
    //  - The bias force term:
    RH_lowerleg_p += -spareMx.transpose() * RH_lowerleg_AI * RH_lowerleg_v;
    
    // + The floating base
    Utils::fillAsForceCrossProductMx(trunk_v, spareMx);
    trunk_p += spareMx * (trunk_AI * trunk_v);
    
    // ---------------------- SECOND PASS ---------------------- //
    InertiaMatrix Ia;
    Force pa;
    
    // + Link RH_lowerleg
    RH_lowerleg_U = RH_lowerleg_AI.col(iit::rbd::AZ);
    RH_lowerleg_D = RH_lowerleg_U.row(iit::rbd::AZ)(0,0);
    RH_lowerleg_u = tau(RH_KFE) - RH_lowerleg_p.row(iit::rbd::AZ)(0,0);
    
    Ia = RH_lowerleg_AI - RH_lowerleg_U/RH_lowerleg_D * RH_lowerleg_U.transpose();
    pa = RH_lowerleg_p + Ia * RH_lowerleg_c + RH_lowerleg_U * RH_lowerleg_u/RH_lowerleg_D;
    
    RH_upperleg_AI += (motionTransforms -> fr_RH_lowerleg_X_fr_RH_upperleg).transpose() * Ia * (motionTransforms -> fr_RH_lowerleg_X_fr_RH_upperleg);
    RH_upperleg_p += (motionTransforms -> fr_RH_lowerleg_X_fr_RH_upperleg).transpose() * pa;
    
    // + Link RH_upperleg
    RH_upperleg_U = RH_upperleg_AI.col(iit::rbd::AZ);
    RH_upperleg_D = RH_upperleg_U.row(iit::rbd::AZ)(0,0);
    RH_upperleg_u = tau(RH_HFE) - RH_upperleg_p.row(iit::rbd::AZ)(0,0);
    
    Ia = RH_upperleg_AI - RH_upperleg_U/RH_upperleg_D * RH_upperleg_U.transpose();
    pa = RH_upperleg_p + Ia * RH_upperleg_c + RH_upperleg_U * RH_upperleg_u/RH_upperleg_D;
    
    RH_hipassembly_AI += (motionTransforms -> fr_RH_upperleg_X_fr_RH_hipassembly).transpose() * Ia * (motionTransforms -> fr_RH_upperleg_X_fr_RH_hipassembly);
    RH_hipassembly_p += (motionTransforms -> fr_RH_upperleg_X_fr_RH_hipassembly).transpose() * pa;
    
    // + Link RH_hipassembly
    RH_hipassembly_U = RH_hipassembly_AI.col(iit::rbd::AZ);
    RH_hipassembly_D = RH_hipassembly_U.row(iit::rbd::AZ)(0,0);
    RH_hipassembly_u = tau(RH_HAA) - RH_hipassembly_p.row(iit::rbd::AZ)(0,0);
    
    Ia = RH_hipassembly_AI - RH_hipassembly_U/RH_hipassembly_D * RH_hipassembly_U.transpose();
    pa = RH_hipassembly_p + Ia * RH_hipassembly_c + RH_hipassembly_U * RH_hipassembly_u/RH_hipassembly_D;
    
    trunk_AI += (motionTransforms -> fr_RH_hipassembly_X_fr_trunk).transpose() * Ia * (motionTransforms -> fr_RH_hipassembly_X_fr_trunk);
    trunk_p += (motionTransforms -> fr_RH_hipassembly_X_fr_trunk).transpose() * pa;
    
    // + Link LH_lowerleg
    LH_lowerleg_U = LH_lowerleg_AI.col(iit::rbd::AZ);
    LH_lowerleg_D = LH_lowerleg_U.row(iit::rbd::AZ)(0,0);
    LH_lowerleg_u = tau(LH_KFE) - LH_lowerleg_p.row(iit::rbd::AZ)(0,0);
    
    Ia = LH_lowerleg_AI - LH_lowerleg_U/LH_lowerleg_D * LH_lowerleg_U.transpose();
    pa = LH_lowerleg_p + Ia * LH_lowerleg_c + LH_lowerleg_U * LH_lowerleg_u/LH_lowerleg_D;
    
    LH_upperleg_AI += (motionTransforms -> fr_LH_lowerleg_X_fr_LH_upperleg).transpose() * Ia * (motionTransforms -> fr_LH_lowerleg_X_fr_LH_upperleg);
    LH_upperleg_p += (motionTransforms -> fr_LH_lowerleg_X_fr_LH_upperleg).transpose() * pa;
    
    // + Link LH_upperleg
    LH_upperleg_U = LH_upperleg_AI.col(iit::rbd::AZ);
    LH_upperleg_D = LH_upperleg_U.row(iit::rbd::AZ)(0,0);
    LH_upperleg_u = tau(LH_HFE) - LH_upperleg_p.row(iit::rbd::AZ)(0,0);
    
    Ia = LH_upperleg_AI - LH_upperleg_U/LH_upperleg_D * LH_upperleg_U.transpose();
    pa = LH_upperleg_p + Ia * LH_upperleg_c + LH_upperleg_U * LH_upperleg_u/LH_upperleg_D;
    
    LH_hipassembly_AI += (motionTransforms -> fr_LH_upperleg_X_fr_LH_hipassembly).transpose() * Ia * (motionTransforms -> fr_LH_upperleg_X_fr_LH_hipassembly);
    LH_hipassembly_p += (motionTransforms -> fr_LH_upperleg_X_fr_LH_hipassembly).transpose() * pa;
    
    // + Link LH_hipassembly
    LH_hipassembly_U = LH_hipassembly_AI.col(iit::rbd::AZ);
    LH_hipassembly_D = LH_hipassembly_U.row(iit::rbd::AZ)(0,0);
    LH_hipassembly_u = tau(LH_HAA) - LH_hipassembly_p.row(iit::rbd::AZ)(0,0);
    
    Ia = LH_hipassembly_AI - LH_hipassembly_U/LH_hipassembly_D * LH_hipassembly_U.transpose();
    pa = LH_hipassembly_p + Ia * LH_hipassembly_c + LH_hipassembly_U * LH_hipassembly_u/LH_hipassembly_D;
    
    trunk_AI += (motionTransforms -> fr_LH_hipassembly_X_fr_trunk).transpose() * Ia * (motionTransforms -> fr_LH_hipassembly_X_fr_trunk);
    trunk_p += (motionTransforms -> fr_LH_hipassembly_X_fr_trunk).transpose() * pa;
    
    // + Link RF_lowerleg
    RF_lowerleg_U = RF_lowerleg_AI.col(iit::rbd::AZ);
    RF_lowerleg_D = RF_lowerleg_U.row(iit::rbd::AZ)(0,0);
    RF_lowerleg_u = tau(RF_KFE) - RF_lowerleg_p.row(iit::rbd::AZ)(0,0);
    
    Ia = RF_lowerleg_AI - RF_lowerleg_U/RF_lowerleg_D * RF_lowerleg_U.transpose();
    pa = RF_lowerleg_p + Ia * RF_lowerleg_c + RF_lowerleg_U * RF_lowerleg_u/RF_lowerleg_D;
    
    RF_upperleg_AI += (motionTransforms -> fr_RF_lowerleg_X_fr_RF_upperleg).transpose() * Ia * (motionTransforms -> fr_RF_lowerleg_X_fr_RF_upperleg);
    RF_upperleg_p += (motionTransforms -> fr_RF_lowerleg_X_fr_RF_upperleg).transpose() * pa;
    
    // + Link RF_upperleg
    RF_upperleg_U = RF_upperleg_AI.col(iit::rbd::AZ);
    RF_upperleg_D = RF_upperleg_U.row(iit::rbd::AZ)(0,0);
    RF_upperleg_u = tau(RF_HFE) - RF_upperleg_p.row(iit::rbd::AZ)(0,0);
    
    Ia = RF_upperleg_AI - RF_upperleg_U/RF_upperleg_D * RF_upperleg_U.transpose();
    pa = RF_upperleg_p + Ia * RF_upperleg_c + RF_upperleg_U * RF_upperleg_u/RF_upperleg_D;
    
    RF_hipassembly_AI += (motionTransforms -> fr_RF_upperleg_X_fr_RF_hipassembly).transpose() * Ia * (motionTransforms -> fr_RF_upperleg_X_fr_RF_hipassembly);
    RF_hipassembly_p += (motionTransforms -> fr_RF_upperleg_X_fr_RF_hipassembly).transpose() * pa;
    
    // + Link RF_hipassembly
    RF_hipassembly_U = RF_hipassembly_AI.col(iit::rbd::AZ);
    RF_hipassembly_D = RF_hipassembly_U.row(iit::rbd::AZ)(0,0);
    RF_hipassembly_u = tau(RF_HAA) - RF_hipassembly_p.row(iit::rbd::AZ)(0,0);
    
    Ia = RF_hipassembly_AI - RF_hipassembly_U/RF_hipassembly_D * RF_hipassembly_U.transpose();
    pa = RF_hipassembly_p + Ia * RF_hipassembly_c + RF_hipassembly_U * RF_hipassembly_u/RF_hipassembly_D;
    
    trunk_AI += (motionTransforms -> fr_RF_hipassembly_X_fr_trunk).transpose() * Ia * (motionTransforms -> fr_RF_hipassembly_X_fr_trunk);
    trunk_p += (motionTransforms -> fr_RF_hipassembly_X_fr_trunk).transpose() * pa;
    
    // + Link LF_lowerleg
    LF_lowerleg_U = LF_lowerleg_AI.col(iit::rbd::AZ);
    LF_lowerleg_D = LF_lowerleg_U.row(iit::rbd::AZ)(0,0);
    LF_lowerleg_u = tau(LF_KFE) - LF_lowerleg_p.row(iit::rbd::AZ)(0,0);
    
    Ia = LF_lowerleg_AI - LF_lowerleg_U/LF_lowerleg_D * LF_lowerleg_U.transpose();
    pa = LF_lowerleg_p + Ia * LF_lowerleg_c + LF_lowerleg_U * LF_lowerleg_u/LF_lowerleg_D;
    
    LF_upperleg_AI += (motionTransforms -> fr_LF_lowerleg_X_fr_LF_upperleg).transpose() * Ia * (motionTransforms -> fr_LF_lowerleg_X_fr_LF_upperleg);
    LF_upperleg_p += (motionTransforms -> fr_LF_lowerleg_X_fr_LF_upperleg).transpose() * pa;
    
    // + Link LF_upperleg
    LF_upperleg_U = LF_upperleg_AI.col(iit::rbd::AZ);
    LF_upperleg_D = LF_upperleg_U.row(iit::rbd::AZ)(0,0);
    LF_upperleg_u = tau(LF_HFE) - LF_upperleg_p.row(iit::rbd::AZ)(0,0);
    
    Ia = LF_upperleg_AI - LF_upperleg_U/LF_upperleg_D * LF_upperleg_U.transpose();
    pa = LF_upperleg_p + Ia * LF_upperleg_c + LF_upperleg_U * LF_upperleg_u/LF_upperleg_D;
    
    LF_hipassembly_AI += (motionTransforms -> fr_LF_upperleg_X_fr_LF_hipassembly).transpose() * Ia * (motionTransforms -> fr_LF_upperleg_X_fr_LF_hipassembly);
    LF_hipassembly_p += (motionTransforms -> fr_LF_upperleg_X_fr_LF_hipassembly).transpose() * pa;
    
    // + Link LF_hipassembly
    LF_hipassembly_U = LF_hipassembly_AI.col(iit::rbd::AZ);
    LF_hipassembly_D = LF_hipassembly_U.row(iit::rbd::AZ)(0,0);
    LF_hipassembly_u = tau(LF_HAA) - LF_hipassembly_p.row(iit::rbd::AZ)(0,0);
    
    Ia = LF_hipassembly_AI - LF_hipassembly_U/LF_hipassembly_D * LF_hipassembly_U.transpose();
    pa = LF_hipassembly_p + Ia * LF_hipassembly_c + LF_hipassembly_U * LF_hipassembly_u/LF_hipassembly_D;
    
    trunk_AI += (motionTransforms -> fr_LF_hipassembly_X_fr_trunk).transpose() * Ia * (motionTransforms -> fr_LF_hipassembly_X_fr_trunk);
    trunk_p += (motionTransforms -> fr_LF_hipassembly_X_fr_trunk).transpose() * pa;
    
    // + The acceleration of the floating base trunk, without gravity
    trunk_a = - trunk_AI.inverse() * trunk_p;
    
    // ---------------------- THIRD PASS ---------------------- //
    Velocity atmp;
    atmp = (motionTransforms -> fr_LF_hipassembly_X_fr_trunk) * trunk_a + LF_hipassembly_c;
    qdd(LF_HAA) = (LF_hipassembly_u - LF_hipassembly_U.transpose() * atmp) / LF_hipassembly_D;
    LF_hipassembly_a = atmp;
    LF_hipassembly_a.row(iit::rbd::AZ)(0,0) += qdd(LF_HAA);
    
    atmp = (motionTransforms -> fr_LF_upperleg_X_fr_LF_hipassembly) * LF_hipassembly_a + LF_upperleg_c;
    qdd(LF_HFE) = (LF_upperleg_u - LF_upperleg_U.transpose() * atmp) / LF_upperleg_D;
    LF_upperleg_a = atmp;
    LF_upperleg_a.row(iit::rbd::AZ)(0,0) += qdd(LF_HFE);
    
    atmp = (motionTransforms -> fr_LF_lowerleg_X_fr_LF_upperleg) * LF_upperleg_a + LF_lowerleg_c;
    qdd(LF_KFE) = (LF_lowerleg_u - LF_lowerleg_U.transpose() * atmp) / LF_lowerleg_D;
    LF_lowerleg_a = atmp;
    LF_lowerleg_a.row(iit::rbd::AZ)(0,0) += qdd(LF_KFE);
    
    atmp = (motionTransforms -> fr_RF_hipassembly_X_fr_trunk) * trunk_a + RF_hipassembly_c;
    qdd(RF_HAA) = (RF_hipassembly_u - RF_hipassembly_U.transpose() * atmp) / RF_hipassembly_D;
    RF_hipassembly_a = atmp;
    RF_hipassembly_a.row(iit::rbd::AZ)(0,0) += qdd(RF_HAA);
    
    atmp = (motionTransforms -> fr_RF_upperleg_X_fr_RF_hipassembly) * RF_hipassembly_a + RF_upperleg_c;
    qdd(RF_HFE) = (RF_upperleg_u - RF_upperleg_U.transpose() * atmp) / RF_upperleg_D;
    RF_upperleg_a = atmp;
    RF_upperleg_a.row(iit::rbd::AZ)(0,0) += qdd(RF_HFE);
    
    atmp = (motionTransforms -> fr_RF_lowerleg_X_fr_RF_upperleg) * RF_upperleg_a + RF_lowerleg_c;
    qdd(RF_KFE) = (RF_lowerleg_u - RF_lowerleg_U.transpose() * atmp) / RF_lowerleg_D;
    RF_lowerleg_a = atmp;
    RF_lowerleg_a.row(iit::rbd::AZ)(0,0) += qdd(RF_KFE);
    
    atmp = (motionTransforms -> fr_LH_hipassembly_X_fr_trunk) * trunk_a + LH_hipassembly_c;
    qdd(LH_HAA) = (LH_hipassembly_u - LH_hipassembly_U.transpose() * atmp) / LH_hipassembly_D;
    LH_hipassembly_a = atmp;
    LH_hipassembly_a.row(iit::rbd::AZ)(0,0) += qdd(LH_HAA);
    
    atmp = (motionTransforms -> fr_LH_upperleg_X_fr_LH_hipassembly) * LH_hipassembly_a + LH_upperleg_c;
    qdd(LH_HFE) = (LH_upperleg_u - LH_upperleg_U.transpose() * atmp) / LH_upperleg_D;
    LH_upperleg_a = atmp;
    LH_upperleg_a.row(iit::rbd::AZ)(0,0) += qdd(LH_HFE);
    
    atmp = (motionTransforms -> fr_LH_lowerleg_X_fr_LH_upperleg) * LH_upperleg_a + LH_lowerleg_c;
    qdd(LH_KFE) = (LH_lowerleg_u - LH_lowerleg_U.transpose() * atmp) / LH_lowerleg_D;
    LH_lowerleg_a = atmp;
    LH_lowerleg_a.row(iit::rbd::AZ)(0,0) += qdd(LH_KFE);
    
    atmp = (motionTransforms -> fr_RH_hipassembly_X_fr_trunk) * trunk_a + RH_hipassembly_c;
    qdd(RH_HAA) = (RH_hipassembly_u - RH_hipassembly_U.transpose() * atmp) / RH_hipassembly_D;
    RH_hipassembly_a = atmp;
    RH_hipassembly_a.row(iit::rbd::AZ)(0,0) += qdd(RH_HAA);
    
    atmp = (motionTransforms -> fr_RH_upperleg_X_fr_RH_hipassembly) * RH_hipassembly_a + RH_upperleg_c;
    qdd(RH_HFE) = (RH_upperleg_u - RH_upperleg_U.transpose() * atmp) / RH_upperleg_D;
    RH_upperleg_a = atmp;
    RH_upperleg_a.row(iit::rbd::AZ)(0,0) += qdd(RH_HFE);
    
    atmp = (motionTransforms -> fr_RH_lowerleg_X_fr_RH_upperleg) * RH_upperleg_a + RH_lowerleg_c;
    qdd(RH_KFE) = (RH_lowerleg_u - RH_lowerleg_U.transpose() * atmp) / RH_lowerleg_D;
    RH_lowerleg_a = atmp;
    RH_lowerleg_a.row(iit::rbd::AZ)(0,0) += qdd(RH_KFE);
    
    
    // + Add gravity to the acceleration of the floating base
    trunk_a += g;
}
