#ifndef IIT_HYQ_COMMONS_JOINT_SWEEPER
#define IIT_HYQ_COMMONS_JOINT_SWEEPER

#include <iit/commons/dog/declarations.h>


namespace iit {
namespace HyQ {

struct JointLim {
    double min;
    double max;
};

struct JointLim jointLimits[] = {
    {-1.5700,  0.5236}, // LF_HAA
    {-0.8727,  1.2217}, // LF_HFE
    {-2.4435, -0.3491}, // LF_KFE

    {-1.5700,  0.5236}, // RF_HAA
    {-0.8727,  1.2217}, // RF_HFE
    {-2.4435, -0.3491}, // RF_KFE

    {-1.5700,  0.5236}, // LH_HAA
    {-1.2217,  0.8727}, // LH_HFE
    { 0.3491,  2.4435}, // LH_KFE

    {-1.5700,  0.5236}, // RH_HAA
    {-1.2217,  0.8727}, // RH_HFE
    { 0.3491,  2.4435}, // RH_KFE
};

class JointStateSweeper
{
public:
    JointStateSweeper(double pstep);
    const HyQ::JointState& next();
    const HyQ::JointState& current() const {return current_state;}
private:
    double step;
    dog::JointState current_state;
};

JointStateSweeper::JointStateSweeper(double pstep) : step(pstep)
{
    for(int i=0; i<current_state.rows(); i++) {
        current_state(i) = jointLimits[i].min;
    }
}

const HyQ::JointState& JointStateSweeper::next()
{
    for(int i=0; i<current_state.rows(); i++) {
        current_state(i) += step;
        if(current_state(i) > jointLimits[i].max) {
            current_state(i) = jointLimits[i].max;
        }
    }
    return current_state;
}

}
}

#endif
