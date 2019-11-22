#include <trunk_controller.hpp>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>

#include <iit/commons/dog/declarations.h>
#include <iit/commons/dog/leg_data_map.h>
#include <iit/commons/dog/leg_bool_map.h>
#include <iit/robots/hyq/transforms.h>
#include <iit/robots/hyq/jacobians.h>
#include <iit/robots/hyq/forward_kinematics.h>
#include <iit/robots/hyq/inverse_kinematics.h>
#include <iit/robots/hyq/feet_jacobians.h>
#include <iit/robots/hyq/inverse_dynamics.h>
#include <iit/robots/hyq/jsim.h>
#include <iit/robots/hyq/inertia_properties.h>
#include <iit/locomotionutils/parameters_getter.h>
#include <iit/commons/geometry/algebra.h>
#include <iit/commons/geometry/rotations.h>

#include <gtest/gtest.h>

using namespace std;
using namespace iit;
using namespace Eigen;
using namespace commons;
using namespace HyQ;
using namespace iit::dog;
using namespace iit::planning;


class TrunkControllerTest : public ::testing::Test
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public:
    TrunkControllerTest(bool verbose = true) :
    hom_transforms(paramsGetter),
      force_transforms(paramsGetter),
      inertia_properties(paramsGetter),
      motion_transforms(paramsGetter),
      idObj(inertia_properties, motion_transforms),
      jsim(inertia_properties, force_transforms),
      jacobians(paramsGetter),
      feet_jacobians(jacobians),
      fwd_kin(paramsGetter),
      inv_kin(paramsGetter),
      stance_legs(true),
	  trunk_ctrl(),
    verbose_(verbose)
    {
    }
    ~TrunkControllerTest(){

    }

public:
iit::HyQ::TestParamsGetter paramsGetter;
HyQ::HomogeneousTransforms hom_transforms;
HyQ::ForceTransforms force_transforms;
HyQ::dyn::InertiaProperties inertia_properties;
HyQ::MotionTransforms motion_transforms;
HyQ::dyn::InverseDynamics idObj;
HyQ::dyn::JSIM jsim;

HyQ::Jacobians jacobians;
HyQ::FeetJacobians feet_jacobians;
HyQ::ForwardKinematics fwd_kin;
HyQ::InverseKinematics inv_kin;

dog::LegDataMap< dog::FootJac > JFoot;

dog::LegDataMap<Eigen::Vector3d> footPos, footVel;
dog::JointState joint_torques;
LegDataMap<Vector3d> des_feet_forces;
iit::rbd::VelocityVector  baseVel;
iit::rbd::VelocityVector  baseAcc_des;
Eigen::Matrix3d R;
dog::JointState q, qd, qdd;
dog::LegBoolMap  stance_legs;


TrunkController trunk_ctrl;

bool verbose_ = true;

};



