#ifndef IIT_ROBOTS_HYQ_INTERNALS_H_
#define IIT_ROBOTS_HYQ_INTERNALS_H_

#include <iit/commons/dog/leg_data_map.h>

namespace iit {
namespace HyQ {

namespace internal {

struct MagicNumbers {
    MagicNumbers();

    dog::LegDataMap<double> HAAJointToMotorOffset;

};

extern const MagicNumbers misc_cfg;

}
}
}



#endif
