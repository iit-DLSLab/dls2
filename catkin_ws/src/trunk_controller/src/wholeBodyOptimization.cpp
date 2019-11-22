/*
 * wholeBodyOptimization.cpp
 *
 *  Created on: Jul 28, 2014
 *      Author: mfocchi
 */

#include "wholeBodyOptimization.h"
#include <iit/commons/geometry/algebra.h>
#include <iit/commons/geometry/rotations.h>
#include <iostream>
#include <iit/commons/dog/joint_id_tricks.h>
#include <iit/commons/dog/jsim.h>

namespace iit {
namespace dog {

using namespace Eigen;
using namespace std;

WholeBodyOptimization::WholeBodyOptimization(HomogeneousTransformsBase &hom_transforms,
                                             MotionTransformsBase &motion_transforms,
                                             InverseDynamicsBase & idObj_in,
                                             ForwardKinematics & fwd_kin_in,
                                             dog::FeetJacobians & feet_jacobians,
                                             dog::JSIMBase & jsim_in,
                                             dog::InertiaPropertiesBase &inertiaProps_in)
    :
   ht(hom_transforms),
   mt(motion_transforms),
   idObj(idObj_in), //this creates the object
   fwd_kin(fwd_kin_in),
   feet_jacobians_(feet_jacobians),
   jsim(jsim_in),
   inertiaProps(inertiaProps_in)
{
	//initialize matrix (without slacks)
	GQ.resize(30,30);
	W.resize(30,30);
	CI.resize(24+24+24,30);
	CE.resize(18,30);
	g0.resize(30);
	ce0.resize(18);
	ci0.resize(24+24+24);
	x.resize(30);

	//set minimization method for contact forces
	min_goal = NORMALS;

	//set weitghing vector for wrench
	W_wrench(rbd::LX) = 5;      W_wrench(rbd::AX) = 10;
	W_wrench(rbd::LY) = 5;      W_wrench(rbd::AY) = 10;
	W_wrench(rbd::LZ) = 10;     W_wrench(rbd::AZ) =10;

	//set weigthing matrix for forces
	W_forces <<1 ,1 ,0.01;
    W_torques = Eigen::Vector3d(5, 1 , 0.2);

	//set PD for swing legs (operational space) conservatively
	Kp_swing = Matrix3d::Identity()*1000;
	Kd_swing = Matrix3d::Identity()*100;

	//joint limits (unless specified)
	q_max = JointState::Constant(10);
	q_min = JointState::Constant(-10);

	//set default values for flags
	frictionConstrFlag = true;
	torqueConstrFlag = true;
	jointConstrFlag = true;
	cpConstrFlag = false;
	swing_task = FORCE_BASED_OSPACE;

	tau_max = dog::JointState::Constant(1000);

    slacks.resize(1 + planning::contactConstrCount*dog::_LEGS_COUNT);
    slacks.setZero();
	dt = 1/250.0;
	cpmargin = 0.0;
	capturePoint.setZero();	

    use_slacks = true;
    stance_legs = true;
    stance_legs_old = true;

    legmap[iit::dog::LF] = "LF";
    legmap[iit::dog::RF] = "RF";
    legmap[iit::dog::LH] = "LH";
    legmap[iit::dog::RH] = "RH";

    initialized = false;
}

WholeBodyOptimization::~WholeBodyOptimization()
{
}


void WholeBodyOptimization::computeOptimization(const dog::LegDataMap<Vector3d> & surf_normal,
                                                      const dog::LegDataMap<double> & muEstimate,
                                                      const LegBoolMap & stance_legs,
                                                      const dog::LegDataMap<double> & force_max,
                                                      const dog::LegDataMap<double> & force_min,
                                                      const dog::JointState & torque_limits,
                                                      const rbd::VelocityVector & baseTwist,
                                                      const LegDataMap<planning::Point3d> &swingFootRef,
                                                      const Matrix3d & R,
                                                      const JointState & q,
                                                      const JointState & qd,
                                                      const rbd::Vector6D & desWrench)
{
	//fill in variables
	this->surf_normal = surf_normal;
	this->muEstimate = muEstimate;
	this->stance_legs = stance_legs;
	swing_legs = !stance_legs;
	this->force_max = force_max;
	this->force_min = force_min;
	this->tau_max = torque_limits;
	this->baseTwist = baseTwist;
	this->swingFootRef = swingFootRef;
	this->R = R;
	this->q = q;
	this->qd = qd;
	this->desWrench = desWrench;


	//find number of stance legs
    cleg_count = dog::compute_stance_legs(stance_legs);
	////////////////useful variables//////////////////////
    contact_forces = planning::contactConstrCount*cleg_count;
	//equality constraints
    swing_constr = planning::contactConstrCount*(dog::_LEGS_COUNT - cleg_count);
    dyn_constr = 6;
    num_eq = contact_forces+   dyn_constr ;//+ stanceSpringConstraintsFlag*contact_forces;//swing_constr
	//ineq constraints
	friction_constr = num_cc*cleg_count;
	torque_constr = 2*dog::jointsCount;
	joint_constr = 2*dog::jointsCount;
	cp_constr = cleg_count;
    num_ineq = frictionConstrFlag*friction_constr +  torqueConstrFlag*torque_constr + jointConstrFlag*joint_constr + swing_constr*2+ cpConstrFlag*cp_constr;
    if (use_slacks)
        number_of_slacks = frictionConstrFlag*1 +  swing_constr;//friction and swing, the slack on the swing equality is always there
    else
        number_of_slacks = 0;
    slacks.resize(number_of_slacks);
	solution.resize(dog::jointsCount+6 + contact_forces);
	//compute by-products
	prepareOptimization();

    //initialize stance spring setpoints
    if (!initialized && stanceSpringConstraintsFlag)
    {
        //assumes you start with 4 legs
        resetStanceSpringConstraints();
        initialized = true;
    }

	//update useful transforms peculiar only of the dynamic case
    jsim(q);


	setCostFunction(GQ, W, g0);
	setEqualities(CE,ce0);
	setInequalities(CI,ci0);

	x.setZero();
    //jointTorques.setZero();
    feet_forces = LegDataMap<Vector3d>(Vector3d::Zero());
    double result = Eigen::solve_quadprog(GQ, g0, CE.transpose(), ce0, CI.transpose(), ci0, x);

	//check if a solution was found
    if(result == std::numeric_limits<double>::infinity()) {
        std::cout << "Couldn't find a feasible solution" << std::endl;
    }
    else { accelerations = x.segment(idx_accel, dog::jointsCount+6);
		//compute the torques M*qdd + h - Jc^T*lambda //TODO selection matrix
        jointTorques = jsim.block(dog::activeJoints, 0, dog::jointsCount, dog::jointsCount+6) * accelerations + h.segment(dog::activeJoints,dog::jointsCount) - Jcq.transpose()*x.segment(idx_forces, contact_forces);
		//map feet forces into a joint state vector because the number of contact forces is variable a for loop is needed
		int cleg_counter = 0;
        for (int leg = 0; leg < dog::_LEGS_COUNT; leg++){
            if (stance_legs[LegID(leg)]){
                feet_forces[LegID(leg)] = x.segment(idx_forces + cleg_counter*planning::contactConstrCount, 3);
				cleg_counter++;
			}
		}
	}

	computeConstraintViolations();
	solution = x.segment(0, dog::jointsCount +6 +contact_forces);
	slacks =  x.segment(dog::jointsCount +6 +contact_forces, number_of_slacks);
	computeCosts(solution, slacks,taskCost, quadCost, slackCost);
    stance_legs_old = stance_legs;
}
void WholeBodyOptimization::prepareOptimization(){

    for(int leg = LF; leg <= RH; leg++)
    {
        //compute foot position in base space via fwd kinematics
        footPos[LegID(leg)]=fwd_kin.getFootPos(q, LegID(leg));
        // store computed jacobians for reuse
        foot_jacs_[LegID(leg)] = feet_jacobians_.getFootJacobian(q, LegID(leg));
        //compute foot velocity in base frame via jacobian
        footVel[LegID(leg)] = foot_jacs_[LegID(leg)] * dog::getLegJointState(LegID(leg), qd);
    }

	//compute relevant stuff
    dog::computeStanceJacobian(Jc, R, stance_legs, footPos, foot_jacs_);
	Jcq = Jc.block(0, dog::activeJoints, contact_forces, dog::jointsCount); //Jcq is in the world frame to map forces in the world into torques
	Jcb = Jc.block(0, dog::baseJoints, contact_forces, 6); //Jcb is in the world frame
	//TODO
    //compute internally baseTwist //dog::computeBaseTwist(footPos, footVel, omega, stance_legs, baseTwist);

	gW <<0.0, 0.0, 0.0, 0.0, 0.0, -iit::rbd::g; gB.setZero();
	gB.segment(rbd::LX,3) = R*gW.segment(rbd::LX,3);
	//Compute h using ID floating base with qdd=0, baseAcc=0
	rbd::ForceVector baseWrench;
	dog::JointState  h_joints;
	idObj.id_fully_actuated(baseWrench, h_joints, gB, baseTwist, rbd::VelocityVector::Zero(), q, qd, dog::JointState::Zero());
	h.segment(dog::baseJoints,6) = baseWrench; //TODO check
	h.segment(dog::activeJoints, dog::jointsCount) = h_joints;         //TODO check
}

void WholeBodyOptimization::setCostFunction(Eigen::MatrixXd & GQ, Eigen::MatrixXd& W, Eigen::VectorXd& g0)
{
	//initialize local matrix
	b.setZero();
	A.resize(6, dog::jointsCount +6 +contact_forces); A.setZero();

	//resize input matrix
	GQ.resize(dog::jointsCount+6 + contact_forces + number_of_slacks, dog::jointsCount+6 + contact_forces + number_of_slacks); GQ.setZero();
	g0.resize(dog::jointsCount+6 + contact_forces + number_of_slacks); g0.setZero();

	W.resize(dog::jointsCount+6 + contact_forces,dog::jointsCount+6 + contact_forces);W.setIdentity();
	W *=0.01;

	//only the feet forces lambda influence the linear momentum of the CoM
	//consistency between joint acceleration and feet forces will enforced by equality constraints later

	int cleg_counter = 0;
	for (int leg=0; leg<dog::_LEGS_COUNT;leg++){
		if (stance_legs[dog::LegID(leg)]){
			//feet forces are already in world frame so they should not be rotated to be mapped into wrenches
			A.block(rbd::LX, dog::jointsCount+6 + 3*cleg_counter, 3 ,3) = Eigen::Matrix3d::Identity();
			//compute weighting matrix
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
                W.block(dog::jointsCount+6 + cleg_counter*planning::contactConstrCount,dog::jointsCount+6 + cleg_counter*planning::contactConstrCount, 3, 3) = BaseChange*0.01*W_forces.asDiagonal()*BaseChange.transpose();
			}
			if (min_goal == TORQUES){
                W.block(dog::jointsCount+6 + cleg_counter*planning::contactConstrCount,dog::jointsCount+6 + cleg_counter*planning::contactConstrCount, 3, 3) =  R.transpose()*(feet_jacobians_.getFootJacobian(q,dog::LegID(leg)))*0.01*W_torques[dog::LegID(leg)].asDiagonal()*(feet_jacobians_.getFootJacobian(q,dog::LegID(leg))).transpose()*R;
			}
		cleg_counter++;
		}
	}