TEST_F(TrunkControllerTest, trunkController)
{
    //1 - set the state
    rbd::Vector6D baseTwist;
    //base orientation
    R = commons::rpyToRot(Eigen::Vector3d(0.0, 0.2,0.3));
    //set joint position/velocity
    q << 0.0 ,  0.75 ,  -1.5,   -0.0   , 0.75  , -1.5  , 0.0,   -0.75  ,  1.5 ,  -0.0,   -0.75,   1.5;
    qd.setZero();
    baseTwist.segment(rbd::AX,3) = Vector3d(0.1,0.1,0.1);
    baseTwist.segment(rbd::LX,3) = Vector3d(0.1,0.1,0.1);

    for(int leg = LF; leg <=RH; leg++) {
        //compute foot position in base space via fwd kinematics
        footPos[LegID(leg)]=fwd_kin.getFootPos(q, LegID(leg));
        JFoot[LegID(leg)] = feet_jacobians.getFootJacobian(q, LegID(leg));
    }

    //2 - trunk controller initialization
    TrunkControllerParams config;
    config.attractorType = AttractorType::WRENCH;
    config.damping_only = true;
    config.is_base_controlled = false;
    config.is_height_controlled = false;

    //set posture gains
//    config.Kp_posture(rbd::LX) = 1000;
//    config.Kp_posture(rbd::LY) = 1000;
//    config.Kp_posture(rbd::LZ) = 1000;
//    config.Kp_posture(rbd::AX) = 1000;
//    config.Kp_posture(rbd::AY) = 1000;
//    config.Kp_posture(rbd::AZ) = 1000;

//    config.Kd_posture(rbd::LX) = 200;
//    config.Kd_posture(rbd::LY) = 200;
//    config.Kd_posture(rbd::LZ) = 200;
//    config.Kd_posture(rbd::AX) = 200;
//    config.Kd_posture(rbd::AY) = 200;
//    config.Kd_posture(rbd::AZ) = 200;
    config.min_goal = MinGoal::NORMALS;
    config.opt_type = OptimizationType::WHOLE_BODY_STATIC;
    config.use_friction_constraints = true;
    config.use_torque_constraints = false;
    config.use_joint_constraints = false;

//    config.verbose = true;
//    config.debug = true;
    trunk_ctrl.init(config);
    //set your user denined
    trunk_ctrl.setGravityCompensation(false); //to be sure Im using the internal one



    //3) set stance legs (stance = true)
    stance_legs[dog::LF] = true;
    stance_legs[dog::RF] = true;
    stance_legs[dog::LH] = true;
    stance_legs[dog::RH] = true;
    trunk_ctrl.setStanceLegs(stance_legs);


    //4 - set actual /desired state (TODO used only with virtual model)
//    iit::planning::Point3d  actual_base;
//    iit::planning::Point3d  actual_CoM;
//    iit::planning::Point3d  actual_orient;
//    iit::rbd::Vector6D comTwist;
//    actual_orient.x = Vector3d(0.0, 0.2,0.3);
//    actual_orient.xd = Vector3d(0.0, 0.2,0.3);
//    //compute actual com position in WF
//    actual_base.x = Vector3d(0.0, 0.2,0.3);
//    actual_base.xd = Vector3d(0.0, 0.2,0.3);
//    actual_CoM.x = iit::dog::getCoMFromBase(q, actual_orient.x,actual_base.x, inertia_properties);
//    //compute actual com velocity in WF
//    comTwist = iit::dog::getWholeBodyCOMVelFB(baseTwist, R.transpose(),  q, qd, inertia_properties, hom_transforms);
//    actual_CoM.xd = rbd::linearPart(comTwist);
//    trunk_ctrl->setActualState(actual_base, actual_orient);
//    trunk_ctrl->setTarget(des_base_pos,des_base_orient);

    //5) set friction cone limits on cone normals
    dog::LegDataMap<double> high_force_limit(10000.0);
    dog::LegDataMap<double> low_force_limit(5.0);
    //set a specific force limit on lF
    high_force_limit[LF] = 200;
    //set friction cone (0 means cone degenerates to a line)
    dog::LegDataMap<double> muEstimate(0.8);
    //set contact surface inclination
    dog::LegDataMap<Vector3d> vec_incl = Vector3d(0,0,1);
    //if you want to set specular normals for hind and front legs uncomment
    //	terr_R_w = trunk_ctrl.rpyToRot(Eigen::Vector3d(terrRoll, -terrPitch, yaw)); //this is the mapping between world and terrain frame
    //	vec_incl[LH] =  terr_R_w.transpose()*Eigen::Vector3d(0,0,1);
    //	vec_incl[RH] =  terr_R_w.transpose()*Eigen::Vector3d(0,0,1);
    trunk_ctrl.setForceLimits(high_force_limit, low_force_limit, vec_incl, muEstimate);

    //6) set your desired wrench
    rbd::ForceVector wrench = rbd::ForceVector::Zero();
    wrench(rbd::LZ) = 9.8*75;
    trunk_ctrl.setFFWDTrunkWrench(wrench);

    //7) get the torques
    trunk_ctrl.setOn(true);
    trunk_ctrl.getJointTorques(R, baseTwist, q, qd, footPos,  JFoot, joint_torques);
    std::cout << "torques:      " << joint_torques.transpose() << std::endl;

    //8) get the feet forces in WF
    trunk_ctrl.getFeetForces(des_feet_forces);
    std::cout << "feet forces:  " << des_feet_forces << std::endl;


  //TODO
//    LegDataMap<Vector3d> feet_forces_expected;
//    feet_forces_expected[LF] << 7.67765e-05, 3.69565e-12, 193.709;
//    feet_forces_expected[RF] << 7.67765e-05, 3.69565e-12, 193.709;
//    feet_forces_expected[LH] << 7.67765e-05, 2.22077e-12, 173.79;
//    feet_forces_expected[RH] << 7.67765e-05, -1.92159e-12, 173.79;

    //std::cout << "If the wrench is realizable the taskCost should be zero" << std::endl;
    //trunk_ctrl.getConstraintViolations(constraints_violation);
    //trunk_ctrl.printCosts();
    //	std::cout<<constraints_violation.friction<<std::endl;
    //	std::cout<<constraints_violation.unilateral<<std::endl;
    //	Eigen::VectorXd slacks;
    //	trunk_ctrl.getSlacks(slacks);*/

}

int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    bool verbose = false;
    std::stringstream ss;
    if (argc > 1){
        ss  << argv[1];

        if (!(ss >> std::boolalpha >> verbose) ) {
            return -1;// Parsing error.
        }
    }



    return RUN_ALL_TESTS();

}



