/*
 * WholeBodyOptimizationStatic.cpp
 *
 *  Created on: Jul 28, 2014
 *      Author: mfocchi
 */

#include "wholeBodyOptimizationStatic.h"
#include <iit/commons/geometry/algebra.h>
#include <iostream>
#include <iit/commons/dog/joint_id_tricks.h>

namespace iit {
namespace dog {

using namespace Eigen;
using namespace std;

WholeBodyOptimizationStatic::WholeBodyOptimizationStatic(dog::MotionTransformsBase & motion_transforms_in,
                                                         dog::InverseDynamicsBase &idObj_in,
                                                         ForwardKinematics & fwd_kin_in,
                                                         dog::FeetJacobians &jacs_in,
                                                         dog::InertiaPropertiesBase & inertiaProps_in)

: mt(motion_transforms_in),
  idObj(idObj_in),
  fwd_kin(fwd_kin_in),
  jacs(jacs_in),
  inertiaProps(inertiaProps_in)
{
	//initialize matrix (without slacks)
	GQ.resize(12+24,12+24);
	W.resize(12,12);
	CI.resize(24,12+24);
	g0.resize(12+24);
	ci0.resize(24+24);
	x.resize(12+24);
	A.resize(6, 12);

	//set minimization method for contact forces
	min_goal = NORMALS;

	//set weitghing vector for wrench
	W_wrench(rbd::LX) = 5;      W_wrench(rbd::AX) = 10;
	W_wrench(rbd::LY) = 5;      W_wrench(rbd::AY) = 10;
	W_wrench(rbd::LZ) = 10;     W_wrench(rbd::AZ) =10;

	//set weigthing matrix for forces
	W_forces <<1 ,1 ,0.01;
	W_torques = Eigen::Vector3d(5, 1 , 0.2);
	desired_forces_weight = 0.0;
	//set default values for flags
	frictionConstrFlag = true;
	baseControlFlag = false;
	use_slacks = true;
	use_multiple_slacks = false;
	number_of_slacks = 0;

}

WholeBodyOptimizationStatic::~WholeBodyOptimizationStatic()
{
}


void WholeBodyOptimizationStatic::computeOptimization(const dog::LegDataMap<Eigen::Vector3d> & surf_normal,//TODO use ROBOTstate to reduce state dimension
								const dog::LegDataMap<double> & muEstimate,
								const dog::LegDataMap<bool> & stance_legs,
								const dog::LegDataMap<double> & force_max,
								const dog::LegDataMap<double> & force_min,
								const Eigen::Matrix3d & R,
								const JointState & q,
								const rbd::Vector6D & desWrench)
{
    Eigen::Vector3d com = 	inertiaProps.getWholeBodyCOM(q);
	if (baseControlFlag){
		computeOptimization( surf_normal, muEstimate, stance_legs, force_max, force_min, R, q, Vector3d::Zero(),	desWrench);
	} else {
		computeOptimization( surf_normal, muEstimate, stance_legs, force_max, force_min, R, q, com,	desWrench);
	}
}

void WholeBodyOptimizationStatic::computeOptimization(const dog::LegDataMap<Eigen::Vector3d> & surf_normal,//TODO use ROBOTstate to reduce state dimension
								const dog::LegDataMap<double> & muEstimate,
			  	  	  	  	  	const dog::LegDataMap<bool> & stance_legs,
								const dog::LegDataMap<double> & force_max,
								const dog::LegDataMap<double> & force_min,
			  	  	  	  	  	const Eigen::Matrix3d & R,
								const JointState & q_in,
								const Eigen::Vector3d & com,
				  	  	  	  	const rbd::Vector6D & desWrench)
{
	//compute by-products
	prepareOptimization(q_in);
    computeOptimization(surf_normal, muEstimate, stance_legs, force_max, force_min, R, q_in, this->footPos, this->JFoot_, com, desWrench);
}


void WholeBodyOptimizationStatic::computeOptimization(const dog::LegDataMap<Eigen::Vector3d> & surf_normal,//TODO use ROBOTstate to reduce state dimension
								const dog::LegDataMap<double> & muEstimate,
			  	  	  	  	  	const dog::LegDataMap<bool> & stance_legs,
								const dog::LegDataMap<double> & force_max,
								const dog::LegDataMap<double> & force_min,
			  	  	  	  	  	const Eigen::Matrix3d & R,
								const JointState & q,
								const dog::LegDataMap<Vector3d> & footPos_in,
                                const dog::LegDataMap<dog::FootJac> & JFoot_in,
								const Eigen::Vector3d & com,
				  	  	  	  	const rbd::Vector6D & desWrench)
{
	//fill in variables
	this->surf_normal = surf_normal;
	this->muEstimate = muEstimate;
	this->stance_legs = stance_legs;
	this->force_max = force_max;
	this->force_min = force_min;
	this->R = R;
	this->q = q;
	this->desWrench = desWrench;
	this->com = com;
	this->footPos = footPos_in;
    this->JFoot_ = JFoot_in;

	//find number of stance legs
    cleg_count = dog::compute_stance_legs(stance_legs);
	////////////////useful variables//////////////////////
	contact_forces= dog::contactConstrCount*cleg_count;
	//ineq constraints
	friction_constr = num_cc*cleg_count;
	num_ineq = frictionConstrFlag*friction_constr;

	if (use_multiple_slacks){
		number_of_slacks = num_ineq;
	} else  {
		number_of_slacks = 1;}

    solution.resize(contact_forces);

	if (use_slacks){
		if (use_multiple_slacks){
			number_of_slacks = num_ineq;
		} else  {
			number_of_slacks = 1;
		}
		slacks.resize(number_of_slacks);
	}

	setCostFunction(GQ, W, g0);
	setInequalities(CI,ci0);

	//no equality constraints
    CE.resize(0,0);ce0.resize(0);



    x.setZero(); //x incorporates grfs and slacks x = [f' s' ]' has size contact_forces + n_ineq
	jointTorques.setZero();
    feet_forces = LegDataMap<Vector3d>(Vector3d::Zero());
	double result = Eigen::solve_quadprog(GQ, g0, CE.transpose(), ce0, CI.transpose(), ci0, x);

	//check if a solution was found
	if(result == std::numeric_limits<double>::infinity())
		{cout<<"couldn't find a feasible solution"<<endl;}
	else{
		//map feet forces into a joint state vector because the number of contact forces is variable a for loop is needed
		dog::LegJointState tau_leg;

		iit::rbd::VelocityVector gW, gB;
		gW <<0.0, 0.0, 0.0, 0.0, 0.0, -iit::rbd::g; gB.setZero();
		gB.segment(rbd::LX,3) = R*gW.segment(rbd::LX,3);
		rbd::ForceVector baseWrench; dog::JointState  h_joints;
        idObj.id_fully_actuated(baseWrench, h_joints, gB, rbd::VelocityVector::Zero(), rbd::VelocityVector::Zero(), q, dog::JointState::Zero(), dog::JointState::Zero());

		int cleg_counter = 0;
		for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
			if (stance_legs[dog::LegID(leg)]){
                feet_forces[LegID(leg)] = x.segment(cleg_counter*dog::contactConstrCount, 3);
				//compute joint torques
                tau_leg =  dog::getLegJointState(dog::LegID(leg), h_joints)-(JFoot_[dog::LegID(leg)]).transpose()*R*feet_forces[LegID(leg)];
				if (!(std::isnan(tau_leg(dog::HAA)) || std::isnan(tau_leg(dog::HFE)) || std::isnan(tau_leg(dog::KFE))))
					jointTorques.segment(dog::LegID(leg)*dog::contactConstrCount,3) = tau_leg;
				cleg_counter++;//
            }
		}


	}


