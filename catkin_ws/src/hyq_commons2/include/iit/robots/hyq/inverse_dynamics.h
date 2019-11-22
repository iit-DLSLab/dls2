#ifndef IIT_HYQ_INVERSE_DYNAMICS_H_
#define IIT_HYQ_INVERSE_DYNAMICS_H_

#include <Eigen/Dense>
#include <iit/rbd/rbd.h>
#include <iit/rbd/InertiaMatrix.h>
#include <iit/rbd/utils.h>

#include <iit/commons/dog/declarations.h>
#include "inertia_properties.h"
#include "transforms.h"
#include <iit/commons/dog/link_data_map.h>
#include <iit/commons/dog/inverse_dynamics.h>

namespace iit {
namespace HyQ {
namespace dyn {

/**
 * The Inverse Dynamics routine for the robot HyQ.
 *
 * In addition to the full Newton-Euler algorithm, specialized versions to compute
 * only certain terms are provided.
 * The parameters common to most of the methods are the joint status \c q, the
 * joint velocities \c qd and the accelerations \c qdd. The \c jForces parameter
 * will be filled with the computed values.
 * Additional overloaded methods are provided without the \c q parameter. These
 * methods use the current configuration of the robot; they are provided for the
 * sake of efficiency, in case the kinematics transforms of the robot have already
 * been updated elsewhere with the most recent configuration (eg by a call to
 * setJointStatus()), so that it is useless to compute them again.
 */
class InverseDynamics  : public iit::dog::InverseDynamicsBase {
public:
    typedef dog::JointState JointState;
    typedef iit::rbd::ForceVector Force;
    typedef iit::rbd::VelocityVector Velocity;
    typedef iit::rbd::VelocityVector Acceleration;
    typedef iit::rbd::InertiaMatrixDense InertiaMatrix;
public:
    /**
     * Default constructor
     * \param in the inertia properties of the links
     * \param tr the container of all the spatial motion transforms of
     *     the robot HyQ, which will be used by this instance
     *     to compute inverse-dynamics.
     */
    InverseDynamics(InertiaProperties& in, MotionTransforms& tr);

    /** \name propagate link velocity and accelerations
     * The first pass forward propagation of velocity/accelerations, sets internal variables
     */ ///@{
    void propagateVelAcc(
        const Acceleration& g, const Velocity& trunk_v,
        const JointState& qd, const JointState& qdd);
    ///@}

    /** \name Inverse dynamics
     * The full algorithm for inverse dynamics for this robot
     */ ///@{
    void id(
        JointState& jForces, Acceleration& trunk_a,
        const Acceleration& g, const Velocity& trunk_v,
        const JointState& q, const JointState& qd, const JointState& qdd,
        const ExtForces& fext = dog::LinkDataMap<iit::rbd::ForceVector>(Force::Zero()));
    void id(
        JointState& jForces, Acceleration& trunk_a,
        const Acceleration& g, const Velocity& trunk_v,
        const JointState& qd, const JointState& qdd,
        const ExtForces& fext = dog::LinkDataMap<iit::rbd::ForceVector>(Force::Zero()));
    ///@}
    /** \name Inverse dynamics, fully actuated base
     * The inverse dynamics algorithm for the floating base robot,
     * in the assumption of a fully actuated base
     */ ///@{
    void id_fully_actuated(
        Force& baseWrench, JointState& jForces,
        const Acceleration& g, const Velocity& trunk_v, const Acceleration& baseAccel,
        const JointState& q, const JointState& qd, const JointState& qdd, const ExtForces & fext = dog::LinkDataMap<iit::rbd::ForceVector>(Force::Zero()));
    void id_fully_actuated(
        Force& baseWrench, JointState& jForces,
        const Acceleration& g, const Velocity& trunk_v, const Acceleration& baseAccel,
        const JointState& qd, const JointState& qdd, const ExtForces & fext = dog::LinkDataMap<iit::rbd::ForceVector>(Force::Zero()));
    ///@}
    /** \name Gravity terms, fully actuated base
     */
    ///@{
    void G_terms_fully_actuated(
        Force& baseWrench, JointState& jForces,
        const Acceleration& g, const JointState& q);
    void G_terms_fully_actuated(
        Force& baseWrench, JointState& jForces,
        const Acceleration& g);
    ///@}
    /** \name Centrifugal and Coriolis terms, fully actuated base
     *
     * These functions take only velocity inputs, that is, they assume
     * a zero spatial acceleration of the base (in addition to zero acceleration
     * at the actuated joints).
     * Note that this is NOT the same as imposing zero acceleration
     * at the virtual 6-dof-floting-base joint, which would result, in general,
     * in a non-zero spatial acceleration of the base, due to velocity
     * product terms.
     */
    ///@{
    void C_terms_fully_actuated(
        Force& baseWrench, JointState& jForces,
        const Velocity& trunk_v, const JointState& q, const JointState& qd);
    void C_terms_fully_actuated(
        Force& baseWrench, JointState& jForces,
        const Velocity& trunk_v, const JointState& qd);
    ///@}
    /** Updates all the kinematics transforms used by the inverse dynamics routine. */
    void setJointStatus(const JointState& q) const;

