#include <iit/robots/hyq/default_parameters_getter.h>
#include <iit/robots/hyq/jsim.h>
#include <iit/robots/hyq/feet_contact_forces.h>
#include <gtest/gtest.h>

using namespace iit::HyQ;
using namespace iit::HyQ::dyn;
using namespace Eigen;

TEST(FeetContactForces, ZeroTorque){
    DefaultParamsGetter default_pg;
    InertiaProperties inertia_properties(default_pg);
    MotionTransforms motion_transforms(default_pg);
    ForceTransforms force_transforms(default_pg);
    InverseDynamics inverse_dynamics(inertia_properties, motion_transforms);
    Jacobians jacobians(default_pg);
    FeetJacobians feet_jacobians(jacobians);
    JSIM jsim(inertia_properties,force_transforms);

    FeetContactForces feet_contact_forces(feet_jacobians,inverse_dynamics,jsim);

    JointState q;
    q << -0.2, 0.7,-1.4, -0.2, 0.7, -1.4, -0.2, -0.7, 1.4, -0.2, -0.7, 1.4;

    JointState qd = JointState::Constant(0);
    JointState tau = JointState::Constant(0);

    Quaterniond orient = Quaterniond::Identity();


    iit::dog::LegDataMap<Vector3d> result = feet_contact_forces.getFeetGRF(q,qd,tau,orient);




        EXPECT_NEAR(-5.9108588135936762, result[iit::dog::LegID(iit::dog::LF)](0), 0.01);
        EXPECT_NEAR( 1.3681982355321942, result[iit::dog::LegID(iit::dog::LF)](1), 0.01);
        EXPECT_NEAR(10.600348398765879, result[iit::dog::LegID(iit::dog::LF)](2), 0.01);

        EXPECT_NEAR(-5.9108588135936762, result[iit::dog::LegID(iit::dog::RF)](0), 0.01);
        EXPECT_NEAR(-1.3681982355321942, result[iit::dog::LegID(iit::dog::RF)](1), 0.01);
        EXPECT_NEAR(10.600348398765879, result[iit::dog::LegID(iit::dog::RF)](2), 0.01);


        EXPECT_NEAR(5.9108588135936762, result[iit::dog::LegID(iit::dog::LH)](0), 0.01);
        EXPECT_NEAR(1.3681982355321942, result[iit::dog::LegID(iit::dog::LH)](1), 0.01);
        EXPECT_NEAR(10.600348398765879, result[iit::dog::LegID(iit::dog::LH)](2), 0.01);


        EXPECT_NEAR(5.9108588135936762, result[iit::dog::LegID(iit::dog::RH)](0), 0.01);
        EXPECT_NEAR(-1.3681982355321942, result[iit::dog::LegID(iit::dog::RH)](1), 0.01);
        EXPECT_NEAR(10.600348398765879, result[iit::dog::LegID(iit::dog::RH)](2), 0.01);


}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