    computeConstraintViolations();
    solution = x.segment(0, contact_forces);
    computeCosts(solution,taskCost, quadCost);

    if (use_slacks){
       slacks =  x.segment(contact_forces, number_of_slacks);
       computeSlackCost(slacks, slackCost);
    }


}


void WholeBodyOptimizationStatic::prepareOptimization(const JointState & q_curr){

    //update jacobians
    for(int leg= dog::LF; leg <= RH; leg++)
    {
        //compute foot position in base space via fwd kinematics
        footPos[LegID(leg)]=fwd_kin.getFootPos(q_curr, LegID(leg));
        JFoot_[LegID(leg)] = jacs.getFootJacobian(q_curr,LegID(leg));
    }
}

void WholeBodyOptimizationStatic::setCostFunction(Eigen::MatrixXd & GQ, Eigen::MatrixXd& W, Eigen::VectorXd& g0)
{
	//initialize local matrix
    b.setZero();
	A.resize(6, contact_forces); A.setZero();
	desired_forces.resize(contact_forces);

	//resize input matrix
	if (use_slacks) {
		GQ.resize(contact_forces + number_of_slacks, contact_forces + number_of_slacks); GQ.setZero();
		g0.resize(contact_forces + number_of_slacks); g0.setZero();
	} else {
		GQ.resize(contact_forces, contact_forces); GQ.setZero();
		g0.resize(contact_forces); g0.setZero();
	}


	W.resize(contact_forces, contact_forces);W.setIdentity();
	W *= 1e-4;

	//A matrix maps feet forces into CoM wrench
	int cleg_counter = 0;
	for (int leg=0; leg<dog::_LEGS_COUNT;leg++){
		if (stance_legs[dog::LegID(leg)]){
			//feet forces are already in world frame so they should not be rotated to be mapped into wrenches
			A.block(rbd::LX,cleg_counter*contactConstrCount,3,3) = Eigen::Matrix3d::Identity();
			//foot pos should be mapped in world coords
			A.block(rbd::AX, cleg_counter*contactConstrCount,3,3)= commons::skew_sim(R.transpose()*(footPos[dog::LegID(leg)] - com));

			if (min_goal == NORMALS) {
				Eigen::Matrix3d BaseChange;
				Eigen::Vector3d tangentDir1, tangentDir2;
				//compute tangent components
				tangentDir1 = Vector3d::UnitX().cross(surf_normal[dog::LegID(leg)]); tangentDir1.normalize(); //in y direction)
				tangentDir2 = surf_normal[dog::LegID(leg)].cross(tangentDir1); tangentDir2.normalize();//in x direction)
				//compute tanget components (old)
//				tangentDir1 = surf_normal[dog::LegID(leg)].cross(Vector3d::UnitY()); tangentDir1.normalize();
//				tangentDir2 = surf_normal[dog::LegID(leg)].cross(tangentDir1); tangentDir2.normalize();
				//compute rotation matrix
				BaseChange<<tangentDir1, tangentDir2,surf_normal[dog::LegID(leg)];
				W.block(cleg_counter*contactConstrCount,cleg_counter*contactConstrCount, 3, 3) = BaseChange*0.01*W_forces.asDiagonal()*BaseChange.transpose();
			}
			if (min_goal == TORQUES){
                W.block(cleg_counter*contactConstrCount,cleg_counter*contactConstrCount, 3, 3) =  R.transpose()*(JFoot_[dog::LegID(leg)])*0.01*W_torques[dog::LegID(leg)].asDiagonal()*(JFoot_[dog::LegID(leg)]).transpose()*R;
			}
		cleg_counter++;
		}
	}



	b = desWrench;
	//set wrench weighting matrix
	S.setIdentity();
	S.diagonal() = W_wrench;

	//Finds x that minimizes f = (Ax-b)' S (Ax-b) + x' W x
	//f = (Ax-b)' S (Ax-b) + x' W x = x'A'SAx - 2x'A'Sb + b'Sb + x'Wx.
	if (desired_forces_weight !=0.0)
	{
		GQ.block(0,0, contact_forces, contact_forces) = A.transpose() * S * A + desired_forces_weight*Eigen::MatrixXd::Identity(contact_forces, contact_forces);
		g0.segment(0, contact_forces) = - b.transpose()* S * A - desired_forces.transpose();
	} else {
		GQ.block(0,0, contact_forces, contact_forces) = A.transpose() * S * A + W;
		g0.segment(0, contact_forces) = - b.transpose()* S * A;
	}
	//add slacks
	double w_slack = 1e8;

	if (use_slacks){
		double w_slack = 1e8;
		if (use_multiple_slacks) {
			Eigen::MatrixXd I_slack(number_of_slacks,number_of_slacks); I_slack.setIdentity();
			GQ.block(contact_forces, contact_forces, number_of_slacks, number_of_slacks) = sqrt(w_slack)*I_slack;
		} else  {
			GQ(contact_forces, contact_forces) = sqrt(w_slack);
		}
	}
}

void WholeBodyOptimizationStatic::setDesiredFeetForces(const dog::LegDataMap<bool> & stance_legs,
		                                               const double weight,
													   const dog::LegDataMap<Vector3d> & input_desired_forces)
{
	this->desired_forces_weight = weight;
    int cleg_count = dog::compute_stance_legs(stance_legs);
	desired_forces.resize(dog::contactConstrCount*cleg_count);
	desired_forces.setZero();
	int cleg_counter = 0;
	for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
		if (stance_legs[dog::LegID(leg)]){
			desired_forces.segment(cleg_counter*dog::contactConstrCount, 3) = input_desired_forces[LegID(leg)];
			cleg_counter++;
		}
	}
}
void WholeBodyOptimizationStatic::printCosts(){

    std::cout<< "taskCost" << taskCost << "  quadCost" << quadCost <<"  slackCost" << slackCost<< std::endl <<std::endl;


    std::cout << "solution " << (solution.transpose())         << std::endl;
    std::cout << "slacks.transpose " << (slacks.transpose())           << std::endl;
    std::cout << "getWrenchError() "<< (getWrenchError().transpose()) << std::endl;

    std::cout << ((CI.leftCols(contact_forces)*solution  + ci0).transpose()) << std::endl;
}

