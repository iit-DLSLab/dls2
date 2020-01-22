#include "trunk_controller.hpp"
#include <commons/geometry/algebra.h>
#include <commons/geometry/rotations.h>
#include "computeJacobians.h"
#include <doglib/rbd/rbd.h>
//#include <iit/robots/hyq/default_parameters_getter.h>
#include <parameters_getter.h>
//#include <iit/locomotionutils/parameters_getter.h>

using namespace Eigen;
using namespace dls::rbd;
using namespace dls::dog;
using namespace dls::commons;

TrunkController::TrunkController(std::shared_ptr<dls::dog::Dog> pDog) :
    Controller
    (
		pDog,
		"dls_trunk_controller",
		std::chrono::duration<double>(dt_),
        dls::ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
        ),

    mu_estimate_(0.8),
    tau_max_(dls::dog::JointState::Constant(1e03)),
    q_max_(dls::dog::JointState::Constant(10)),
    q_min_(dls::dog::JointState::Constant(-10)),
    high_force_limit_(2.0e10),
    low_force_limit_(5.0),
    surf_normal_(Vector3d(0.0, 0.0, 1.0)),
    des_vm_wrench_(dls::rbd::ForceVector::Zero()),
    des_wrench_(dls::rbd::ForceVector::Zero()),
    des_ffwd_wrench_(dls::rbd::ForceVector::Zero()),
    gravity_wrench_(dls::rbd::ForceVector::Zero()),
    stance_legs_(true),
    wait_for_stance_(false),
    Kp_posture_(dls::rbd::Vector6D::Zero()),
    Kd_posture_(dls::rbd::Vector6D::Zero()),
    wrench_error_(dls::rbd::ForceVector::Zero()),
    use_internal_virtual_model_(false),
leg_torque_weights_(Eigen::Vector3d(1, 5 , 0.2)),
first_time_(true)

{

    std::cout << "Initializing DLS trunkcontroller" << std::endl;

    //	int argc = 0;
    //	ros::init(argc, nullptr, getID());
    //	ros::NodeHandle controller_nh;
    //	if (!controller_nh.getParam("robot_name", robot_name_))
    //	{
    //		ROS_ERROR("No robot_name option given in: %s.", controller_nh.getNamespace().c_str());
    //		return;
    //	}

    //	// getting the names of the joints from ROS parameter pool
    //	if (!controller_nh.getParam("joints", joint_names_))
    //	{
    //		ROS_ERROR("No joints given in the namespace: %s.", controller_nh.getNamespace().c_str());
    //		return;
    //	}

    //	if (!controller_nh.getParam("end_effectors", end_effectors_names_))
    //	{
    //		ROS_ERROR("No end_effectors given in the namespace: %s.", controller_nh.getNamespace().c_str());
    //		return;
    //	}

    //	ros::NodeHandle nh; //Nodehandle without prefix
    //	std::string robot_description;
    //	if (!nh.getParam("robot_description", robot_description))
    //	{
    //		ROS_ERROR("Could not find robot_description.");
    //		return;
    //	}
    //	urdf::Model robot_model;
    //	if (!robot_model.initString(robot_description))
    //	{
    //		ROS_ERROR("Failed to parse urdf file");
    //		return;
    //	}


    //robot_params_.reset(new dls::dog::UrdfParamsGetter(robot_model));
    robot_params_.reset(new dls::HyQ::TestParamsGetter());
	
	
    //robot_params_.reset(new dls::dog::HyQ::DefaultParamsGetter());

    hyq_jacobians_.reset(new dls::dog::HyQ::Jacobians(*robot_params_));
    hyq_hom_transforms_.reset(new dls::dog::HyQ::HomogeneousTransforms(*robot_params_));
    hyq_inertia_props_.reset(new dls::dog::HyQ::dyn::InertiaProperties(*robot_params_));
    hyq_motion_transforms_.reset(new dls::dog::HyQ::MotionTransforms(*robot_params_));
    hyq_force_transforms_.reset(new dls::dog::HyQ::ForceTransforms(*robot_params_));


    ik_.reset(new dls::dog::HyQ::InverseKinematics(*robot_params_));
    inv_dyn_.reset(new dls::dog::HyQ::dyn::InverseDynamics(*hyq_inertia_props_,*hyq_motion_transforms_));
    fwd_kin_.reset(new dls::dog::HyQ::ForwardKinematics(*robot_params_));
    feet_jacobians_.reset(new dls::dog::HyQ::FeetJacobians(*hyq_jacobians_));
    shin_jacobians_.reset(new dls::dog::HyQ::ShinJacobians(*robot_params_));
    jsim_.reset(new dls::dog::HyQ::dyn::JSIM(*hyq_inertia_props_,*hyq_force_transforms_));
    inertia_props_.reset(hyq_inertia_props_.get());
    hom_transforms_.reset(hyq_hom_transforms_.get());
    motion_transforms_.reset(hyq_motion_transforms_.get());
    robot_limits_.reset(new dls::dog::HyQ::Limits());
    feet_forces_.reset(new dls::dog::HyQ::FeetContactForces(*feet_jacobians_, *inv_dyn_, *jsim_));




    //additional trunk controller config (necessary anyway to switch bw controllers)
    TrunkControllerParams config;


    config.attractorType = AttractorType::WRENCH;
    config.damping_only = true;
    config.is_base_controlled = false;
    config.is_height_controlled = false;

    config.min_goal = MinGoal::NORMALS;
    config.opt_type = OptimizationType::WHOLE_BODY_STATIC;
    config.use_friction_constraints = true;
    config.use_torque_constraints = false;
    config.use_joint_constraints = false;


    init(config);

    useInternalVirtualModel(false);
    setGravityCompensation(true);
    setJointLimits(q_max_, q_min_);


    switch(params_.opt_type)
    {
    case OptimizationType::NO_OPTMIMIZATION:
        std::cout<<"trunk controller started without optimization [2]"<<std::endl;
        break;
    case OptimizationType::WHOLE_BODY_DYNAMIC:
        std::cout<<"Whole Body Optimization ON Dynamic [1]"<<std::endl;
        break;
    case OptimizationType::WHOLE_BODY_STATIC:
        std::cout<<"Whole Body Optimization ON Static [0]"<<std::endl;
        break;
    default:
        break;
    }

}

