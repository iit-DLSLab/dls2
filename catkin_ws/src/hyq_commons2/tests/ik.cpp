
#include <iit/commons/dog/forward_kinematics.h>
#include "iit/robots/hyq/forward_kinematics.h"
#include "iit/robots/hyq/inverse_kinematics.h"
#include "iit/robots/hyq/inverse_dynamics.h"
#include <iit/commons/dog/test_k.h>
#include <iit/robots/hyq/jacobians.h>

#include <iit/commons/dog/declarations.h>
#include <iit/robots/hyq/transforms.h>
#include <iit/robots/hyq/forward_kinematics.h>
#include <iit/robots/hyq/inverse_kinematics.h>
#include <iit/robots/hyq/joint_state_sweeper.h>

#include "iit/robots/hyq/default_parameters_getter.h"
#include <iit/rbd/rbd.h>

#include <iostream>
#include <gtest/gtest.h>


using namespace std;
using namespace iit::HyQ;
using namespace iit::rbd;
using namespace Eigen;
using namespace iit::dog;

using namespace iit;


TEST(InverseKinematics, PositionConsistency) {
    HyQ::DefaultParamsGetter pGetter;
    HyQ::JointStateSweeper sweeper(0.005);
    HyQ::HomogeneousTransforms ht(pGetter);
    HyQ::JointState q;
    q.setZero();
    HyQ::ForwardKinematics fk(pGetter);
    HyQ::InverseKinematics ik(pGetter);

    dog::TestK testk(fk, ik);

    typedef dog::TestK::Result Res;
    Res res;
    q = sweeper.next();
    for(int i=0; i<20; i++)
    {
        q = sweeper.next();
        testk.testFK_IK(q, res);

        for(int i = 0; i<res.q.size(); i++){
            EXPECT_NEAR(res.q(i), res.q_ik(i), 1e-14);
        }
    }
}

/**
 * @brief This test computes the velocity of a foot given a certain joint
 * velocity using InverseDynamics. Then, it computes the joint velocity from
 * InverseKinematics and compares the two joint velocities.
 */
TEST (InverseKinematics, VelocityConsistency){
    DefaultParamsGetter default_pg;

    iit::HyQ::InverseKinematics ik(default_pg);
    HomogeneousTransforms ht(default_pg);

    JointState q;
    JointState qd;
    JointState qdd;

    // TODO
    // fetch actual joint limits and set these parameters accordingly
    double qd_increment = 0.05;
    double q_increment = 0.05;
    int iterations = 10;


    q <<-0.2174 , 0.5427 ,-1.2621,-0.2997,0.6941,-1.2098, -0.2901,-0.6674,1.1602,-0.2689,-0.5008,1.2326;
    qd << -1 ,0.7, -2,0.5,-0.9,0,0,-0.43,0.5,0.2,0.1,0.17;

    JointState q_init = q;
    JointState qd_init = qd;

    qdd = JointState::Zero();

    for(int joint_i = 0; joint_i<12; joint_i++){
        for( int joint_j = 0; joint_j < 12; joint_j++){
            for(int it = -iterations; it < iterations; it++){

            q(joint_i) = q_init(joint_i) + iterations * q_increment;
            qd(joint_j) = qd_init(joint_j) + iterations * qd_increment;


            iit::rbd::Vector6D base_twist = Vector6D::Zero();
            //base_twist << 0,1,0,0,0,0;

            Vector6D g = Vector6D::Zero();
            //g.segment(iit::rbd::LX,3) << 0, 0, -iit::rbd::g;

            dyn::InertiaProperties ip(default_pg);
            MotionTransforms mt(default_pg);

            dyn::InverseDynamics id(ip, mt);

            Vector6D lower_leg_vel;

            LegDataMap<Vector3d> foot_vel;

            lower_leg_vel = id.getLinkVelocity(q, qd, qdd, g, base_twist, iit::dog::LF_LOWERLEG);
            Vector6D foot_vel_inv_dyn = mt.getTransform(q, OriginFrame::LF_LOWERLEG, DestFrame::LF_FOOT) * lower_leg_vel;
            foot_vel[LF] = linearPart(foot_vel_inv_dyn);

            Matrix<double,4,4> t = ht.getTransform(q,OriginFrame::LF_FOOT,DestFrame::TRUNK);

            foot_vel[LF] = t.block<3,3>(0,0) * foot_vel[LF];

            lower_leg_vel = id.getLinkVelocity(q,qd,qdd,g,base_twist, iit::dog::RF_LOWERLEG);
            foot_vel_inv_dyn = mt.getTransform(q, OriginFrame::RF_LOWERLEG, DestFrame::RF_FOOT) * lower_leg_vel;
            foot_vel[RF] = linearPart(foot_vel_inv_dyn);

            t = ht.getTransform(q,OriginFrame::RF_FOOT,DestFrame::TRUNK);

            foot_vel[RF] = t.block<3,3>(0,0) * foot_vel[RF];

            lower_leg_vel = id.getLinkVelocity(q,qd,qdd,g,base_twist, iit::dog::LH_LOWERLEG);
            foot_vel_inv_dyn = mt.getTransform(q, OriginFrame::LH_LOWERLEG, DestFrame::LH_FOOT) * lower_leg_vel;
            foot_vel[LH] = linearPart(foot_vel_inv_dyn);

            t = ht.getTransform(q,OriginFrame::LH_FOOT,DestFrame::TRUNK);

            foot_vel[LH] = t.block<3,3>(0,0) * foot_vel[LH];

            lower_leg_vel = id.getLinkVelocity(q,qd,qdd,g,base_twist, iit::dog::RH_LOWERLEG);
            foot_vel_inv_dyn = mt.getTransform(q, OriginFrame::RH_LOWERLEG, DestFrame::RH_FOOT) * lower_leg_vel;
            foot_vel[RH] = linearPart(foot_vel_inv_dyn);

            t = ht.getTransform(q,OriginFrame::RH_FOOT,DestFrame::TRUNK);

            foot_vel[RH] = t.block<3,3>(0,0) * foot_vel[RH];



            JointState qd_ik;

            if(ik.getJointVelocity(foot_vel, q, qd_ik)) {
                for(int i=0; i < qd.rows(); i++){
                    EXPECT_NEAR(qd(i), qd_ik(i),1e-14);
                }
            } else {
                GTEST_FAIL();
            }
        }
    }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
/*
    dog::JointState q_max, q_min;

    q_max.setConstant(1e08);
    q_min.setConstant(-1e08);

    q_min(dog::LF_HAA) = -0.1;
    ik.setKinematicLimits(q_min, q_max);

    dog::LegJointBool joitViol;
    Eigen::Vector3d qleg;
    ik.getJointPosition(Eigen::Vector3d(0.2,0.3,-0.5),dog::LF,qleg, true, joitViol);
    std::cout<<qleg<<std::endl;
    std::cout<<joitViol<<std::endl;

    dog::JointBoolMap joitViol2;
    dog::LegDataMap<Eigen::Vector3d> footPos;
    footPos[dog::LF] = Eigen::Vector3d(0.2,0.3,-0.5);
    footPos[dog::RF] = Eigen::Vector3d(0.2,-0.3,-0.5);
    footPos[dog::LH] = Eigen::Vector3d(-0.2,0.3,-0.5);
    footPos[dog::RH] = Eigen::Vector3d(-0.2,-0.3,-0.5);



    ik.getJointPosition(footPos, q, true, joitViol2);
    std::cout<<q<<std::endl;
    std::cout<<joitViol<<std::endl;



    //return 0;
*/



    return 0;
}
