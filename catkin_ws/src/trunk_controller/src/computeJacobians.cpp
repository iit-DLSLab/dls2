/*
 * computeJacobians.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: mfocchi
 */

#include "computeJacobians.h"
#include <iit/commons/geometry/algebra.h>
#include <iit/commons/geometry/rotations.h>
#include <doglib/rbd/rbd.h>
#include <doglib/rbd/utils.h>
#include <iit/commons/planning/planning.h>


namespace dls {
namespace dog {

//the output of Jcb*[w ; xd_base] is the stance feet velocity
//if R = eye(3) then output vectors are expressed in the same frame
//otherwise output vectors are in a frame which is rotated wrt to the input vector's
//one with a matrix R' (e.g. if R it means that the feet velocities are in the world frame while the input is in the base frame)
//if you use base_wrench = Jcb' Foot_forces
// in this case, if R!=eye(3) the output wrench is expressed in a frame which
//is oriented wrt the input one according to R
void computeJcb(Eigen::Matrix<double, Eigen::Dynamic, 6> & Jcb,
							  const Eigen::Matrix3d R,
							  const dog::LegDataMap<bool> & stance_legs,
							  const dog::LegDataMap<Eigen::Vector3d> & feet){

// create the Jc matrix
  int cleg_count = compute_stance_legs(stance_legs);
  int num_const = cleg_count*dog::jointsLegCount;

  Jcb.resize(num_const, 6);
  Jcb.setZero();

  // create the Jc matrix
  cleg_count = 0;
  for (int i = 0; i<dog::_LEGS_COUNT; i++){
	  if (stance_legs[dog::LegID(i)]) // if leg is constrained then leg is in stance, ie no need to check all eff_dof
	  {
		  //base contribution to
	  	  Jcb.block(cleg_count*dog::jointsLegCount , dog::baseJoints + rbd::AX, 3,3) = -R.transpose()*iit::commons::skew_sim(feet[dog::LegID(i)]); //need to map from base to world frame cause feet is in base frame
	  	  Jcb.block(cleg_count*dog::jointsLegCount , dog::baseJoints + rbd::LX, 3,3) = R.transpose()*Eigen::Matrix3d::Identity();//base linear velocity is assumed in the base frame
	  	  cleg_count++;
  	  }
  }
}
//the result is finds the feet velocities in a frame rotated R' wrt to base (e.g. world)
void computeJcq(Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount> & Jcq,
                              const Eigen::Matrix3d R,
                              const dog::LegDataMap<bool> & stance_legs,
                              const LegDataMap<FootJac> & jacs){

// create the Jc matrix
  int cleg_count = compute_stance_legs(stance_legs);
  int num_const = cleg_count * iit::planning::contactConstrCount;

  Jcq.resize(num_const, dog::jointsCount);
  Jcq.setZero();

  // create the Jcq matrix
  cleg_count = 0;
  for (int i = 0; i < dog::_LEGS_COUNT; i++){
	  if (stance_legs[dog::LegID(i)]) // if leg is constrained then leg is in stance, ie no need to check all eff_dof
	  {
		  //joint contribution to feet vleocity
          Jcq.block(cleg_count * iit::planning::contactConstrCount , LegID(i) * dog::jointsLegCount, 3, 3) =  R.transpose() * (jacs[LegID(i)]);
		  cleg_count++;
  	  }
  }

}



void computeStanceJacobian(Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount+6> & Jc,
                              const Eigen::Matrix3d R,
                              const dog::LegDataMap<bool> & stance_legs,
                              const dog::LegDataMap<Eigen::Vector3d> & feet,
                              const dog::LegDataMap<dog::FootJac> &jacs){

Eigen::Matrix<double, Eigen::Dynamic, 6>  Jcb;
Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount>  Jcq;

// create the Jc matrix
  int cleg_count = compute_stance_legs(stance_legs);
  int num_const = cleg_count*dog::jointsLegCount;

  Jc.resize(num_const, dog::jointsCount+6);
  Jc.setZero();

  //base contribution to feet velocity
  computeJcb(Jcb, R, stance_legs, feet);
  Jc.block(0 , dog::baseJoints, num_const, 6) = Jcb;

  // create the Jc matrix
  computeJcq(Jcq, R, stance_legs, jacs);
  Jc.block(0 , dog::activeJoints, num_const, dog::jointsCount) = Jcq;

}

void computeStanceJacobianVirtual(Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount+6> & Jc,
                              const Eigen::Matrix3d R,
                              const dog::LegDataMap<bool> & stance_legs,
                              const dog::LegDataMap<Eigen::Vector3d> & feet,
                              const dog::LegDataMap<dog::FootJac> &jacs){

Eigen::Matrix<double, Eigen::Dynamic, 6>  Jcb;
Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount>  Jcq;

// create the Jc matrix
  int cleg_count = compute_stance_legs(stance_legs);
  int num_const = cleg_count*dog::jointsLegCount;

  Jc.resize(num_const, dog::jointsCount+6);
  Jc.setZero();

  //base contribution to feet velocity
  computeJcb(Jcb, R, stance_legs, feet);

  //trasform euler rate in omega rate
  Eigen::Matrix<double, 6, 6> T;

  T.block(rbd::AX, rbd::AX, 3,3) = iit::commons::rpyToEar(iit::commons::rotTorpy(R));
  T.block(rbd::LX, rbd::LX, 3,3).setIdentity();

  Jc.block(0 , dog::baseJoints, num_const, 6) = Jcb*T;
  // create the Jc matrix
  computeJcq(Jcq, R, stance_legs, jacs);
  Jc.block(0 , dog::activeJoints, num_const, dog::jointsCount) = Jcq;

}

void computeStanceJacobian(Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount+6> & Jc,
							  Eigen::Matrix<double, dog::jointsCount+6, dog::jointsCount+6> & Nc,
							  const Eigen::Matrix3d R,
							  const dog::LegDataMap<bool> & stance_legs,
							  const dog::LegDataMap<Eigen::Vector3d> & feet,
                              const dog::LegDataMap<dog::FootJac>& jacs){

Eigen::Matrix<double, dog::jointsCount+6, Eigen::Dynamic> Jc_pinv;

computeStanceJacobian(Jc, R, stance_legs, feet, jacs);
Jc_pinv.resize(dog::jointsCount+6, Jc.rows());
Jc_pinv.setZero();
iit::commons::psdInv(Jc, Jc_pinv, 1E-04);	// compute pseudoinverse of Jc and null space

//compute null space
Nc.setIdentity();
Nc -= Jc_pinv*Jc;
}




//compute Jcdqd for the constraint feet in the word frame
//for the base frame set R to eye(3)
void computeJcdQd(Eigen::Matrix<double, Eigen::Dynamic, 1> & JcdQd,
        const dog::LegDataMap<bool> & stance_legs,
        const rbd::VelocityVector &  baseVel,
        const rbd::VelocityVector &  gB,
        const Eigen::Matrix3d & R,
        const JointState & q,
        const JointState & qd,
        dog::InverseDynamicsBase &id,
        dog::MotionTransformsBase& mt,
        dog::ForwardKinematicsBase& fwd_kin)
{

	JointState qdd = JointState::Zero();

	int cleg_count = compute_stance_legs(stance_legs);
    int num_const = cleg_count * iit::planning::contactConstrCount;

	JcdQd.resize(num_const);
	JcdQd.setZero();
	// create the vector
	cleg_count = 0;

    rbd::VelocityVector vfoot;
    rbd::VelocityVector afoot;



	if (stance_legs[dog::LF]) // if leg is constrained then leg is in stance, ie no need to check all eff_dof
	{
		//get the foot twist while update the accessory motion transforms you are going to use
        vfoot = mt.getTransform(q, OriginFrame::LF_LOWERLEG, DestFrame::LF_FOOT) * id.getLinkVelocity(q, qd, qdd, gB, baseVel, LF_LOWERLEG);
		//get the linear part of the spatial acceleration at the foot
        afoot =  mt.getTransform(q, OriginFrame::LF_LOWERLEG, DestFrame::LF_FOOT) * id.getLinkAcceleration(q, qd, qdd, gB, baseVel, LF_LOWERLEG);

		//remove the coriolis term only in the linear part to get the euclidean acceleration
		afoot.segment(rbd::LX,3) = rbd::linearPart(afoot) + rbd::angularPart(vfoot).cross(rbd::linearPart(vfoot));
		//rotate to the world frame (the same is defined Jc) (first map to base frame and update the transform then rotate to world frame)
        JcdQd.segment(cleg_count*dog::jointsLegCount,3) = R.transpose() * fwd_kin.getFootOrientation(q,LF)*rbd::linearPart(afoot);
		cleg_count++;
	}
	if (stance_legs[dog::RF]) // if leg is constrained then leg is in stance, ie no need to check all eff_dof
	{
		//get the foot twist while update the accessory motion transforms you are going to use
        vfoot = mt.getTransform(q, OriginFrame::RF_LOWERLEG, DestFrame::RF_FOOT) * id.getLinkVelocity(q, qd, qdd, gB, baseVel, RF_LOWERLEG);
		//get the linear part of the spatial acceleration at the foot
        afoot =  mt.getTransform(q, OriginFrame::RF_LOWERLEG, DestFrame::RF_FOOT) * id.getLinkAcceleration(q, qd, qdd, gB, baseVel, RF_LOWERLEG);

		//remove the coriolis term only in the linear part to get the euclidean acceleration
		afoot.segment(rbd::LX,3) = rbd::linearPart(afoot) + rbd::angularPart(vfoot).cross(rbd::linearPart(vfoot));
		//rotate to the world frame (the same is defined Jc) (first map to base frame and update the transform then rotate to world frame)
        JcdQd.segment(cleg_count*dog::jointsLegCount,3) = R.transpose() * fwd_kin.getFootOrientation(q,RF)*rbd::linearPart(afoot);
		cleg_count++;
	}
	if (stance_legs[dog::LH]) // if leg is constrained then leg is in stance, ie no need to check all eff_dof
	{
		//get the foot twist while update the accessory motion transforms you are going to use
        vfoot = mt.getTransform(q, OriginFrame::LH_LOWERLEG, DestFrame::LH_FOOT) * id.getLinkVelocity(q, qd, qdd, gB, baseVel, LH_LOWERLEG);
        //get the linear part of the spatial acceleration at the foot
		//get the linear part of the spatial acceleration at the foot
        afoot = mt.getTransform(q, OriginFrame::LH_LOWERLEG, DestFrame::LH_FOOT) * id.getLinkAcceleration(q, qd, qdd, gB, baseVel, LH_LOWERLEG);

		//remove the coriolis term only in the linear part to get the euclidean acceleration
		afoot.segment(rbd::LX,3) = rbd::linearPart(afoot) + rbd::angularPart(vfoot).cross(rbd::linearPart(vfoot));
		//rotate to the world frame (the same is defined Jc) (first map to base frame and update the transform then rotate to world frame)
        JcdQd.segment(cleg_count*dog::jointsLegCount,3) = R.transpose() * fwd_kin.getFootOrientation(q,LH)*rbd::linearPart(afoot);
		cleg_count++;
	}
	if (stance_legs[dog::RH]) // if leg is constrained then leg is in stance, ie no need to check all eff_dof
	{
		//get the foot twist while update the accessory motion transforms you are going to use
        vfoot = mt.getTransform(q, OriginFrame::RH_LOWERLEG, DestFrame::RH_FOOT) * id.getLinkVelocity(q, qd, qdd, gB, baseVel, RH_LOWERLEG);
		//get the linear part of the spatial acceleration at the foot
        afoot =  mt.getTransform(q, OriginFrame::RH_LOWERLEG, DestFrame::RH_FOOT) * id.getLinkAcceleration(q, qd, qdd, gB, baseVel, RH_LOWERLEG);

		//remove the coriolis term only in the linear part to get the euclidean acceleration
		afoot.segment(rbd::LX,3) = rbd::linearPart(afoot) + rbd::angularPart(vfoot).cross(rbd::linearPart(vfoot));
		//rotate to the world frame (the same is defined Jc) (first map to base frame and update the transform then rotate to world frame)
        JcdQd.segment(cleg_count*dog::jointsLegCount,3) = R.transpose() * fwd_kin.getFootOrientation(q,RH)*rbd::linearPart(afoot);
		cleg_count++;
	}

}


//the output of this function is not a twist but 2 euclidean velocities mapped in the base frames
rbd::Vector6D revoluteGJac(const rbd::Vector3d & p, const rbd::Vector3d & origin, const rbd::Vector3d & axis)
{
    rbd::Vector6D column;
	column.segment(rbd::AX,3) = axis;
	column.segment(rbd::LX,3) = rbd::Utils::buildCrossProductMatrix(axis)*(p-origin);
	return column;
}



//compute the jacobian of the CoM in base frame
Eigen::Matrix<double,  6, dog::jointsCount> getWholeBodyCOMJacobian(
		const JointState& q,
        const dog::InertiaPropertiesBase& in,
        dog::HomogeneousTransformsBase& ht)
{

        Eigen::Matrix<double,  6, dog::jointsCount> Jcog;
        Eigen::Vector3d joint_origin, joint_axis , linkCOMposition;
        Eigen::VectorXd link_masses;
        Jcog.setZero();
        double total_mass = in.getTotalMass();



        //LF_HAA
        link_masses.resize(3);
        link_masses << in.getHipAssemblyMass(LF), in.getUpperLegMass(LF), in.getLowerLegMass(LF);

        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LF_HAA, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::LF_HAA, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = (rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LF_HIPASSEMBLY_COM, DestFrame::TRUNK))*link_masses(0) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LF_UPPERLEG_COM, DestFrame::TRUNK))*link_masses(1) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LF_LOWERLEG_COM, DestFrame::TRUNK))*link_masses(2))/link_masses.sum();
        Jcog.col(dog::LF_HAA) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //LF_HFE
        link_masses.resize(2);
        link_masses << in.getUpperLegMass(LF), in.getLowerLegMass(LF);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LF_HFE, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::LF_HFE, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = (rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LF_UPPERLEG_COM, DestFrame::TRUNK))*link_masses(0) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LF_LOWERLEG_COM, DestFrame::TRUNK))*link_masses(1))/link_masses.sum();
        Jcog.col(dog::LF_HFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //LF_KFE
        link_masses.resize(1);
        link_masses << in.getLowerLegMass(LF);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LF_KFE, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::LF_KFE, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LF_LOWERLEG_COM, DestFrame::TRUNK));
        Jcog.col(dog::LF_KFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //RF_HAA
        link_masses.resize(3);
        link_masses << in.getHipAssemblyMass(RF), in.getUpperLegMass(RF), in.getLowerLegMass(RF);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RF_HAA, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::RF_HAA, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = (rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RF_HIPASSEMBLY_COM, DestFrame::TRUNK))*link_masses(0) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RF_UPPERLEG_COM, DestFrame::TRUNK))*link_masses(1) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RF_LOWERLEG_COM, DestFrame::TRUNK))*link_masses(2))/link_masses.sum();
        Jcog.col(dog::RF_HAA) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //RF_HFE
        link_masses.resize(2);
        link_masses << in.getUpperLegMass(RF), in.getLowerLegMass(RF);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RF_HFE, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::RF_HFE, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = (rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RF_UPPERLEG_COM, DestFrame::TRUNK))*link_masses(0) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RF_LOWERLEG_COM, DestFrame::TRUNK))*link_masses(1))/link_masses.sum();
        Jcog.col(dog::RF_HFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //RF_KFE
        link_masses.resize(1);
        link_masses << in.getLowerLegMass(RF);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RF_KFE, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::RF_KFE, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RF_LOWERLEG_COM, DestFrame::TRUNK));
        Jcog.col(dog::RF_KFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;



        //LH_HAA
        link_masses.resize(3);
        link_masses << in.getHipAssemblyMass(LH), in.getUpperLegMass(LH), in.getLowerLegMass(LH);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LH_HAA, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::LH_HAA, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = (rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LH_HIPASSEMBLY_COM, DestFrame::TRUNK))*link_masses(0) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LH_UPPERLEG_COM, DestFrame::TRUNK))*link_masses(1) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LH_LOWERLEG_COM, DestFrame::TRUNK))*link_masses(2))/link_masses.sum();
        Jcog.col(dog::LH_HAA) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //LH_HFE
        link_masses.resize(2);
        link_masses << in.getUpperLegMass(LH), in.getLowerLegMass(LH);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LH_HFE, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::LH_HFE, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = (rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LH_UPPERLEG_COM, DestFrame::TRUNK))*link_masses(0) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LH_LOWERLEG_COM, DestFrame::TRUNK))*link_masses(1))/link_masses.sum();
        Jcog.col(dog::LH_HFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //LH_KFE
        link_masses.resize(1);
        link_masses << in.getLowerLegMass(LH);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LH_KFE, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::LH_KFE, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::LH_LOWERLEG_COM, DestFrame::TRUNK));
        Jcog.col(dog::LH_KFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //RH_HAA
        link_masses.resize(3);
        link_masses << in.getHipAssemblyMass(RH), in.getUpperLegMass(RH), in.getLowerLegMass(RH);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RH_HAA, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::RH_HAA, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = (rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RH_HIPASSEMBLY_COM, DestFrame::TRUNK))*link_masses(0) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RH_UPPERLEG_COM, DestFrame::TRUNK))*link_masses(1) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RH_LOWERLEG_COM, DestFrame::TRUNK))*link_masses(2))/link_masses.sum();
        Jcog.col(dog::RH_HAA) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //RH_HFE
        link_masses.resize(2);
        link_masses << in.getUpperLegMass(RH), in.getLowerLegMass(RH);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RH_HFE, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::RH_HFE, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = (rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RH_UPPERLEG_COM, DestFrame::TRUNK))*link_masses(0) +
                          rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RH_LOWERLEG_COM, DestFrame::TRUNK))*link_masses(1))/link_masses.sum();
        Jcog.col(dog::RH_HFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        //RH_KFE
        link_masses.resize(1);
        link_masses <<  in.getLowerLegMass(RH);
        joint_origin = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RH_KFE, DestFrame::TRUNK)); //position of the joint origin in base frame
        joint_axis = rbd::Utils::zAxis(ht.getTransform(q, OriginFrame::RH_KFE, DestFrame::TRUNK))*link_masses.sum(); //axis of the joint in base frame
        linkCOMposition = rbd::Utils::positionVector(ht.getTransform(q, OriginFrame::RH_LOWERLEG_COM, DestFrame::TRUNK));
        Jcog.col(dog::RH_KFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

        #ifdef USECENTAUR

            //R_SAA
            link_masses.resize(6);
            link_masses << in.getMass_R_Shoulder_AA(), in.getMass_R_Shoulder_FE(), in.getMass_R_Humerus_R(), in.getMass_R_Elbow_FE(), in.getMass_R_Wrist_R(), in.getMass_R_Wrist_FE();
            joint_origin = rbd::Utils::positionVector(ht.fr_trunk_X_fr_R_SAA); //position of the joint origin in base frame
            joint_axis = rbd::Utils::zAxis(ht.fr_trunk_X_fr_R_SAA)*link_masses.sum(); //axis of the joint in base frame
            linkCOMposition = (rbd::Utils::positionVector(ht.fr_trunk_X_R_Shoulder_AACOM)*link_masses(0) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Shoulder_FECOM)*link_masses(1) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Humerus_RCOM)*link_masses(2) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Elbow_FECOM)*link_masses(3) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_RCOM)*link_masses(4) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_FECOM)*link_masses(5) )/link_masses.sum();
            Jcog.col(dog::R_SAA) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

            //R_SFE
            link_masses.resize(5);
            link_masses << in.getMass_R_Shoulder_FE(), in.getMass_R_Humerus_R(), in.getMass_R_Elbow_FE(), in.getMass_R_Wrist_R(), in.getMass_R_Wrist_FE();
            joint_origin = rbd::Utils::positionVector(ht.fr_trunk_X_fr_R_SFE); //position of the joint origin in base frame
            joint_axis = rbd::Utils::zAxis(ht.fr_trunk_X_fr_R_SFE)*link_masses.sum(); //axis of the joint in base frame
            linkCOMposition = (rbd::Utils::positionVector(ht.fr_trunk_X_R_Shoulder_FECOM)*link_masses(1-1) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Humerus_RCOM)*link_masses(2-1) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Elbow_FECOM)*link_masses(3-1) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_RCOM)*link_masses(4-1) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_FECOM)*link_masses(5-1) )/link_masses.sum();
            Jcog.col(dog::R_SFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

            //R_HR
            link_masses.resize(4);
            link_masses << in.getMass_R_Humerus_R(), in.getMass_R_Elbow_FE(), in.getMass_R_Wrist_R(), in.getMass_R_Wrist_FE();
            joint_origin = rbd::Utils::positionVector(ht.fr_trunk_X_fr_R_HR); //position of the joint origin in base frame
            joint_axis = rbd::Utils::zAxis(ht.fr_trunk_X_fr_R_HR)*link_masses.sum(); //axis of the joint in base frame
            linkCOMposition = (rbd::Utils::positionVector(ht.fr_trunk_X_R_Humerus_RCOM)*link_masses(2-2) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Elbow_FECOM)*link_masses(3-2) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_RCOM)*link_masses(4-2) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_FECOM)*link_masses(5-2) )/link_masses.sum();
            Jcog.col(dog::R_HR) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

            //R_EFE
            link_masses.resize(3);
            link_masses << in.getMass_R_Elbow_FE(), in.getMass_R_Wrist_R(), in.getMass_R_Wrist_FE();
            joint_origin = rbd::Utils::positionVector(ht.fr_trunk_X_fr_R_EFE); //position of the joint origin in base frame
            joint_axis = rbd::Utils::zAxis(ht.fr_trunk_X_fr_R_EFE)*link_masses.sum(); //axis of the joint in base frame
            linkCOMposition = (rbd::Utils::positionVector(ht.fr_trunk_X_R_Elbow_FECOM)*link_masses(3-3) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_RCOM)*link_masses(4-3) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_FECOM)*link_masses(5-3) )/link_masses.sum();
            Jcog.col(dog::R_EFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

            //R_WR
            link_masses.resize(2);
            link_masses << in.getMass_R_Wrist_R(), in.getMass_R_Wrist_FE();
            joint_origin = rbd::Utils::positionVector(ht.fr_trunk_X_fr_R_WR); //position of the joint origin in base frame
            joint_axis = rbd::Utils::zAxis(ht.fr_trunk_X_fr_R_WR)*link_masses.sum(); //axis of the joint in base frame
            linkCOMposition = (rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_RCOM)*link_masses(4-4) +
                    rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_FECOM)*link_masses(5-4) )/link_masses.sum();
            Jcog.col(dog::R_WR) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;

            //R_WFE
            link_masses.resize(1);
            link_masses << in.getMass_R_Wrist_FE();
            joint_origin = rbd::Utils::positionVector(ht.fr_trunk_X_fr_R_WFE); //position of the joint origin in base frame
            joint_axis = rbd::Utils::zAxis(ht.fr_trunk_X_fr_R_WFE)*link_masses.sum(); //axis of the joint in base frame
            linkCOMposition = (rbd::Utils::positionVector(ht.fr_trunk_X_R_Wrist_FECOM)*link_masses(5-5) )/link_masses.sum();
            Jcog.col(dog::R_WFE) = revoluteGJac(linkCOMposition, joint_origin, joint_axis)/total_mass;


        #endif


        return Jcog;
    }