void TrunkController::run(const std::chrono::system_clock::time_point &time)
{



    //add some desired wrench
    //setFFWDTrunkWrench(wrench);
    dls::dog::LegBoolMap stance_legs(true);

    dls::rbd::Vector6D baseTwist, comTwist;
    Eigen::Matrix3d b_R_w = Eigen::Matrix3d::Identity();
    dls::dog::JointState q_curr, qd_curr, des_q, des_qd;
	iit::planning::Point3d  actual_base, actual_CoM, desired_com_pos, desired_base_pos;

    // TODO move this memory assignment
    auto pBlind_state_signal = this->readBlindStateSignal();
    if(!pBlind_state_signal)
    {
        std::cout << "return early -- blind" << std::endl;
        return;
    }


    //extract variables
    q_curr = pBlind_state_signal->joint_state.position;
    qd_curr = pBlind_state_signal->joint_state.velocity;


    //orientation
	actual_angular_state_.x = pBlind_state_signal->base_pose_world.toRpy();
	b_R_w = rpyToRot(actual_angular_state_.x);
	actual_angular_state_.xd = b_R_w * pBlind_state_signal->base_velocity_world.getAngular(); //virtualModel.cpp wants in the base frame...

    //base
    actual_base.x = pBlind_state_signal->base_pose_world.toPosition();
    actual_base.xd = pBlind_state_signal->base_velocity_world.getLinear();

    //com
	actual_CoM.x = dls::dog::getCoMFromBase(q_curr, actual_angular_state_.x,actual_base.x, *inertia_props_);
    //compute actual com velocity in WF
    Vector6D body_velocity_base;
    body_velocity_base= dls::dog::motionVectorTransform(Vector3d::Zero(),b_R_w) * pBlind_state_signal->base_velocity_world.data();
    comTwist = dls::dog::getWholeBodyCOMVelFB(body_velocity_base, b_R_w.transpose(),  q_curr, qd_curr, *inertia_props_, *hom_transforms_);
    actual_CoM.xd = dls::rbd::linearPart(comTwist);

	if (isBaseControlled())
	{
	 actual_linear_state_ = actual_base;
	} else{
	 actual_linear_state_ = actual_CoM;
	}
    //TODO
    //Eigen::Vector3d offCoM =  inertiaProps_->getWholeBodyCOM(q_curr);
    //actual_CoM_height = actual_base_height + (terr_R_w * R.transpose()*offCoM)(dls::rbd::Z);
    //actual_CoM_heightd = (terr_R_w*actual_CoM.xd)(dls::rbd::Z);

	// TODO move this memory assignment
	auto pGait_signal = this->readGaitSignal();
	if(!pGait_signal )
	{
	   if (first_time_)
	   {
		//set To actual
		des_q = q_curr;
		des_qd = qd_curr;
		des_linear_state_ = actual_linear_state_;
		des_angular_state_ = actual_angular_state_;
		first_time_ = false;
	   }
	} else {

	   des_q = pGait_signal->desired_joint_state.position;
	   des_qd = pGait_signal->desired_joint_state.velocity;


	   if (isBaseControlled())
	   {
		  desired_base_pos.x = pGait_signal->desired_base_pose_world.toPosition();
		  desired_base_pos.xd = pGait_signal->desired_base_velocity_world.getLinear();
		  des_linear_state_ = desired_base_pos;

	   } else{
		  desired_com_pos.x = pGait_signal->desired_com_pose_world.toPosition();
		  desired_com_pos.xd = pGait_signal->desired_com_velocity_world.getLinear();
		  des_linear_state_ = desired_com_pos;
	   }
	   des_angular_state_.x = pGait_signal->desired_base_pose_world.toRpy();
	   des_angular_state_.xd = pGait_signal->desired_base_velocity_world.getAngular();
	}

    dls::dog::JointState trunk_ctrl_tau(dls::dog::JointState::Zero());


//    std::cout << "actual_linear_state_x" <<actual_linear_state_.x.transpose() <<std::endl;

//    std::cout << "actual_linear_state_xd" <<actual_linear_state_.xd.transpose() <<std::endl;

//    std::cout << "actual_linear_state_xdd" <<actual_linear_state_.xdd.transpose() <<std::endl;
    //TODO
    //	if(params_.is_height_controlled){
    //		actual_height_ = ;
    //		terrain_R_world_ = ;
    //	}

    //TODO handle shin collision
    dls::dog::LegDataMap<Eigen::Vector3d> contactPos(Eigen::Vector3d::Zero());
    dls::dog::LegDataMap<dls::dog::FootJac> contactJacs;


    //TODO fill in shin_contact_position_ from gazebo
    for (int leg = dls::dog::LF; leg<=dls::dog::RH; leg++)
    {
        if (getShinCollisionFlag(dls::dog::LegID(leg)) &&  (shin_contact_position_[leg] != 0.0))
        {
            contactPos[dls::dog::LegID(leg)] = fwd_kin_->getShinPos(q_curr,shin_contact_position_[leg], dls::dog::LegID(leg));
            contactJacs[dls::dog::LegID(leg)] = shin_jacobians_->getShinJacobian(q_curr, shin_contact_position_[leg], dls::dog::LegID(leg));
        }
        else
        {
            contactPos[dls::dog::LegID(leg)] = fwd_kin_->getFootPos(q_curr, dls::dog::LegID(leg));
            contactJacs[dls::dog::LegID(leg)] = feet_jacobians_->getFootJacobian(q_curr, dls::dog::LegID(leg));
        }
        //for swing foot dont make distinction
        des_swing_foot_state_[dls::dog::LegID(leg)].x = fwd_kin_->getFootPos(des_q, dls::dog::LegID(leg));
        des_swing_foot_state_[dls::dog::LegID(leg)].xd = feet_jacobians_->getFootJacobian(des_q, dls::dog::LegID(leg)) * dls::dog::getLegJointState(dls::dog::LegID(leg), des_qd);
    }


//    std::cout << "b_R_w" <<b_R_w <<std::endl;
//    std::cout << "baseTwist" <<baseTwist <<std::endl;
//    std::cout << "q_curr" <<q_curr <<std::endl;
//    std::cout << "contactPos" <<contactPos <<std::endl;
//    std::cout << "contactJacs" <<contactJacs <<std::endl;

	if (params_.is_height_controlled) {
	 std::cout << "WARNING: desired height not set!" << std::endl;
	 std::cout <<  "Using the default value of ";
	 std::cout << params_.default_des_height << " meters." << std::endl;
	 //TODO
	 des_height_ =  params_.default_des_height;
	}


	setStanceLegs(stance_legs);


    getJointTorques(b_R_w, baseTwist, q_curr, qd_curr, contactPos,  contactJacs, trunk_ctrl_tau);


	// TODO real time memory allocation
    dls::ControlSignal control_signal;
	control_signal.torques.resize(12);
	control_signal.torques << trunk_ctrl_tau;
	//std::cout<<trunk_ctrl_tau.transpose()<<std::endl;
    publishSignal(control_signal);

    //      // Plot desired forces (blue) / and actual forces (green)
    //      dls::dog::LegDataMap<Eigen::Vector3d> feetDesForces(Eigen::Vector3d::Zero());
    //      dls::dog::LegDataMap<Eigen::Vector3d> actualFeetForces(Eigen::Vector3d::Zero());
    //      trunk_ctrl_->getFeetForces(feetDesForces); //they are in world frame
    //      //
    //      // THIS LINE HAS BEEN COMMENTED IN ORDER TO RUN THE DEMO
    //      //
    //      ctrl_->writeOutFeetForces(actualFeetForces);
    //      //
    //      // END OF THIS LINE HAS BEEN COMMENTED IN ORDER TO RUN THE DEMO
    //      //
    //      ctrl_->logCommonVariables(trunk_ctrl_);
    //      ctrl_->getLogPtr()->writeOutDebugVars(debug_);

    //      //plot stuff
    //      dwl::ArrowProperties arrow(0.02, 0.05, 0.0);

    //      for (int leg = dls::dog::LF; leg<=dls::dog::RH; leg++)
    //      {
    //        if (contactPos[dls::dog::LegID(leg)].allFinite() && feetDesForces[dls::dog::LegID(leg)].allFinite() && actualFeetForces[dls::dog::LegID(leg)].allFinite())
    //        {
    //                display_.drawArrow(contactPos[dls::dog::LegID(leg)],
    //                                   contactPos[dls::dog::LegID(leg)] + b_R_w * feetDesForces[dls::dog::LegID(leg)] / (dls::rbd::g*inertiaProps_->getTotalMass()),
    //                                   arrow,
    //                                   dwl::Color(dwl::ColorType::Blue, 1.),
    //                                   "base_link");
    //                display_.drawArrow(contactPos[dls::dog::LegID(leg)],
    //                                   contactPos[dls::dog::LegID(leg)] + actualFeetForces[dls::dog::LegID(leg)] / (dls::rbd::g*inertiaProps_->getTotalMass()),
    //                                   arrow,
    //                                   dwl::Color(dwl::ColorType::Green, 1.),
    //                                   "base_link");
    //        }
    //      }
    //      if  ((supervisorServoTime - pushTime)<=perturbationDuration)
    //      {
    //          if (B_applicationPoint.allFinite() && B_ext_force.allFinite() && (B_ext_force.norm()>0.01))
    //          {
    //              display_.drawArrow(B_applicationPoint,
    //                             B_applicationPoint +B_ext_force / 500.0,
    //                             arrow, dwl::Color(dwl::ColorType::Red, 1.),
    //                             "base_link");
    //          }
    //      }

    //                  controller_in_use = false;
    //          }




    time.time_since_epoch();
}

