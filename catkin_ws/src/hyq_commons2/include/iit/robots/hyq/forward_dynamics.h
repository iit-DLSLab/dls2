#ifndef IIT_ROBOT_HYQ_FORWARD_DYNAMICS_H_
#define IIT_ROBOT_HYQ_FORWARD_DYNAMICS_H_

#include <Eigen/Dense>
#include <iit/rbd/rbd.h>
#include <iit/rbd/InertiaMatrix.h>
#include <iit/rbd/utils.h>

#include <iit/commons/dog/declarations.h>
#include "transforms.h"
#include "inertia_properties.h"
#include <iit/commons/dog/link_data_map.h>

namespace iit {
namespace HyQ {
namespace dyn {

typedef iit::rbd::InertiaMatrixDense InertiaMatrix;
typedef iit::dog::JointState JointState;

/**
 * The Forward Dynamics routine for the robot HyQ.
 *
 * The parameters common to most of the methods are the joint status \c q, the
 * joint velocities \c qd and the joint forces \c tau. The accelerations \c qdd
 * will be filled with the computed values. Overloaded methods without the \c q
 * parameter use the current configuration of the robot; they are provided for
 * the sake of efficiency, in case the kinematics transforms of the robot have
 * already been updated elsewhere with the most recent configuration (eg by a
 * call to setJointStatus()), so that it would be useless to compute them again.
 */
class ForwardDynamics {
public:
    // Convenient type aliases:
    typedef dog::LinkDataMap<iit::rbd::ForceVector> ExtForces;
    typedef iit::rbd::ForceVector Force;
    typedef iit::rbd::VelocityVector Velocity;
    typedef iit::rbd::VelocityVector Acceleration;
public:
    /**
     * Default constructor
     * \param in the inertia properties of the links
     * \param tr the container of all the spatial motion transforms of
     *     the robot HyQ, which will be used by this instance
     *     to compute the dynamics.
     */
    ForwardDynamics(InertiaProperties& in, MotionTransforms& tr);
    /** \name Forward dynamics
     * The Articulated-Body-Algorithm to compute the joint accelerations
     */ ///@{
    /**
     * \param qdd the joint accelerations vector (output parameter).
     * \param trunk_a
     * \param trunk_v
     * \param g the gravity acceleration vector, expressed in the
     *          base coordinates
     * \param q the joint status vector
     * \param qd the joint velocities vector
     * \param tau the joint forces (torque or force)
     * \param fext the external forces, optional. Each force must be
     *              expressed in the reference frame of the link it is
     *              exerted on.
     */
    void fd(
       JointState& qdd, Acceleration& trunk_a, // output parameters,
       const Velocity& trunk_v, const Acceleration& g,
       const JointState& q, const JointState& qd, const JointState& tau, const ExtForces& fext = zeroExtForces);
    void fd(
        JointState& qdd, Acceleration& trunk_a, // output parameters,
        const Velocity& trunk_v, const Acceleration& g,
        const JointState& qd, const JointState& tau, const ExtForces& fext = zeroExtForces);
    ///@}

    /** Updates all the kinematics transforms used by this instance. */
    void setJointStatus(const JointState& q) const;

private:
    InertiaProperties* inertiaProps;
    MotionTransforms* motionTransforms;

    iit::rbd::Matrix66d spareMx; // support variable
    // Link 'trunk'
    InertiaMatrix trunk_AI;
    Force trunk_p;

    // Link 'LF_hipassembly' :
    InertiaMatrix LF_hipassembly_AI;
    Velocity LF_hipassembly_a;
    Velocity LF_hipassembly_v;
    Velocity LF_hipassembly_c;
    Force    LF_hipassembly_p;

    iit::rbd::Column6d LF_hipassembly_U;
    double LF_hipassembly_D;
    double LF_hipassembly_u;
    // Link 'LF_upperleg' :
    InertiaMatrix LF_upperleg_AI;
    Velocity LF_upperleg_a;
    Velocity LF_upperleg_v;
    Velocity LF_upperleg_c;
    Force    LF_upperleg_p;

    iit::rbd::Column6d LF_upperleg_U;
    double LF_upperleg_D;
    double LF_upperleg_u;
    // Link 'LF_lowerleg' :
    InertiaMatrix LF_lowerleg_AI;
    Velocity LF_lowerleg_a;
    Velocity LF_lowerleg_v;
    Velocity LF_lowerleg_c;
    Force    LF_lowerleg_p;

    iit::rbd::Column6d LF_lowerleg_U;
    double LF_lowerleg_D;
    double LF_lowerleg_u;
    // Link 'RF_hipassembly' :
    InertiaMatrix RF_hipassembly_AI;
    Velocity RF_hipassembly_a;
    Velocity RF_hipassembly_v;
    Velocity RF_hipassembly_c;
    Force    RF_hipassembly_p;

