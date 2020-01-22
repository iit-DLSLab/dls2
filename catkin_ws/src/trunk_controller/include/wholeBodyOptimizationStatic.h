/*
 * WholeBodyOptimizationStatic.h
 *
 *  Created on: Jul 28, 2014
 *      Author: mfocchi
 */

#ifndef IIT_DOG_WHOLE_BODY_OPTIMIZATION_STATIC_H_
#define IIT_DOG_WHOLE_BODY_OPTIMIZATION_STATIC_H_

#include <Eigen/Dense>
#include <doglib/rbd/rbd.h>
#include <doglib/rbd/utils.h>
#include <doglib/base/inverse_dynamics.hpp>
#include <doglib/base/jsim.hpp>

#include "eiquadprog.hpp"
#include "computeJacobians.h"

namespace dls {
namespace dog {

class WholeBodyOptimizationStatic {

public:

	struct ConstrViolationStatic {
      dog::LegDataMap<double> friction;
      dog::LegDataMap<double> unilateral;
      ConstrViolationStatic(): friction(0), unilateral(0)
      {

      }
	};

    enum MinMethod{WRENCHDIRECTION = 0, NORMALS, TORQUES}; //TODO Torque
	/**
     * Default constructor
     */
    WholeBodyOptimizationStatic(dog::MotionTransformsBase & motion_transforms_in,
                                dog::InverseDynamicsBase &idObj_in,
                                dog::ForwardKinematicsBase & fwd_kin_in,
                                dog::FeetJacobiansBase &jacs_in,
                                dog::InertiaPropertiesBase & inertiaProps_in);

	~WholeBodyOptimizationStatic();

	//the convention for the optimization variables is that lambda are defined in the world frame
	//desired wrenches are in world frame
	void computeOptimization(const dog::LegDataMap<Eigen::Vector3d> & surf_normal,//TODO use ROBOTstate to reduce state dimension
									const dog::LegDataMap<double> & muEstimate,
				  	  	  	  	  	const dog::LegDataMap<bool> & stance_legs,
									const dog::LegDataMap<double> & force_max,
									const dog::LegDataMap<double> & force_min,
				  	  	  	  	  	const Eigen::Matrix3d & R,
									const JointState & q,
						 	  	  	const rbd::Vector6D & desWrench);

	void computeOptimization(const dog::LegDataMap<Eigen::Vector3d> & surf_normal,//TODO use ROBOTstate to reduce state dimension
									const dog::LegDataMap<double> & muEstimate,
				  	  	  	  	  	const dog::LegDataMap<bool> & stance_legs,
									const dog::LegDataMap<double> & force_max,
									const dog::LegDataMap<double> & force_min,
				  	  	  	  	  	const Eigen::Matrix3d & R,
									const JointState & q,
									const Eigen::Vector3d & com,
						 	  	  	const rbd::Vector6D & desWrench);
    void computeOptimization(const dog::LegDataMap<Eigen::Vector3d> & surf_normal, //TODO use ROBOTstate to reduce state dimension
                                    const dog::LegDataMap<double> & muEstimate,
                                    const dog::LegDataMap<bool> & stance_legs,
                                    const dog::LegDataMap<double> & force_max,
                                    const dog::LegDataMap<double> & force_min,
                                    const Eigen::Matrix3d & R,
                                    const JointState & q,
                                    const dog::LegDataMap<Eigen::Vector3d> & footPos_in,
                                    const dog::LegDataMap<FootJac> &JFoot_in,
                                    const Eigen::Vector3d & com,
                                    const rbd::Vector6D & desWrench);

	//traits for getters//
    void getFeetForces(LegDataMap<Vector3d> & feet_forces);
	rbd::Vector6D getWrenchError();
	//static case
	void getConstraintViolations(WholeBodyOptimizationStatic::ConstrViolationStatic & constraints_violation);
	//these methods are not available in the static case
    void getJointTorques(JointState & jointTorques);
	double getSlack();
	void getSlacks(Eigen::VectorXd  & slacks_out);

	//traits for setters//
	void setMinimizationMethod(MinMethod min_goal);
	void setWrenchWeight(rbd::Vector6D & W_wrench);
	void setContactForceWeight(Eigen::Vector3d & W_forces);
	void setTorqueWeight(Eigen::Vector3d & W_torques_leg);
	void setTorqueWeight(const dog::LegID & leg, Eigen::Vector3d & W_torques_leg);
	void setFrictionConstraint(bool frictionConstrFlag);
	void setDesiredFeetForces(const dog::LegDataMap<bool> & stance_legs,
			                  const double weight,
							  const dog::LegDataMap<Eigen::Vector3d> & input_desired_forces);
	void setBaseControl(bool baseControlFlag);
	void useSlacks(bool flag);
	void printCosts();


protected:

	void prepareOptimization(const JointState & q_curr);
	void setCostFunction(Eigen::MatrixXd & GQ, Eigen::MatrixXd& W, Eigen::VectorXd& g0);
	void setEqualities(Eigen::MatrixXd& CE, Eigen::VectorXd& ce0);
	void setInequalities(Eigen::MatrixXd& CI, Eigen::VectorXd& ci0);
	void computeConstraintViolations();
    void computeCosts(const Eigen::VectorXd & solution,  double & taskCost, double & quadCost);
    void computeSlackCost(const Eigen::VectorXd & slacks, double & slackCost);
	//constructor variables

    dog::FeetJacobiansBase& jacs; // class to compute jacobians
    dog::LegDataMap<dog::FootJac> JFoot_; // already computed jacobians
    dog::ForwardKinematicsBase & fwd_kin;
    dog::MotionTransformsBase & mt;
    dog::InertiaPropertiesBase & inertiaProps;
    dog::InverseDynamicsBase & idObj;

	//input variable for optimization
	Eigen::Matrix3d R;
	rbd::Vector6D desWrench;
	dog::LegDataMap<bool>  stance_legs;
    dog::LegDataMap<Eigen::Vector3d>  surf_normal;
    dog::LegDataMap<Eigen::Vector3d>  footPos, footVel;
    dls::rbd::VelocityVector baseTwist;
    JointState q;
	dog::LegDataMap<double> force_max, force_min;
	dog::LegDataMap<double> muEstimate;
	int number_of_slacks;
	bool use_slacks, use_multiple_slacks;

    //optimization variables (we just optimize for leg variables)
    JointState  jointTorques;
    LegDataMap<Vector3d> feet_forces;
	Eigen::Vector3d com;

    //user defined variables
	MinMethod min_goal;
	Eigen::Vector3d W_forces;
    dog::LegDataMap<Eigen::Vector3d> W_torques;
	double desired_forces_weight;
	rbd::Vector6D W_wrench;
	bool frictionConstrFlag;
	bool baseControlFlag;

	//constraint violations
	ConstrViolationStatic constr_violation;

	//internal variables
	int cleg_count, contact_forces;	//number of stance legs
    int friction_constr,num_ineq;

    static const int num_cc = 6; //number of friction constraints per leg

	//internal variables for Whole body Optimization
	Eigen::MatrixXd GQ, W, CI, CE, A; //A is used for computing wrencherror
	Eigen::VectorXd g0, ce0, ci0, x, slacks, solution, desired_forces;

    Eigen::Matrix<double, 6, 6> S;
    Eigen::Matrix<double,6, 1> b;

    //for debug
    double quadCost;
    double taskCost;
    double slackCost;
};

}
}
#endif /* WholeBodyOptimizationStatic_H_ */