	//select the trunk angular acceleration omega_base (in base frame) and multiply by Ic_ang (in base frame) to get the angular momentum
	//since it is sudpposed to be in the BF I should map it to the WF for wrench consistency
    A.block(rbd::AX, rbd::AX, 3 ,3) = R.transpose()*jsim.getWholeBodyInertia().block(rbd::AX, rbd::AX,3,3)*Eigen::Matrix3d::Identity();
	b = desWrench;
	//set wrench weighting matrix
	S.setIdentity();
	S.diagonal() = W_wrench;

	//Finds x that minimizes f = (Ax-b)' S (Ax-b) + x' W x
	//f = (Ax-b)' S (Ax-b) + x' W x = x'A'SAx - 2x'A'Sb + b'Sb + x'Wx.
	GQ.block(0,0, dog::jointsCount+6 + contact_forces, dog::jointsCount+6 + contact_forces) = A.transpose() * S * A + W;
	g0.segment(0, dog::jointsCount+6 + contact_forces)  = - b.transpose()* S * A;

	//add slacks
	if (number_of_slacks >0){
        double w_slack = 1e6;
        Eigen::MatrixXd I_slack(number_of_slacks,number_of_slacks); I_slack.setIdentity();
        I_slack*=w_slack; //this is for friction
        if (number_of_slacks>1)
        {
            I_slack.block(1,1,swing_constr, swing_constr).setIdentity(); //this is for swing task joint limits
            I_slack.block(1,1,swing_constr, swing_constr)*=100;
        }
        GQ.block(dog::jointsCount+6 + contact_forces, dog::jointsCount+6 + contact_forces, number_of_slacks, number_of_slacks) =  I_slack;
	}
//    //add regularization on accel
//    double w_base_qdd = 100;
//    Eigen::MatrixXd I_qdd(6,6); I_qdd.setIdentity();
//    GQ.block(0,0,6 , 6)  += w_base_qdd * I_qdd;

}

void WholeBodyOptimization::printCosts(){

   //std::cout<< "taskCost" << taskCost << "  quadCost" << quadCost <<"  slackCost" << slackCost<< std::endl <<std::endl;
   //
   //
   //std::cout << "solution " << (solution.transpose())         << std::endl<<std::endl;
   //std::cout << "slacks.transpose " << (slacks.transpose())           << std::endl<<std::endl;
   //std::cout << "getWrenchError() "<< (getWrenchError().transpose()) << std::endl<<std::endl;

    //std::cout <<"CE :" <<(CE.leftCols(dog::jointsCount+6 + contact_forces)*solution  + ce0).transpose() << std::endl<<std::endl;
    //std::cout <<"CI :" << (CI.leftCols(dog::jointsCount+6 + contact_forces)*solution  + ci0).transpose() << std::endl<<std::endl;


    //cout << "GQ  : "  << cleg_count  << endl;
    //cout << "GQ  : "  << contact_forces  << endl;
    //cout << "GQ  : "  << stance_legs  << endl;
    //cout << "GQ  : "  << dog::_LEGS_COUNT  << endl;

    prt(num_ineq)
            prt(swing_constr)


    cout << "GQ  : "  << GQ.rows() << " x " << GQ.cols()  << endl;    // cout << GQ  << endl;
    cout << "g0  : "  << g0.rows() << " x " << g0.cols()  << endl;    // cout << g0  << endl;
    cout << "CI  : "  << CI.rows() << " x " << CI.cols()  << endl;    // cout << CI  << endl;
    cout << "ci0 : " << ci0.rows() << " x " << ci0.cols()  << endl;   // cout << ci0  << endl;
                                                                     //
    cout << "CE  : "  << CE.rows() << " x " << CE.cols()  << endl;    // cout << CE  << endl;
                                                                     //
    cout << "ce0 : " << ce0.rows() << " x " << ce0.cols()  << endl;   // cout << ce0  << endl;

    cout << "GQ  : "  << endl << GQ  << endl;

    cout << "g0  : "   << endl << g0  << endl;

    cout << "CI  : "  << endl << CI  << endl;

    cout << "ci0 : "  << endl << ci0  << endl;

    cout << "CE  : "  << endl << CE  << endl;

    cout << "ce0 : "  << endl << ce0  << endl;

}
void WholeBodyOptimization::computeCosts(const Eigen::VectorXd & solution, const Eigen::VectorXd & slacks, double & taskCost, double & quadCost, double & slackCost){
	//var error

	taskCost=  (A*solution-b).transpose()* (A*solution-b); //this is without the reg term and should be zero!
	double linCost = - b.transpose()* S * A*solution;
	quadCost= 0.5 * solution.transpose() * GQ.block(0,0, dog::jointsCount+6 + contact_forces, dog::jointsCount+6 + contact_forces) * solution  + linCost; //this includes the reg and cano be zero
	Eigen::MatrixXd I_slack(number_of_slacks,number_of_slacks); I_slack.setIdentity();
	slackCost = slacks.transpose()*I_slack*slacks;
}

void WholeBodyOptimization::setEqualities(Eigen::MatrixXd& CE, Eigen::VectorXd& ce0)
{
	bool physicsConstOn = true;

	//init matrix
	CE.resize(num_eq,  dog::jointsCount+6 + contact_forces + number_of_slacks); CE.setZero();
	ce0.resize(num_eq); ce0.setZero();

    //1-enforce contact constraints  Jc qdd  + Jcdqd=0  zero accel in the world frame
	//baseTwist is in the base frame the computed JcdQd should be in the world frame
    dog::computeJcdQd(JcdQd, stance_legs, baseTwist, gB, R, q, qd, idObj, mt, fwd_kin);
	//set the constraints
	CE.block(0, 0, contact_forces, dog::jointsCount+6) = Jc;//Jc is in world frame
    ce0.segment(0, contact_forces) = JcdQd;

	if (physicsConstOn){
        //3- enforce consistency beween joint accel constraints and feet forces,
        //to void complications since the output wrench of Cterms and Gterms are in base frame
        //we write the constraint in base frame, thus evaluatinf Jcb i base_frame
        //Jcb = Jc.leftCols(6); //Jcb is in the world frame
        //[[Ic F] -Jc^T]x + h(6) =0
        CE.block(contact_forces, 0,  dyn_constr, dog::jointsCount +6) = jsim.topRows(6);
        CE.block(contact_forces, dog::jointsCount+6,  dyn_constr,  contact_forces) = -Jcb.transpose();

    //	Compute h(6) using ID floating base with qdd=0 (not used), then the base acc wrench is found and
    //	I should multiply it for the Ic to get the base wrench h_base (this approach does not account for gravity at the joints!)
    //	rbd::VelocityVector baseAcc;
    //	dog::JointState  h_joints;
    //	idObj.id(h_joints, baseAcc, gB, baseTwist, q, qd, dog::JointState::Zero());
    //	h.segment(dog::baseJoints,6) = -jsim.getWholeBodyInertia()*baseAcc; //the sign should be flipped to get the torque!
    //	h.segment(dog::activeJoints, dog::jointsCount) = h_joints;  //!! there is not gravity in here

        //h has been computed in prepare optimization

        ce0.segment(contact_forces,  dyn_constr) = h.segment(dog::baseJoints,6);
	}

    //define relative stance constraints in the base frame
    if (stanceSpringConstraintsFlag)
    {
        //the inout is already a stance leg so I can cycle on stance_idx
        //check if there is any stance change
        if (detectStanceChange())
            resetStanceSpringConstraints();

        //set the constraints
        //CE.block(contact_forces + dyn_constr, 0, contact_forces, dog::jointsCount+6) = Jc;//Jc is in the base frame

        //add stance zero accel  constraints in the world frame
        Eigen::Matrix<double, Eigen::Dynamic, 1> des_accelerations;
        des_accelerations.resize(contact_forces);
        int cleg_count = 0;
        for (int i = 0; i<dog::_LEGS_COUNT; i++){
            if (stance_legs[dog::LegID(i)]) // if leg is constrained then leg is in stance, ie no need to check all eff_dof
            {
                des_accelerations.segment(cleg_count*dog::jointsLegCount,3) = computeStanceAcceleration(dog::LegID(i));
                cleg_count++;
            }
        }
        std::cout<<"des_accelerations: "<<des_accelerations.transpose() <<std::endl;
        //ce0.segment(contact_forces + dyn_constr, contact_forces) = b_JcdQd - des_accelerations;
    }
}

//this task is implemented at the force level
void  WholeBodyOptimization::computeForceBasedOperationalSpaceSwingTask(rbd::VelocityVector & gB, Eigen::MatrixXd & B, Eigen::MatrixXd & b)
{
    dog::computeJcdQd(JswdQd, swing_legs, baseTwist, gB, Matrix3d::Identity(), q, qd, idObj, mt, fwd_kin); //JcdQd is in base frame cause the task is in base frame
	VectorXd tauDes;

	//can have more than one swing leg at time!
	B.resize(swing_constr,  jointsCount +6); B.setZero();
	b.resize(swing_constr,1); b.setZero();

	tauDes.resize(swing_constr); tauDes.setZero();//has the same size of


	int sleg_count = 0;
	for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
		if (swing_legs[dog::LegID(leg)]){
			dog::JointIdentifiers id = dog::toJointID(dog::LegID(leg),dog::HAA);
            tauDes.segment(jointsLegCount*sleg_count,3) = jsim.getFixedBaseBlock().block(id, id ,3,3) * (feet_jacobians_.getFootJacobian(q,LegID(leg)).inverse()*(swingFootRef[LegID(leg)].xdd - JswdQd.segment(sleg_count*dog::jointsLegCount,3)))
                + feet_jacobians_.getFootJacobian(q,LegID(leg)).transpose()*(Kp_swing*(swingFootRef[dog::LegID(leg)].x - footPos[dog::LegID(leg)])  + Kd_swing*(swingFootRef[dog::LegID(leg)].xd - footVel[dog::LegID(leg)]))
				- h.segment(activeJoints + id,3);
			//NB I should  need to compensate for h here, but this would cancel out with the h coming from the torque
			//- h.segment(activeJoints + id,3) + h.segment(activeJoints + id,3) = 0;

			//CE = [ [F'  Mj] -Jcq^t = 0]  I have no contact forces because they are swing legs B = [F'  Mj]
			//set M (fill in first n+6 columns)
            B.block(jointsLegCount*sleg_count, 0, 3, jointsCount+6) = jsim.block(activeJoints + id, 0 , 3 ,jointsCount+6);
			sleg_count++;
		}
	}