//compute the jacobian of the CoM in in a frame rotated as rotationMx about baseframe (e.g. world-> rotationMx = Rt)
Eigen::Matrix<double,  6, dog::jointsCount + 6> getWholeBodyCOMJacobianFB(
        const Eigen::Matrix3d & rotationMx,
        const dog::JointState& q,
        dog::InertiaPropertiesBase& in,
        dog::HomogeneousTransformsBase &ht)
{
	Eigen::Matrix<double, 6, dog::jointsCount + 6> Jcog;
	Jcog.setZero();

    Jcog.block(0, dog::activeJoints, 6, dog::jointsCount) = dog::motionVectorTransform(Eigen::Vector3d(0,0,0), rotationMx) * getWholeBodyCOMJacobian(q, in, ht);
    Jcog.block<6, 6>(0, dog::baseJoints) = dog::motionVectorTransform(in.getWholeBodyCOM(q), rotationMx);
	return Jcog;
}



//compute spatial velocity of the CoM (in base frame) (only joint influence)
rbd::Vector6D getWholeBodyCOMVel(const JointState & q,
                                 const JointState & qd,
                                 const dog::InertiaPropertiesBase& inertiaProps,
                                 HomogeneousTransformsBase &ht)
{
    rbd::Vector6D CoMvel;


    // The actual calculus
    CoMvel = dog::getWholeBodyCOMJacobian(q, inertiaProps, ht)*qd;
	return CoMvel;
}



