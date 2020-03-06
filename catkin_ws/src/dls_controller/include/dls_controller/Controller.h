#ifndef DLS_CONTROLLER__CONTROLLER__H
#define DLS_CONTROLLER__CONTROLLER__H

//External Libraries
#include <iostream>
#include <memory>
#include <map>
#include <Eigen/Dense>
#include <dls_controller/Logger.h>
#include <dls_controller/BaseState.h>
#include <dls_controller/FTSensorData.h>
#include <dls_controller/ContactSensorData.h>
#include <dls_controller/ShinSensorData.h>
#include <dwl/WholeBodyState.h>
#include <dwl/model/FloatingBaseSystem.h>
#include <dwl/model/WholeBodyKinematics.h>
#include <dwl/model/WholeBodyDynamics.h>
#include <terrain_server/TerrainMapInterface.h>
#include <dwl_rviz_plugin/DisplayInterface.h>
#include <dls_map_interface/GridMapInterface.hpp>
#include <dls_msgs/DesiredTrajectory.h>
#include <geometry_msgs/WrenchStamped.h>

#include <iit/commons/dog/inverse_kinematics.h>
#include <iit/locomotionutils/TrunkController.h>
#include <iit/commons/dog/shin_jacobians.h>
#include <iit/commons/dog/kin_dyn_params.h>
#include <iit/commons/dog/robot_limits.h>
#include <iit/commons/dog/feet_contact_forces.h>

 // Boost includes
#include <boost/shared_ptr.hpp>


namespace dls_controller
{

class Controller
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef iit::dog::JointState JointState;
public:
	/**
	 * @brief Controller class is the base class for all the DLS controllers.
	 * It contains generic behaviors such as reading and writing controller variables
	 */
	Controller();
        virtual ~Controller() {}

	/**
	 * @brief initBase is used to initialized base class behaviors when 
	 * instantiating child classes.
	 */
	bool initBase(int nJoints,
				  int nFTSensors,
				  int taskRate,
				  std::string robot);

	/**
	 * @brief construct functions is used to implement code that could exist on 
	 * the child class constructors because of limitation with PLUGINLIB. 
	 * 
	 */
	virtual bool construct(int nJoints,
						   int nFTSensors,
						   int taskRate,
						   std::string robot) = 0;
	
	virtual bool init(double time) = 0;
	virtual void run(double time,
					double period) = 0;
	virtual void kill() = 0;
	
	virtual bool triggerDesiredPose(dls_msgs::DesiredPose &pose) {};
	virtual void triggerDesiredTrajectory() {};

	/**
	 * @brief Sets the floating-base system information
	 *  This method provides an interface to the floating-base system
	 *  information. For instead you could obtain information about joint
	 *  ids, joint limits, contact names, etc.
	 * @param dwl::model::FloatingBaseSystem& Floating-base system reference
	 **/
	void setFloatingBaseSystem(dwl::model::FloatingBaseSystem& fbs);

	/**
	 * @brief Sets the whole-body kinematics
	 *  This method includes routine for kinematic computation
	 * @param dwl::model::WholeBodyKinematics& Whole-body kinematics reference
	 **/
	void setWholeBodyKinematics(dwl::model::WholeBodyKinematics& wkin);

        /**
         * @brief setInverseKinematics This function resets the Inverse Kinematics
         * object used to compute inverse kinematics of any dog-like robot
         * @param ik any object implementing the dog interface for inverse kinematics
         */
        void setInverseKinematics(std::shared_ptr<iit::dog::InverseKinematics> &ik);

        virtual void setRobotModels(std::shared_ptr<iit::dog::FeetJacobians>& feet_jacs,
                                    std::shared_ptr<iit::dog::ForwardKinematics>& fwd_kin,
                                    std::shared_ptr<iit::dog::ShinJacobians>& shin_jacs,
                                    std::shared_ptr<iit::dog::InertiaPropertiesBase>& inertia_props,
                                    std::shared_ptr<iit::dog::HomogeneousTransformsBase>& hom_transforms,
                                    std::shared_ptr<iit::dog::JSIMBase>& jsim,
                                    std::shared_ptr<iit::dog::InverseDynamicsBase>& inv_dyn,
                                    std::shared_ptr<iit::dog::KinDynParams>& params,
                                    std::shared_ptr<iit::dog::LimitsBase>& limits,
                                    std::shared_ptr<iit::dog::FeetContactForces>& feet_forces);

        void setTrunkController(iit::dog::TrunkController::Ptr &trunk_ctrl);

	void setDesiredTrajectory(std::shared_ptr<dls_msgs::DesiredTrajectory> &desired_trajectory);