	b = -tauDes;
}

//this task is implemented at the force level (this does not work I dont know why)
void  WholeBodyOptimization::computeForceBasedJointSpaceSwingTask(rbd::VelocityVector & gB, Eigen::MatrixXd & B, Eigen::MatrixXd & b)
{
    dog::computeJcdQd(JswdQd, swing_legs, baseTwist, gB, Matrix3d::Identity(), q, qd, idObj, mt, fwd_kin); //JcdQd is in base frame cause the task is in base frame
	VectorXd tauDes;

	//can have more than one swing leg at time!
	B.resize(swing_constr,  jointsCount +6); B.setZero();
	b.resize(swing_constr,1); b.setZero();

	tauDes.resize(swing_constr); tauDes.setZero();//has the same size of




	int sleg_count = 0;
	for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
		if (swing_legs[dog::LegID(leg)]){
            dog::JointIdentifiers id = dog::toJointID(dog::LegID(leg),dog::HAA);

			Vector3d leg_swing_qd_des;
			if (computeQd_des)
                leg_swing_qd_des = feet_jacobians_.getFootJacobian(q,LegID(leg)).inverse()*swingFootRef[dog::LegID(leg)].xd;
			else
                leg_swing_qd_des = dog::getLegJointState(LegID(leg),swing_qd_des);

			//NB I dont need to compensate for h here, inv dyn it comes naturally from the optimization
            tauDes.segment(jointsLegCount*sleg_count,3) = jsim.getFixedBaseBlock().block(id, id ,3,3) * (feet_jacobians_.getFootJacobian(q,LegID(leg)).inverse()*(swingFootRef[LegID(leg)].xdd - JswdQd.segment(sleg_count*dog::jointsLegCount,3)))+
                    Kp_swing*(dog::getLegJointState(LegID(leg),swing_q_des) - dog::getLegJointState(LegID(leg),q))  +
                    Kd_swing*(leg_swing_qd_des - dog::getLegJointState(LegID(leg),qd))
					- h.segment(activeJoints + id,3);
			//NB I should  need to compensate for h here, but this would cancel out with the h coming from the torque
			//- h.segment(activeJoints + id,3) +  h.segment(activeJoints + id,3) = 0;

			//CE = [ [F'  Mj] -Jcq^t = 0]  I have no contact forces because they are swing legs B = [F'  Mj]
			//set M (fill in first n+6 columns)
            B.block(jointsLegCount*sleg_count, 0, 3, jointsCount+6) = jsim.block(activeJoints + id, 0 , 3 ,jointsCount+6);
			sleg_count++;
		}
	}

	b = -tauDes;
}

