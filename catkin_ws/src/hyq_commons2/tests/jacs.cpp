
#include <iit/commons/dog/feet_jacobians.h>

#include <iit/commons/dog/declarations.h>
#include <iit/robots/hyq/transforms.h>
#include <iit/robots/hyq/feet_jacobians.h>
#include <iit/robots/hyq/joint_state_sweeper.h>
#include <iit/robots/hyq/default_parameters_getter.h>
#include <gtest/gtest.h>

#include <iostream>


using namespace std;

using namespace iit;

TEST(HyqJacs, hyq_jacs){

    HyQ::DefaultParamsGetter pGetter;
    HyQ::JointStateSweeper sweeper(0.005);
    HyQ::Jacobians jacs(pGetter);
    dog::JointState q;
    q.setZero();
    HyQ::FeetJacobians fj(jacs);

    for(int i=0; i<5; i++)
    {
        q = sweeper.next();
        cout << fj.getFootJacobianLF(q) << endl << endl;
        cout << fj.getFootJacobian(q, dog::LF, 0.341, 0) << endl << endl;
    }

    ASSERT_NEAR(fj.getFootJacobianLF(q)(rbd::X,dog::HAA),0           ,0.0001);
    ASSERT_NEAR(fj.getFootJacobianLF(q)(rbd::Y,dog::HAA),0.000690176 ,0.0001);
    ASSERT_NEAR(fj.getFootJacobianLF(q)(rbd::Z,dog::HAA),0.0267489   ,0.0001);
    ASSERT_NEAR(fj.getFootJacobianLF(q)(rbd::X,dog::HFE), 0.106758   ,0.0001);
    ASSERT_NEAR(fj.getFootJacobianLF(q)(rbd::Y,dog::HFE), 0.219962   ,0.0001);
    ASSERT_NEAR(fj.getFootJacobianLF(q)(rbd::Z,dog::HFE),-0.00567546 ,0.0001);
    ASSERT_NEAR(fj.getFootJacobianLF(q)(rbd::X,dog::KFE), 0.338356   ,0.0001);
    ASSERT_NEAR(fj.getFootJacobianLF(q)(rbd::Y,dog::KFE), -0.0423671 ,0.0001);
    ASSERT_NEAR(fj.getFootJacobianLF(q)(rbd::Z,dog::KFE), 0.00109316 ,0.0001);

}


int main(int argc, char** argv)
{

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
