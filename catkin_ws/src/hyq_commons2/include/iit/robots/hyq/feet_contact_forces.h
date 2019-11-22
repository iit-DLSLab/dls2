#ifndef IIT_ROBOTS_HYQ_FEET_CONTACT_FORCES_H_
#define IIT_ROBOTS_HYQ_FEET_CONTACT_FORCES_H_

#include <iit/commons/dog/feet_contact_forces.h>
#include <iit/robots/hyq/feet_jacobians.h>
#include <iit/robots/hyq/inverse_dynamics.h>
#include <iit/robots/hyq/jsim.h>

namespace  iit {
namespace  HyQ {

/**
 * @brief The FeetContactForces class provides the functions to compute
 * the Ground Reaction Forces (GRF) at the HyQ's feet.
 */
class FeetContactForces : public iit::dog::FeetContactForces {
public:
    typedef typename dog::Vector3d Vector3d;
    typedef typename Eigen::Quaterniond Quaterniond;
    typedef typename dog::FeetJacobians FeetJacobians;
    typedef typename dog::JointState JointState;
    typedef typename dog::LegID LegID;

public:
    FeetContactForces(FeetJacobians &feet_jacobians,
                      dog::InverseDynamicsBase &inverse_dynamics, dog::JSIMBase &jsim);
    ~FeetContactForces() {}

    inline Vector3d getFootGRF(const JointState& q,
                               const JointState& qd,
                               const JointState& tau,
                               const Quaterniond& orient,
                               const LegID& leg,
                               const JointState& qdd = JointState::Zero(),
                               const Vector3d& xd = Vector3d::Zero(),
                               const Vector3d& xdd = Vector3d::Zero(),
                               const Vector3d& omega = Vector3d::Zero(),
                               const Vector3d& omegad = Vector3d::Zero()) {
        dog::Vector3d res;
        getFootGRF(q, qd, tau, orient, leg, res, qdd, xd, xdd, omega, omegad);
        return res;
    }

    bool getFootGRF(const JointState& q,
                    const JointState& qd,
                    const JointState& tau,
                    const Quaterniond& orient,
                    const LegID& leg,
                    Vector3d& foot_grf,
                    const JointState& qdd = JointState::Zero(),
                    const Vector3d& xd = Vector3d::Zero(),
                    const Vector3d& xdd = Vector3d::Zero(),
                    const Vector3d& omega = Vector3d::Zero(),
                    const Vector3d& omegad = Vector3d::Zero());

    inline bool getFeetGRF(const JointState& q,
                           const JointState& qd,
                           const JointState& tau,
                           const Quaterniond& orient,
                           dog::LegDataMap<Vector3d>& feet_grf,
                           const JointState& qdd = JointState::Zero(),
                           const Vector3d& xd = Vector3d::Zero(),
                           const Vector3d& xdd = Vector3d::Zero(),
                           const Vector3d& omega = Vector3d::Zero(),
                           const Vector3d& omegad = Vector3d::Zero()) {
        bool res_lf = getFootGRF(q, qd, tau, orient, dog::LF, feet_grf[dog::LF], qdd, xd, xdd, omega, omegad);
        bool res_rf = getFootGRF(q, qd, tau, orient, dog::RF, feet_grf[dog::RF], qdd, xd, xdd, omega, omegad);
        bool res_lh = getFootGRF(q, qd, tau, orient, dog::LH, feet_grf[dog::LH], qdd, xd, xdd, omega, omegad);
        bool res_rh = getFootGRF(q, qd, tau, orient, dog::RH, feet_grf[dog::RH], qdd, xd, xdd, omega, omegad);

        return(res_lf && res_rf && res_lh && res_rh);
    }

    inline dog::LegDataMap<Vector3d> getFeetGRF(const JointState& q,
            const JointState& qd,
            const JointState& tau,
            const Quaterniond& orient,
            const JointState& qdd = JointState::Zero(),
            const Vector3d& xd = Vector3d::Zero(),
            const Vector3d& xdd = Vector3d::Zero(),
            const Vector3d& omega = Vector3d::Zero(),
            const Vector3d& omegad = Vector3d::Zero()) {
        dog::LegDataMap<Vector3d> res;
        getFeetGRF(q, qd, tau, orient, res, qdd, xd, xdd, omega, omegad);
        return res;
    }

    /**
     * @brief setContactPoint sets the contact point w.r.t. the center origin of the lowerleg frame  (expressed in  the lowerleg frame)
     * @param foot_x
     * @param foot_y
     */
    void setContactPoint(dog::LegID leg, double user_foot_x, double user_foot_y);

private:
    dog::FeetJacobians& feet_jacobians_;
    dog::InverseDynamicsBase& inverse_dynamics_;
    dog::JSIMBase& jsim_;
    dog::LegDataMap<double> foot_x = 0.0;
    dog::LegDataMap<double> foot_y = 0.0;
};

} // namespace HyQ
} // namespace iit

#endif
