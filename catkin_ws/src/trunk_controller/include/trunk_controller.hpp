#ifndef TRUNK_CONTROLLER_HPP
#define TRUNK_CONTROLELR_HPP

#include "dls2/controller/controller.hpp"

#include <iit/commons/dog/kin_dyn_params.h>
#include <iit/commons/dog/robot_limits.h>
#include <iit/commons/dog/forward_kinematics.h>
#include <iit/commons/dog/inverse_kinematics.h>
#include <iit/commons/dog/transforms.h>
#include <iit/commons/dog/inverse_dynamics.h>
#include <iit/commons/dog/feet_jacobians.h>
#include <iit/commons/dog/jsim.h>
#include <iit/commons/dog/inertia_properties.h>
#include <iit/commons/dog/shin_jacobians.h>
#include <iit/commons/dog/robot_limits.h>
#include <iit/commons/dog/feet_contact_forces.h>
#include <iit/commons/planning/planning.h>
#include <iit/rbd/rbd.h>

//robot dependent:hyq
#include <iit/robots/hyq/forward_kinematics.h>
#include <iit/robots/hyq/inverse_kinematics.h>
#include <iit/robots/hyq/transforms.h>
#include <iit/robots/hyq/inverse_dynamics.h>
#include <iit/robots/hyq/feet_jacobians.h>
#include <iit/robots/hyq/jsim.h>
#include <iit/robots/hyq/inertia_properties.h>
#include <iit/robots/hyq/shin_jacobians.h>
#include <iit/robots/hyq/robot_limits.h>
#include <iit/robots/hyq/feet_contact_forces.h>

#include "VirtualModel.hpp"
#include "wholeBodyOptimization.h"
#include "wholeBodyOptimizationStatic.h"

// TODO robot model class

#include <ros/node_handle.h>
#include <urdf/model.h>
//Urdf Parameter Getter
//#include <dls_supervisor/urdf_params_getter.h>


enum class OptimizationType {WHOLE_BODY_STATIC = 0,
                             WHOLE_BODY_DYNAMIC,
                             NO_OPTMIMIZATION};

enum class AttractorType {WRENCH = 0, ACCELERATION};

typedef iit::dog::WholeBodyOptimization::MinMethod MinGoal;
typedef iit::dog::WholeBodyOptimization::swing_type SwingType;
typedef iit::dog::WholeBodyOptimization::ConstrViolation ConstrViolation;

/**
 * @brief The TrunkControllerParams struct incorporates all the parameters
 * the user can set for the TrunkController, including: optimization types,
 * swing types, gains, etc.
 */
struct TrunkControllerParams {
    OptimizationType opt_type = OptimizationType::WHOLE_BODY_STATIC;
    MinGoal min_goal = MinGoal::NORMALS;
    SwingType swing_type = SwingType::FORCE_BASED_JSPACE;
    AttractorType attractorType = AttractorType::WRENCH; //TODO Acceleration

    iit::rbd::Vector6D Kp_posture = iit::rbd::Vector6D::Zero();
    iit::rbd::Vector6D Kd_posture = iit::rbd::Vector6D::Zero();

    iit::rbd::Vector3d Kp_swing = iit::rbd::Vector3d::Zero() ;
    iit::rbd::Vector3d Kd_swing = iit::rbd::Vector3d::Zero() ;

    iit::rbd::ForceVector wrench_error_threshold;

    bool is_height_controlled = false;
    bool is_base_controlled = false; // robot is CoM controlled by default

    bool use_internal_stance_computation = false;

    bool use_gravity_compensation = false;

    bool damping_only = true;

    bool use_friction_constraints = true;
    bool use_torque_constraints = false;
    bool use_joint_constraints = false;

    bool verbose = false;
    bool debug = false;

    double default_des_height = 0.62;

    TrunkControllerParams() {
        wrench_error_threshold << 40.0, 40.0, 40.0, 40.0, 40.0, 400.0;
    }
};