//this tasks are implemented at the accel level
void  WholeBodyOptimization::computeOperationalSpaceSwingtask(rbd::VelocityVector & gB, Eigen::MatrixXd & B, Eigen::MatrixXd & b)
{
    dog::computeStanceJacobian(Jsw, Matrix3d::Identity(), swing_legs, footPos, foot_jacs_);//Jsw and JswdQd are in base frame
    dog::computeJcdQd(JswdQd, swing_legs, baseTwist, gB, Matrix3d::Identity(), q, qd, idObj, mt, fwd_kin);
	VectorXd swing_accDes;
	//can have more than one swing leg at time!
	B.resize(swing_constr, jointsCount +6); B.setZero();
	b.resize(swing_constr,1); b.setZero();
	swing_accDes.resize(swing_constr);	swing_accDes.setZero();

    int sleg_count = 0;
    for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
        if (swing_legs[dog::LegID(leg)]){
            //TODO do not do compute when frame change
            //compute acceleration and fill in the constraints
            //set the constraints
            //these variables are in base frame

            //compute PD in operational space
            dog::JointIdentifiers id = dog::toJointID(dog::LegID(leg),dog::HAA);

            //idyn ospace
            swing_accDes.segment(planning::contactConstrCount*sleg_count,3) = swingFootRef[dog::LegID(leg)].xdd +
                    Kp_swing*(swingFootRef[dog::LegID(leg)].x - footPos[dog::LegID(leg)])  + Kd_swing*(swingFootRef[dog::LegID(leg)].xd - footVel[dog::LegID(leg)]);
            sleg_count++;
        }
    }
    B = Jsw;
	b = -swing_accDes + JswdQd;
}


