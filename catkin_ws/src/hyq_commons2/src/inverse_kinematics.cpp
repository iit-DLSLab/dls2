#include <iit/rbd/rbd.h>
#include <iit/commons/dog/joint_id_tricks.h>

#include <iit/robots/hyq/inverse_kinematics.h>
#include <iit/commons/dog/joint_data_map.h>
#include <iit/robots/hyq/constants.h>
#include <iit/commons/dog/joint_id_tricks.h>
#include <limits>


namespace iit {
namespace HyQ {

using namespace iit::dog;


InverseKinematics::InverseKinematics(dog::KinDynParams& param_getter) :
    haa_sign_flip(1), kfe_sign_flip(1),
    haaXOffset_sign_flip(1), haaYOffset_sign_flip(1),
    bodyLengths(param_getter),
    jacobians_(param_getter),
    q_min_(JointState::Constant(-10.0)),
    q_max_(JointState::Constant(10.0))

{
    feet_jacobians_[LF] = &jacobians_.fr_trunk_J_LF_foot;
    feet_jacobians_[RF] = &jacobians_.fr_trunk_J_RF_foot;
    feet_jacobians_[LH] = &jacobians_.fr_trunk_J_LH_foot;
    feet_jacobians_[RH] = &jacobians_.fr_trunk_J_RH_foot;


    haa_sign_flip[RF] = haa_sign_flip[RH] = -1;
    kfe_sign_flip[LH] = kfe_sign_flip[RH] = -1;

    haaXOffset_sign_flip[LH] = haaXOffset_sign_flip[RH] = -1;
    haaYOffset_sign_flip[RF] = haaYOffset_sign_flip[RH] = -1;
}

bool InverseKinematics::getJointPosition(const FootPosition &foot_position,
                                              const LegID& leg_id,
                                              LegJointState& q_leg,
                                              bool clamp,
                                              LegJointBool &q_violation_leg) const
{
    using rbd::X;
    using rbd::Y;
    using rbd::Z;

    double lowleg_length = bodyLengths.getFoot_x();
    double lowleg_length_sqr = lowleg_length * lowleg_length;

    double temp;
    rbd::Vector3d foot_haa_frame(foot_position);
    foot_haa_frame(X) -= (haaXOffset_sign_flip[leg_id] * bodyLengths.getHAA_x());
    foot_haa_frame(Y) -= (haaYOffset_sign_flip[leg_id] * bodyLengths.getHAA_y());

    // The line you see when looking at the Flexion-Extension plane from the back of the leg
    rbd::Vector3d fe_plane_line(0.0, foot_haa_frame(Y), foot_haa_frame(Z));

    // HAA-to-foot distance in the YZ plane, that is, the plan orthogonal to the sagittal one
    double haa2foot_yz = std::sqrt( foot_haa_frame(Y) * foot_haa_frame(Y) +
                                    foot_haa_frame(Z) * foot_haa_frame(Z) );

    // HFE-to-foot distance in the YZ plane; computed below
    double hfe2foot_yz = 0;
    double haa_hfe = bodyLengths.getDist_HAA_HFE();
    if( foot_haa_frame(Z) > 0 ) {
        // The damn foot is above the HAA origin. Just invert the vector to go
        // back to the negative half-plane. That would not change the HAA angle
        fe_plane_line = - fe_plane_line;
        // In addition, the HFE-to-foot distance has to be computed like this
        hfe2foot_yz = haa2foot_yz + haa_hfe;
    } else {
        hfe2foot_yz = haa2foot_yz - haa_hfe;
    }

    // HFE-to-foot distance, squared and plain
    double hfe2foot_sqr = hfe2foot_yz * hfe2foot_yz + foot_haa_frame(X) * foot_haa_frame(X);
    double hfe2foot = std::sqrt( hfe2foot_sqr );


    //
    // HAA
    //
    q_leg(HAA) = - std::atan2(fe_plane_line(Y), -fe_plane_line(Z)) * haa_sign_flip[leg_id];

    //
    // HFE
    //

    temp = (upleg_length_sqr + hfe2foot_sqr - lowleg_length_sqr) / (2 * upleg_length * hfe2foot);
    temp = (temp > 1 ? 1 : (temp < -1 ? -1 : temp));

    // Foot X coordinate in the HAA frame; swap the sign if necessary to map it
    //  to the LF_HAA frame, so that we can use the equations that work with the
    //  geometry of the LF leg only
    double foot_x = foot_haa_frame(X) * kfe_sign_flip[leg_id];

    // Distinguish whether the foot is above or below the HFE line in the FE
    //  plane (the line passing through HFE at an angle of PI/2 in HFE conventions)
    if(haa2foot_yz < haa_hfe) {
        q_leg(HFE) = - M_PI + std::asin( foot_x / hfe2foot ) + std::acos( temp );
    } else {
        q_leg(HFE) = - std::asin( foot_x / hfe2foot ) + std::acos( temp );
    }
    // Now swap the sign again to account for the actual leg
    q_leg(HFE) = q_leg(HFE) * kfe_sign_flip[leg_id]; // not an error, use the same sign_flip of KFE

    //
    // KFE
    //

    const double twiceUpperLower = 2 * upleg_length * lowleg_length;
    // Compute the argument of the arc cosine; because of numerical errors, it
    //  might be slightly outside [-1,1], therefore I cap it.
    temp = (upleg_length_sqr + lowleg_length_sqr - hfe2foot_sqr) / twiceUpperLower;
    temp = (temp > 1 ? 1 : (temp < -1 ? -1 : temp));
    q_leg(KFE) = (-M_PI + std::acos(temp)) * kfe_sign_flip[leg_id];

    //Check if the outputs are inf or nan
    for (int joint = LegJoints::HAA; joint <= LegJoints::KFE; joint++) {
        if (!std::isfinite(q_leg(joint))) {
            std::cerr << "Position of joint " << joint << " and leg " << leg_id
                      << " is not finite !!!" << std::endl;
            return false;
        }
    }


    for (int leg_joint = HAA; leg_joint <= KFE; leg_joint++){
        q_violation_leg(leg_joint) = false;
        if (q_leg(leg_joint) > getLegJointState(leg_id, q_max_)(leg_joint)){
            if (clamp){
                q_leg(leg_joint) = getLegJointState(leg_id, q_max_)(leg_joint);
            }
            q_violation_leg(leg_joint) = true;
        }

        if (q_leg(leg_joint) < getLegJointState(leg_id, q_min_)(leg_joint)){
            if(clamp){
                q_leg(leg_joint) = getLegJointState(leg_id, q_min_)(leg_joint);
            }
            q_violation_leg(leg_joint) = true;
        }
    }

    return true;
}


bool InverseKinematics::getJointVelocity(const FootVelocity &foot_velocity,
        const LegID &leg_id,
        const Eigen::Vector3d &q_leg,
        Eigen::Vector3d &qd_leg) {

    JointState q_dummy = JointState::Zero();
    setLegJointState(leg_id, q_leg, q_dummy);

    jacobians_.updateParameters();//just in case the getter has changed...
    jacobians_.fr_trunk_J_LF_foot(q_dummy);
    jacobians_.fr_trunk_J_RF_foot(q_dummy);
    jacobians_.fr_trunk_J_LH_foot(q_dummy);
    jacobians_.fr_trunk_J_RH_foot(q_dummy);


    LegJointState my_qd_leg = (*feet_jacobians_[leg_id]).block<3, 3>(rbd::LX, 0).inverse() * foot_velocity;

    if(my_qd_leg.allFinite()){
        qd_leg = my_qd_leg;
        return true;
    } else {
        std::cerr << "[InverseKinematics::getJointVelocity] A joint velocity ";
        std::cerr << "for leg "<< leg_id << " is not finite!!!" << std::endl;
    }

    return true;
}

bool InverseKinematics::getJointState(const FootPosition& foot_position,
                                           const FootVelocity& foot_velocity,
                                           const FootAcceleration &foot_acceleration,
                                           const dog::LegID &leg_id,
                                           Eigen::Vector3d &q_leg,
                                           Eigen::Vector3d &qd_leg,
                                           Eigen::Vector3d &qdd_leg,
                                           bool clamp,
                                           dog::LegJointBool& q_violation_leg)
{
    // if we can compute position
    if(getJointPosition(foot_position, leg_id, q_leg, clamp, q_violation_leg)) {
        // and we can compute velocity
        if(getJointVelocity(foot_velocity, leg_id, q_leg, qd_leg)) {
            // compute inverse kinematics for joint accelerations
            qdd_leg = (*feet_jacobians_[leg_id]).block<3, 3>(rbd::LX, 0).inverse() *
                                             (foot_acceleration - (1.0/dt_)*((*feet_jacobians_[leg_id]).block<3, 3>(rbd::LX, 0)
                                                     - old_feet_jacobians_[leg_id]) * foot_velocity);

            old_feet_jacobians_[leg_id] = (*feet_jacobians_[leg_id]).block<3, 3>(rbd::LX, 0);

            //Check if the outputs are inf or nan
            for (int joint = dog::LegJoints::HAA; joint <= dog::LegJoints::KFE; joint++) {
                if (!std::isfinite(qdd_leg(joint))) {
                    std::cerr << "Acceleration of joint " << joint << " and leg " << leg_id
                              << " is not finite !!!" << std::endl;
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    return false;
}

bool InverseKinematics::getJointState(const FootPositions &foot_positions,
                                           const FootVelocities &foot_velocities,
                                           const FootAccelerations &foot_accelerations,
                                           JointState &q,
                                           JointState &qd,
                                           JointState &qdd,
                                           bool clamp)
{
    bool ret = true;
    for(int leg = dog::LF; leg<= dog::RH; leg++){
        LegJointState q_leg;
        LegJointState qd_leg;
        LegJointState qdd_leg;

        ret = ret && getJointState(foot_positions[leg],
                                   foot_velocities[leg],
                                   foot_accelerations[leg],
                                   LegID(leg),
                                   q_leg, qd_leg, qdd_leg, clamp);
        setLegJointState(LegID(leg),q_leg,q);
        setLegJointState(LegID(leg),qd_leg,qd);
        setLegJointState(LegID(leg),qdd_leg,qdd);
    }
    return ret;
}

bool InverseKinematics::getJointState(const FootPositions &foot_positions,
                                           const FootVelocities &foot_velocities,
                                           const FootAccelerations &foot_accelerations,
                                           JointState &q,
                                           JointState &qd,
                                           JointState &qdd,
                                           bool clamp,
                                           dog::JointBoolMap& q_violation)
{
    bool ret = true;
    for(int leg = dog::LF; leg<=RH; leg++){
        LegJointState q_leg;
        LegJointState qd_leg;
        LegJointState qdd_leg;
        LegJointBool q_violation_leg;
        ret = ret && getJointState(foot_positions[leg],
                                   foot_velocities[leg],
                                   foot_accelerations[leg],
                                   LegID(leg),
                                   q_leg, qd_leg, qdd_leg, clamp, q_violation_leg);

        setLegJointState(LegID(leg), q_leg, q);
        setLegJointState(LegID(leg), qd_leg, qd);
        setLegJointState(LegID(leg), qdd_leg, qdd);

        for(int leg_link = HAA; leg_link <= KFE; leg_link++){
            q_violation[toJointID(LegID(leg),LegJoints(leg_link))] =
                    q_violation_leg(LegJoints(leg_link));
        }

    }
    return ret;
}

void InverseKinematics::setKneeConfiguration(const LegBoolMap &is_knee_backward){
    for(int leg = LF; leg <= RH; leg++){
        is_knee_backward[leg] ? kfe_sign_flip[leg] = 1 : kfe_sign_flip[leg] = -1;
    }
}

dog::LegBoolMap InverseKinematics::calculate(const dog::LegDataMap<rbd::Vector3d>& des_foot_pos,
                                             const dog::LegDataMap<rbd::Vector3d>& des_foot_vel,
                                             const dog::LegDataMap<rbd::Vector3d>& des_foot_accel,
                                             dog::LegDataMap<rbd::Vector3d>& des_joint_pos,
                                             dog::LegDataMap<rbd::Vector3d>& des_joint_vel,
                                             dog::LegDataMap<rbd::Vector3d>& des_joint_accel,
                                             const JointState &q_)
{
    // START hyq_ik package's private members.
    double taskServoRate = dt_;
    Eigen::Matrix<double, 6,3 >* JFootLF = feet_jacobians_[LF];
    Eigen::Matrix<double, 6,3 >* JFootRF = feet_jacobians_[RF];
    Eigen::Matrix<double, 6,3 >* JFootLH = feet_jacobians_[LH];
    Eigen::Matrix<double, 6,3 >* JFootRH = feet_jacobians_[RH];
    // END hyq_ik package's private members.

    static dog::LegDataMap<rbd::Vector3d> aux_foot_pos;
    static dog::LegDataMap<rbd::Matrix33d> last_JFoot;
    static dog::LegDataMap<double> hfe2foot;
    dog::LegBoolMap joint_range_check(true);
    static dog::LegDataMap<rbd::Vector3d> BASE2HAA_offsets;

    //TODO: Should be from Model
    static double LUPPER = 0.35;
    static double LLOWER = 0.33;
    static double HAA2HFEZ = -0.08;

    // center of base distance from leg hip in x direction
    static double BASE2HAAX  = 0.3735;
    // center of base distance from leg hip in y direction
    static double BASE2HAAY  = 0.207;
    static double ZOFFSET = 0.0;

    BASE2HAA_offsets[dog::LF] << BASE2HAAX, BASE2HAAY, ZOFFSET;
    BASE2HAA_offsets[dog::RF] << BASE2HAAX, -BASE2HAAY, ZOFFSET;
    BASE2HAA_offsets[dog::LH] << -BASE2HAAX, BASE2HAAY, ZOFFSET;
    BASE2HAA_offsets[dog::RH] << -BASE2HAAX, -BASE2HAAY, ZOFFSET;

    //Joint limits
    static double min_hfe_angle_front_legs = -50 * M_PI / 180; //this must be gotten from the model
    static double max_hfe_angle_front_legs = 70 * M_PI / 180; //this must be gotten from the model
    static double min_hfe_angle_hind_legs = -70 * M_PI / 180; //this must be gotten from the model
    static double max_hfe_angle_hind_legs = 50 * M_PI / 180; //this must be gotten from the model
    static double min_knee_angle = 25 * M_PI / 180; //absolute value //this must be gotten from the model
    static double max_knee_angle = 133 * M_PI / 180; //absolute value //this must be gotten from the model

    //Limits imposed by the minimum and maximum knee angles
    static double hfe2foot_min = sqrt(LLOWER * LLOWER + LUPPER * LUPPER - 2 * LLOWER * LUPPER * cos(M_PI - max_knee_angle));
    static double hfe2foot_max = sqrt(LLOWER * LLOWER + LUPPER * LUPPER - 2 * LLOWER * LUPPER * cos(M_PI - min_knee_angle));


    //Compute inverse kinematics for joint positions
    for (int leg = dog::LF; leg <= dog::RH; leg++){

        aux_foot_pos[leg] = des_foot_pos[leg] - BASE2HAA_offsets[leg];

        //Note that HAA2HFEZ is negative;
        hfe2foot[leg] = sqrt(aux_foot_pos[leg](rbd::Y) * aux_foot_pos[leg](rbd::Y) +
                aux_foot_pos[leg](rbd::Z) * aux_foot_pos[leg](rbd::Z)) + HAA2HFEZ;
        hfe2foot[leg] = sqrt(hfe2foot[leg] * hfe2foot[leg] +
                             aux_foot_pos[leg](rbd::X) * aux_foot_pos[leg](rbd::X));

        //Constraint according to joint limits
        if(hfe2foot[leg] < hfe2foot_min) {
            //std::cout << "Reached KFE joint limit of Leg " << leg << std::endl;
            //std::cout << "required hfe2foot: " << hfe2foot[leg] << std::endl;
            //std::cout << "minimum hfe2foot: " << hfe2foot_min << std::endl << std::endl;
            hfe2foot = hfe2foot_min;
        }

        if(hfe2foot[leg] > hfe2foot_max) {
            //std::cout << "Reached KFE joint limit of Leg " << leg << std::endl;
            //std::cout << "required hfe2foot: " << hfe2foot[leg] << std::endl;
            //std::cout << "maximum hfe2foot: " << hfe2foot_max << std::endl << std::endl;
            hfe2foot = hfe2foot_max;
        }


        //HAA joints
        if((leg == dog::LF) || (leg == dog::LH))
            des_joint_pos[leg](dog::LegJoints::HAA) = -atan2(aux_foot_pos[leg](rbd::Y), -aux_foot_pos[leg](rbd::Z));
        //std::cout << "HAA Leg: " << leg << "  pos: " << des_joint_pos[leg](dog::LegJoints::HAA) << std::endl;


        if((leg == dog::RF) || (leg == dog::RH))
            des_joint_pos[leg](dog::LegJoints::HAA) = -atan2(-aux_foot_pos[leg](rbd::Y), -aux_foot_pos[leg](rbd::Z));
        //std::cout << "HAA Leg: " << leg << "  pos: " << des_joint_pos[leg](dog::LegJoints::HAA) << std::endl;


        //HFE and KFE joints
        if((leg == dog::LF) || (leg == dog::RF)) {

            des_joint_pos[leg](dog::LegJoints::KFE) = - M_PI +
                    acos((LUPPER * LUPPER + LLOWER * LLOWER - hfe2foot[leg] * hfe2foot[leg]) / (2 * LUPPER * LLOWER));

            des_joint_pos[leg](dog::LegJoints::HFE) = -asin(aux_foot_pos[leg](rbd::X)/hfe2foot[leg]) +
                    acos((LUPPER * LUPPER + hfe2foot[leg] * hfe2foot[leg] - LLOWER * LLOWER) / (2 * LUPPER * hfe2foot[leg]));

            //Constraint according to joint limits
            if(des_joint_pos[leg](dog::LegJoints::HFE) < min_hfe_angle_front_legs){
                des_joint_pos[leg](dog::LegJoints::HFE) = min_hfe_angle_front_legs;
                //std::cout << "Reached minimum HFE joint limit of Leg " << leg << std::endl;
            }
            if(des_joint_pos[leg](dog::LegJoints::HFE) > max_hfe_angle_front_legs){
                des_joint_pos[leg](dog::LegJoints::HFE) = max_hfe_angle_front_legs;
                //std::cout << "Reached maximum HFE joint limit of Leg " << leg << std::endl;
            }
        }

        if((leg == dog::LH) || (leg == dog::RH)) {

            des_joint_pos[leg](dog::LegJoints::KFE) = + M_PI -
                    acos((LUPPER * LUPPER + LLOWER * LLOWER - hfe2foot[leg] * hfe2foot[leg]) / (2 * LUPPER * LLOWER));

            des_joint_pos[leg](dog::LegJoints::HFE) = -asin(aux_foot_pos[leg](rbd::X)/hfe2foot[leg]) -
                    acos((LUPPER * LUPPER + hfe2foot[leg] * hfe2foot[leg] - LLOWER * LLOWER) / (2 * LUPPER * hfe2foot[leg]));

            //Constraint according to joint limits
            if(des_joint_pos[leg](dog::LegJoints::HFE) < min_hfe_angle_hind_legs){
                des_joint_pos[leg](dog::LegJoints::HFE) = min_hfe_angle_hind_legs;
                //std::cout << "Reached minimum HFE joint limit of Leg " << leg << std::endl;
            }
            if(des_joint_pos[leg](dog::LegJoints::HFE) > max_hfe_angle_hind_legs){
                des_joint_pos[leg](dog::LegJoints::HFE) = max_hfe_angle_hind_legs;
                //std::cout << "Reached maximum HFE joint limit of Leg " << leg << std::endl;
            }
        }

    }


    //Compute inverse kinematics for joint velocities
    Eigen::Matrix<double, 12,1> aux_joint_pos;
    aux_joint_pos.segment<3>(0) = des_joint_pos[dog::LF];
    aux_joint_pos.segment<3>(3) = des_joint_pos[dog::RF];
    aux_joint_pos.segment<3>(6) = des_joint_pos[dog::LH];
    aux_joint_pos.segment<3>(9) = des_joint_pos[dog::RH];

    jacobians_.fr_trunk_J_LF_foot(q_);
    jacobians_.fr_trunk_J_RF_foot(q_);
    jacobians_.fr_trunk_J_LH_foot(q_);
    jacobians_.fr_trunk_J_RH_foot(q_);

    des_joint_vel[dog::LF] = ((*JFootLF).block<3,3>(rbd::LX,0)).inverse() * des_foot_vel[dog::LF];
    des_joint_vel[dog::RF] = ((*JFootRF).block<3,3>(rbd::LX,0)).inverse() * des_foot_vel[dog::RF];
    des_joint_vel[dog::LH] = ((*JFootLH).block<3,3>(rbd::LX,0)).inverse() * des_foot_vel[dog::LH];
    des_joint_vel[dog::RH] = ((*JFootRH).block<3,3>(rbd::LX,0)).inverse() * des_foot_vel[dog::RH];

    //Compute inverse kinematics for joint accelerations
    des_joint_accel[dog::LF] = (*JFootLF).block<3,3>(rbd::LX,0).inverse() * (des_foot_accel[dog::LF]
            - (double)taskServoRate * ((*JFootLF).block<3,3>(rbd::LX,0)
                                       - last_JFoot[dog::LF]) * des_foot_vel[dog::LF]);

    last_JFoot[dog::LF] = (*JFootLF).block<3,3>(rbd::LX,0);

    des_joint_accel[dog::RF] = (*JFootRF).block<3,3>(rbd::LX,0).inverse() * (des_foot_accel[dog::RF]
            - (double)taskServoRate * ((*JFootRF).block<3,3>(rbd::LX,0)
                                       - last_JFoot[dog::RF]) * des_joint_vel[dog::RF]);

    last_JFoot[dog::RF] = (*JFootRF).block<3,3>(rbd::LX,0);

    des_joint_accel[dog::LH] = (*JFootLH).block<3,3>(rbd::LX,0).inverse() * (des_foot_accel[dog::LH]
            - (double)taskServoRate * ((*JFootLH).block<3,3>(rbd::LX,0)
                                       - last_JFoot[dog::LH]) * des_joint_vel[dog::LH]);

    last_JFoot[dog::LH] = (*JFootLH).block<3,3>(rbd::LX,0);

    des_joint_accel[dog::RH] = (*JFootRH).block<3,3>(rbd::LX,0).inverse() * (des_foot_accel[dog::RH]
            - (double)taskServoRate * ((*JFootRH).block<3,3>(rbd::LX,0)
                                       - last_JFoot[dog::RH]) * des_joint_vel[dog::RH]);

    last_JFoot[dog::RH] = (*JFootRH).block<3,3>(rbd::LX,0);


    //Re-assigning Jacobians according to actual joint position.
    jacobians_.fr_trunk_J_LF_foot(q_);
    jacobians_.fr_trunk_J_RF_foot(q_);
    jacobians_.fr_trunk_J_LH_foot(q_);
    jacobians_.fr_trunk_J_RH_foot(q_);


    //Check if the outputs are inf or nan
    for (int leg = dog::LF; leg <= dog::RH; leg++){
        for (int joint = dog::LegJoints::HAA; joint <= dog::LegJoints::KFE; joint++) {
            if (!std::isfinite(des_joint_pos[leg](joint))) {
                joint_range_check[leg] = false;
                std::cout << "Position of joint " << joint << " of leg " << leg
                          << " is " << des_joint_pos[leg](joint) << " !!!" << std::endl;
            }

            if (!std::isfinite(des_joint_vel[leg](joint))) {
                joint_range_check[leg] = false;
                std::cout << "Velocity of joint " << joint << " of leg " << leg
                          << " is " << des_joint_vel[leg](joint) << " !!!" << std::endl;

            }
            if (!std::isfinite(des_joint_accel[leg](joint))) {
                joint_range_check[leg] = false;
                std::cout << "Acceleration of joint " << joint << " of leg " << leg
                          << " is " << des_joint_accel[leg](joint) << " !!!" << std::endl;
            }
        }

    }

    return joint_range_check;
}
}
}