void WholeBodyOptimizationStatic::computeCosts(const Eigen::VectorXd & solution, double & taskCost, double & quadCost){
     //var error

     taskCost=  (A*solution-b).transpose()* (A*solution-b); //this is without the reg term and should be zero!
     double linCost = - b.transpose()* S * A*solution;
     quadCost= 0.5 * solution.transpose() * GQ.block(0,0, contact_forces, contact_forces) * solution  + linCost; //this includes the reg and cano be zero
}

void WholeBodyOptimizationStatic::computeSlackCost(const Eigen::VectorXd & slacks, double & slackCost){
    //var error
	Eigen::MatrixXd I_slack(number_of_slacks,number_of_slacks); I_slack.setIdentity();
    slackCost = slacks.transpose()*I_slack*slacks;
}

void WholeBodyOptimizationStatic::setInequalities(Eigen::MatrixXd& CI, Eigen::VectorXd& ci0)
{

	double number_of_columns = 0;
	//init matrix
	if (use_slacks) {
		CI.resize(num_ineq,  contact_forces + number_of_slacks); CI.setZero();
		ci0.resize(num_ineq); ci0.setZero();
	} else {
		CI.resize(num_ineq,  contact_forces); CI.setZero();
		ci0.resize(num_ineq); ci0.setZero();
	}


	if (frictionConstrFlag){
		//set friction cone limits
		Vector3d tangentDir1, tangentDir2;
		int cleg_counter = 0; //contsrained leg count
		for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
			if (stance_legs[dog::LegID(leg)]){
				//0 constraint -- fi.n >= 0  no pulling forces, only pushing...
				CI.block(num_cc*cleg_counter+0,  contactConstrCount*cleg_counter,1,3) = surf_normal[dog::LegID(leg)].transpose();
				ci0(num_cc*cleg_counter + 0) = -force_min[dog::LegID(leg)];

				//1 constraint -- fi.n <= Fzmax => -fi.n >= -Fzmax limit normal force
				CI.block(num_cc*cleg_counter+1,  contactConstrCount*cleg_counter,1,3) = -surf_normal[dog::LegID(leg)].transpose();
				ci0(num_cc*cleg_counter + 1) =  force_max[dog::LegID(leg)];

				//add 2-6 cone constraints
				//for each tangent direction t,
				//we want: -mu*n . fi <= fi . t <= mu*n . fi,
				//(n.fi == normal component of force, t.fi = tangential component along vector t)
				//which is equivalent to the two constraints: mu*n . fi >= - fi . t and mu*n . fi >= fi . t, or equivalently
				//mu*n . fi + fi . t >=0 and mu*n . fi - fi . t >= 0


	    		//compute tanget components
				tangentDir1 = Vector3d::UnitX().cross(surf_normal[dog::LegID(leg)]); tangentDir1.normalize(); //in y direction)
				tangentDir2 = surf_normal[dog::LegID(leg)].cross(tangentDir1); tangentDir2.normalize();//in x direction)
				//compute tanget components (old)
//				tangentDir1 = surf_normal[dog::LegID(leg)].cross(Vector3d::UnitY()); tangentDir1.normalize();
//				tangentDir2 = surf_normal[dog::LegID(leg)].cross(tangentDir1); tangentDir2.normalize();
				//compute 4 constraints
				Vector3d cone_constr1, cone_constr2, cone_constr3, cone_constr4;
				cone_constr1 =  surf_normal[dog::LegID(leg)]*muEstimate[dog::LegID(leg)] + tangentDir1;
				cone_constr2 =  surf_normal[dog::LegID(leg)]*muEstimate[dog::LegID(leg)] - tangentDir1;
				cone_constr3 =  surf_normal[dog::LegID(leg)]*muEstimate[dog::LegID(leg)] + tangentDir2;
				cone_constr4 =  surf_normal[dog::LegID(leg)]*muEstimate[dog::LegID(leg)] - tangentDir2;

				CI.block(num_cc*cleg_counter + 2, contactConstrCount*cleg_counter,1,3) = cone_constr1.transpose();
				CI.block(num_cc*cleg_counter + 3, contactConstrCount*cleg_counter,1,3) = cone_constr2.transpose();
				CI.block(num_cc*cleg_counter + 4, contactConstrCount*cleg_counter,1,3) = cone_constr3.transpose();
				CI.block(num_cc*cleg_counter + 5, contactConstrCount*cleg_counter,1,3) = cone_constr4.transpose();
				ci0.segment(num_cc*cleg_counter + 2, 4).setZero();

				cleg_counter++;
			}

		}
		//add slacks
		if (use_slacks)
		{
			if (use_multiple_slacks){ //add an Identity matrix
				MatrixXd I_slack(number_of_slacks,number_of_slacks); I_slack.setIdentity();
				CI.block(0, contact_forces, number_of_slacks, number_of_slacks) = -I_slack;
			} else { //add a column
				CI.block(0, contact_forces, num_ineq, 1) = - MatrixXd::Ones(num_ineq,1);
			}
		}

	}
}