    Velocity getLinkVelocity(const JointState &q,
                             const JointState &qd,
                             const JointState &qdd,
                             const Acceleration &gravity_acceleration,
                             const Velocity &trunk_velocity,
                             const dog::LinkIdentifiers &link_id);

    Acceleration getLinkAcceleration(const JointState &q,
                                     const JointState &qd,
                                     const JointState &qdd,
                                     const Acceleration &gravity_acceleration,
                                     const Velocity &trunk_velocity,
                                     const dog::LinkIdentifiers &link_id);

public:
    /** \name Getters */
    ///@{
    const Force& getForce_trunk() const { return trunk_f; }
    const Velocity& getVelocity_LF_hipassembly() const { return LF_hipassembly_v; }
    const Acceleration& getAcceleration_LF_hipassembly() const { return LF_hipassembly_a; }
    const Force& getForce_LF_hipassembly() const { return LF_hipassembly_f; }
    const Velocity& getVelocity_LF_upperleg() const { return LF_upperleg_v; }
    const Acceleration& getAcceleration_LF_upperleg() const { return LF_upperleg_a; }
    const Force& getForce_LF_upperleg() const { return LF_upperleg_f; }
    const Velocity& getVelocity_LF_lowerleg() const { return LF_lowerleg_v; }
    const Acceleration& getAcceleration_LF_lowerleg() const { return LF_lowerleg_a; }
    const Force& getForce_LF_lowerleg() const { return LF_lowerleg_f; }
    const Velocity& getVelocity_RF_hipassembly() const { return RF_hipassembly_v; }
    const Acceleration& getAcceleration_RF_hipassembly() const { return RF_hipassembly_a; }
    const Force& getForce_RF_hipassembly() const { return RF_hipassembly_f; }
    const Velocity& getVelocity_RF_upperleg() const { return RF_upperleg_v; }
    const Acceleration& getAcceleration_RF_upperleg() const { return RF_upperleg_a; }
    const Force& getForce_RF_upperleg() const { return RF_upperleg_f; }
    const Velocity& getVelocity_RF_lowerleg() const { return RF_lowerleg_v; }
    const Acceleration& getAcceleration_RF_lowerleg() const { return RF_lowerleg_a; }
    const Force& getForce_RF_lowerleg() const { return RF_lowerleg_f; }
    const Velocity& getVelocity_LH_hipassembly() const { return LH_hipassembly_v; }
    const Acceleration& getAcceleration_LH_hipassembly() const { return LH_hipassembly_a; }
    const Force& getForce_LH_hipassembly() const { return LH_hipassembly_f; }
    const Velocity& getVelocity_LH_upperleg() const { return LH_upperleg_v; }
    const Acceleration& getAcceleration_LH_upperleg() const { return LH_upperleg_a; }
    const Force& getForce_LH_upperleg() const { return LH_upperleg_f; }
    const Velocity& getVelocity_LH_lowerleg() const { return LH_lowerleg_v; }
    const Acceleration& getAcceleration_LH_lowerleg() const { return LH_lowerleg_a; }
    const Force& getForce_LH_lowerleg() const { return LH_lowerleg_f; }
    const Velocity& getVelocity_RH_hipassembly() const { return RH_hipassembly_v; }
    const Acceleration& getAcceleration_RH_hipassembly() const { return RH_hipassembly_a; }
    const Force& getForce_RH_hipassembly() const { return RH_hipassembly_f; }
    const Velocity& getVelocity_RH_upperleg() const { return RH_upperleg_v; }
    const Acceleration& getAcceleration_RH_upperleg() const { return RH_upperleg_a; }
    const Force& getForce_RH_upperleg() const { return RH_upperleg_f; }
    const Velocity& getVelocity_RH_lowerleg() const { return RH_lowerleg_v; }
    const Acceleration& getAcceleration_RH_lowerleg() const { return RH_lowerleg_a; }
    const Force& getForce_RH_lowerleg() const { return RH_lowerleg_f; }
    ///@}
protected:
    void secondPass_fullyActuated(JointState& jForces);

private:
    InertiaProperties* inertiaProps;
    MotionTransforms* xm;
private:
    iit::rbd::Matrix66d spareMx; // support variable
    // Link 'LF_hipassembly' :
    const InertiaMatrix& LF_hipassembly_I;
    Velocity      LF_hipassembly_v;
    Acceleration  LF_hipassembly_a;
    Force         LF_hipassembly_f;
    // Link 'LF_upperleg' :
    const InertiaMatrix& LF_upperleg_I;
    Velocity      LF_upperleg_v;
    Acceleration  LF_upperleg_a;
    Force         LF_upperleg_f;
    // Link 'LF_lowerleg' :
    const InertiaMatrix& LF_lowerleg_I;
    Velocity      LF_lowerleg_v;
    Acceleration  LF_lowerleg_a;
    Force         LF_lowerleg_f;
    // Link 'RF_hipassembly' :
    const InertiaMatrix& RF_hipassembly_I;
    Velocity      RF_hipassembly_v;
    Acceleration  RF_hipassembly_a;
    Force         RF_hipassembly_f;
    // Link 'RF_upperleg' :
    const InertiaMatrix& RF_upperleg_I;
    Velocity      RF_upperleg_v;
    Acceleration  RF_upperleg_a;
    Force         RF_upperleg_f;
    // Link 'RF_lowerleg' :
    const InertiaMatrix& RF_lowerleg_I;
    Velocity      RF_lowerleg_v;
    Acceleration  RF_lowerleg_a;
    Force         RF_lowerleg_f;
    // Link 'LH_hipassembly' :
    const InertiaMatrix& LH_hipassembly_I;
    Velocity      LH_hipassembly_v;
    Acceleration  LH_hipassembly_a;
    Force         LH_hipassembly_f;
    // Link 'LH_upperleg' :
    const InertiaMatrix& LH_upperleg_I;
    Velocity      LH_upperleg_v;
    Acceleration  LH_upperleg_a;
    Force         LH_upperleg_f;
    // Link 'LH_lowerleg' :
    const InertiaMatrix& LH_lowerleg_I;
    Velocity      LH_lowerleg_v;
    Acceleration  LH_lowerleg_a;
    Force         LH_lowerleg_f;
    // Link 'RH_hipassembly' :
    const InertiaMatrix& RH_hipassembly_I;
    Velocity      RH_hipassembly_v;
    Acceleration  RH_hipassembly_a;
    Force         RH_hipassembly_f;
    // Link 'RH_upperleg' :
    const InertiaMatrix& RH_upperleg_I;
    Velocity      RH_upperleg_v;
    Acceleration  RH_upperleg_a;
    Force         RH_upperleg_f;
    // Link 'RH_lowerleg' :
    const InertiaMatrix& RH_lowerleg_I;
    Velocity      RH_lowerleg_v;
    Acceleration  RH_lowerleg_a;
    Force         RH_lowerleg_f;