    iit::rbd::Column6d RF_hipassembly_U;
    double RF_hipassembly_D;
    double RF_hipassembly_u;
    // Link 'RF_upperleg' :
    InertiaMatrix RF_upperleg_AI;
    Velocity RF_upperleg_a;
    Velocity RF_upperleg_v;
    Velocity RF_upperleg_c;
    Force    RF_upperleg_p;

    iit::rbd::Column6d RF_upperleg_U;
    double RF_upperleg_D;
    double RF_upperleg_u;
    // Link 'RF_lowerleg' :
    InertiaMatrix RF_lowerleg_AI;
    Velocity RF_lowerleg_a;
    Velocity RF_lowerleg_v;
    Velocity RF_lowerleg_c;
    Force    RF_lowerleg_p;

    iit::rbd::Column6d RF_lowerleg_U;
    double RF_lowerleg_D;
    double RF_lowerleg_u;
    // Link 'LH_hipassembly' :
    InertiaMatrix LH_hipassembly_AI;
    Velocity LH_hipassembly_a;
    Velocity LH_hipassembly_v;
    Velocity LH_hipassembly_c;
    Force    LH_hipassembly_p;

    iit::rbd::Column6d LH_hipassembly_U;
    double LH_hipassembly_D;
    double LH_hipassembly_u;
    // Link 'LH_upperleg' :
    InertiaMatrix LH_upperleg_AI;
    Velocity LH_upperleg_a;
    Velocity LH_upperleg_v;
    Velocity LH_upperleg_c;
    Force    LH_upperleg_p;

    iit::rbd::Column6d LH_upperleg_U;
    double LH_upperleg_D;
    double LH_upperleg_u;
    // Link 'LH_lowerleg' :
    InertiaMatrix LH_lowerleg_AI;
    Velocity LH_lowerleg_a;
    Velocity LH_lowerleg_v;
    Velocity LH_lowerleg_c;
    Force    LH_lowerleg_p;

    iit::rbd::Column6d LH_lowerleg_U;
    double LH_lowerleg_D;
    double LH_lowerleg_u;
    // Link 'RH_hipassembly' :
    InertiaMatrix RH_hipassembly_AI;
    Velocity RH_hipassembly_a;
    Velocity RH_hipassembly_v;
    Velocity RH_hipassembly_c;
    Force    RH_hipassembly_p;

    iit::rbd::Column6d RH_hipassembly_U;
    double RH_hipassembly_D;
    double RH_hipassembly_u;
    // Link 'RH_upperleg' :
    InertiaMatrix RH_upperleg_AI;
    Velocity RH_upperleg_a;
    Velocity RH_upperleg_v;
    Velocity RH_upperleg_c;
    Force    RH_upperleg_p;

    iit::rbd::Column6d RH_upperleg_U;
    double RH_upperleg_D;
    double RH_upperleg_u;
    // Link 'RH_lowerleg' :
    InertiaMatrix RH_lowerleg_AI;
    Velocity RH_lowerleg_a;
    Velocity RH_lowerleg_v;
    Velocity RH_lowerleg_c;
    Force    RH_lowerleg_p;

    iit::rbd::Column6d RH_lowerleg_U;
    double RH_lowerleg_D;
    double RH_lowerleg_u;
private:
    static const ExtForces zeroExtForces;
};

inline void ForwardDynamics::setJointStatus(const JointState& q) const {
    (motionTransforms -> fr_LF_hipassembly_X_fr_trunk)(q);
    (motionTransforms -> fr_LF_upperleg_X_fr_LF_hipassembly)(q);
    (motionTransforms -> fr_LF_lowerleg_X_fr_LF_upperleg)(q);
    (motionTransforms -> fr_RF_hipassembly_X_fr_trunk)(q);
    (motionTransforms -> fr_RF_upperleg_X_fr_RF_hipassembly)(q);
    (motionTransforms -> fr_RF_lowerleg_X_fr_RF_upperleg)(q);
    (motionTransforms -> fr_LH_hipassembly_X_fr_trunk)(q);
    (motionTransforms -> fr_LH_upperleg_X_fr_LH_hipassembly)(q);
    (motionTransforms -> fr_LH_lowerleg_X_fr_LH_upperleg)(q);
    (motionTransforms -> fr_RH_hipassembly_X_fr_trunk)(q);
    (motionTransforms -> fr_RH_upperleg_X_fr_RH_hipassembly)(q);
    (motionTransforms -> fr_RH_lowerleg_X_fr_RH_upperleg)(q);
}

inline void ForwardDynamics::fd(
    JointState& qdd, Acceleration& trunk_a, // output parameters,
    const Velocity& trunk_v, const Acceleration& g,
    const JointState& q,
    const JointState& qd,
    const JointState& tau,
    const ExtForces& fext/* = zeroExtForces */)
{
    setJointStatus(q);
    fd(qdd, trunk_a, trunk_v, g, qd, tau, fext);
}

}
}
}

#endif