void WholeBodyOptimizationStatic::computeConstraintViolations()
{
	double distance, distance1, distance2 = 0.0;
	double K_infl = 0.001; //tuned with the friction cone errors

	int cleg_counter = 0;
	if (frictionConstrFlag){
		for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
			if (stance_legs[dog::LegID(leg)])
			{
				//consider only cone constraints not the unilaterality ones
				Matrix<double, 4,1> coneConstrVector, coneViolation;
				coneConstrVector= (CI.block(num_cc*cleg_counter+2, 0, num_cc-2, contact_forces)*x.segment(0, contact_forces)
						+ ci0.segment(num_cc*cleg_counter+2,num_cc-2));
				coneViolation = (coneConstrVector.array()>0.0).select(coneConstrVector.array(), 0.0); //it should be positive!
				distance= coneViolation.minCoeff();
				constr_violation.friction[dog::LegID(leg)] = 1/(1 + K_infl*distance*distance);

				//unilateral (only  pushing) take only one row of force min ; //n*f -force_min >0
				distance = ( CI.block(num_cc*cleg_counter + 0, 0, 1, contact_forces)*x.segment(0, contact_forces) )(0) +  ci0(num_cc*cleg_counter+0);
				if (distance <0.0) //it should be positive!
				    distance = 0.0;
				//distance = 111.0;//TODO remove it
				constr_violation.unilateral[dog::LegID(leg)] =  1/(1 + K_infl*distance*distance);
				cleg_counter++;
			}
			else
			{
				constr_violation.friction[dog::LegID(leg)] = 0.0;
				constr_violation.unilateral[dog::LegID(leg)] = 0.0;
			}
		}

	}
}
//////////////////////
//traits for getters
//////////////////////