TrunkController::Status TrunkController::eStop()
{

    std::cout<<"deactivating TrunkController"<<std::endl;


    return getStatus();
}





//this should be called after gl has init
void TrunkController::init(const TrunkControllerParams& config)
{
    params_ = config;
    //associate pointer to external object
    //get the type of controller
    createObjects();

    //set min goal
    if (params_.opt_type != OptimizationType::NO_OPTMIMIZATION){
        setMinimizationGoal(params_.min_goal);
    }

    // virtual model params
    Kp_posture_ = params_.Kp_posture;
    Kd_posture_ = params_.Kd_posture;

    vmodel_.setGains(Kp_posture_, Kd_posture_);

    vmodel_.setWrenchErrorThreshold(params_.wrench_error_threshold);



}

void TrunkController::setStanceThreshold(double th){
    if(params_.use_internal_stance_computation){
        force_th_ = th;
    } else {
        std::cout << "WARNING: internal stance computation not set!" << std::endl;
    }
}



WholeBodyOptimization::MinMethod TrunkController::minGoal(){
    return params_.min_goal;
}

void TrunkController::setOptimizationMethod(const OptimizationType &optimMethod){
    params_.opt_type = optimMethod;
    createObjects();
}

void TrunkController::setMinimizationGoal(const MinGoal &minGoal){
    params_.min_goal = minGoal;

    switch(params_.opt_type)
    {
    case  OptimizationType::WHOLE_BODY_DYNAMIC:
        wb_opt_->setMinimizationMethod(WholeBodyOptimization::MinMethod(minGoal));
        break;
    case  OptimizationType::WHOLE_BODY_STATIC:
        wb_opt_static_->setMinimizationMethod(WholeBodyOptimizationStatic::MinMethod(minGoal));
        break;
    case  OptimizationType::NO_OPTMIMIZATION:
        std::cout << "WARNING: there is no optimization!" << std::endl;
        break;
    default:
        break;
    }
}

