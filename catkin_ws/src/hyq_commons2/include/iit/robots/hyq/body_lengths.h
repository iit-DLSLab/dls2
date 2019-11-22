#ifndef _IIT_HYQ_COMMONS_BODYLENGTHS_H_
#define _IIT_HYQ_COMMONS_BODYLENGTHS_H_

#include <iit/commons/dog/kin_dyn_params.h>
#include <iit/commons/dog/body_lengths.h>

#include "constants.h"
#include "kinematics_parameters.h"

namespace iit {
namespace HyQ {


class HyQLengths : public dog::RobotLengths
{
public:
    HyQLengths(dog::KinDynParams& getter) : pGetter(getter) {}
    ~HyQLengths() {}

    double getHAA_x() { return BASE2HAAX; }
    double getHAA_y() { return BASE2HAAY; }
    double getHAA_z() { return 0; }

    double getDist_HAA_HFE() { return HAA2HFE; }
    double getDist_HFE_KFE() { return upleg_length; }
    double getFoot_x() { return pGetter.getValue_foot_x(); }
    double getFoot_y() { return 0; }
    double getFoot_z() { return 0; }

private:
    dog::KinDynParams& pGetter;
};


}
}

#endif