void WholeBodyOptimizationStatic::getFeetForces(LegDataMap<Vector3d> &feet_forces)
{
	feet_forces = this->feet_forces;
}

void WholeBodyOptimizationStatic::getJointTorques(JointState &jointTorques)
{
	jointTorques = this->jointTorques;
}

double WholeBodyOptimizationStatic::getSlack()
{
    if (!use_multiple_slacks){
        return this->slacks(0);
    } else {
        std::cout << "wrong number of slacks" << std::endl;
    }

}

void WholeBodyOptimizationStatic::getSlacks(Eigen::VectorXd & slacks_out)
{
	slacks_out.resize(number_of_slacks);
	slacks_out = slacks;
}

rbd::Vector6D WholeBodyOptimizationStatic::getWrenchError()
{
	return A*x.segment(0, contact_forces)-desWrench;
}

void WholeBodyOptimizationStatic::getConstraintViolations(WholeBodyOptimizationStatic::ConstrViolationStatic & constraints_violation)
{
	if (frictionConstrFlag)
	{
		constraints_violation.friction = this->constr_violation.friction;
		constraints_violation.unilateral = this->constr_violation.unilateral;

	}
	else
	{
		constraints_violation.friction = 0.0;
		constraints_violation.unilateral= 0.0;
	}

}


////////////////////
//traits for setters
/////////////////////

void WholeBodyOptimizationStatic::setBaseControl(bool flag){
	baseControlFlag = flag;
}

void WholeBodyOptimizationStatic::setMinimizationMethod(MinMethod min_goal)
{
	this->min_goal = min_goal;
}

void WholeBodyOptimizationStatic::setWrenchWeight(rbd::Vector6D & W_wrench)
{
	this-> W_wrench = W_wrench;
}

void WholeBodyOptimizationStatic::setContactForceWeight(Vector3d & W_forces)
{
	this-> W_forces = W_forces;
}

void WholeBodyOptimizationStatic::setTorqueWeight(Eigen::Vector3d & W_torques_leg)
{
	this-> W_torques = W_torques_leg;
}

void WholeBodyOptimizationStatic::setTorqueWeight(const dog::LegID & leg, Eigen::Vector3d & W_torques_leg)
{
    this-> W_torques[leg] = W_torques_leg;
}

void WholeBodyOptimizationStatic::setFrictionConstraint(bool frictionConstrFlag)
{
	this->frictionConstrFlag = frictionConstrFlag;
}

void WholeBodyOptimizationStatic::useSlacks(bool flag)
{
	this->use_slacks = flag;
}

//close namespace
}
}