/**
 * @brief The TrunkController class computes the joint torques (and, optionally,
 * the feet forces) to be applied to the base (or the CoM) of the robot in
 * order to have a desired wrench (or desired position + velocity) at the base
 * (or CoM)
 * @date May 13th, 2015
 * @authors Michele Focchi (michele.focchi@iit.it)
 * Marco Camurri (marco.camurri@iit.it)
 */
class TrunkController : public dls::Controller
{
public:
    typedef std::shared_ptr<TrunkController> Ptr;
    typedef std::shared_ptr<const TrunkController> ConstPtr;
    TrunkController();
    void run(const std::chrono::system_clock::time_point &time) override;
    Status eStop() override;


    /**
     * @brief init gets the parameters used by the TrunkController class
     * @param config an instance of type TrunkControllerParams incorporating
     * the internal parameters of the TrunkController, such as the type of
     * optimization used, the gains, etc.
     * @sa TrunkControllerParams
     */
    void init(const TrunkControllerParams &config);

    /**
     * @brief setOptimizationMethod lets the user choose which type of
     * optimization to perform
     * @param optimMethod an OptimizationType enum class describing the type of
     * optimization to use to stabilize the trunk. This function will take care
     * of the re-initialization of the internal optimizer.
     * @sa OptimizationType
     */
    void setOptimizationMethod(const OptimizationType&  optimMethod);
    /**
     * @brief setMinimizationGoal lets the user choose what quantity should be
     * optimized. Current options include wrench direction, ground reaction
     * forces and torques
     * @param minGoal an instance of MinGoal
     * @sa MinGoal
     */
    void setMinimizationGoal(const MinGoal&  minGoal);

    /**
     * @brief setTorqueWeights weights the importance of the joint torques for
     * all the legs
     * @param weights_leg
     */
    void setTorqueWeights(Eigen::Vector3d & weights_leg);
    /**
     * @brief setTorqueWeights weights the importance of the torques for a
     * specific leg
     * @param leg
     * @param weights_leg
     */
    void setLegTorqueWeights(const iit::dog::LegID & leg, Eigen::Vector3d & weights_leg);

    /**
     * @brief TrunkController::setSwingType set the type of swing attractor
     * @param swing_type type of swing : namely, FORCE_BASED_OSPACE=0,FORCE_BASED_JSPACE,
     * ACC_BASED_OSPACE, ACC_BASED_JSPACE
     */
    void setSwingType(SwingType swing_type);

    /**
     * @brief setSwingLegGains resets the gains for the swing phase. This is
     * used only when the WholeBody Static Optimizer is active.
     * @param Kp_swing
     * @param Kd_swing
     */
    void setSwingLegGains(Eigen::Vector3d & Kp_swing, Eigen::Vector3d & Kd_swing);

    /**
     * @brief setForceLimits constraints the magnitude and the direction of the
     * forces at the feet, as well as the static friction coefficients. This is
     * used only if the WholeBody Dynamic Optimizer is used.
     * @param high_force_limit
     * @param low_force_limit
     * @param surf_normal
     * @param mu_estimate
     */
    void setForceLimits(const iit::dog::LegDataMap<double> & high_force_limit,
                        const iit::dog::LegDataMap<double> & low_force_limit,
                        const iit::dog::LegDataMap<Eigen::Vector3d> & surf_normal,
                        const iit::dog::LegDataMap<double> & mu_estimate);

    /**
     * @brief setTorqueLimits constraints the maximum torque at the joints
     * @param tau_max
     */
    void setTorqueLimits(iit::dog::JointState & tau_max);

    /**
     * @brief setJointLimits constraints the minimum and maximum joint position
     * @param q_max
     * @param q_min
     */
    void setJointLimits(iit::dog::JointState & q_max, iit::dog::JointState & q_min);

    /**
     * @brief setSurfaceNormal sets the normal at the contact point for a single leg
     * @param leg leg index
     * @param normal normal at the contact point for leg
     */
    void setSurfaceNormal(const iit::dog::LegID leg, const Eigen::Vector3d & surf_normal);