//compute spatial velocity of the CoM (base and joint influence)
//the twist should be expressed in base frame and the velocity is rotated according to matrix R
//compute spatial velocity of the CoM (base and joint influence) with update
rbd::Vector6D getWholeBodyCOMVelFB(const rbd::VelocityVector &  baseVel,
                                   const Eigen::Matrix3d & rotationMx,
                                   const JointState & q,
                                   const JointState & qd,
                                   dog::InertiaPropertiesBase& in,
                                   dog::HomogeneousTransformsBase &ht)
{

    rbd::Vector6D CoMVel;
	//compute joint influence
    CoMVel = dog::motionVectorTransform(Eigen::Vector3d(0,0,0), rotationMx)*dog::getWholeBodyCOMJacobian(q, in, ht)*qd;
	//add base motion shifted to the COM
	Eigen::Vector3d Com;
    CoMVel += dog::motionVectorTransform(in.getWholeBodyCOM(q), rotationMx)*baseVel;

	return CoMVel;
}



void computeBaseTwist(const dog::LegDataMap<Eigen::Vector3d> & feet,
                      const dog::LegDataMap<Eigen::Vector3d> & feet_vel,
                      const Eigen::Vector3d omega,
                      const dog::LegDataMap<bool> & stance_legs,
                      rbd::VelocityVector & baseVel)
{
int cleg_count = compute_stance_legs(stance_legs);

//we need to map the base_twist into feet vel (in base frame)
Eigen::MatrixXd A(cleg_count*3,6); A.setZero();
Eigen::VectorXd b(cleg_count*3); b.setZero();
cleg_count = 0;
for (int i = 0; i<dog::_LEGS_COUNT; i++){
	if (stance_legs[dog::LegID(i)])
	{
		A.block(cleg_count*3, 0, 3,3) = -iit::commons::skew_sim(feet[dog::LegID(i)]);
		A.block(cleg_count*3, 3, 3,3).setIdentity();
		b.segment(cleg_count*3, 3) = -feet_vel[dog::LegID(i)];
		cleg_count++;
	}
}
switch (cleg_count)
{
case 0:{
	//flight phase
	baseVel.segment(rbd::AX, 3) = omega;
	break;} //dont compute anything!
case 2:
	{ //if underactuation augment with constraint fot the omega
  	A.conservativeResize(cleg_count*3 + 3, 	6);
	b.conservativeResize(cleg_count*3 + 3);
	A.block(cleg_count*3,rbd::AX, 3,3).setIdentity();
	A.block(cleg_count*3,rbd::LX, 3,3).setZero();
	b.segment(cleg_count*3,3) = omega;//the previous part has already been set
	//compute base twist
	baseVel = iit::commons::psdInv(A, 1E-04)*b;
	break;
	}
default:
	{
	//compute base twist //with 3/4 legs omega will be discarded
	baseVel = iit::commons::psdInv(A, 1E-04)*b;
	break;
	}
}

}