        /**
         * @brief setTaskServoTime This function enables to set the controller internal time variable
         * that can be used to syncronize console functions with the update() function
         **/
        void setTaskServoTime(double time);

	/**
	 * @brief Sets the whole-body dynamics
	 *  This method includes routine for dynamics computation
	 * @param dwl::model::WholeBodyDynamics& Whole-body dynamics reference
	 **/
	void setWholeBodyDynamics(dwl::model::WholeBodyDynamics& wdyn);

	/**
	 * @brief Sets the terrain map interface
	 *  This method includes routines for getting the terrain data
	 * @param terrain_server::TerrainMapInterface& Terrain interface
	 **/
	void setTerrainInterface(terrain_server::TerrainMapInterface& terrain);

        void setGridMapTerrain(dls::perception::TerrainInterface &terrain);

            /**
             * @brief Sets the display interface
             *  This method includes routines for visualizing in rviz lines, arrows,
             *  spheres, cones, etc.
             * @param dwl_rviz_plugin::DisplayInterface& Display interface
             **/
        void setDisplayInterface(dwl_rviz_plugin::DisplayInterface& display);


        void readInJointDataFromRobot(const std::vector<double> &actual_q,
                                      const std::vector<double> &actual_qd,
                                      const std::vector<double> &actual_tau);
        void readInDesiredJointData(const std::vector<double> read_desired_q,
                                    const std::vector<double> read_desired_qd,
                                    const std::vector<double> read_desired_tau);
        void readInActualWholeBodyState(const dwl::WholeBodyState &actual_state);
        void readInPlannedWholeBodyState(const dwl::WholeBodyState &planned_state);
        void resetInitialPlan(const dwl::WholeBodyState& plan);
        void checkResetPlan(dwl::WholeBodyState & planned_ws);

        void readInExecutePlan(bool& execute_plan);

        void readInStateDataFromRobot(const std::vector<double> orientation,
                                      const std::vector<double> angular_velocity,
                                      const std::vector<double> linear_acceleration);
        void readInForceTorqueDataFromRobot(const std::vector<std::vector<double> > force,
                                            const std::vector<std::vector<double> > torque,
                                            const std::vector<std::vector<double> > normal);
        void readInFootContactDataFromRobot(const std::vector<bool> contact);
        void readInShinContactDataFromRobot(const std::vector<double> shin_contact_position);

        void writeOutDesiredWholeBodyState(dwl::WholeBodyState& desired_state);
	void writeOutJointDataToRobot(std::vector<double>& desired_q,
                                      std::vector<double>& desired_qd,
                                      std::vector<double>& desired_tau);
	void readBaseStateFromRobot(BaseState &bs);
	void readBaseStateGroundTruth(BaseState &bs);
	void readInJointGains(const std::vector<double> gain_p,
                              const std::vector<double> gain_i,
                              const std::vector<double> gain_d);
	void writeOutJointGains(std::vector<double>& gain_p,
                                std::vector<double>& gain_i,
                                std::vector<double>& gain_d);
	void readInJointNames(const std::vector<std::string> names);
	void readInSimStateDataFromRobot(const std::vector<double> linear_position,
                                         const std::vector<double> linear_velocity);

        inline void readInReferenceVel(const Eigen::Vector3d& vel,
                                       const Eigen::Vector3d& ang_vel)
        {
            ref_vel = vel;
            ref_ang_vel = ang_vel;
        }

        inline void readInJoystickCommands(const Eigen::Matrix<double, 6,1>& axes,
                                           const Eigen::Matrix<int, 12,1>& buttons)
        {
	    joystick_axes = axes;
	    joystick_buttons = buttons;
	}
	
	
	
	void readInDesiredJointCommand(const std::vector<double> desired_q,
                                          std::vector<double>& desired_qd);

	inline std::vector<std::string> consoleGetFunctionList() {
		return console_function_names_;
	}

	inline void consoleSetFunctionList(std::vector<std::string> funcNames) {
		console_function_names_ = funcNames;
	}

	inline std::string getControllerName() {
		return controllerName;
	}

	inline void setControllerName(std::string name) {
		controllerName = name;
	}

	inline void setServoRate(int taskRate) {
		taskServoRate = taskRate;
	}
	
	inline void setIsRobotReal(int isReal) {
		isRobotReal = isReal;
	}

        inline void setKinematicLimits(iit::dog::JointState q_min, iit::dog::JointState q_max) {
            q_min_ = q_min;
            q_max_ = q_max;
        }

        void logCommonVariables(std::shared_ptr<iit::dog::TrunkController> trunk_ctrl_);

        void startStateEstimator();