    /**
     * @brief setSurfaceNormal sets the normals at all the contact points
     * @param normal normals at the contact points
     */
    void setSurfaceNormal(const iit::dog::LegDataMap<Eigen::Vector3d> & surf_normal);


    /**
     * @brief setFrictionCoefficient sets the friction coefficient at the contact point for a single leg
     * @param leg leg index
     * @param mu_estimate friction coefficient at the contact point for leg
     */
    void setFrictionCoefficient(const iit::dog::LegID leg, const double & mu_estimate);

    /**
     * @brief setFrictionCoefficient sets the normals at all the contact points
     * @param mu_estimate friction coefficients at the contact points
     */
    void setFrictionCoefficient(const iit::dog::LegDataMap<double> & mu_estimate);


    /**
     * @brief useInternalVirtualModel lets the user decide whether the trunk
     * controller should use an internal virtual model to compute the desired
     * wrench or not. If not, only the feed-forward wrench will be used
     * @param flag true if the TrunkController should use an internal virtual
     * model get from the user a desired base (or CoM) state. False if the
     * user should specify the wrench to be applied with setFFWDTrunkWrench().
     * @sa setFFWDTrunkWrench()
     */
    void useInternalVirtualModel(bool flag);

    /**
     * @brief return the status of use_internal_virtual_model_
     */

    bool useInternalVirtualModel();
    /**
     * @brief useInternalStanceComputation let the user decide whether the
     * TrunkController should compute the stance internally
     * @param use_internal_stance_comp
     * @deprecated
     * @warning This function is deprecated, shouldn't be used.
     */


    void useInternalStanceComputation(const bool& use_internal_stance_comp);


    void getTarget(iit::planning::Point3d & des_pos,
                   iit::planning::Point3d & des_orient);

    void getTarget(iit::planning::Point3d & des_pos,
                   iit::planning::Point3d & des_orient,
                   double & des_height);


    /**
     * @brief getActualState returns the actual state of the base (or CoM).
     * This is useful to notify it to the controllers at the moment the virtual model is reset.
     *
     * @param[out] actual_linear_state
     * @param[out] actual_angular_state
     */
    void getActualState(iit::planning::Point3d &actual_linear_state,
                        iit::planning::Point3d &actual_angular_state);

    /**
     * @brief setBaseControl let the user specify if the trunk should be
     * controlled using the base or the CoM as application point. This is used
     * only if also the internal virtual model is used.
     * @param use_base_control true if the trunk is controlled at the base,
     * false if it must be controlled in the CoM
     */
    void setBaseControl(const bool& use_base_control);


    /**
     * @brief setFFWDTrunkWrench sets a user defined wrench to be applied
     * to the base (or CoM). This should be used if the internal virtual model
     * is not set.
     * @param wrenchDes
     * @sa useInternalVirtualModel()
     */
    void setFFWDTrunkWrench(const iit::rbd::Vector6D  & wrenchDes);

    /**
     * @brief setGains set the proportional and derivative gains for the
     * internal virtual model. Does nothing if the virtual model is not used.
     * @param Kp_posture
     * @param Kd_posture
     * @sa useInternalVirtualModel()
     */
    void setGains(const iit::rbd::Vector6D  & Kp_posture,
                  const iit::rbd::Vector6D  & Kd_posture);

    /**
     * @brief setGravityCompensation sets the gravity compensation for the
     * internal virtual model. Does nothing if the virtual model is not used.
     * @param flag true if you want the gravity compensation, false otherwise.
     * @sa useInternalVirtualModel()
     */
    void setGravityCompensation(const bool & flag);

    /**
     * @brief setStanceThreshold sets the ground reaction force threshold after
     * which the stance is triggered. Used only if the stance is computed
     * internally.
     * @param th a force threshold, in newtons.
     * @sa
     */
    void setStanceThreshold(double th);