void TrunkController::setSwingType(SwingType swing_type)
{
    if (params_.opt_type == OptimizationType::WHOLE_BODY_DYNAMIC){
        wb_opt_->setJointSpaceSwing(swing_type);
    } else {
        std::cout << "WARNING: swing control is dealt out of the wbcontroller" << std::endl;
    }
}

void TrunkController::setSwingLegGains(Vector3d & Kp_swing, Vector3d & Kd_swing){
    if (params_.opt_type == OptimizationType::WHOLE_BODY_DYNAMIC){
        wb_opt_->setSwingGains(Kp_swing, Kd_swing);
    }
    else {
        std::cout << "WARNING: swing control is dealt out of the wbcontroller" << std::endl;
    }
}


void TrunkController::getTarget(iit::planning::Point3d & des_pos,
                                iit::planning::Point3d & des_orient)
{
    des_pos = this->des_linear_state_;
    des_orient = this->des_angular_state_;
}


void TrunkController::getTarget(iit::planning::Point3d & des_pos,
                                iit::planning::Point3d & des_orient,
                                double & des_height)
{
    des_pos = this->des_linear_state_;
    des_orient = this->des_angular_state_;
    des_height = this->des_height_;
}


void TrunkController::setForceLimits(const LegDataMap<double> & high_force_limit,
                                     const LegDataMap<double> & low_force_limit,
                                     const LegDataMap<Eigen::Vector3d> & surf_normal,
                                     const LegDataMap<double> & mu_estimate)
{
    if  (params_.opt_type == OptimizationType::WHOLE_BODY_DYNAMIC ||
         params_.opt_type == OptimizationType::WHOLE_BODY_STATIC)
    {
        this->high_force_limit_ = high_force_limit;
        this->low_force_limit_ = low_force_limit;
        this->surf_normal_ = surf_normal;
        this->mu_estimate_ = mu_estimate;
    }
}

void TrunkController::setTorqueLimits(dls::dog::JointState & tau_max)
{
    if (params_.opt_type == OptimizationType::WHOLE_BODY_DYNAMIC) {
        this->tau_max_ = tau_max;
    }
}

void TrunkController::setJointLimits(dls::dog::JointState & q_max, dls::dog::JointState & q_min)
{
    if (params_.opt_type == OptimizationType::WHOLE_BODY_DYNAMIC){
        this->q_max_ = q_max;
        this->q_min_ = q_min;
        wb_opt_->setJointLimits(q_max, q_min);
    }
}

//directly set desired trunk wrench
void TrunkController::setFFWDTrunkWrench(const dls::rbd::Vector6D  & wrench){
    des_ffwd_wrench_ = wrench;
}


void TrunkController::getTotalWrench(dls::rbd::Vector6D & wrench)
{
    wrench = des_wrench_;
}


void TrunkController::getFeedForwardWrench(dls::rbd::Vector6D & wrench)
{
    wrench = des_ffwd_wrench_ + gravity_wrench_;
}

void TrunkController::getFeedBackWrench(dls::rbd::Vector6D & wrench)
{
    wrench = des_vm_wrench_;
}









void TrunkController::getConstraintViolations(WholeBodyOptimization::ConstrViolation & constr_viol)
{
    constr_viol = this->constr_viol_;
}


void TrunkController::getFeetForces(LegDataMap<Vector3d> &des_feet_forces){
    des_feet_forces = this->des_feet_forces_;
}

void TrunkController::useInternalVirtualModel(bool flag)
{
    use_internal_virtual_model_ = flag;
}


bool TrunkController::useInternalVirtualModel()
{
    return  use_internal_virtual_model_;
}

void TrunkController::setGravityCompensation(const bool & flag){
    params_.use_gravity_compensation = flag;
}

void TrunkController::setGains(const dls::rbd::Vector6D  & Kp_posture,
                               const dls::rbd::Vector6D  & Kd_posture)
{
    Kp_posture_ = Kp_posture;
    Kd_posture_ = Kd_posture;

    vmodel_.setGains(Kp_posture_, Kd_posture_);
}

void TrunkController::getGains(dls::rbd::Vector6D  & Kp_posture, dls::rbd::Vector6D  & Kd_posture)
{
    //update the gains from the vm (there can be some degradation)
    vmodel_.getGains(Kp_posture_, Kd_posture_);
    //send them out
    Kp_posture = Kp_posture_;
    Kd_posture = Kd_posture_;
}