void  WholeBodyOptimization::computeJointSpaceSwingTask(rbd::VelocityVector & gB, Eigen::MatrixXd & B, Eigen::MatrixXd & b)
{
	VectorXd qdd_des;
    dog::computeJcdQd(JswdQd, swing_legs, baseTwist, gB, Matrix3d::Identity(), q, qd, idObj, mt, fwd_kin);
	//can have more than one swing leg at time!
	B.resize(swing_constr, jointsCount +6); B.setZero();
	b.resize(swing_constr,1); b.setZero();
	qdd_des.resize(swing_constr);	qdd_des.setZero();

	int sleg_count = 0;
	for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
		 if (swing_legs[dog::LegID(leg)]){
			 //these variables are in joint frame
			 //compute PD in joint space
             qdd_des.segment(jointsLegCount*sleg_count,3) = feet_jacobians_.getFootJacobian(q,LegID(leg)).inverse()*(swingFootRef[LegID(leg)].xdd - JswdQd.segment(sleg_count*dog::jointsLegCount,3)) +
                     Kp_swing*(dog::getLegJointState(LegID(leg),swing_q_des) - dog::getLegJointState(LegID(leg),q))  +
                     Kd_swing*(feet_jacobians_.getFootJacobian(q,LegID(leg)).inverse()*swingFootRef[dog::LegID(leg)].xd - dog::getLegJointState(LegID(leg),qd));
             B.block(planning::contactConstrCount*sleg_count, activeJoints + jointsLegCount*LegID(leg), 3, 3).setIdentity();
			 sleg_count++;
		 }
	}
	b = -qdd_des;
}

void WholeBodyOptimization::setInequalities(Eigen::MatrixXd& CI, Eigen::VectorXd& ci0)
{

	if (!(frictionConstrFlag)&& !(torqueConstrFlag) && !(jointConstrFlag))
		CI.resize(0,0); ci0.resize(0);
	//init matrix
    CI.resize(num_ineq +  number_of_slacks,  dog::jointsCount+6 + contact_forces + number_of_slacks); CI.setZero();
    ci0.resize(num_ineq+  number_of_slacks); ci0.setZero();

	if (frictionConstrFlag){
		//set friction cone limits
		Vector3d tangentDir1, tangentDir2;
		int cleg_counter = 0; //contsrained leg count
		for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
			if (stance_legs[dog::LegID(leg)]){

				//0 constraint -- fi.n >= 0  no pulling forces, only pushing...
				CI.block(num_cc*cleg_counter+0, dog::jointsCount+6 + 3*cleg_counter,1,3) = surf_normal[dog::LegID(leg)].transpose();
				ci0(num_cc*cleg_counter + 0) = -force_min[dog::LegID(leg)];

				//1 constraint -- fi.n <= Fzmax => -fi.n >= -Fzmax limit normal force
				CI.block(num_cc*cleg_counter+1, dog::jointsCount+6 + 3*cleg_counter,1,3) = -surf_normal[dog::LegID(leg)].transpose();
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

				CI.block(num_cc*cleg_counter + 2, dog::jointsCount+6 + 3*cleg_counter,1,3) = cone_constr1.transpose();
				CI.block(num_cc*cleg_counter + 3, dog::jointsCount+6 + 3*cleg_counter,1,3) = cone_constr2.transpose();
				CI.block(num_cc*cleg_counter + 4, dog::jointsCount+6 + 3*cleg_counter,1,3) = cone_constr3.transpose();
				CI.block(num_cc*cleg_counter + 5, dog::jointsCount+6 + 3*cleg_counter,1,3) = cone_constr4.transpose();
				ci0.segment(num_cc*cleg_counter + 2, 4).setZero();

				cleg_counter++;
			}

		}
        if (number_of_slacks>0){
            //add slacks (I set only the slacks of ones (s1), the other column (s2) if exist it will be of zeros and it will be already initialized
            CI.block(0,   dog::jointsCount+6 + contact_forces, friction_constr, 1) =  MatrixXd::Ones(friction_constr,1); //first slack is
        }
	}

	int idx_torque_constr = frictionConstrFlag*friction_constr;
	if (torqueConstrFlag){

		//set torque limits (no slacks here!)
		// - [ [F'  Mj] -Jcq^t] x -h(12) + Tau_max >0     	tau<Tau_max  -tau>-Tau_max
		// [ [F'  Mj] -Jcq^t] x + h(12) -Tau_min >0 	    tau>Tau_min
		//set max torque constraints (12)
        CI.block(idx_torque_constr,	 dog::baseJoints, dog::jointsCount, dog::jointsCount+6) = (-1.0) * jsim.bottomRows(dog::jointsCount);
		CI.block(idx_torque_constr,  dog::jointsCount+6, dog::jointsCount, contact_forces) = (-1.0)*(-Jcq.transpose());
		ci0.segment(idx_torque_constr,  dog::jointsCount) = -h.segment(dog::activeJoints, dog::jointsCount) + tau_max;
		//set min torque constraints (12)
        CI.block(idx_torque_constr +  dog::jointsCount, dog::baseJoints, dog::jointsCount, dog::jointsCount+6) = (1.0) *  jsim.bottomRows(dog::jointsCount);;
		CI.block(idx_torque_constr +  dog::jointsCount, dog::jointsCount+6, dog::jointsCount, contact_forces) = (1.0)*(-Jcq.transpose());
		ci0.segment(idx_torque_constr + dog::jointsCount,dog::jointsCount) = h.segment(dog::activeJoints, dog::jointsCount) + tau_max;// Tau_min = - Tau_max therefore -Tau_min = Tau_max
	}
	int idx_joint_constr = frictionConstrFlag*friction_constr + torqueConstrFlag*torque_constr;

    if (jointConstrFlag){
        //set joint limits as acceleration constraints qdd > lower_bound
        dog::JointState accelMax, accelMin;
        computeJointLimits(accelMax,accelMin);
        //upper bound
        CI.block(idx_joint_constr, dog::activeJoints, dog::jointsCount, dog::jointsCount) = -1.0 * Eigen::Matrix<double,dog::jointsCount, dog::jointsCount>::Identity(); //upper bound qmax
        ci0.segment(idx_joint_constr, dog::jointsCount) = accelMax; //qdd< Amax => -qdd>-Amax
        //lower bound
        CI.block(idx_joint_constr + dog::jointsCount,  dog::activeJoints, dog::jointsCount, dog::jointsCount) = 1.0*Eigen::Matrix<double,dog::jointsCount, dog::jointsCount>::Identity(); //lower bound qmin
        ci0.segment(idx_joint_constr + dog::jointsCount, dog::jointsCount) = -accelMin;//qdd> Amin
    }

	int idx_cp_constr = frictionConstrFlag*friction_constr + torqueConstrFlag*torque_constr + jointConstrFlag*joint_constr;
	if ((cpConstrFlag)&&(cleg_count>2)){
		Eigen::MatrixXd B,b;
		computeCPConstraints(B,b);
		CI.block(idx_cp_constr, dog::baseJoints, cp_constr, dog::jointsCount+6) = B;
		ci0.segment(idx_cp_constr,	cp_constr) = b;
	}

    //enforce swing constraint (if any) Jsw qdd  + (Jswdqd - Xsw_dd ) = 0 (in base Frame not in the world frame like before!)
    //
    Eigen::MatrixXd B,b;

    switch (swing_task){
    case FORCE_BASED_OSPACE:
        computeForceBasedOperationalSpaceSwingTask(gB,B,b);
        break;
    case FORCE_BASED_JSPACE:
        computeForceBasedJointSpaceSwingTask(gB,B,b);
        break;//TODO
    case ACC_BASED_OSPACE:
        computeOperationalSpaceSwingtask(gB,B,b);
        break;
    case ACC_BASED_JSPACE:
        computeJointSpaceSwingTask(gB,B,b);
        break;
    }

    //set equality (get only accel)
    CI.block(idx_cp_constr, 0, swing_constr, dog::jointsCount+6) = B;
    ci0.segment(idx_cp_constr, swing_constr) = b;


    CI.block(idx_cp_constr+swing_constr, 0, swing_constr, dog::jointsCount+6) = -B;
    ci0.segment(idx_cp_constr+swing_constr, swing_constr) = -b;

    if (number_of_slacks>0)
    { //ensure positivitiy of slacks
        //lower bound part slack of swing constraint CIx+ ci0  > -eps , CIx+ ci0 +eps >0
        CI.block(idx_cp_constr, dog::jointsCount+6 + contact_forces + 1,  swing_constr, swing_constr).setIdentity(); //first slack is
        //upper bound  part slack CIx+ ci0  < eps , CIx+ci0 -eps <0 , -CIx -ci0 +eps >0
        CI.block(idx_cp_constr+swing_constr, dog::jointsCount+6 + contact_forces + 1,  swing_constr, swing_constr).setIdentity(); //first slack is
        CI(idx_cp_constr+2*swing_constr, dog::jointsCount+6 + contact_forces) = 1; //friction slack positivity
        CI.block(idx_cp_constr+2*swing_constr+1, dog::jointsCount+6 + contact_forces+1,swing_constr, swing_constr).setIdentity();//swing slacks positivity
    }


}