    /**
     * @brief setDesiredFeetForces sets the feet forces to be applied at the
     * robot. This should be used only if the WholeBody Static Optimizer is
     * active.
     * @param stance_legs
     * @param weight
     * @param input_desired_forces
     */
    void setDesiredFeetForces(const iit::dog::LegBoolMap & stance_legs,
                              const double weight,
                              const iit::dog::LegDataMap<Eigen::Vector3d> & input_desired_forces);

    /**
     * @brief getDesiredFeetForces returns the latest computed (or set) desired
     * feet contact forces
     * @param des_forces
     */
    void getDesiredFeetForces(iit::dog::LegDataMap<Eigen::Vector3d> & des_forces);

    /**
     * @brief getFeetForces returns the latest actual feet contact forces
     * @param feet_forces
     */
    void getFeetForces(iit::dog::LegDataMap<Eigen::Vector3d> & des_feet_forces);

        //TODO delete
    /**
     * @brief getJointTorques
     * @param stance_legs
     * @param R
     * @param baseTwist
     * @param q
     * @param qd
     * @param swingFootRef
     * @param torques
     */
    void getJointTorques(const iit::dog::LegBoolMap & stance_legs,
                         const Eigen::Matrix3d & R,
                         const iit::rbd::VelocityVector & baseTwist,
                         const iit::dog::JointState & q,
                         const iit::dog::JointState & qd,
                         const iit::dog::LegDataMap<iit::planning::Point3d> & swingFootRef,
                         iit::dog::JointState & torques);


    //TODO delete
    /**
         * @brief getJointTorques
         * @param stance_legs
         * @param R
         * @param baseTwist
         * @param q
         * @param footPos_in
         * @param JFoot_in
         * @param torques
         * @note this enables to set different feet position (e.g. when you have shin collision)
         */
    void getJointTorques(const iit::dog::LegBoolMap & stance_legs,
                         const Eigen::Matrix3d & R,
                         const iit::dog::JointState & q,
                         const iit::dog::LegDataMap<Eigen::Vector3d> & footPos_in,
                         const iit::dog::LegDataMap<iit::dog::FootJac> & JFoot_in,
                         iit::dog::JointState & torques);



    void getConstraintViolations(iit::dog::WholeBodyOptimization::ConstrViolation & constr_viol);

    void getTotalWrench(iit::rbd::Vector6D & wrench);

    void getFeedForwardWrench(iit::rbd::Vector6D & wrench);

    void getFeedBackWrench(iit::rbd::Vector6D & wrench);

    void getGains(iit::rbd::Vector6D  & Kp_posture, iit::rbd::Vector6D  & Kd_posture);

    inline bool isBaseControlled(){
        return params_.is_base_controlled;
    }

    iit::dog::LegBoolMap getActualStance();

    /**
     * @brief useDampingOnly sets the TrunkController to not use the
     * derivative terms.
     * @param use_damping_only if true, the TrunkController will consider
     * only the proportional terms for the internal virtual model
     */
    void useDampingOnly(bool use_damping_only);



    iit::dog::WholeBodyOptimization::MinMethod minGoal();

    SwingType swingType();

    //optional sets that should happen in the controller