void TrunkController::useDampingOnly(bool use_damping_only){
    params_.damping_only = use_damping_only;
}

dls::dog::LegBoolMap TrunkController::getActualStance()
{
    return this->stance_legs_;
}

void TrunkController::setDesiredFeetForces(const LegBoolMap & stance_legs,
                                           const double weight,
                                           const LegDataMap<Vector3d> & input_desired_forces)
{
    if(params_.opt_type == OptimizationType::WHOLE_BODY_STATIC){
        des_forces_weight_ = weight;
        des_forces_ = input_desired_forces;
        wb_opt_static_->setDesiredFeetForces(stance_legs, des_forces_weight_, des_forces_);
    } else {
        std::cout << "WARNING: Whole Body Static Optimization not set!" << std::endl;
    }
}

void TrunkController::getDesiredFeetForces(LegDataMap<Vector3d> & des_forces)
{
    des_forces = this->des_forces_;
}

void TrunkController::setTorqueWeights(Vector3d & weights_leg)
{
    setLegTorqueWeights(LF, weights_leg);
    setLegTorqueWeights(RF, weights_leg);
    setLegTorqueWeights(LH, weights_leg);
    setLegTorqueWeights(RH, weights_leg);
    leg_torque_weights_ = weights_leg;
}

void TrunkController::setLegTorqueWeights(const LegID & leg, Vector3d & weights_leg)
{
    if (params_.min_goal == WholeBodyOptimization::TORQUES)
    {
        //save in member variable
        leg_torque_weights_[LegID(leg)] = weights_leg;
        switch(params_.opt_type)
        {
        case  OptimizationType::WHOLE_BODY_DYNAMIC:
            wb_opt_->setTorqueWeight(leg, weights_leg);

            break;
        case  OptimizationType::WHOLE_BODY_STATIC:
            wb_opt_static_->setTorqueWeight(leg, weights_leg);

            break;
        case OptimizationType::NO_OPTMIMIZATION:
            break;
        default:
            break;
        }
    } else {
        std::cout << "WARNING: Torque optimization is OFF" << std::endl;
    }
}

// PRIVATE MEMBERS
void TrunkController::createObjects(){
    switch(params_.opt_type)
    {
    case  OptimizationType::WHOLE_BODY_DYNAMIC:
    {

        //create objects
        wb_opt_.reset(new WholeBodyOptimization(*hom_transforms_,
                                                *motion_transforms_,
                                                *inv_dyn_,
                                                *fwd_kin_,
                                                *feet_jacobians_,
                                                *jsim_,
                                                *inertia_props_));

        //swing tpe
        wb_opt_->setJointSpaceSwing(params_.swing_type);
        //optimization options
        wb_opt_->setFrictionConstraint(params_.use_friction_constraints);
        wb_opt_->setTorqueConstraint(params_.use_torque_constraints);
        wb_opt_->setJointConstraint(params_.use_joint_constraints);

        //set swing gains
        Kp_swing_ = params_.Kp_swing;
        Kd_swing_ = params_.Kd_swing;

        double default_kp = 500;
        double default_kd = 10;
        if  (params_.swing_type == SwingType::FORCE_BASED_JSPACE)
        {
            if (Kd_swing_(dls::rbd::X) > default_kd)
                Kd_swing_(dls::rbd::X) = default_kd;
            if (Kd_swing_(dls::rbd::Y) > default_kd)
                Kd_swing_(dls::rbd::Y) = default_kd;
            if (Kd_swing_(dls::rbd::Z) > default_kd)
                Kd_swing_(dls::rbd::Z) = default_kd;
            if (Kp_swing_(dls::rbd::X) > 600)
                Kp_swing_(dls::rbd::X) = default_kp;
            if (Kp_swing_(dls::rbd::Y) > 600)
                Kp_swing_(dls::rbd::Y) = default_kp;
            if (Kp_swing_(dls::rbd::Z) > 600)
                Kp_swing_(dls::rbd::Z) = default_kp;
        }
        params_.Kp_swing = Kp_swing_;
        params_.Kd_swing = Kd_swing_;

        wb_opt_->setSwingGains(Kp_swing_, Kd_swing_);
        for (int leg=LF; leg<=RH;leg++)
        {
            wb_opt_->setTorqueWeight(LegID(leg), leg_torque_weights_[LegID(leg)]);
        }
        wb_opt_->setMinimizationMethod(WholeBodyOptimization::MinMethod(params_.min_goal));
    }
        break;
    case  OptimizationType::WHOLE_BODY_STATIC:
    {
        wb_opt_static_.reset(new WholeBodyOptimizationStatic(*motion_transforms_,
                                                             *inv_dyn_,
                                                             *fwd_kin_,
                                                             *feet_jacobians_,
                                                             *inertia_props_));

        wb_opt_static_->setFrictionConstraint(params_.use_friction_constraints);
        wb_opt_static_->useSlacks(false);
        for (int leg=LF; leg<=RH;leg++)
        {
            wb_opt_static_->setTorqueWeight(LegID(leg), leg_torque_weights_[LegID(leg)]);
        }
        wb_opt_static_->setMinimizationMethod(WholeBodyOptimizationStatic::MinMethod(params_.min_goal));
    }
        break;
    case  OptimizationType::NO_OPTMIMIZATION:
        break;
    default:
        break;
    }
}


void TrunkController::getActualState(iit::planning::Point3d &actual_linear_state,
                                     iit::planning::Point3d &actual_angular_state)
{
    actual_linear_state = actual_linear_state_;
    actual_angular_state = actual_angular_state_;
}