/**
 * @brief motionVectorTransform Tranforms twists from A to B (b_X_a)   \in R^6 \times 6
 * where A is the origin frame and B the destination frame.
 * @param position coordinate vector expressing OaOb in A coordinates
 * @param rotationMx rotation matrix that transforms 3D vectors from A to B coordinates
 * @return
 */
rbd::Matrix66d motionVectorTransform(const rbd::Vector3d & position,
                                     const Eigen::Matrix3d & rotationMx)
{
    rbd::Matrix66d X=rbd::Matrix66d::Zero();

	X.block<3,3>(rbd::AX, rbd::AX) = rotationMx;
	X.block<3,3>(rbd::LX, rbd::AX) = -rotationMx*rbd::Utils::buildCrossProductMatrix(position);
	X.block<3,3>(rbd::LX, rbd::LX) = rotationMx;

	return X;
}

/**
 * @brief forceVectorTransform Tranforms wrenches from A to B (b_X_a)   \in R^6 \times 6
 * where A is the origin frame and B the destination frame.
 * @param position coordinate vector expressing OaOb in A coordinates
 * @param rotationMx rotation matrix that transforms 3D vectors from A to B coordinates
 * @return
 */
rbd::Matrix66d forceVectorTransform(const rbd::Vector3d & position,
                                    const Eigen::Matrix3d & rotationMx)
{
    rbd::Matrix66d X=rbd::Matrix66d::Zero();

	X.block<3,3>(rbd::AX, rbd::AX) = rotationMx;
	X.block<3,3>(rbd::AX, rbd::LX) = -rotationMx*rbd::Utils::buildCrossProductMatrix(position);
	X.block<3,3>(rbd::LX, rbd::LX) = rotationMx;

	return X;
}