	/** @brief Calls a console command */
	void consoleCallFunction(std::string func_name);

	/** @brief Reads the position from the console */
	Eigen::Vector2d readPosition();

        iit::dog::JointState eigenToJointState(const Eigen::VectorXd &urdf_joint_state) const;

        void smoothChangePIDGains(double taskTime);
        bool isPIDGainChanging();

        void writeOutFeetForces(iit::dog::LegDataMap<Eigen::Vector3d>& feet_grf);
        void updateFeetForces();

        std::shared_ptr<Logger> getLogPtr() { return log_; }

        void consoleCallFunctionGUI(std::string func_name, std::vector<double>& args, bool client);

protected:

        ros::NodeHandle n;
        std::vector<double> service_args;
        bool supervisorClient;
        bool onGUI;

	typedef std::function<void (void)> FunctionPtr;
	struct ConsoleCommand
	{
		ConsoleCommand(const std::string _command,
                               const std::string _comment,
                               const FunctionPtr _func_ptr) : command(_command),
                                       comment(_comment), func_ptr(_func_ptr) {}
		std::string command;
		std::string comment;
		FunctionPtr func_ptr;
	};

	template<class T>
	void addConsoleFunction(const std::string command,
                                const std::string comment,
                                void(T::*fp)(void), T* obj);

	// Console commands
	void manPrint();
	void resetTerrainMap();
	void terrainData();
	void terrainCost();
	void terrainHeight();
	void terrainNormal();

        void set_vm_lin_gains();

        void set_vm_ang_gains();

        void toggle_min_goal(void);

        void setTorqueWeights(void);

        virtual void change_trunk_controller_type();
        virtual void toggle_trunk_controller();

        //joint PID smoothing feature
        void setGains();
        void setGainsIndex();

        /**
         * @brief changePIDLeg smooth change PD gains ask gains to the user
         * @note smooth time 1 second
         */
        void changePDgains();
        /**
         * @brief changePIDLeg smooth change PD gains for all joints of one leg (leg_id)
         * @param leg_id ID of the leg (e.g. LF, RF, LH, RH)
         * @param Kp proportional gain
         * @param Kd derivative gain
         * @note smooth time duration is 3 seconds
         */
        void changePIDLeg(iit::dog::LegID leg_id, double Kp, double Kd);

        /**
         * @brief setChangePIDGains programmatically change PD gains
         * @param smooth_time duration of the spline
         * @param taskTime time at which the spline starts
         */
        void setChangePIDGains(double smooth_time, double taskTime);

        /**
         * @brief setChangePIDGains programmatically change PD gains
         * @param smooth_time duration of the spline
         * @note it uses the internal time stamp to init the spline. IMPORTANT do not use in the init() only in the run()
         * becasue there might be time shifts.
         */
        void setChangePIDGains(double smooth_time);

        /**
         * @brief getTrunkCoM gets the value of the com of the trunk, of the robot and the robot total mass
         */
        void getTrunkCoM();

        /**
         * @brief setTrunkCoM sets the value of the com of the trunk and its mass
         */
        void setTrunkCoM();

        double smoothPIDtime = 0.0;
        bool smoothChangePIDFlag = false;
        double splinePIDgainsT0 = 0.0; //start time for the spline
        double trans_multPID = 0.0;
        JointState old_kp;
        JointState old_kd;
        JointState des_leg_joint_kp;
        JointState des_leg_joint_kd;

        /** @brief controller name */
        std::string controllerName;

	/** @brief controller frequency (1/period) */
	int taskServoRate;

        /** @brief time elapsed */
        double taskServoTime = 0.0;

	/** @brief number of degrees of freedom(joints) */
	unsigned int nDOF;

	/** @brief robot name */
	std::string robotName;

	/** @brief joint name */
	std::vector<std::string> joint_names;

	/** @brief Actual whole-body state information */
	std::shared_ptr<dwl::WholeBodyState> actual_ws;

	/** @brief Planned whole-body state information */
	std::shared_ptr<dwl::WholeBodyState> planned_ws;

	/** @brief Desired whole-body state information */
	std::shared_ptr<dwl::WholeBodyState> desired_ws;

	/** @brief Floating-base system information */
	std::shared_ptr<dwl::model::FloatingBaseSystem> fbs_;

	/** @brief Whole-body kinematics */
	std::shared_ptr<dwl::model::WholeBodyKinematics> wkin_;
	
	std::shared_ptr<dls_msgs::DesiredTrajectory> desired_trajectory_;


