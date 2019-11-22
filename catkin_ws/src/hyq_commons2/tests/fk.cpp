
#include <iit/commons/dog/declarations.h>
#include <iit/robots/hyq/transforms.h>
#include <iit/robots/hyq/forward_kinematics.h>

#include <iit/robots/hyq/default_parameters_getter.h>

#include <iit/commons/dog/forward_kinematics.h>
#include <iit/commons/dog/test_fk.h>

#include <iostream>

#include <gtest/gtest.h>


using namespace std;

using namespace iit;


TEST(HyqFk, hyq_fk){
    HyQ::DefaultParamsGetter pGetter;
    HyQ::HomogeneousTransforms ht(pGetter);
    dog::JointState q;
    q.setZero();
    HyQ::ForwardKinematics fk(pGetter);

    dog::TestFK testfk(fk);
    testfk.printPositions(q);

    ASSERT_EQ(fk.getFootPosLF(q)(rbd::X),0.3735);
    ASSERT_EQ(fk.getFootPosLF(q)(rbd::Y),0.207);
    ASSERT_EQ(fk.getFootPosLF(q)(rbd::Z),-0.771);

    ASSERT_EQ(fk.getFootPosRF(q)(rbd::X),0.3735);
    ASSERT_EQ(fk.getFootPosRF(q)(rbd::Y),-0.207);
    ASSERT_EQ(fk.getFootPosRF(q)(rbd::Z),-0.771);


    ASSERT_EQ(fk.getFootPosLH(q)(rbd::X),-0.3735);
    ASSERT_EQ(fk.getFootPosLH(q)(rbd::Y),0.207);
    ASSERT_EQ(fk.getFootPosLH(q)(rbd::Z),-0.771);

    ASSERT_EQ(fk.getFootPosRH(q)(rbd::X),-0.3735);
    ASSERT_EQ(fk.getFootPosRH(q)(rbd::Y),-0.207);
    ASSERT_EQ(fk.getFootPosRH(q)(rbd::Z),-0.771);


}


int main(int argc, char** argv)
{

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
