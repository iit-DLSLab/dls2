/*
 * wholeBodyOptimization.h
 *
 *  Created on: Jul 28, 2014
 *      Author: mfocchi
 */

#ifndef IIT_DOG_WHOLE_BODY_OPTIMIZATION_H_
#define IIT_DOG_WHOLE_BODY_OPTIMIZATION_H_

#include <Eigen/Dense>
#include <iit/rbd/rbd.h>
#include <iit/rbd/utils.h>

#include <iit/commons/dog/inverse_dynamics.h>
#include <iit/commons/dog/jsim.h>

#include "eiquadprog.hpp"
#include "computeJacobians.h"

#include <iit/commons/dog/leg_bool_map.h>
#include <iit/commons/dog/feet_jacobians.h>
#include <iit/commons/planning/planning.h>

namespace iit {
namespace dog {

class WholeBodyOptimization {

public:

	struct ConstrViolation {
      LegDataMap<double> friction;
      LegDataMap<double> unilateral;
      LegDataMap<double> torque;
      LegDataMap<double> joint;
	  Eigen::VectorXd cp;
	  ConstrViolation() : friction(0), unilateral(0), torque(0), joint(0)  {
		  cp.resize(dog::_LEGS_COUNT);
		  cp.setZero();
	  }
	  //ConstrViolation() : friction(0), torque(0), joint(0) {}
	};

	enum MinMethod{WRENCHDIRECTION =0, NORMALS, TORQUES}; //TODO Torque
	enum swing_type{FORCE_BASED_OSPACE=0,FORCE_BASED_JSPACE, ACC_BASED_OSPACE, ACC_BASED_JSPACE};
	/**
     * Default constructor
     */
   WholeBodyOptimization(HomogeneousTransformsBase & hom_transforms,
                         MotionTransformsBase & motion_transforms,
                         InverseDynamicsBase &idObj_in,
                         dog::ForwardKinematics & fwd_kin_in,
                         dog::FeetJacobians & feet_jacobians_,                     
                         JSIMBase &jsim_in,
                         InertiaPropertiesBase & inertiaProps_in);

	~WholeBodyOptimization();

	//the convention for the optimization variables is that lambda are defined in the world frame
	// while base accelerations are defined in the base frame, all matrix should be computed sticking to this convention
	//desired wrenches are in world frame

    void computeOptimization(const LegDataMap<Eigen::Vector3d> & surf_normal,//TODO use ROBOTstate to reduce state dimension
                                const LegDataMap<double> & muEstimate,
                                const LegBoolMap & stance_legs,
                                const LegDataMap<double> & force_max,
                                const LegDataMap<double> & force_min,
                                const JointState & torque_limits,
								const rbd::VelocityVector & baseTwist,
                                const LegDataMap<planning::Point3d> & swingFootRef,
								const Eigen::Matrix3d & R,
								const JointState & q,
								const JointState & qd,
			  	  	  	  	  	const rbd::Vector6D & desWrench);

	//traits for getters//
    void getFeetForces(LegDataMap<Vector3d> &feet_forces);
	rbd::Vector6D getWrenchError();
	//static case
	void getConstraintViolations(WholeBodyOptimization::ConstrViolation & constraints_violation);
	//these methods are not available in the static case
	void getAccelerations(JointState & jointAccel, rbd::VelocityVector & baseAccel);
	void getJointTorques(JointState & jointTorques);
	Eigen::Vector3d getCP(); //get CP in base frame
	void getSlacks(Eigen::VectorXd & slacks_out);

	//traits for setters//
	void setMinimizationMethod(MinMethod min_goal);
	void setWrenchWeight(rbd::Vector6D & W_wrench);
	void setContactForceWeight(Eigen::Vector3d & W_forces);
	void setTorqueWeight(Eigen::Vector3d & W_torques_leg);
    void setTorqueWeight(const LegID & leg, Eigen::Vector3d & W_torques_leg);
	void setFrictionConstraint(bool frictionConstrFlag);

	void setTorqueConstraint(bool torqueConstrFlag);
    void setJointLimits(JointState & q_max, JointState & q_min);
	void setJointConstraint(bool jointConstrFlag);
	void setSwingGains(double & Kp, double & Kd);
	void setSwingGains(Eigen::Vector3d & Kp, Eigen::Vector3d & Kd);
	void setServoRate(int servo_rate);
	void setCPConstraints(bool cpConstrFlag);
	void setCPMargin(double cpmargin);
    void setSwingQdes(JointState & swing_q_des);
    void setSwingQdes(JointState & swing_q_des, JointState & swing_qd_des);
	void setSwingQdesLeg(LegID leg, Eigen::Vector3d & swing_q_des_leg);
	void setJointSpaceSwing(swing_type swing_task);
    void useStanceSpringConstraints(bool flag);
	void printCosts();
    void useSlacks(bool flag);
    std::vector<planning::LineCoeff2d> lineCoeff;

protected:

	void prepareOptimization();
	void setCostFunction(Eigen::MatrixXd & GQ, Eigen::MatrixXd& W, Eigen::VectorXd& g0);
	void setEqualities(Eigen::MatrixXd& CE, Eigen::VectorXd& ce0);
	void setInequalities(Eigen::MatrixXd& CI, Eigen::VectorXd& ci0);
    void computeJointLimits(JointState & accelMax, JointState & accelMin);
	void computeConstraintViolations(void);
    void computeForceBasedOperationalSpaceSwingTask(rbd::VelocityVector & gB, Eigen::MatrixXd & B, Eigen::MatrixXd & b);
    void computeForceBasedJointSpaceSwingTask(rbd::VelocityVector & gB, Eigen::MatrixXd & B, Eigen::MatrixXd & b);
    void computeOperationalSpaceSwingtask(rbd::VelocityVector & gB, Eigen::MatrixXd & B, Eigen::MatrixXd & b);
    void computeJointSpaceSwingTask(rbd::VelocityVector & gB, Eigen::MatrixXd & B, Eigen::MatrixXd & b);
	//for cp
	void computeCoMState(Eigen::Vector3d & com, Eigen::Vector3d & dcom);
	void computeCPConstraints(Eigen::MatrixXd & B, Eigen::MatrixXd & b);
    Eigen::Vector3d computeStanceAcceleration(LegID leg);
	void computeCosts(const Eigen::VectorXd & solution, const Eigen::VectorXd & slacks, double & taskCost, double & quadCost, double & slackCost);
    void resetStanceSpringConstraints();
    bool detectStanceChange();

    //constructor variables
    LegDataMap<FootJac> foot_jacs_; // array of computed jacobians

    MotionTransformsBase & mt;
    HomogeneousTransformsBase & ht;
    InverseDynamicsBase & idObj;
    FeetJacobians& feet_jacobians_; // class to compute feet jacobians given q
    dog::ForwardKinematics& fwd_kin;
    JSIMBase& jsim;
    InertiaPropertiesBase & inertiaProps;

	//input variable for optimization
	Eigen::Matrix3d R;
	rbd::Vector6D desWrench;
    LegBoolMap  stance_legs, swing_legs, stance_legs_old;
    LegDataMap<Eigen::Vector3d>  surf_normal;
    LegDataMap<Eigen::Vector3d>  footPos, footVel, desStanceFootPos;

    rbd::VelocityVector baseTwist;
    LegDataMap<planning::Point3d> swingFootRef;
    JointState q, qd;
    LegDataMap<double> force_max, force_min;
    LegDataMap<double> muEstimate;

    //optimization variables
    LegDataMap<Vector3d> feet_forces;
	JointState jointTorques;
    Eigen::Matrix<double, dog::jointsCount+6, 1> accelerations; //the floating base part is expressed in the base frame

    rbd::VelocityVector gW, gB;
    //user defined variables
	MinMethod min_goal;
	Eigen::Vector3d W_forces;
    LegDataMap<Eigen::Vector3d> W_torques;
	rbd::Vector6D W_wrench;
	bool frictionConstrFlag;
	bool torqueConstrFlag;
	bool jointConstrFlag;
	bool cpConstrFlag;
	bool computeQd_des = true;
	swing_type swing_task;
	double dt;
	Eigen::Vector3d capturePoint;
	double cpmargin;
	//constraint violations
	ConstrViolation constr_violation;
	int number_of_slacks;

	//internal variables
    int cleg_count; //number of stance legs
    int swing_constr;
    int contact_forces;
    int dyn_constr;
    int cp_constr;
    int num_eq;
    int torque_constr;
    int friction_constr;
    int joint_constr;
    int num_ineq;
    static const int idx_forces = dog::jointsCount+6;
    static const int idx_accel = 0;
    static const int num_cc = 6; //number of friction constraints per leg

	JointState q_max, q_min;
    JointState tau_max, accelMax, accelMin;
	Eigen::Matrix3d Kp_swing;
	Eigen::Matrix3d Kd_swing;
	JointState swing_q_des, swing_qd_des;

	//internal variables for Whole body Optimization
	Eigen::MatrixXd GQ, W, CI, CE, A; //A is used for computing wrencherror
	Eigen::Matrix<double, 6, 6> S;
	Eigen::Matrix<double,6, 1> b;
	Eigen::VectorXd g0, ce0, ci0, x, slacks, solution;

	Eigen::Matrix<double, Eigen::Dynamic, 6> Jcb;
	Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount> Jcq;
	Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount+6> Jc;
	Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount+6> Jsw;
	Eigen::Matrix<double, Eigen::Dynamic, 1> JcdQd;
	Eigen::Matrix<double, Eigen::Dynamic, 1> JswdQd;
	Eigen::Matrix<double, dog::jointsCount+6, 1> h;

    std::vector<Vector3d> sampledRelativeFeetVectors;
    std::vector<int> stance_idx;
    bool stanceSpringConstraintsFlag = false;
    bool initialized = false;

    //for debug
	double quadCost;
	double taskCost;
	double slackCost;
    bool use_slacks;
    std::map<int, std::string> legmap;

};

}
}
#endif /* WHOLEBODYOPTIMIZATION_H_ */