    // The robot base
    const InertiaMatrix& trunk_I;
    InertiaMatrix trunk_Ic;
    Force         trunk_f;
    // The composite inertia tensors
    InertiaMatrix LF_hipassembly_Ic;
    InertiaMatrix LF_upperleg_Ic;
    const InertiaMatrix& LF_lowerleg_Ic;
    InertiaMatrix RF_hipassembly_Ic;
    InertiaMatrix RF_upperleg_Ic;
    const InertiaMatrix& RF_lowerleg_Ic;
    InertiaMatrix LH_hipassembly_Ic;
    InertiaMatrix LH_upperleg_Ic;
    const InertiaMatrix& LH_lowerleg_Ic;
    InertiaMatrix RH_hipassembly_Ic;
    InertiaMatrix RH_upperleg_Ic;
    const InertiaMatrix& RH_lowerleg_Ic;

private:

};

inline void InverseDynamics::setJointStatus(const JointState& q) const
{
    (xm->fr_LF_hipassembly_X_fr_trunk)(q);
    (xm->fr_LF_upperleg_X_fr_LF_hipassembly)(q);
    (xm->fr_LF_lowerleg_X_fr_LF_upperleg)(q);
    (xm->fr_RF_hipassembly_X_fr_trunk)(q);
    (xm->fr_RF_upperleg_X_fr_RF_hipassembly)(q);
    (xm->fr_RF_lowerleg_X_fr_RF_upperleg)(q);
    (xm->fr_LH_hipassembly_X_fr_trunk)(q);
    (xm->fr_LH_upperleg_X_fr_LH_hipassembly)(q);
    (xm->fr_LH_lowerleg_X_fr_LH_upperleg)(q);
    (xm->fr_RH_hipassembly_X_fr_trunk)(q);
    (xm->fr_RH_upperleg_X_fr_RH_hipassembly)(q);
    (xm->fr_RH_lowerleg_X_fr_RH_upperleg)(q);
}

inline void InverseDynamics::id(
    JointState& jForces, Acceleration& trunk_a,
    const Acceleration& g, const Velocity& trunk_v,
    const JointState& q, const JointState& qd, const JointState& qdd,
    const ExtForces& fext)
{
    setJointStatus(q);
    id(jForces, trunk_a, g, trunk_v,
       qd, qdd, fext);
}

inline void InverseDynamics::G_terms_fully_actuated(
    Force& baseWrench, JointState& jForces,
    const Acceleration& g, const JointState& q)
{
    setJointStatus(q);
    G_terms_fully_actuated(baseWrench, jForces, g);
}

inline void InverseDynamics::C_terms_fully_actuated(
    Force& baseWrench, JointState& jForces,
    const Velocity& trunk_v, const JointState& q, const JointState& qd)
{
    setJointStatus(q);
    C_terms_fully_actuated(baseWrench, jForces, trunk_v, qd);
}

inline void InverseDynamics::id_fully_actuated(
        Force& baseWrench, JointState& jForces,
        const Acceleration& g, const Velocity& trunk_v, const Acceleration& baseAccel,
        const JointState& q, const JointState& qd, const JointState& qdd, const ExtForces& fext)
{
    setJointStatus(q);
    id_fully_actuated(baseWrench, jForces, g, trunk_v,
        baseAccel, qd, qdd, fext);
}

}
}
}

#endif