int compute_stance_legs(const dog::LegDataMap<bool> & stance_legs)
{
	  int cleg_count = 0;
	  for (int i = 0; i<dog::_LEGS_COUNT; i++){
		  if (stance_legs[dog::LegID(i)])
			  cleg_count++;
	  }
	  return cleg_count;
}
/**
 * @brief getCoMFromBase
 * @param q
 * @param base_orient
 * @param base_pos  base is supposed to be expressed in the world frame
 * @param in
 * @return
 */
Eigen::Vector3d getCoMFromBase(const JointState & q,
                               const Eigen::Vector3d & base_orient,
                               const Eigen::Vector3d & base_pos,
                               dog::InertiaPropertiesBase& in)
{
	Eigen::Matrix3d R = iit::commons::rpyToRot(base_orient);
    Eigen::Vector3d offCoM = in.getWholeBodyCOM(q);
	return base_pos + R.transpose()*offCoM; //CoM is in the world frame off CoM is in base frame
}
/**
 * @brief getBaseFromCoM
 * @param q
 * @param base_orient
 * @param CoM CoM position in world coordinates
 * @param in
 * @return
 */
Eigen::Vector3d getBaseFromCoM(const JointState & q,
                               const Eigen::Vector3d & base_orient,
                               const Eigen::Vector3d & CoM,
                               dog::InertiaPropertiesBase &in)
{
        Eigen::Matrix3d b_R_w = iit::commons::rpyToRot(base_orient);
    Eigen::Vector3d offCoM = in.getWholeBodyCOM(q);
        return CoM - b_R_w.transpose()*offCoM; //CoM is in the world frame off CoM is in base frame
}

} // namespace dog
} // namespace iit