//is in the base frame TODO in the world frame
void WholeBodyOptimization::computeCPConstraints(Eigen::MatrixXd & B, Eigen::MatrixXd & b)
{

	//compute line coeffs

	std::vector<double> factor_a, factor_b, factor_c;

	lineCoeff.resize(cp_constr); //TODO
	factor_a.resize(cp_constr); factor_b.resize(cp_constr); factor_c.resize(cp_constr);

	B.resize(cp_constr, dog::jointsCount+6); b.resize(cp_constr, 1);
	B.setZero();b.setZero();

	//compute com  position and velocity in the base frame
	Vector3d com, dcom;
	computeCoMState(com, dcom);


	//compute omega
	double omega = sqrt(iit::rbd::g/com(rbd::Z));
	double factor_d = (dt*(omega*dt+2))/(2*omega);

	//compute CP for debugging purposes
	capturePoint = com + dcom/omega;

    std::vector<Vector3d> footCCwiseSorted; footCCwiseSorted.resize(cp_constr);
	int edgeCounter = 0;
	//extract polygon vertex in the base frame
	for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
		if (stance_legs[dog::LegID(leg)]){
			//TODO double check coeffs
            footCCwiseSorted[edgeCounter] = footPos[dog::LegID(leg)].head(2);
			edgeCounter++;
		}

	}
    planning::CounterClockwiseSort(footCCwiseSorted);

	// the com projection must always lay on one side of triangle side:
   	// p*xcp + q*ycp  +r  > + stability_margin
	//compute line coeff
	Eigen::Matrix<double, 1, 6> S;
	S.setZero();
	//compute com jacobian (in base frame)
	//since base accelerations are in the base frame and I want to set a constraint in the base frame I set R = eye(3)
    Eigen::Matrix<double, 6, dog::jointsCount+6> Jcom = getWholeBodyCOMJacobianFB(Matrix3d::Identity(), q, inertiaProps , ht);

	for(int edgeCounter = 0; edgeCounter<lineCoeff.size(); edgeCounter++){
			//compute the coeffs of the line between two feet
            lineCoeff[edgeCounter] = planning::LineCoeff(footCCwiseSorted[edgeCounter],	footCCwiseSorted[(edgeCounter + 1) % (cp_constr)], false); //I set true to normalize and use stab margin
 			factor_a[edgeCounter] = lineCoeff[edgeCounter].p*dt*factor_d;
			factor_b[edgeCounter] = lineCoeff[edgeCounter].q*dt*factor_d;
			factor_c[edgeCounter] = lineCoeff[edgeCounter].p*com(rbd::X) + lineCoeff[edgeCounter].q*com(rbd::Y) +
							 lineCoeff[edgeCounter].p*dcom(rbd::X) + lineCoeff[edgeCounter].q*dcom(rbd::Y) * (1/omega +dt) +  lineCoeff[edgeCounter].r;

			S(rbd::LX) = factor_a[edgeCounter]; S(rbd::LY) = factor_b[edgeCounter];
			// CI^T x + ci0 >= 0
			B.row(edgeCounter) = S*Jcom;

			b(edgeCounter) = factor_c[edgeCounter] ; // I should  add dJcomQd but I neglect it TODO
	}

}

//computes com in a frame with the origin on the support polygon and aligned with the base frame
void WholeBodyOptimization::computeCoMState(Eigen::Vector3d & com, Eigen::Vector3d & dcom)
{
	//compute the base position about the support polygon
	Vector3d feet_cumsum, basePos;
	feet_cumsum.setZero();
	int leg_count = 0;
	for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
		if (stance_legs[dog::LegID(leg)]){ // if leg _X_ is constrained then leg is in stance, ie no need to check all eff_dofs
			feet_cumsum += footPos[dog::LegID(leg)];
			leg_count++;
		}
	}
    if (leg_count >0)
    {
        basePos = -feet_cumsum/(double)leg_count;

        //add the comoffset (in base frame)
        com  = inertiaProps.getWholeBodyCOM(q) + Vector3d(0,0, basePos(rbd::Z));
        //since base accelerations are in the base frame and I want to set a constraint in the base frame I set R = eye(3)
        rbd::VelocityVector comTwist  = getWholeBodyCOMVelFB(baseTwist, Matrix3d::Identity(), q, qd, inertiaProps, ht);
        dcom = rbd::linearPart(comTwist);
    }
}