    void setStanceLegs(const iit::dog::LegBoolMap & input_stance_legs,
                       const iit::dog::LegDataMap<Eigen::Vector3d> & feet_grf = iit::dog::LegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()));
    void getJointTorques(const Eigen::Matrix3d & R,
                         const iit::rbd::VelocityVector & baseTwist,
                         const iit::dog::JointState & q,
                         const iit::dog::JointState & qd,
                         const iit::dog::LegDataMap<Eigen::Vector3d> & footPos_in,
                         const iit::dog::LegDataMap<iit::dog::FootJac> & JFoot_in,
                         iit::dog::JointState & jointTorques);

    inline iit::dog::JointState probeJointTorques(){
        return ffwd_torques_;
    }

    inline void printRelevantConfigOptions()
    {

        std::cout<<"gravity compensation is [0/1]: " << params_.use_gravity_compensation << std::endl;
        std::cout<<"virtual model is [0/1]: " << use_internal_virtual_model_ << std::endl;
        std::cout<<"internal wrench is :"<< des_wrench_.transpose() <<std::endl;
        std::cout<<"Kp gains are : " << Kp_posture_.transpose() << std::endl;
        std::cout<<"Kd gains are : " << Kd_posture_.transpose() << std::endl;
        std::cout<<"optim type is wbstatic[0], wbdynamic[1], nooptim[2] :"<< (int) params_.opt_type <<std::endl;
        std::cout<<"min goal is Wrench[0], Normals[1], torques[2] :"<< (int) params_.min_goal <<std::endl;

    }

    inline bool getShinCollisionFlag(const iit::dog::LegID leg)
    {
        return handleShinCollision[leg];
    }
    inline void setShinCollisionFlag(const iit::dog::LegID leg, bool flag)
    {
        handleShinCollision[leg] = flag;
    }

    void getSlacks(Eigen::VectorXd & slacks_out);
    void printCosts();
    void useSlacks(bool flag);
    inline void useStanceSpringConstraints(bool flag)
    {
        if (flag)
            wb_opt_->useStanceSpringConstraints(true);
        else
            wb_opt_->useStanceSpringConstraints(false);
    }

    //switching controller variables
    double spline_duration = 2.0;
    const double dt_ = 0.001;

