/*
 * footest.cpp
 *
 *  Created on: May 21, 2013
 *      Author: mfrigerio
 */

#include <iit/commons/dog/declarations.h>
#include <iit/robots/hyq/hydraulics/hydraulics.h>
#include <iit/robots/hyq/control/joints_pid_control.h>
#include <iit/commons/dog/leg_data_map.h>
#include <iit/robots/hyq/cylinder_lever_arm.h>
#include <gtest/gtest.h>

using namespace iit::HyQ;
using namespace iit::dog;

// Initial values of the joints (HIND LEGS)
static const double q_in_hfe = -1.2217;
static const double q_in_kfe =  0.3491;

static JointIdentifiers joints[]  = {LF_HFE, RF_HFE, LH_HFE, RH_HFE,
                                     LF_KFE, RF_KFE, LH_KFE, RH_KFE};


class JointStateSweeper
{
public:
    JointStateSweeper(double pstep);
    const JointState& next();
    const JointState& current() const {return current_state;}
private:
    double step;
    JointState current_state;
};

JointStateSweeper::JointStateSweeper(double pstep) : step(pstep)
{
    current_state(LF_HAA) = -1.5708;
    current_state(LF_HFE) = -0.8727;
    current_state(LF_KFE) = -2.4435;
    current_state(RF_HAA) = -1.5708;
    current_state(RF_HFE) = -0.8727;
    current_state(RF_KFE) = -2.4435;
    current_state(LH_HAA) = -1.5708;
    current_state(LH_HFE) = -1.2217;
    current_state(LH_KFE) =  0.3491;
    current_state(RH_HAA) = -1.5708;
    current_state(RH_HFE) = -1.2217;
    current_state(RH_KFE) =  0.3491;
}

const JointState& JointStateSweeper::next()
{
    current_state = current_state + JointState::Ones() * step;
    return current_state;
}


static void printCylinderLenghts()
{
    double qk = q_in_kfe;
    double qh = q_in_hfe;

    for(int i=0; i<200; i++) {
        std::cout <<       getCylinderLength(LF_HFE, qh)
                << ", " << getCylinderLength(RF_HFE, qh)
                << ", " << getCylinderLength(LH_HFE, qh)
                << ", " << getCylinderLength(RH_HFE, qh)

                << ", " << getCylinderLength(LF_KFE, qk)
                << ", " << getCylinderLength(RF_KFE, qk)
                << ", " << getCylinderLength(LH_KFE, qk)
                << ", " << getCylinderLength(RH_KFE, qk) << std::endl;
        qk += 0.02;
        qh += 0.02;
    }
}

static void printCylinderLevers()
{
    double qk = q_in_kfe;
    double qh = q_in_hfe;
    for(int i=0; i<200; i++) {
        std::cout <<       getCylinderLeverArm(LF_HFE, getCylinderLength(LF_HFE, qh))
                << ", " << getCylinderLeverArm(RF_HFE, getCylinderLength(RF_HFE, qh))
                << ", " << getCylinderLeverArm(LH_HFE, getCylinderLength(LH_HFE, qh))
                << ", " << getCylinderLeverArm(RH_HFE, getCylinderLength(RH_HFE, qh))

                << ", " << getCylinderLeverArm(LF_KFE, getCylinderLength(LF_KFE, qk))
                << ", " << getCylinderLeverArm(RF_KFE, getCylinderLength(RF_KFE, qk))
                << ", " << getCylinderLeverArm(LH_KFE, getCylinderLength(LH_KFE, qk))
                << ", " << getCylinderLeverArm(RH_KFE, getCylinderLength(RH_KFE, qk)) << std::endl;
        qk += 0.02;
        qh += 0.02;;
    }
}

TEST(LeverArm, Consistency)
{
    JointStateSweeper sweeper(0.2);
    JointIdentifiers jid;
    JointState qv = sweeper.current();
    for(int i=0; i<150; i++)
    {
        for(int j=0; j<8; j++)
        {
            jid = joints[j];
            double q    = qv(jid);
            double cyl  = getCylinderLength(jid, q);
            double diff = q - getJointStatus(jid, cyl);

            EXPECT_NEAR(std::abs(diff), 0.0, 1e-4);
        }
        std::cout << std::endl;
        sweeper.next();
    }
}

int main(int argc, char** argv) {
    //printCylinderLenghts();
    //printCylinderLevers(); // you can redirect stdout to get a log file
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

	return 0;
}