void TrunkController::computeDesiredWrench(const dls::dog::JointState& q,
                                           dls::rbd::Vector6D & wrench)
{
    wrench = dls::rbd::ForceVector::Zero();
    Vector3d height_forceW = Vector3d::Zero();
    Vector3d height_force_terr = Vector3d::Zero();
    //compute height force
    if (params_.is_height_controlled){
        //des height control
        // test for the whole body controllersetForceLimits:
        // des_height = 0.6 + 0.05*sin(2*M_PI*0.5*taskServoTime);
        height_force_terr(dls::rbd::Z) = Kp_posture_(dls::rbd::LZ) *
                (des_height_ - actual_height_.x) - Kd_posture_(dls::rbd::LZ) * actual_height_.xd;
        //this force is in a terrain frame (you want to push you away from it!)
        //we should map to world frame to add it
        height_forceW = terrain_R_world_.transpose() * height_force_terr;
        //this will result in no action on Z component
        vmodel_.setGain(dls::rbd::LZ, dls::control::VirtualModel::PROPORTIONAL, 0);
        vmodel_.setGain(dls::rbd::LZ, dls::control::VirtualModel::DERIVATIVE, 0);
    }

    Vector3d offCoM = inertia_props_->getWholeBodyCOM(q);

    InertiaMatrix Ic =  jsim_->getWholeBodyInertia();
    // if we work in the CoM we need to shift
    if (!params_.is_base_controlled){
        //the position of base wrt to com is -offCoM!
        dls::rbd::Matrix66d base_X_com  = motionVectorTransform(-offCoM,  Matrix3d::Identity());
        //the two frames are aligned
        dls::rbd::Matrix66d com_X_base_f = forceVectorTransform(offCoM,  Matrix3d::Identity());
        // we have to do the assignment in two steps
        // no idea why, probably some Eigen inheritance problem
        Ic = com_X_base_f * Ic * base_X_com;
    }

    if (params_.damping_only){
        des_angular_state_.xd = Vector3d::Zero();
        des_linear_state_.xd = Vector3d::Zero();
    }

    //we are controlling angular velocity
    wrench = vmodel_.getTotalWrench(des_linear_state_,
                                    actual_linear_state_,
                                    des_angular_state_,
                                    actual_angular_state_,
                                    inertia_props_->getTotalMass(), Ic, wrench_error_);

    //add height force if any...(we add only z component to avoid conflicts)
    wrench(dls::rbd::LZ) += height_forceW(dls::rbd::Z);
}

void TrunkController::wrenchStaticMapping(const dls::rbd::ForceVector wrench,
                                          const Eigen::Matrix3d R,
                                          const dls::dog::JointState q,
                                          const LegBoolMap & stance_legs,
                                          const LegDataMap<FootJac> &foot_jacs,
                                          const LegDataMap<Vector3d> & feet,
                                          dls::dog::JointState& joint_torques)
{
    Eigen::Matrix<double, Eigen::Dynamic, 6> Jcb;
    Eigen::Matrix<double, Eigen::Dynamic, 6> Jcbt_inv;
    Eigen::Matrix<double, Eigen::Dynamic, dls::dog::jointsCount> Jcq;

    if (compute_stance_legs(stance_legs) != 0) {
        //if you set identity the forces will be in the same frame of the wrench
        computeJcb(Jcb, Matrix3d::Identity(), stance_legs, feet);
        computeJcq(Jcq, Matrix3d::Identity(), stance_legs, foot_jacs);
        if (params_.min_goal == MinGoal::TORQUES){
            Eigen::MatrixXd W;
            W.resize(contactConstrCount*compute_stance_legs(stance_legs), contactConstrCount*compute_stance_legs(stance_legs));
            W.setZero();
            int cleg_counter = 0;
            for (int leg=0; leg<dls::dog::_LEGS_COUNT;leg++){
                if (stance_legs[dls::dog::LegID(leg)]){
                    W.block(cleg_counter*contactConstrCount,cleg_counter*contactConstrCount, 3, 3) =  foot_jacs[dls::dog::LegID(leg)]*
                            leg_torque_weights_[dls::dog::LegID(leg)].asDiagonal()*
                            foot_jacs[dls::dog::LegID(leg)].transpose();
                    cleg_counter++;
                }

            }
            Jcbt_inv =psdInvW(Jcb.transpose(), W,   1E-06);
        } else { //mingoal wrench (normals TODO)
            Jcbt_inv =psdInv(Jcb.transpose(),  1E-06);
        }
        //project torques into the constraint space
        //since the wrench is in the world frame I need to rotate it
        dls::rbd::Matrix66d b_X_w = dls::rbd::Matrix66d::Zero();
        b_X_w.block<3,3>(0,0) = R;
        b_X_w.block<3,3>(3,3) = R;

        if(params_.verbose && params_.debug){
            std::cout  << "b_X_w           = " << b_X_w           << std::endl;
            std::cout  << "wrench          = " << wrench          << std::endl;
            std::cout  << "feet            = " << feet            << std::endl;
            std::cout  << "foot_jacs       = " << foot_jacs       << std::endl;
            std::cout  << "Jcb.transpose() = " << Jcb.transpose() << std::endl;
            std::cout  << "Jcbt_inv        = " << Jcbt_inv        << std::endl;
            std::cout  << "Jcq.transpose() = " << Jcq.transpose() << std::endl;
        }
        dls::dog::JointState h_joints = getLegWeightTorques(R, q);
        joint_torques =  h_joints -Jcq.transpose() * Jcbt_inv * b_X_w * wrench;
    } else {
        joint_torques = dls::dog::JointState::Zero();
    }
}