        std::shared_ptr<iit::dog::InverseKinematics> ik_;
        std::shared_ptr<iit::dog::FeetJacobians> feet_jacs_;
        std::shared_ptr<iit::dog::ForwardKinematics> fwd_kin_;
        std::shared_ptr<iit::dog::ShinJacobians> shin_jacobians_;
        std::shared_ptr<iit::dog::InertiaPropertiesBase> inertia_props_;
        std::shared_ptr<iit::dog::HomogeneousTransformsBase> hom_transforms_;
        std::shared_ptr<iit::dog::JSIMBase> jsim_;
        std::shared_ptr<iit::dog::InverseDynamicsBase> inv_dyn_;
        std::shared_ptr<iit::dog::KinDynParams> robot_params_;
        std::shared_ptr<iit::dog::LimitsBase> robot_limits_;
        std::shared_ptr<iit::dog::FeetContactForces> feet_forces_;

        std::shared_ptr<iit::dog::TrunkController> trunk_ctrl_;

	/** @brief Whole-body dynamics */
	std::shared_ptr<dwl::model::WholeBodyDynamics> wdyn_;

	/** @brief Terrain map interface */
	std::shared_ptr<terrain_server::TerrainMapInterface> terrain_;

        std::shared_ptr<dls::perception::TerrainInterface> grid_map_terrain_;

	/** @brief Display interface */
	std::shared_ptr<dwl_rviz_plugin::DisplayInterface> display_;

    /** @brief Estimated groung reaction forces in the base frame*/
    iit::dog::LegDataMap<Eigen::Vector3d> grForces;

    /** @brief Desired ground reaction forces in the world frame*/
    iit::dog::LegDataMap<Eigen::Vector3d> feetForces;

public:
        // some functions to get the joint names or indexes from DLS to DWL convention
        std::string getDWLJointName(const iit::dog::JointIdentifiers& joint_id) const;
        unsigned int getDWLJointId(const iit::dog::JointIdentifiers& joint_id) const;

        std::string getDWLLegName(const iit::dog::LegID& joint_id) const;
        unsigned int getDWLLegId(const iit::dog::LegID& joint_id) const;

public:
        /** @brief Execute plan information */
        std::shared_ptr<bool> execute_plan;
        /** @brief Tells there is an update from the controller (e.g. initial plan position) */
        bool plan_update = false;
        bool ask_start_state_estimator = false;

protected:


	/** @brief current joints position */
        JointState q_;

	/** @brief current joints velocity */
        JointState qd_;

        /** @brief current joints acceleration */
        JointState qdd_;

	/** @brief desired joints position */
        JointState des_q_;

	/** @brief desired joints velocity */
        JointState des_qd_;

	/** @brief desired joints acceleration */
        JointState des_qdd_;

	/** @brief current joints torque */
        JointState tau_;

	/** @brief desired joints torque */
        JointState des_tau_;

        JointState tau_max_;

        JointState q_min_;

        JointState q_max_;

	/** @brief joints position gain */
        JointState kp;

	/** @brief joitns integral gain*/
        JointState ki;

	/** @brief joints derivative gain*/
        JointState kd;

	/** @brief foot sensor reaction forces*/
	std::shared_ptr<FTSensorData> footSensor;

	/** @brief foot sensor switch*/
        std::shared_ptr<ContactSensorData> contactSensor;

	/** @brief shin sensor */
	std::shared_ptr<ShinSensorData> shinSensor;

        /** @brief sensor generated base state information*/
	std::shared_ptr<BaseState> bs;

	/** @brief ground truth base state information ( simulation only) */
	std::shared_ptr<BaseState> bs_gt;

	/** @brief base state reference velocity */
        Eigen::Vector3d ref_vel;
        Eigen::Vector3d ref_ang_vel;

	/** @brief joystick axes */
	Eigen::Matrix<double, 6, 1> joystick_axes;

	/** @brief joystick buttons */
	Eigen::Matrix<int, 12, 1> joystick_buttons;

	/** @brief joints reference state */
        std::vector<double> input_reference_q;
        std::vector<double> input_reference_qd;

	/** @brief flat that define if we are running simulation or in real hardware */
	bool isRobotReal = false;

	/** @brief console functions names and commands */
	std::vector<std::string> console_function_names_;
	std::vector<ConsoleCommand> menu_console_;

    std::map<int, std::string> legmap;

private:
        /** @brief Logger */
        std::shared_ptr<Logger> log_;
};


template<class T>
void Controller::addConsoleFunction(const std::string command,
									const std::string comment,
									void(T::*fp)(void), T* obj)
{
	console_function_names_.push_back(command);
	menu_console_.push_back(ConsoleCommand(command, comment, std::bind(fp, obj)));
}

} //@namespace dls_controller

#endif