void WholeBodyOptimization::computeJointLimits(dog::JointState & accelMax, dog::JointState & accelMin)
{
	//real velocity estimation
    double Dt = 5*dt;
	accelMax = (q_max- q - Dt*qd)*2/(Dt*Dt);
	accelMin = (q_min - q - Dt*qd)*2/(Dt*Dt);
	//alternative methods
	//using tanh
	//	accelMax =  -100* (0.5*tanh(50.0* (q  + 0.1 - q_max))  -0.5);
	//  accelMin =  -100* (0.5*tanh(50.0* (q - 0.1 - q_min))  +0.5);
	//using sigmoids
	//	double K = 30; //sigmoid starts to apply around 0.4 and gets 0.5 at 0.2 and 0 at 0
	//	accelMax = 100*1/(exp(-K*(q_max - q -0.2))+1);
	//  accelMin = -100*1/(exp(-K*(q - q_min-0.2)) +1);
}


void WholeBodyOptimization::computeConstraintViolations()
{
	double distance, distance1, distance2 = 0.0;
	double K_infl = 0.001; //tuned with the friction cone errors

	int cleg_counter = 0;
	if (frictionConstrFlag){
		for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
			if (stance_legs[dog::LegID(leg)]){

			    //consider only cone constraints not the unilaterality ones
			    Matrix<double, 4,1> coneConstrVector, coneViolation;
                coneConstrVector= (CI.block(num_cc*cleg_counter+2, 0, num_cc-2, dog::fbjointsCount +contact_forces)*x.segment(0, dog::fbjointsCount + contact_forces)
			            + ci0.segment(num_cc*cleg_counter+2,num_cc-2));
			    coneViolation = (coneConstrVector.array()>0.0).select(coneConstrVector.array(), 0.0); //it should be positive!
			    distance= coneViolation.minCoeff();
			    constr_violation.friction[dog::LegID(leg)] = 1/(1 + K_infl*distance*distance);

			    //unilateral (only  pushing) take only one row of force min ; //n*f -force_min >0
			    distance = ( CI.block(num_cc*cleg_counter + 0, 0, 1, dog::jointsCount + 6 +contact_forces)*x.segment(0,dog::jointsCount + 6 +contact_forces) )(0) +  ci0(num_cc*cleg_counter+0);
			    if (distance <0.0) //it should be positive!
			        distance = 0.0;
			    //distance = 111.0;//TODO remove it
			    constr_violation.unilateral[dog::LegID(leg)] =  1/(1 + K_infl*distance*distance);
			    cleg_counter++;
			} else{
				constr_violation.friction[dog::LegID(leg)] = 0.0;
			    constr_violation.unilateral[dog::LegID(leg)] = 0.0;
				}
		}

	}



	if (torqueConstrFlag){
		for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
			//positive torque
		    Matrix<double, 3,1> legVectorUp, legVectorDown, legViolUp, legViolDown;
		    //selects the vector3d for a leg
		    legVectorUp = CI.block(frictionConstrFlag*friction_constr + jointsLegCount*LegID(leg), 0, jointsLegCount, dog::fbjointsCount + contact_forces)*x.segment(0, dog::fbjointsCount +contact_forces)
						+ ci0.segment(frictionConstrFlag*friction_constr + jointsLegCount*LegID(leg),jointsLegCount);
			//negative torque
		    legVectorDown= CI.block(frictionConstrFlag*friction_constr + jointsLegCount*LegID(leg) + jointsCount, 0, jointsLegCount, dog::fbjointsCount + contact_forces)*x.segment(0, dog::fbjointsCount +contact_forces)
						+ ci0.segment(frictionConstrFlag*friction_constr + jointsLegCount*LegID(leg) + jointsCount,jointsLegCount);
		    legViolUp = (legVectorUp.array()>0.0).select(legVectorUp.array(), 0.0);
		    legViolDown = (legVectorDown.array()>0.0).select(legVectorDown.array(), 0.0);

		    distance1 = legViolUp.minCoeff();
		    distance2 = legViolDown.minCoeff();

			distance = std::min(distance1, distance2);
			constr_violation.torque[dog::LegID(leg)] = 1/(1 + K_infl*distance*distance);
		}
	}

	if (jointConstrFlag){
		for (int leg = 0; leg<dog::_LEGS_COUNT; leg++){
				Matrix<double, 3,1> legVectorUp, legVectorDown, legViolUp, legViolDown;
				//selects the vector3d for a leg
				legVectorUp = CI.block(frictionConstrFlag*friction_constr + torqueConstrFlag*torque_constr + jointsLegCount*LegID(leg), 0, jointsLegCount, dog::fbjointsCount + contact_forces)*x.segment(0, dog::jointsCount + 6 +contact_forces)
                                    + ci0.segment(frictionConstrFlag*friction_constr + torqueConstrFlag*torque_constr + jointsLegCount*LegID(leg),jointsLegCount);
				//negative torque
				legVectorDown= CI.block(frictionConstrFlag*friction_constr + torqueConstrFlag*torque_constr + jointsLegCount*LegID(leg) + jointsCount, 0, jointsLegCount, dog::fbjointsCount + contact_forces)*x.segment(0, dog::jointsCount + 6 +contact_forces)
                                    + ci0.segment(frictionConstrFlag*friction_constr + torqueConstrFlag*torque_constr + jointsLegCount*LegID(leg) + jointsCount,jointsLegCount);
                legViolUp = (legVectorUp.array()>0.0).select(legVectorUp.array(), 0.0);
				legViolDown = (legVectorDown.array()>0.0).select(legVectorDown.array(), 0.0);

				distance1 = legViolUp.minCoeff();
				distance2 = legViolDown.minCoeff();

				distance = std::min(distance1, distance2);
				constr_violation.joint[dog::LegID(leg)] = 1/(1 + K_infl*distance*distance);
			}
	}
	if (cpConstrFlag){
		K_infl = 30; //adjusted otherwise is always 0.99..
		constr_violation.cp.resize(cleg_count);
		constr_violation.cp.setZero();
		for (int edge = 0; edge<cleg_count;edge ++){
			int idx_cop_constr = frictionConstrFlag*friction_constr + torqueConstrFlag*torque_constr + jointConstrFlag*joint_constr + edge;
			distance= CI.row(idx_cop_constr)*x.segment(0, dog::jointsCount + 6 +contact_forces)	+ ci0(idx_cop_constr);
			constr_violation.cp[edge] = 1/(1 + K_infl*distance*distance);
		}
	}
}


//////////////////////
//traits for getters
//////////////////////
void WholeBodyOptimization::getAccelerations(JointState & jointAccel, rbd::VelocityVector & baseAccel)
{
	jointAccel = this->accelerations.segment(activeJoints, dog::jointsCount);
	baseAccel =  this->accelerations.segment(baseJoints, 6);
}

void WholeBodyOptimization::getFeetForces(LegDataMap<Vector3d> & feet_forces)
{
	feet_forces = this->feet_forces;
}

void WholeBodyOptimization::getJointTorques(JointState & jointTorques)
{
	jointTorques = this->jointTorques;
}