void TrunkController::computeDesiredAcceleration()
{
    InertiaMatrix Ic_accelOnly;
    // we have to do the assignment in two steps
    // no idea why, probably some Eigen inheritance problem
    Ic_accelOnly = InertiaMatrix::Identity();

    // for the inverse dynamics the attractor is expressed at the
    // acceleration level. Output is accel
    des_vm_accel_ = vmodel_.getTotalWrench(des_linear_state_,
                                           actual_linear_state_,
                                           des_angular_state_,
                                           actual_angular_state_,
                                           Ic_accelOnly.getMass(),
                                           Ic_accelOnly);
}

void TrunkController::setBaseControl(const bool &use_base_control){
    params_.is_base_controlled = use_base_control;
    if (params_.opt_type == OptimizationType::WHOLE_BODY_STATIC){
        wb_opt_static_->setBaseControl(params_.is_base_controlled);
    }
}

void TrunkController::useInternalStanceComputation(const bool &use_internal_stance_comp){
    params_.use_internal_stance_computation = use_internal_stance_comp;
}


void TrunkController::setStanceLegs(const LegBoolMap & input_stance_legs, const LegDataMap<Vector3d> & feet_grf)
{
    //internal stance computation
    if (params_.use_internal_stance_computation)
    {
        if (allclose(feet_grf[LF], Vector3d::Zero(), 0.0, 0.5) &&
               allclose(feet_grf[RF], Vector3d::Zero(), 0.0, 0.5) &&
               allclose(feet_grf[LH], Vector3d::Zero(), 0.0, 0.5) &&
               allclose(feet_grf[RH], Vector3d::Zero(), 0.0, 0.5))
        {
            std::cerr << " grfs not set in internal stance computation using user stance" <<std::endl;
            stance_legs_ = input_stance_legs;
        } else {
            //we assume input_stance_legs is the desired stance
            //at liftoff this is the same at touchdown is driven by grfs
            computeStance(input_stance_legs, feet_grf, stance_legs_);
        }
    }
    else{
        stance_legs_ = input_stance_legs;
    }

}



void TrunkController::getJointTorques(const Eigen::Matrix3d & R,
                                      const dls::rbd::VelocityVector & baseTwist,
                                      const dls::dog::JointState & q,
                                      const dls::dog::JointState & qd,
                                      const LegDataMap<Vector3d> & footPos_in,
                                      const LegDataMap<FootJac> & JFoot_in,
                                      dls::dog::JointState & jointTorques)
{
    //init
    ffwd_torques_ = dls::dog::JointState::Zero();
    des_feet_forces_ = LegDataMap<Vector3d>(Vector3d::Zero());
    des_vm_wrench_ = dls::rbd::ForceVector::Zero();
    gravity_wrench_ = dls::rbd::ForceVector::Zero();
    Vector3d offCoM = Vector3d::Zero();


    if (!params_.is_base_controlled)
    {
        offCoM= inertia_props_->getWholeBodyCOM(q);
    }

    //--- 1 step - compute wrench
    if (use_internal_virtual_model_)
    {
        computeDesiredWrench(q, des_vm_wrench_);
    }
    //add gravitycompensation if required
    if (params_.use_gravity_compensation)
    {
        gravity_wrench_(dls::rbd::LZ) = dls::rbd::g * inertia_props_->getTotalMass();
    }

    //add ffwd wrench
    des_wrench_ = des_vm_wrench_ + des_ffwd_wrench_ + gravity_wrench_;

    //--- 2 step -  compute torque according to  trunk controller type
    switch(params_.opt_type)
    {
    case OptimizationType::NO_OPTMIMIZATION:
    {
        // to have the mapping at the com (in case isbasecontrolled is false)
        //we need to add the offset to the
        // feet pos (pi - off = pi_com)
        LegDataMap<Eigen::Vector3d> footPosCom;
        for (int i = 0; i < _LEGS_COUNT; i++){
            footPosCom[LegID(i)] = footPos_in[LegID(i)] - offCoM;
        }
        wrenchStaticMapping(des_wrench_, R, q, stance_legs_, JFoot_in, footPosCom, ffwd_torques_);
        //compute feetforces
        Vector3d leg_force;
        for (int i = 0; i < _LEGS_COUNT; i++) {
            if (stance_legs_[LegID(i)]) {
                //the feetForces should be in the world frame
                leg_force = -R.transpose()*(JFoot_in[LegID(i)]).transpose().inverse() * getLegJointState(LegID(i),  ffwd_torques_);
            } else {
                leg_force = Vector3d::Zero();
            }
            des_feet_forces_[LegID(i)] = leg_force;
        }
    }
        break;

    case OptimizationType::WHOLE_BODY_STATIC:
    {
        wb_opt_static_->computeOptimization(surf_normal_,
                                            mu_estimate_,
                                            stance_legs_,
                                            high_force_limit_,
                                            low_force_limit_,
                                            R,
                                            q,
                                            footPos_in,
                                            JFoot_in,
                                            offCoM,
                                            des_wrench_);

        wb_opt_static_->getJointTorques(ffwd_torques_);
        wb_opt_static_->getFeetForces(des_feet_forces_);
        wrench_error_ = wb_opt_static_->getWrenchError();
        //compute constraint violations
        WholeBodyOptimizationStatic::ConstrViolationStatic constr_violationStatic;
        wb_opt_static_->getConstraintViolations(constr_violationStatic);
        constr_viol_.unilateral = constr_violationStatic.unilateral;
        constr_viol_.friction = constr_violationStatic.friction;
        constr_viol_.torque = 0.0;
        constr_viol_.joint = 0.0;

    }
        break;

    case OptimizationType::WHOLE_BODY_DYNAMIC:
    {
        //if the swing is in the joint space we should compute the IK
        if (params_.swing_type == SwingType::FORCE_BASED_JSPACE)
        {
            for (int i = 0; i < _LEGS_COUNT; i++)
            {
                if (!stance_legs_[LegID(i)])
                {
                    //only for swing legs
                    Vector3d q_des, qd_des;
                    bool clamp_kin_limits = true;
                    ik_->getJointState(des_swing_foot_state_[LegID(i)].x,
                            des_swing_foot_state_[LegID(i)].xd,
                            LegID(i),
                            q_des,
                            qd_des,
                            clamp_kin_limits);
                    wb_opt_->setSwingQdesLeg(LegID(i), q_des);
                }
            }
        }
        wb_opt_->computeOptimization(surf_normal_,
                                     mu_estimate_,
                                     stance_legs_,
                                     high_force_limit_,
                                     low_force_limit_,
                                     tau_max_,
                                     baseTwist,
                                     des_swing_foot_state_,
                                     R,
                                     q,
                                     qd,
                                     des_wrench_);

        wb_opt_->getFeetForces(des_feet_forces_);
        wb_opt_->getJointTorques(ffwd_torques_);
        wb_opt_->getConstraintViolations(constr_viol_);

        wrench_error_ = wb_opt_->getWrenchError();

    }
        break;

    default:

        break;
    }

    jointTorques = ffwd_torques_;
}