private:

    void createObjects();
    void computeDesiredWrench(const iit::dog::JointState &q, iit::rbd::Vector6D & des_wrench_);

    void wrenchStaticMapping(const  iit::rbd::ForceVector wrench,
                             const Eigen::Matrix3d R, const iit::dog::JointState q_curr,
                             const iit::dog::LegBoolMap & stance_legs_,
                             const iit::dog::LegDataMap<iit::dog::FootJac> &foot_jacs,
                             const iit::dog::LegDataMap<Eigen::Vector3d> & feet,
                             iit::dog::JointState& joint_torques);

    iit::dog::JointState getLegWeightTorques(const Eigen::Matrix3d Rcurr, const iit::dog::JointState & q_curr);

    void computeDesiredAcceleration();

    void computeOptimization(const iit::dog::LegBoolMap & input_stance_legs,
                             const Eigen::Matrix3d & R,
                             const iit::rbd::VelocityVector & baseTwist,
                             const iit::dog::JointState & q,
                             const iit::dog::JointState & qd,
                             const iit::dog::LegDataMap<iit::planning::Point3d> &swingFootRef,
                             const iit::rbd::ForceVector & wrenchToRealize,
                             iit::dog::JointState & optimalTorques,
                             iit::dog::LegDataMap<Eigen::Vector3d> & optimalFeetForces);

    void computeStance(const iit::dog::LegBoolMap & des_stance,
                       const iit::dog::LegDataMap<Eigen::Vector3d> &actual_grfs,
                       iit::dog::LegBoolMap & actual_stance);


	//////////////////////////// TODO This should go in model class


    // Robot Variables:
    /** @brief number of joints */
    unsigned int num_joints_;
    /** @brief number of end-effectors */
    unsigned int num_end_effectors_;
    /** @brief robot name */
    std::string robot_name_;
    /** @brief joint names */
    std::vector<std::string> joint_names_;
    std::vector<std::string> joint_names_CAPS = joint_names_; //only use if really necessary
    /** @brief joint names map */
    std::map<std::string, unsigned int> joint_names_map_;
    /** @brief end effector names */
    std::vector<std::string> end_effectors_names_;
    /** @brief joints position upper limit */
    std::vector<double> joint_upper_limits_;
    /** @brief joints position lower limit */
    std::vector<double> joint_lower_limits_;
	std::vector<double> shin_contact_position_;


    std::shared_ptr<iit::dog::InverseKinematics> ik_;
    std::shared_ptr<iit::dog::InverseDynamicsBase> inv_dyn_;
    std::shared_ptr<iit::dog::ForwardKinematics> fwd_kin_;
    std::shared_ptr<iit::dog::FeetJacobians> feet_jacobians_;
    std::shared_ptr<iit::dog::ShinJacobians> shin_jacobians_;
    std::shared_ptr<iit::dog::JSIMBase> jsim_;
    std::shared_ptr<iit::dog::InertiaPropertiesBase> inertia_props_;
    std::shared_ptr<iit::dog::HomogeneousTransformsBase> hom_transforms_;
      std::shared_ptr<iit::dog::MotionTransformsBase> motion_transforms_;
    std::shared_ptr<iit::dog::KinDynParams> robot_params_;
    std::shared_ptr<iit::dog::FeetContactForces> feet_forces_;

    std::shared_ptr<iit::dog::LimitsBase> robot_limits_;

    std::shared_ptr<iit::HyQ::Jacobians> hyq_jacobians_;
    std::shared_ptr<iit::HyQ::HomogeneousTransforms> hyq_hom_transforms_;
    std::shared_ptr<iit::HyQ::dyn::InertiaProperties> hyq_inertia_props_;
    std::shared_ptr<iit::HyQ::MotionTransforms> hyq_motion_transforms_;
    std::shared_ptr<iit::HyQ::ForceTransforms> hyq_force_transforms_;

	//////////////////////////// TODO This should go in model classs


	std::shared_ptr<iit::dog::WholeBodyOptimizationStatic > wb_opt_static_;
    std::shared_ptr<iit::dog::WholeBodyOptimization > wb_opt_;
    ConstrViolation constr_viol_;

    //trunk controller enable
    bool is_on_ = false;

    iit::dog::LegBoolMap handleShinCollision = false;


    //stance legs computation
    iit::dog::LegBoolMap stance_legs_; //memory of the stance state
    iit::dog::LegBoolMap wait_for_stance_;
    double force_th_ = 35;

    //virtual models
    iit::rbd::Vector6D Kp_posture_;
    iit::rbd::Vector6D Kd_posture_;
    Eigen::Vector3d Kp_swing_;
    Eigen::Vector3d Kd_swing_;
    //with gravity compensation
    iit::control::VirtualModel vmodel_;

    //optimization constraints
    iit::dog::LegDataMap<double> high_force_limit_;
    iit::dog::LegDataMap<double> low_force_limit_;

    iit::dog::JointState tau_max_;
    iit::dog::JointState q_max_;
    iit::dog::JointState q_min_;

    //terrain properties
    iit::dog::LegDataMap<Eigen::Vector3d> surf_normal_;
    iit::dog::LegDataMap<double>  mu_estimate_;

    //des target
    iit::planning::Point3d des_linear_state_;
    iit::planning::Point3d des_angular_state_;

    iit::planning::Point3d actual_linear_state_;
    iit::planning::Point3d actual_angular_state_;

    iit::planning::Point actual_height_;
    Eigen::Matrix3d terrain_R_world_;
    double des_height_;


    iit::dog::LegDataMap<iit::planning::Point3d> des_swing_foot_state_;

    //outputs

    iit::rbd::ForceVector des_wrench_;
    iit::rbd::ForceVector des_ffwd_wrench_;
    iit::rbd::ForceVector gravity_wrench_;
    iit::rbd::ForceVector des_vm_wrench_;
    iit::rbd::ForceVector des_vm_accel_;
    iit::rbd::ForceVector wrench_error_;

    iit::dog::JointState ffwd_torques_;
    iit::dog::LegDataMap<Eigen::Vector3d> des_feet_forces_;
    iit::dog::LegDataMap<Eigen::Vector3d>  des_forces_;
    double des_forces_weight_;

    iit::dog::LegDataMap<Eigen::Vector3d> leg_torque_weights_;


    TrunkControllerParams params_;
    bool use_internal_virtual_model_;

	bool first_time_;

};









#endif