rbd::Vector6D WholeBodyOptimization::getWrenchError()
{
	return A*x.segment(0,dog::jointsCount+6 + contact_forces)-desWrench;
}

void WholeBodyOptimization::getConstraintViolations(WholeBodyOptimization::ConstrViolation & constraints_violation)
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

	if (torqueConstrFlag)
		constraints_violation.torque = this->constr_violation.torque;
	else
		constraints_violation.torque = 0.0;

	if (jointConstrFlag)
		constraints_violation.joint = this->constr_violation.joint;
	else
		constraints_violation.joint = 0.0;

	if (cpConstrFlag)
		constraints_violation.cp = this->constr_violation.cp;
	else
		constraints_violation.cp.setZero();
}

Eigen::Vector3d WholeBodyOptimization::getCP(){
	return this->capturePoint;
}

void WholeBodyOptimization::getSlacks(Eigen::VectorXd & slacks_out)
{
    //always resize to the maximum size
    slacks_out.resize(1 + planning::contactConstrCount*dog::_LEGS_COUNT);
    slacks_out.segment(0,number_of_slacks) = slacks;
}
////////////////////
//traits for setters
/////////////////////

void WholeBodyOptimization::setMinimizationMethod(MinMethod min_goal)
{
	this->min_goal = min_goal;
}

void WholeBodyOptimization::setJointLimits(dog::JointState & q_max, dog::JointState & q_min)
{
	this->q_max = q_max;
	this->q_min = q_min;
}

void WholeBodyOptimization::setWrenchWeight(rbd::Vector6D & W_wrench)
{
	this-> W_wrench = W_wrench;
}

void WholeBodyOptimization::setContactForceWeight(Vector3d & W_forces)
{
	this-> W_forces = W_forces;
}

void WholeBodyOptimization::setTorqueWeight(Eigen::Vector3d & W_torques_leg)
{
	this-> W_torques = W_torques_leg; //set the same fir all legs
}

void WholeBodyOptimization::setTorqueWeight(const dog::LegID & leg, Eigen::Vector3d & W_torques_leg)
{
    this-> W_torques[leg] = W_torques_leg;
}

void WholeBodyOptimization::setFrictionConstraint(bool frictionConstrFlag)
{
	this->frictionConstrFlag = frictionConstrFlag;
}

void WholeBodyOptimization::setTorqueConstraint(bool torqueConstrFlag)
{
	this->torqueConstrFlag = torqueConstrFlag;
}

void WholeBodyOptimization::setJointConstraint(bool jointConstrFlag)
{
	this->jointConstrFlag = jointConstrFlag;
}

void WholeBodyOptimization::setSwingGains(double & Kp, double & Kd){

	this->Kp_swing = Matrix3d::Identity()*Kp;
	this->Kd_swing = Matrix3d::Identity()*Kd;
}

void WholeBodyOptimization::setSwingGains(Vector3d & Kp, Vector3d & Kd){

	this->Kp_swing = Kp.asDiagonal();
	this->Kd_swing = Kd.asDiagonal();
}

void WholeBodyOptimization::setCPConstraints(bool cpConstrFlag)
{
	this->cpConstrFlag = cpConstrFlag;
}
void WholeBodyOptimization::setServoRate(int servo_rate){
	dt = 1/(double)servo_rate;
}

void WholeBodyOptimization::setCPMargin(double cpmargin){
	this->cpmargin = cpmargin;
}

void WholeBodyOptimization::setSwingQdes(dog::JointState & swing_q_des){
	this->swing_q_des = swing_q_des;
}

void WholeBodyOptimization::setSwingQdes(dog::JointState & swing_q_des, dog::JointState & swing_qd_des){
	this->computeQd_des = false;
	this->swing_q_des = swing_q_des;
	this->swing_qd_des = swing_qd_des;
}

void WholeBodyOptimization::setSwingQdesLeg(dog::LegID leg, Vector3d  & swing_q_des_leg){
    setLegJointState(leg, swing_q_des_leg, this->swing_q_des);
}
//close namespace
void WholeBodyOptimization::setJointSpaceSwing(swing_type swing_task){
	this->swing_task = swing_task;
}

bool WholeBodyOptimization::detectStanceChange()
{
    bool changeInStanceCondition = false;
    //detect if there is any change in the stance condition (lift-off or touchdown)
    for(int leg = LF; leg <= RH; leg++)
    {
           //if there is
           if (stance_legs[leg] != stance_legs_old[leg])
                {
                changeInStanceCondition = true;
                prt("stance change")
           }
    }
    return changeInStanceCondition;
}

void WholeBodyOptimization::resetStanceSpringConstraints()
{

    stance_idx.resize(0);
    sampledRelativeFeetVectors.resize(0);

    //create a map with the idx of the legs in stance
    for(int leg = LF; leg <= RH; leg++)
    {
        //if there is
        if (stance_legs[leg])
            stance_idx.push_back(leg);

    }
    //loop over the stance idxs to sample the relative distances
    for(int i = 0; i <stance_idx.size(); i++)
    {
        sampledRelativeFeetVectors.push_back(R.transpose()*footPos[stance_idx[i]] - R.transpose()*footPos[stance_idx[(i+1) % stance_idx.size()]]);

    }
    std::cout<<"Sampling stance vectors"<<std::endl;
    for (auto i = sampledRelativeFeetVectors.begin(); i != sampledRelativeFeetVectors.end(); ++i)
        std::cout << *i << ' '<<std::endl<<std::endl;


}

void WholeBodyOptimization::useStanceSpringConstraints(bool flag)
{
    this->stanceSpringConstraintsFlag = flag;
}


Eigen::Vector3d  WholeBodyOptimization::computeStanceAcceleration(dog::LegID leg)
{
    double Kp = 1000;
    double Kd = 0;
    Vector3d accelW;
    accelW.setZero();


    for(int i = 0; i <stance_idx.size(); i++)
    {
        if (stance_idx[i] ==leg)
        {
            //std::cout<<"LEG "<<legmap[stance_idx[i]]<<std::endl;
            //create a spring along the direction of the
            Vector3d  errorP = sampledRelativeFeetVectors[i] - ( R.transpose()*footPos[stance_idx[i]] - R.transpose()*footPos[stance_idx[(i+1) % stance_idx.size()]]);
            Vector3d errorV =  R.transpose()*footVel[stance_idx[i]] - R.transpose()*footVel[ stance_idx[(i+1) % stance_idx.size()] ];

            //std::cout<<"errorP "<<errorP<<std::endl;

            accelW = (Kp*errorP - Kd*errorV);
        }
    }
    return accelW;
}

void WholeBodyOptimization::useSlacks(bool flag)
{
    this->use_slacks = flag;
}
//close namespace

}
}