void TrunkController::computeStance(const LegBoolMap & des_stance,
                                    const LegDataMap<Vector3d>& actual_grfs,
                                    LegBoolMap & actual_stance)
{
    for (int leg = LF; leg <= RH; leg++)
    {
        if ((des_stance[leg]) && (!wait_for_stance_[leg]))
        {
            actual_stance[leg] = true;
        } else {
            actual_stance[leg] = false;
            wait_for_stance_[leg] = true;
        }
        if (wait_for_stance_[leg]) {
            if (surf_normal_[leg].dot(actual_grfs[LegID(leg)]) > force_th_)
            {
                //at the touch down be haptic
                actual_stance[leg] = true;
                wait_for_stance_[leg] = false;
            }
        }
    }
}

void TrunkController::getSlacks(Eigen::VectorXd & slacks_out)
{

    switch(params_.opt_type)
    {
    case OptimizationType::NO_OPTMIMIZATION:{slacks_out.resize(0);}
        break;

    case OptimizationType::WHOLE_BODY_STATIC:{wb_opt_static_->getSlacks(slacks_out);}
        break;

    case OptimizationType::WHOLE_BODY_DYNAMIC:{wb_opt_->getSlacks(slacks_out);}
        break;
    default:
        break;
    }
}

void TrunkController::printCosts()
{
    switch(params_.opt_type)
    {
    case OptimizationType::NO_OPTMIMIZATION:{}
        break;

    case OptimizationType::WHOLE_BODY_STATIC:{wb_opt_static_->printCosts();}
        break;

    case OptimizationType::WHOLE_BODY_DYNAMIC:{wb_opt_->printCosts();}
        break;
    default:
        break;
    }
}
void TrunkController::useSlacks(bool flag)
{
    switch(params_.opt_type)
    {
    case OptimizationType::NO_OPTMIMIZATION:{}
        break;

    case OptimizationType::WHOLE_BODY_STATIC:{wb_opt_static_->useSlacks(flag);}
        break;

    case OptimizationType::WHOLE_BODY_DYNAMIC:{wb_opt_->useSlacks(flag);}
        break;
    default:
        break;
    }
}

void TrunkController::setSurfaceNormal(const dls::dog::LegID leg, const Eigen::Vector3d & surf_normal)
{
    this->surf_normal_[leg] = surf_normal;
}

void TrunkController::setSurfaceNormal(const LegDataMap<Eigen::Vector3d> & surf_normal)
{
    this->surf_normal_ = surf_normal;
}


void TrunkController::setFrictionCoefficient(const dls::dog::LegID leg, const double & mu_estimate)
{
    this->mu_estimate_[leg] = mu_estimate;
}

void TrunkController::setFrictionCoefficient(const LegDataMap<double> & mu_estimate)
{
    this->mu_estimate_ = mu_estimate;
}

dls::dog::JointState TrunkController::getLegWeightTorques(const Eigen::Matrix3d Rcurr, const dls::dog::JointState & q_curr)
{

    dls::rbd::VelocityVector gW, gB;
    gW <<0.0, 0.0, 0.0, 0.0, 0.0, -dls::rbd::g; gB.setZero();
    gB.segment(dls::rbd::LX,3) = Rcurr*gW.segment(dls::rbd::LX,3);
    dls::rbd::ForceVector baseWrench; dls::dog::JointState  h_joints;
    inv_dyn_->id_fully_actuated(baseWrench, h_joints, gB, dls::rbd::VelocityVector::Zero(), dls::rbd::VelocityVector::Zero(), q_curr, dls::dog::JointState::Zero(), dls::dog::JointState::Zero());
    return h_joints;

}


#include <doglib/factory/robot_factory.hpp>
extern "C" dls::Controller *create()
{
	using namespace dls::dog;
	auto p = new TrunkController(RobotFactory::buildRobot(RobotFactory::RobotType::HyQ));
	return p;
}

extern "C" void destroy(dls::Controller *p)
{
	delete p;
}


