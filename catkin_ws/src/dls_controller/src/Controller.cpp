#include <dls_controller/Controller.h>
#include <dls_controller/support/ConsoleUtility.h>
#include <iit/rbd/rbd.h>
#include <iit/commons/geometry/rotations.h>

using namespace iit;
using namespace iit::dog;

namespace dls_controller
{

Controller::Controller() : controllerName("Controller"), taskServoRate(0),
		robotName("Robot")
{
	fbs_.reset(new dwl::model::FloatingBaseSystem());
	wkin_.reset(new dwl::model::WholeBodyKinematics());
	wdyn_.reset(new dwl::model::WholeBodyDynamics());
	terrain_.reset(new terrain_server::TerrainMapInterface());
	display_.reset(new dwl_rviz_plugin::DisplayInterface());
        log_.reset(new Logger());
    execute_plan.reset(new bool);
  supervisorClient = false;
  if (n.hasParam("/gui/robot_name")) onGUI=true;
  else onGUI=false;
	// Adding the console functions
	addConsoleFunction("man",
					   "This function prints menu options",
					   &Controller::manPrint, this);
	addConsoleFunction("resetTerrainMap",
					   "This function resets the terrain map",
					   &Controller::resetTerrainMap, this);
	addConsoleFunction("terrainData",
					   "This function gets the terrain data given a position",
					   &Controller::terrainData, this);
	addConsoleFunction("terrainCost",
					   "This function gets the terrain cost given a position",
						&Controller::terrainCost, this);
	addConsoleFunction("terrainHeight",
					   "This function gets the terrain height given a position",
					   &Controller::terrainHeight, this);
	addConsoleFunction("terrainNormal",
					   "This function gets the terrain normal given a position",
						&Controller::terrainNormal, this);

    addConsoleFunction("changeMinGoal",
                       "change minimization goal",
                       &Controller::toggle_min_goal, this);

    addConsoleFunction("vmLin",
                       "set virtual model linear gains",
                       &Controller::set_vm_lin_gains, this);

    addConsoleFunction("vmAng",
                       "set virtual model angular gains",
                       &Controller::set_vm_ang_gains, this);

    addConsoleFunction("changeTrunkCont",
                       "change trunk controller method",
                       &Controller::change_trunk_controller_type, this);

    addConsoleFunction("trunkCont",
                       "toggle trunk controller",
                       &Controller::toggle_trunk_controller, this);

    addConsoleFunction("setTorqueWeights",
                       "set torque minimization weights",
                       &Controller::setTorqueWeights, this);

    addConsoleFunction("changePDgains",
                       "Set Gains for all the Joints without asking are you sure!",
                       &Controller::changePDgains, this);
    addConsoleFunction("setGains",
                       "Set Gains for all the Joints",
                       &Controller::setGains, this);
    addConsoleFunction("setGainsIndex",
                       "Set Gains for specific Joint (input index)",
                       &Controller::setGainsIndex, this);

    addConsoleFunction("setCom",
                       "set user derfined com",
                       &Controller::setTrunkCoM, this);
    addConsoleFunction("getCom",
                       "get the com identified with id_static",
                        &Controller::getTrunkCoM, this);

    legmap[iit::dog::LF] = "LF";
    legmap[iit::dog::RF] = "RF";
    legmap[iit::dog::LH] = "LH";
    legmap[iit::dog::RH] = "RH";
}


void Controller::setFloatingBaseSystem(dwl::model::FloatingBaseSystem& fbs)
{
	fbs_.reset(&fbs);
}


void Controller::setWholeBodyKinematics(dwl::model::WholeBodyKinematics& wkin)
{
	wkin_.reset(&wkin);
}


void Controller::setWholeBodyDynamics(dwl::model::WholeBodyDynamics& wdyn)
{
	wdyn_.reset(&wdyn);
}

void Controller::setInverseKinematics(std::shared_ptr<InverseKinematics>& ik){
    ik_= ik;
}

void Controller::setTrunkController(TrunkController::Ptr &trunk_ctrl){
    trunk_ctrl_ = trunk_ctrl;
    feetForces =  Vector3d::Zero();
    grForces = Vector3d::Zero();
}

void Controller::setTerrainInterface(terrain_server::TerrainMapInterface& terrain)
{
	terrain_.reset(&terrain);
}

void Controller::setGridMapTerrain(dls::perception::TerrainInterface& terrain)
{
    grid_map_terrain_.reset(&terrain);
}


void Controller::setDisplayInterface(dwl_rviz_plugin::DisplayInterface& display)
{
	display_.reset(&display);
}


void Controller::setRobotModels(std::shared_ptr<iit::dog::FeetJacobians>& feet_jacs,
                                std::shared_ptr<iit::dog::ForwardKinematics>& fwd_kin,
                                std::shared_ptr<iit::dog::ShinJacobians>& shin_jacs,
                                std::shared_ptr<iit::dog::InertiaPropertiesBase>& inertia_props,
                                std::shared_ptr<iit::dog::HomogeneousTransformsBase>& hom_transforms,
                                std::shared_ptr<iit::dog::JSIMBase>& jsim,
                                std::shared_ptr<iit::dog::InverseDynamicsBase>& inv_dyn,
                                std::shared_ptr<iit::dog::KinDynParams>& params,
                                std::shared_ptr<iit::dog::LimitsBase>& limits,
                                std::shared_ptr<iit::dog::FeetContactForces>& feet_forces){
    feet_jacs_ = feet_jacs;
    fwd_kin_ = fwd_kin;
    shin_jacobians_ = shin_jacs;
    inertia_props_ = inertia_props;
    hom_transforms_ = hom_transforms;
    jsim_ = jsim;
    inv_dyn_ = inv_dyn;
    robot_params_ = params;
    robot_limits_ = limits;
    feet_forces_ = feet_forces;
}

void Controller::setDesiredTrajectory(std::shared_ptr<dls_msgs::DesiredTrajectory> &desired_trajectory) {
    desired_trajectory_ = desired_trajectory;   
}

void Controller::readBaseStateFromRobot(BaseState &bs)
{
	(*this->bs) = bs;
}


void Controller::readBaseStateGroundTruth(BaseState &bs_gt)
{
	(*this->bs_gt) = bs_gt;
}


bool Controller::initBase(int nJoints, int nFTSensors, int taskRate, std::string robot)
{
	std::cout << " === Base class " << controllerName << " for " << robot << " got " << nJoints << " - "
		 << nFTSensors << " - " << taskRate << std::endl;
	robotName = robot;
	taskServoRate = taskRate;
	nDOF = nJoints;
	footSensor.reset(new FTSensorData(nFTSensors));
	contactSensor.reset(new ContactSensorData(nFTSensors));
	shinSensor.reset(new ShinSensorData(nFTSensors)); //we assume one shin sensor per leg
	actual_ws.reset(new dwl::WholeBodyState());
	planned_ws.reset(new dwl::WholeBodyState());
	desired_ws.reset(new dwl::WholeBodyState());
	bs.reset(new BaseState());
	bs_gt.reset(new BaseState());
	actual_ws->setJointDoF(nDOF);
	desired_ws->setJointDoF(nDOF);
	joint_names.resize(nDOF);
	input_reference_q.resize(nDOF);
	input_reference_qd.resize(nDOF);

	return true;
}


void Controller::readInJointDataFromRobot(const std::vector<double>& actual_q,
                                          const std::vector<double>& actual_qd,
                                          const std::vector<double>& actual_tau)
{
    if((unsigned)actual_tau.size() < tau_.rows()) {
        std::cout << "WARNING: vector size mismatch (";
        std::cout << actual_tau.size() << " < " << tau_.rows() << ")" << std::endl;
    }
    for (unsigned int i = 0; i < tau_.rows(); i++) {
        tau_(i) = actual_tau[i];
        q_(i) = actual_q[i];
        qd_(i) = actual_qd[i];        
	}
    //compute torque limits according to the actual config of the robot
    tau_max_ = robot_limits_->getTorqueLimits(q_);
    trunk_ctrl_->setTorqueLimits(tau_max_);

}


void Controller::readInActualWholeBodyState(const dwl::WholeBodyState& actual_ws)
{
	(*this->actual_ws) = actual_ws;
}


void Controller::readInPlannedWholeBodyState(const dwl::WholeBodyState& planned_ws)
{
	(*this->planned_ws) = planned_ws;
}

void Controller::resetInitialPlan(const dwl::WholeBodyState& plan)
{
    (*this->desired_ws) = plan;
    plan_update = true;
}

void Controller::checkResetPlan(dwl::WholeBodyState & planned_ws){
    if (plan_update)
    {
        std::cout<<"resetting plan from controller" <<std::endl;
        planned_ws = (*this->desired_ws);
        plan_update = false;
    }
}


void Controller::readInExecutePlan(bool& execute_plan)
{
    (*this->execute_plan) = execute_plan;
}

std::string Controller::getDWLJointName(const iit::dog::JointIdentifiers& joint_id) const{
    // the custom hyq_urdf_model.urdf has forced names so that it follows our
    // conventional order LF RF LH RH and not the alphabetical one.
    switch(joint_id){
    case JointIdentifiers::LF_HAA:
        return "01_lf_haa_joint";
    case JointIdentifiers::LF_HFE:
        return "01_lf_hfe_joint";
    case JointIdentifiers::LF_KFE:
        return "01_lf_kfe_joint";
    case JointIdentifiers::RF_HAA:
        return "02_rf_haa_joint";
    case JointIdentifiers::RF_HFE:
        return "02_rf_hfe_joint";
    case JointIdentifiers::RF_KFE:
        return "02_rf_kfe_joint";
    case JointIdentifiers::LH_HAA:
        return "03_lh_haa_joint";
    case JointIdentifiers::LH_HFE:
        return "03_lh_hfe_joint";
    case JointIdentifiers::LH_KFE:
        return "03_lh_kfe_joint";
    case JointIdentifiers::RH_HAA:
        return "04_rh_haa_joint";
    case JointIdentifiers::RH_HFE:
        return "04_rh_hfe_joint";
    case JointIdentifiers::RH_KFE:
        return "04_rh_kfe_joint";
    }
    return ""; // if the joint is not found, we return an empty string
}

unsigned int Controller::getDWLJointId(const iit::dog::JointIdentifiers& joint_id) const{
    return fbs_->getJointId(getDWLJointName(joint_id));
}

void Controller::readInDesiredJointData(const std::vector<double> read_desired_q,
										const std::vector<double> read_desired_qd,
										const std::vector<double> read_desired_tau)
{
    for (unsigned int i = 0; i < tau_.size(); i++) {
        des_tau_[i] = read_desired_tau[i];
        des_q_[i] = read_desired_q[i];
        des_qd_[i] = read_desired_qd[i];
	}
}

std::string Controller::getDWLLegName(const iit::dog::LegID& joint_id) const{
    switch(joint_id){
    case JointIdentifiers::LF_HAA:
        return "01_lf_foot";
    case JointIdentifiers::LF_HFE:
        return "02_rf_foot";
    case JointIdentifiers::LF_KFE:
        return "03_lh_foot";
    case JointIdentifiers::RF_HAA:
        return "04_rh_foot";
    }
    return ""; // if the leg is not found, we return an empty string
}
unsigned int Controller::getDWLLegId(const iit::dog::LegID& joint_id) const{
    return fbs_->getEndEffectorId(getDWLLegName(joint_id));
}


void Controller::readInStateDataFromRobot(const std::vector<double> orientation,
										  const std::vector<double> angular_velocity,
										  const std::vector<double> linear_acceleration)

{
	bs->setAcceleration_B(Eigen::Vector3d(linear_acceleration[0],
										  linear_acceleration[1],
										  linear_acceleration[2]));

	bs->setOrientation_W(Eigen::Quaterniond(orientation[0],orientation[1],orientation[2],orientation[3]));

	bs->setRotationRate_B(Eigen::Vector3d(angular_velocity[0],
										  angular_velocity[1],
										  angular_velocity[2]));
}


void Controller::readInSimStateDataFromRobot(const std::vector<double> linear_position,
											 const std::vector<double> linear_velocity)
{
	bs->setPosition_W(Eigen::Vector3d(linear_position[0],linear_position[1],linear_position[2]));
	bs->setVelocity_W(Eigen::Vector3d(linear_velocity[0],linear_velocity[1],linear_velocity[2]));
}


void Controller::readInForceTorqueDataFromRobot(const std::vector<std::vector<double> > force,
                                                const std::vector<std::vector<double> > torque,
                                                const std::vector<std::vector<double> > normal)
{
	// Update foot sensors
    for (unsigned int i = 0; i < footSensor->force.size(); i++) {
		footSensor->force[i] << force[i][0],force[i][1],force[i][2];
		footSensor->torque[i] << torque[i][0],torque[i][1],torque[i][2];
        footSensor->normal[i] << normal[i][0],normal[i][1],normal[i][2];
	}
}


void Controller::readInFootContactDataFromRobot(const std::vector<bool> contact)
{
	// Update contact sensors
    for (unsigned int i = 0; i < contactSensor->contact_.size(); i++) {
		contactSensor->contact_[i] = contact[i];
	}
}

void Controller::readInShinContactDataFromRobot(const std::vector<double> shin_contact_position)
{
	// Update contact sensors
    for (unsigned int i = 0; i < shinSensor->contact_position_.size(); i++) {
		shinSensor->contact_position_[i] = shin_contact_position[i];
	}
}

void Controller::writeOutDesiredWholeBodyState(dwl::WholeBodyState& desired_state)
{
	desired_state = (*this->desired_ws);
}


void Controller::writeOutJointDataToRobot(std::vector<double>& desired_q,
										  std::vector<double>& desired_qd,
										  std::vector<double>& desired_tau)
{
    for (unsigned int i = 0; i != des_tau_.size(); i++) {
        desired_tau[i] = des_tau_[i];
        desired_q[i] = des_q_[i];
        desired_qd[i] = des_qd_[i];
	}
}


void Controller::readInJointGains(const std::vector<double> gain_p,
								  const std::vector<double> gain_i,
								  const std::vector<double> gain_d)
{
	for (unsigned int i = 0; i != gain_p.size(); i++) {
		kp[i] = gain_p[i];
		ki[i] = gain_i[i];
		kd[i] = gain_d[i];
	}
    //init also the support variables for smoothing
    des_leg_joint_kp = old_kp = kp;
    des_leg_joint_kd = old_kd = kd;
}


void Controller::writeOutJointGains(std::vector<double>& gain_p,
									std::vector<double>& gain_i,
									std::vector<double>& gain_d)
{
	for (unsigned int i = 0; i != gain_p.size(); i++) {
		gain_p[i] = kp[i];
		gain_i[i] = ki[i];
		gain_d[i] = kd[i];
	}
}


void Controller::readInJointNames(const std::vector<std::string> names)
{
	joint_names = names;
}


void Controller::readInDesiredJointCommand(const std::vector<double> desired_q,
										   std::vector<double>& desired_qd)
{
    for (unsigned int i = 0; i < desired_q.size(); i++) {
		input_reference_q[i] = desired_q[i];
		input_reference_qd[i] = desired_qd[i];
	}
}


void Controller::consoleCallFunction(std::string func_name)
{
	for (unsigned int i = 0; i < menu_console_.size(); i++) {
		if (func_name == menu_console_[i].command) {
			(menu_console_[i].func_ptr)();
		}
	}
}


Eigen::Vector2d Controller::readPosition()
{
	std::string str;
	Eigen::Vector2d position = Eigen::Vector2d::Zero();
	printf("Position:\n");

	// Reading the x component
	printf("  x[%f]: ", 0.);
	std::getline(std::cin, str);
	if (!str.empty()) {
		try {
			position(dwl::rbd::X) = std::stod(str);
		} catch(std::invalid_argument& e) {
            printf(YELLOW_ "Invalid argument: setting the default"
                    " value\n" COLOR_RESET);
		}
	}

	// Reading the y component
	printf("  y[%f]: ", 0.);
	std::getline(std::cin, str);
	if (!str.empty()) {
		try {
			position(dwl::rbd::Y) = std::stod(str);
		} catch(std::invalid_argument& e) {
            printf(YELLOW_ "Invalid argument: setting the default"
                    " value\n" COLOR_RESET);
		}
	}

	return position;
}


void Controller::manPrint()
{
	std::cout << "Menu options:"<< std::endl;
	if (menu_console_.size() == 0) {
		std::cout << "No prompts available!" << std::endl;
	} else {
		for (unsigned int i = 0; i < menu_console_.size(); i++) {
			std::cout << menu_console_[i].command << " - ";
			std::cout << menu_console_[i].comment << std::endl;
		}
	}
}


void Controller::resetTerrainMap()
{
	terrain_->resetTerrainMap();
}


void Controller::terrainData()
{
	// Getting the terrain state
	Eigen::Vector2d position = readPosition();
	dwl::TerrainCell terrain_cell = terrain_->requestTerrainData(position);

	// Printing the terrain state
	printf("cost: %.10e\n", terrain_cell.cost);
	printf("height: %f\n", terrain_cell.height);
	printf("normal: [%f, %f, %f]\n",
			terrain_cell.normal(dwl::rbd::X),
			terrain_cell.normal(dwl::rbd::Y),
			terrain_cell.normal(dwl::rbd::Z));
}


void Controller::terrainCost()
{
	// Getting the terrain cost
	Eigen::Vector2d position = readPosition();
	double cost = terrain_->requestTerrainCost(position);

	printf("cost: %.10e\n", cost);
}


void Controller::terrainHeight()
{
	// Getting the terrain height
	Eigen::Vector2d position = readPosition();
	double height = terrain_->requestTerrainHeight(position);

	printf("height: %f\n", height);
}


void Controller::terrainNormal()
{
	// Getting the terrain normal
	Eigen::Vector2d position = readPosition();
	Eigen::Vector3d normal = terrain_->requestTerrainNormal(position);

	printf("normal: [%f, %f, %f]\n",
			normal(dwl::rbd::X),
			normal(dwl::rbd::Y),
			normal(dwl::rbd::Z));
}


void Controller::set_vm_lin_gains()
{
    iit::rbd::Vector6D  Kp_posture,  Kd_posture;
    trunk_ctrl_->getGains(Kp_posture,  Kd_posture);
    newline::getDouble("Kp_postureLx :", Kp_posture[rbd::LX], Kp_posture[rbd::LX]);
    newline::getDouble("Kp_postureLy :", Kp_posture[rbd::LY], Kp_posture[rbd::LY]);
    newline::getDouble("Kp_postureLz :", Kp_posture[rbd::LZ], Kp_posture[rbd::LZ]);
    newline::getDouble("Kd_postureLx :", Kd_posture[rbd::LX], Kd_posture[rbd::LX]);
    newline::getDouble("Kd_postureLy :", Kd_posture[rbd::LY], Kd_posture[rbd::LY]);
    newline::getDouble("Kd_postureLz :", Kd_posture[rbd::LZ], Kd_posture[rbd::LZ]);
    trunk_ctrl_->setGains(Kp_posture, Kd_posture);
}


void Controller::set_vm_ang_gains()
{
    iit::rbd::Vector6D  Kp_posture,  Kd_posture;
    trunk_ctrl_->getGains(Kp_posture,  Kd_posture);
    newline::getDouble("Kp_posture roll :",  Kp_posture[rbd::AX],  Kp_posture[rbd::AX]);
    newline::getDouble("Kp_posture pitch :", Kp_posture[rbd::AY],  Kp_posture[rbd::AY]);
    newline::getDouble("Kp_posture yaw :",   Kp_posture[rbd::AZ],  Kp_posture[rbd::AZ]);
    newline::getDouble("Kd_posture roll :",  Kd_posture[rbd::AX],  Kd_posture[rbd::AX]);
    newline::getDouble("Kd_posture pitch :", Kd_posture[rbd::AY],  Kd_posture[rbd::AY]);
    newline::getDouble("Kd_posture yaw :",   Kd_posture[rbd::AZ],  Kd_posture[rbd::AZ]);
    trunk_ctrl_->setGains(Kp_posture,  Kd_posture);
}



void Controller::toggle_min_goal(void)
{
    int ans;
    newline::getInt("set min goal: wrench_direction[0], normals[1], torques[2]   :", trunk_ctrl_->minGoal(), ans);
    switch(ans){
    case 0:
        std::cout<< "min set to WRENCHDIRECTION"<< std::endl;
        break;
    case 1:
        std::cout<< "min set to NORMALS"<< std::endl;
        break;
    case 2:
        std::cout<< "min set to TORQUES" << std::endl;
        break;
    default:
        std::cout<< "wrong choice" << std::endl;
        break;
    }
    trunk_ctrl_->setMinimizationGoal(iit::dog::MinGoal(ans));
}

void Controller::change_trunk_controller_type()
{
    int ans = 0;
    if (!supervisorClient)
      newline::getInt("set optim method: wbstatic[0], wbdynamic[1], nooptim[2]   :", (int)trunk_ctrl_->optimMethod(), ans);
    else
      ans=service_args[0];

    switch(OptimizationType(ans))
    {
        case OptimizationType::WHOLE_BODY_STATIC:
        {
            std::cout<< "wb static"<<std::endl;
            trunk_ctrl_->setOptimizationMethod(OptimizationType::WHOLE_BODY_STATIC);
        }
        break;
        case OptimizationType::WHOLE_BODY_DYNAMIC:
        {
            std::cout<< "wb dynamic"<<std::endl;
            trunk_ctrl_->setOptimizationMethod(OptimizationType::WHOLE_BODY_DYNAMIC);
        }
        break;
        case OptimizationType::NO_OPTMIMIZATION:
        {
            std::cout<< "whole dynamics OFF - no optim"<<std::endl;
            trunk_ctrl_->setOptimizationMethod(OptimizationType::NO_OPTMIMIZATION);
        }
        break;
    default:
        break;
    }
    if (onGUI)
      n.setParam("/gui/trunk_type",ans);
}


void Controller::toggle_trunk_controller()
{
    if (trunk_ctrl_->isOn())
    {
        std::cout<< "trunkController OFF"<<std::endl;
        trunk_ctrl_->setOn(false);
        trunk_ctrl_->useInternalVirtualModel(false);
    }
    else{
        switch(trunk_ctrl_->optimMethod())
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
        trunk_ctrl_->setOn(true);
    }
}

void Controller::setTorqueWeights()
{
    Vector3d torque_weights = Vector3d::Ones();
    newline::getDouble("set torque weights [HAA]: ", torque_weights(0), torque_weights(0));
    newline::getDouble("set torque weights [HFE]: ", torque_weights(1), torque_weights(1));
    newline::getDouble("set torque weights [KFE]: ", torque_weights(2), torque_weights(2));
    trunk_ctrl_->setTorqueWeights(torque_weights);
}

iit::dog::JointState Controller::eigenToJointState(const Eigen::VectorXd & urdf_joint_state) const
{

    // if vectorxd is shorter than the joint state, we fill in only the size of
    // vectorxd. The rest is zero. Otherwise we fill everything.
    dog::JointState result(dog::JointState::Zero());
    unsigned int block_size = std::min(urdf_joint_state.rows(), result.rows());
    result.block(0, 0, block_size, 1) = urdf_joint_state.block(0, 0, block_size, 1);

    return result;
}


//assumes you already init act_leg_joint_kp with smoothchangepid
void Controller::setChangePIDGains(double smooth_time, double taskTime)
{
    smoothPIDtime = smooth_time;
    if (smoothPIDtime <= 0.0)
    {
        std::cerr<<"smoothing time cannot be zero or negative, setting it to 1s" <<std::endl;
        smoothPIDtime = 1.0;
    }
    smoothChangePIDFlag = true;
    //update the smoothing var to the actual
    old_kp = kp;
    old_kd = kd;
    splinePIDgainsT0 = taskTime; //sample it
}

void Controller::setChangePIDGains(double smooth_time)
{
    setChangePIDGains(smooth_time, taskServoTime);
}

void Controller::smoothChangePIDGains(double taskTime)
{

    trans_multPID = (taskTime - splinePIDgainsT0)/smoothPIDtime;
    if ((trans_multPID >= 0.0) && (trans_multPID <= 1.0))
    {
        dog::JointDataMap<double> temp_kp, temp_kd;
        kp = des_leg_joint_kp + (old_kp - des_leg_joint_kp)* (1-trans_multPID);
        kd = des_leg_joint_kd + (old_kd - des_leg_joint_kd)* (1-trans_multPID);        
    } else {
        if (trans_multPID >= 1.0) //if transmult <0 let's wait cause the start is in the future
        {
            smoothChangePIDFlag = false;
            old_kp = des_leg_joint_kp;
            old_kd = des_leg_joint_kd;
        }
    }
}

bool  Controller::isPIDGainChanging()
{
    return smoothChangePIDFlag;
}


void  Controller::changePIDLeg(iit::dog::LegID leg_id, double Kp_ref, double Kd_ref)
{
//keep the previous gains in gait_th,thd
    kp[toJointID(leg_id, iit::dog::HAA)] = Kp_ref;	kd[toJointID(leg_id, iit::dog::HAA)] = Kd_ref;
    kp[toJointID(leg_id, iit::dog::HFE)] = Kp_ref;	kd[toJointID(leg_id, iit::dog::HFE)] = Kd_ref;
    kp[toJointID(leg_id, iit::dog::KFE)] = Kp_ref;	kd[toJointID(leg_id, iit::dog::KFE)] = Kd_ref;
}

void Controller::changePDgains()
{
    double  desKp, desKd;
    newline::getDouble("P gain : ", des_leg_joint_kp[LF_HAA], desKp);
    newline::getDouble("D gain : ", des_leg_joint_kd[LF_HAA], desKd);
    des_leg_joint_kp.setConstant(desKp);
    des_leg_joint_kd.setConstant(desKd);
    setChangePIDGains(3.0, taskServoTime);
}


void Controller::setGains()
{
    //double auxd;

    if (!supervisorClient) {
        double auxd;

        for (int i = 0; i < 12; i++)
        {
          std::cout << joint_names[i] << ":" << std::endl;
          newline::getDouble("Kp ", kp[i], auxd);
          if ((auxd < 0.0) || (auxd > 1000))
            printf("Value out of range!\n");
          else {
              des_leg_joint_kp[i] = auxd;

              std::string str = "/" + robotName + "/dls_supervisor/gains/" + joint_names[i] + "/p";
              n.setParam(str, auxd);
          }

          newline::getDouble("Kd ", kd[i], auxd);
          if ((auxd < 0.0) || (auxd > 1000))
            printf("Value out of range!\n");
          else {
            des_leg_joint_kd[i] = auxd;

            std::string str = "/" + robotName + "/dls_supervisor/gains/" + joint_names[i] + "/d";
            n.setParam(str, auxd);
          }
        }
    }
    else {
        for (int i = 0; i < 12; ++i) {
            std::string str = "/" + robotName + "/dls_supervisor/gains/" + joint_names[i];

            //P
            des_leg_joint_kp[i] = service_args[i+1*i];
            std::string str_p = str + "/p";
            n.setParam(str_p, des_leg_joint_kp[i]);

            //D
            des_leg_joint_kd[i] = service_args[i+1*i+1];
            std::string str_d = str + "/d";
            n.setParam(str_d, des_leg_joint_kd[i]);
        }
    }


    setChangePIDGains(3.0, taskServoTime);
}

void Controller::setGainsIndex()
{
    int index=0;
    double auxd;
    newline::getInt("Insert index of the desired joint (0-11): ", index, index);
    std::cout << joint_names[index] << ":";
    newline::getDouble("Kp ", kp[index], auxd);
    if ((auxd < 0.0) || (auxd > 1000))
      printf("Value out of range!\n");
    else {
        des_leg_joint_kp[index] = auxd;
        std::string str = "/" + robotName + "/dls_supervisor/gains/" + joint_names[index] + "/p";
        n.setParam(str, auxd);

    }
    newline::getDouble("Kd ", kd[index], auxd);
    if ((auxd < 0.0) || (auxd > 1000))
      printf("Value out of range!\n");
    else {
        des_leg_joint_kd[index] = auxd;
        std::string str = "/" + robotName + "/dls_supervisor/gains/" + joint_names[index] + "/d";
        n.setParam(str, auxd);
    }


    setChangePIDGains(3.0, taskServoTime);
}


void Controller::logCommonVariables(std::shared_ptr<dog::TrunkController> trunk_ctrl_)
{

        log_->debugPush("time", taskServoTime);

        log_->debugPush("LF_HAAmax", tau_max_[LF_HAA]);
        log_->debugPush("RF_HAAmax", tau_max_[RF_HAA]);
        log_->debugPush("LH_HAAmax", tau_max_[LH_HAA]);
        log_->debugPush("RH_HAAmax", tau_max_[RH_HAA]);
        log_->debugPush("LF_HFEmax", tau_max_[LF_HFE]);
        log_->debugPush("RF_HFEmax", tau_max_[RF_HFE]);
        log_->debugPush("LH_HFEmax", tau_max_[LH_HFE]);
        log_->debugPush("RH_HFEmax", tau_max_[RH_HFE]);
        log_->debugPush("LF_KFEmax", tau_max_[LF_KFE]);
        log_->debugPush("RF_KFEmax", tau_max_[RF_KFE]);
        log_->debugPush("LH_KFEmax", tau_max_[LH_KFE]);
        log_->debugPush("RH_KFEmax", tau_max_[RH_KFE]);

        log_->debugPush("RF_HAAmin", -tau_max_[RF_HAA]);
        log_->debugPush("LH_HAAmin", -tau_max_[LH_HAA]);
        log_->debugPush("LF_HAAmin", -tau_max_[LF_HAA]);
        log_->debugPush("RH_HAAmin", -tau_max_[RH_HAA]);
        log_->debugPush("LF_HFEmin", -tau_max_[LF_HFE]);
        log_->debugPush("RF_HFEmin", -tau_max_[RF_HFE]);
        log_->debugPush("LH_HFEmin", -tau_max_[LH_HFE]);
        log_->debugPush("RH_HFEmin", -tau_max_[RH_HFE]);
        log_->debugPush("LF_KFEmin", -tau_max_[LF_KFE]);
        log_->debugPush("RF_KFEmin", -tau_max_[RF_KFE]);
        log_->debugPush("LH_KFEmin", -tau_max_[LH_KFE]);
        log_->debugPush("RH_KFEmin", -tau_max_[RH_KFE]);

        JointState tau_margin = tau_max_.array() - tau_.array().abs();
        JointState tau_lim = (tau_margin.array() <  6.0).select(JointState::Constant(1.0), JointState::Constant(0.0));
        log_->debugPush("RF_HAA_tau_lim", tau_lim[RF_HAA]);
        log_->debugPush("LH_HAA_tau_lim", tau_lim[LH_HAA]);
        log_->debugPush("LF_HAA_tau_lim", tau_lim[LF_HAA]);
        log_->debugPush("RH_HAA_tau_lim", tau_lim[RH_HAA]);
        log_->debugPush("LF_HFE_tau_lim", tau_lim[LF_HFE]);
        log_->debugPush("RF_HFE_tau_lim", tau_lim[RF_HFE]);
        log_->debugPush("LH_HFE_tau_lim", tau_lim[LH_HFE]);
        log_->debugPush("RH_HFE_tau_lim", tau_lim[RH_HFE]);
        log_->debugPush("LF_KFE_tau_lim", tau_lim[LF_KFE]);
        log_->debugPush("RF_KFE_tau_lim", tau_lim[RF_KFE]);
        log_->debugPush("LH_KFE_tau_lim", tau_lim[LH_KFE]);
        log_->debugPush("RH_KFE_tau_lim", tau_lim[RH_KFE]);

        JointState totalFFWDTorques;
        JointState trunkContTorques = trunk_ctrl_->probeJointTorques();
        totalFFWDTorques = des_tau_ + trunkContTorques;

        log_->debugPush("LF_HAA_uff", totalFFWDTorques(LF_HAA));
        log_->debugPush("LF_HFE_uff", totalFFWDTorques(LF_HFE));
        log_->debugPush("LF_KFE_uff", totalFFWDTorques(LF_KFE));
        log_->debugPush("RF_HAA_uff", totalFFWDTorques(RF_HAA));
        log_->debugPush("RF_HFE_uff", totalFFWDTorques(RF_HFE));
        log_->debugPush("RF_KFE_uff", totalFFWDTorques(RF_KFE));
        log_->debugPush("LH_HAA_uff", totalFFWDTorques(LH_HAA));
        log_->debugPush("LH_HFE_uff", totalFFWDTorques(LH_HFE));
        log_->debugPush("LH_KFE_uff", totalFFWDTorques(LH_KFE));
        log_->debugPush("RH_HAA_uff", totalFFWDTorques(RH_HAA));
        log_->debugPush("RH_HFE_uff", totalFFWDTorques(RH_HFE));
        log_->debugPush("RH_KFE_uff", totalFFWDTorques(RH_KFE));

        log_->debugPush("LF_HAA_load" , tau_(LF_HAA));
        log_->debugPush("LF_HFE_load" , tau_(LF_HFE));
        log_->debugPush("LF_KFE_load" , tau_(LF_KFE));
        log_->debugPush("RF_HAA_load" , tau_(RF_HAA));
        log_->debugPush("RF_HFE_load" , tau_(RF_HFE));
        log_->debugPush("RF_KFE_load" , tau_(RF_KFE));
        log_->debugPush("LH_HAA_load" , tau_(LH_HAA));
        log_->debugPush("LH_HFE_load" , tau_(LH_HFE));
        log_->debugPush("LH_KFE_load" , tau_(LH_KFE));
        log_->debugPush("RH_HAA_load" , tau_(RH_HAA));
        log_->debugPush("RH_HFE_load" , tau_(RH_HFE));
        log_->debugPush("RH_KFE_load" , tau_(RH_KFE));

        log_->debugPush("LF_HAA_th", q_(LF_HAA));
        log_->debugPush("LF_HFE_th", q_(LF_HFE));
        log_->debugPush("LF_KFE_th", q_(LF_KFE));
        log_->debugPush("RF_HAA_th", q_(RF_HAA));
        log_->debugPush("RF_HFE_th", q_(RF_HFE));
        log_->debugPush("RF_KFE_th", q_(RF_KFE));
        log_->debugPush("LH_HAA_th", q_(LH_HAA));
        log_->debugPush("LH_HFE_th", q_(LH_HFE));
        log_->debugPush("LH_KFE_th", q_(LH_KFE));
        log_->debugPush("RH_HAA_th", q_(RH_HAA));
        log_->debugPush("RH_HFE_th", q_(RH_HFE));
        log_->debugPush("RH_KFE_th", q_(RH_KFE));

        log_->debugPush("LF_HAA_th_max", q_max_(LF_HAA));
        log_->debugPush("LF_HFE_th_max", q_max_(LF_HFE));
        log_->debugPush("LF_KFE_th_max", q_max_(LF_KFE));
        log_->debugPush("RF_HAA_th_max", q_max_(RF_HAA));
        log_->debugPush("RF_HFE_th_max", q_max_(RF_HFE));
        log_->debugPush("LH_HAA_th_max", q_max_(LH_HAA));
        log_->debugPush("LH_HFE_th_max", q_max_(LH_HFE));
        log_->debugPush("LH_KFE_th_max", q_max_(LH_KFE));
        log_->debugPush("RF_KFE_th_max", q_max_(RF_KFE));
        log_->debugPush("RH_HAA_th_max", q_max_(RH_HAA));
        log_->debugPush("RH_HFE_th_max", q_max_(RH_HFE));
        log_->debugPush("RH_KFE_th_max", q_max_(RH_KFE));

        log_->debugPush("LF_HAA_th_min", q_min_(LF_HAA));
        log_->debugPush("LF_HFE_th_min", q_min_(LF_HFE));
        log_->debugPush("LF_KFE_th_min", q_min_(LF_KFE));
        log_->debugPush("RF_HAA_th_min", q_min_(RF_HAA));
        log_->debugPush("RF_HFE_th_min", q_min_(RF_HFE));
        log_->debugPush("RF_KFE_th_min", q_min_(RF_KFE));
        log_->debugPush("LH_HAA_th_min", q_min_(LH_HAA));
        log_->debugPush("LH_HFE_th_min", q_min_(LH_HFE));
        log_->debugPush("LH_KFE_th_min", q_min_(LH_KFE));
        log_->debugPush("RH_HAA_th_min", q_min_(RH_HAA));
        log_->debugPush("RH_HFE_th_min", q_min_(RH_HFE));
        log_->debugPush("RH_KFE_th_min", q_min_(RH_KFE));


        JointState joint_margin_up = q_max_.array() - q_.array();
        JointState joint_lim_up = (joint_margin_up.array() <  0.01).select(JointState::Constant(1.0), JointState::Constant(0.0));
        JointState joint_margin_down = q_.array() - q_min_.array();
        JointState joint_lim_down = (joint_margin_down.array() <  0.01).select(JointState::Constant(1.0), JointState::Constant(0.0));
        Eigen::Array<bool, 12,1> joint_lim;

        for (int i = 0; i<12;i++)
        {
            joint_lim[i] = joint_lim_up[i] || joint_lim_down[i];
        }

        log_->debugPush("LF_HAA_th_lim", joint_lim(LF_HAA));
        log_->debugPush("LF_HFE_th_lim", joint_lim(LF_HFE));
        log_->debugPush("LF_KFE_th_lim", joint_lim(LF_KFE));
        log_->debugPush("RF_HAA_th_lim", joint_lim(RF_HAA));
        log_->debugPush("RF_HFE_th_lim", joint_lim(RF_HFE));
        log_->debugPush("RF_KFE_th_lim", joint_lim(RF_KFE));
        log_->debugPush("LH_HAA_th_lim", joint_lim(LH_HAA));
        log_->debugPush("LH_HFE_th_lim", joint_lim(LH_HFE));
        log_->debugPush("LH_KFE_th_lim", joint_lim(LH_KFE));
        log_->debugPush("RH_HAA_th_lim", joint_lim(RH_HAA));
        log_->debugPush("RH_HFE_th_lim", joint_lim(RH_HFE));
        log_->debugPush("RH_KFE_th_lim", joint_lim(RH_KFE));

        log_->debugPush("LF_HAA_thd", qd_(LF_HAA));
        log_->debugPush("LF_HFE_thd", qd_(LF_HFE));
        log_->debugPush("LF_KFE_thd", qd_(LF_KFE));
        log_->debugPush("RF_HAA_thd", qd_(RF_HAA));
        log_->debugPush("RF_HFE_thd", qd_(RF_HFE));
        log_->debugPush("RF_KFE_thd", qd_(RF_KFE));
        log_->debugPush("LH_HAA_thd", qd_(LH_HAA));
        log_->debugPush("LH_HFE_thd", qd_(LH_HFE));
        log_->debugPush("LH_KFE_thd", qd_(LH_KFE));
        log_->debugPush("RH_HAA_thd", qd_(RH_HAA));
        log_->debugPush("RH_HFE_thd", qd_(RH_HFE));
        log_->debugPush("RH_KFE_thd", qd_(RH_KFE));

        log_->debugPush("LF_HAA_des_th", des_q_(LF_HAA));
        log_->debugPush("LF_HFE_des_th", des_q_(LF_HFE));
        log_->debugPush("LF_KFE_des_th", des_q_(LF_KFE));
        log_->debugPush("RF_HAA_des_th", des_q_(RF_HAA));
        log_->debugPush("RF_HFE_des_th", des_q_(RF_HFE));
        log_->debugPush("RF_KFE_des_th", des_q_(RF_KFE));
        log_->debugPush("LH_HAA_des_th", des_q_(LH_HAA));
        log_->debugPush("LH_HFE_des_th", des_q_(LH_HFE));
        log_->debugPush("LH_KFE_des_th", des_q_(LH_KFE));
        log_->debugPush("RH_HAA_des_th", des_q_(RH_HAA));
        log_->debugPush("RH_HFE_des_th", des_q_(RH_HFE));
        log_->debugPush("RH_KFE_des_th", des_q_(RH_KFE));

        log_->debugPush("LF_HAA_des_thd", des_qd_(LF_HAA));
        log_->debugPush("LF_HFE_des_thd", des_qd_(LF_HFE));
        log_->debugPush("LF_KFE_des_thd", des_qd_(LF_KFE));
        log_->debugPush("RF_HAA_des_thd", des_qd_(RF_HAA));
        log_->debugPush("RF_HFE_des_thd", des_qd_(RF_HFE));
        log_->debugPush("RF_KFE_des_thd", des_qd_(RF_KFE));
        log_->debugPush("LH_HAA_des_thd", des_qd_(LH_HAA));
        log_->debugPush("LH_HFE_des_thd", des_qd_(LH_HFE));
        log_->debugPush("LH_KFE_des_thd", des_qd_(LH_KFE));
        log_->debugPush("RH_HAA_des_thd", des_qd_(RH_HAA));
        log_->debugPush("RH_HFE_des_thd", des_qd_(RH_HFE));
        log_->debugPush("RH_KFE_des_thd", des_qd_(RH_KFE));

        //trunk controller stuff
        iit::dog::WholeBodyOptimization::ConstrViolation constr_viol;
        iit::rbd::ForceVector CoMDesWrench, wrenchError;

        trunk_ctrl_->getTotalWrench(CoMDesWrench);
        trunk_ctrl_->getConstraintViolations(constr_viol);

        log_->debugPush("comWrenchLX", CoMDesWrench(rbd::LX));
        log_->debugPush("comWrenchLY", CoMDesWrench(rbd::LY));
        log_->debugPush("comWrenchLZ", CoMDesWrench(rbd::LZ));
        log_->debugPush("comWrenchAX", CoMDesWrench(rbd::AX));
        log_->debugPush("comWrenchAY", CoMDesWrench(rbd::AY));
        log_->debugPush("comWrenchAZ", CoMDesWrench(rbd::AZ));
        log_->debugPush("wrenchErrorLx", wrenchError(rbd::LX));
        log_->debugPush("wrenchErrorLy", wrenchError(rbd::LY));
        log_->debugPush("wrenchErrorLz", wrenchError(rbd::LZ));
        log_->debugPush("wrenchErrorAx", wrenchError(rbd::AX));
        log_->debugPush("wrenchErrorAy", wrenchError(rbd::AY));
        log_->debugPush("wrenchErrorAz", wrenchError(rbd::AZ));
        log_->debugPush("friction_violLF", constr_viol.friction[LF]);
        log_->debugPush("friction_violRF", constr_viol.friction[RF]);
        log_->debugPush("friction_violLH", constr_viol.friction[LH]);
        log_->debugPush("friction_violRH", constr_viol.friction[RH]);

        log_->debugPush("unilateral_violLF", constr_viol.unilateral[LF]);
        log_->debugPush("unilateral_violRF", constr_viol.unilateral[RF]);
        log_->debugPush("unilateral_violLH", constr_viol.unilateral[LH]);
        log_->debugPush("unilateral_violRH", constr_viol.unilateral[RH]);

        //ground truth for grfs
        log_->debugPush("grForcesLFWx_gt", footSensor->force[dog::LF](rbd::X));
        log_->debugPush("grForcesLFWy_gt", footSensor->force[dog::LF](rbd::Y));
        log_->debugPush("grForcesLFWz_gt", footSensor->force[dog::LF](rbd::Z));
        log_->debugPush("grForcesRFWx_gt", footSensor->force[dog::RF](rbd::X));
        log_->debugPush("grForcesRFWy_gt", footSensor->force[dog::RF](rbd::Y));
        log_->debugPush("grForcesRFWz_gt", footSensor->force[dog::RF](rbd::Z));
        log_->debugPush("grForcesLHWx_gt", footSensor->force[dog::LH](rbd::X));
        log_->debugPush("grForcesLHWy_gt", footSensor->force[dog::LH](rbd::Y));
        log_->debugPush("grForcesLHWz_gt", footSensor->force[dog::LH](rbd::Z));
        log_->debugPush("grForcesRHWx_gt", footSensor->force[dog::RH](rbd::X));
        log_->debugPush("grForcesRHWy_gt", footSensor->force[dog::RH](rbd::Y));
        log_->debugPush("grForcesRHWz_gt", footSensor->force[dog::RH](rbd::Z));

        trunk_ctrl_->getFeetForces(feetForces);

        //trunk controller desired ground reaction forces in the WF
        log_->debugPush("desFeetForcesWLFx", feetForces[LF](rbd::X));
        log_->debugPush("desFeetForcesWLFy", feetForces[LF](rbd::Y));
        log_->debugPush("desFeetForcesWLFz", feetForces[LF](rbd::Z));
        log_->debugPush("desFeetForcesWRFx", feetForces[RF](rbd::X));
        log_->debugPush("desFeetForcesWRFy", feetForces[RF](rbd::Y));
        log_->debugPush("desFeetForcesWRFz", feetForces[RF](rbd::Z));
        log_->debugPush("desFeetForcesWLHx", feetForces[LH](rbd::X));
        log_->debugPush("desFeetForcesWLHy", feetForces[LH](rbd::Y));
        log_->debugPush("desFeetForcesWLHz", feetForces[LH](rbd::Z));
        log_->debugPush("desFeetForcesWRHx", feetForces[RH](rbd::X));
        log_->debugPush("desFeetForcesWRHy", feetForces[RH](rbd::Y));
        log_->debugPush("desFeetForcesWRHz", feetForces[RH](rbd::Z));

        //trunk controller desired force in the BF
        iit::dog::LegDataMap<Eigen::Vector3d> feetForcesBase(Vector3d::Zero());
        for (int leg = LF; leg<=RH;leg++)
        {

            feetForcesBase[LegID(leg)] = iit::commons::rpyToRot(bs->getRPY_W())* feetForces[LegID(leg)];
        }

        log_->debugPush("desFeetForcesBLFx", feetForcesBase[LF](rbd::X));
        log_->debugPush("desFeetForcesBLFy", feetForcesBase[LF](rbd::Y));
        log_->debugPush("desFeetForcesBLFz", feetForcesBase[LF](rbd::Z));
        log_->debugPush("desFeetForcesBRFx", feetForcesBase[RF](rbd::X));
        log_->debugPush("desFeetForcesBRFy", feetForcesBase[RF](rbd::Y));
        log_->debugPush("desFeetForcesBRFz", feetForcesBase[RF](rbd::Z));
        log_->debugPush("desFeetForcesBLHx", feetForcesBase[LH](rbd::X));
        log_->debugPush("desFeetForcesBLHy", feetForcesBase[LH](rbd::Y));
        log_->debugPush("desFeetForcesBLHz", feetForcesBase[LH](rbd::Z));
        log_->debugPush("desFeetForcesBRHx", feetForcesBase[RH](rbd::X));
        log_->debugPush("desFeetForcesBRHy", feetForcesBase[RH](rbd::Y));
        log_->debugPush("desFeetForcesBRHz", feetForcesBase[RH](rbd::Z));

        //estimated actual ground reaction forces in the BF
        log_->debugPush("grForcesBLFy", grForces[LF](rbd::Y));
        log_->debugPush("grForcesBRFy", grForces[RF](rbd::Y));
        log_->debugPush("grForcesBLHy", grForces[LH](rbd::Y));
        log_->debugPush("grForcesBRHy", grForces[RH](rbd::Y));
        log_->debugPush("grForcesBLFz", grForces[LF](rbd::Z));
        log_->debugPush("grForcesBRFz", grForces[RF](rbd::Z));
        log_->debugPush("grForcesBLHz", grForces[LH](rbd::Z));
        log_->debugPush("grForcesBRHz", grForces[RH](rbd::Z));
        log_->debugPush("grForcesBLFx", grForces[LF](rbd::X));
        log_->debugPush("grForcesBRFx", grForces[RF](rbd::X));
        log_->debugPush("grForcesBLHx", grForces[LH](rbd::X));
        log_->debugPush("grForcesBRHx", grForces[RH](rbd::X));

}

void Controller::setTaskServoTime(double time)
{
    taskServoTime = time;
}

void Controller::writeOutFeetForces(iit::dog::LegDataMap<Vector3d>& feet_grf)
{

    //NB qdd_should be computed somewhere from controllers TODO fix this
    //compute feet forces and assign them in grForces
    updateFeetForces();
    feet_grf = grForces;
}

void Controller::updateFeetForces()
{

    //NB qdd_should be computed somewhere from controllers TODO fix this
    //compute feet forces and assign them in grForces
    feet_forces_->getFeetGRF(q_,
                             qd_,
                             tau_,
                             bs->getOrientation_W(),
                             grForces,
                             qdd_,
                             bs->getVelocity_B(),
                             Vector3d::Zero(),
                             bs->getRotationRate_B(),
                             Vector3d::Zero());
}

void Controller::startStateEstimator()
{
    ask_start_state_estimator = true;
}


void Controller::setTrunkCoM()
{
    double mass, comx, comy, comz;
    newline::getDouble("trunk mass:",  robot_params_->getValue_trunk_mass(), mass);
    newline::getDouble("trunk com x:", robot_params_->getValue_trunk_com_x(), comx);
    newline::getDouble("trunk com y:", robot_params_->getValue_trunk_com_y(), comy);
    newline::getDouble("trunk com z:", robot_params_->getValue_trunk_com_z(), comz);

    robot_params_->setValue_trunk_mass(mass);
    robot_params_->setValue_trunk_com_x(comx);
    robot_params_->setValue_trunk_com_y(comy);
    robot_params_->setValue_trunk_com_z(comz);
}

void Controller::getTrunkCoM()
{

    Vector3d trunkCom;
    double trunkMass  = robot_params_->getValue_trunk_mass();
    trunkCom(rbd::X) = robot_params_->getValue_trunk_com_x();
    trunkCom(rbd::Y) = robot_params_->getValue_trunk_com_y();
    trunkCom(rbd::Z) = robot_params_->getValue_trunk_com_z();
    std::cout<<"Trunk CoM is set to: "<<trunkCom.transpose()<<" and trunk mass to:"<< trunkMass<<std::endl<<std::endl;
    std::cout<<"Robot CoM is set to: "<< inertia_props_->getWholeBodyCOM(q_)<<" and robot mass to:"<< inertia_props_->getTotalMass()<<std::endl<<std::endl;
}

//GUI
void Controller::consoleCallFunctionGUI(std::string func_name, std::vector<double>& args, bool client) {

  service_args = args;
  supervisorClient = client;

  for (unsigned int i = 0; i < menu_console_.size(); i++) {
    if (func_name == menu_console_[i].command) {
      (menu_console_[i].func_ptr)();
    }
  }

    supervisorClient = false;
    service_args.clear();
}


} //@namespace dls_controller
