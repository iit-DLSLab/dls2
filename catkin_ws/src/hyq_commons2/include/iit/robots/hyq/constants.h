#ifndef IIT_HYQ_CONSTANTS_H_
#define IIT_HYQ_CONSTANTS_H_

namespace iit {
namespace HyQ {

// Look at the standard definitions of the robot
static const double a1    = 0.3219;
static const double b1    = 0.045;
static const double eps11 = 0.1089;
static const double d11   = 0.32;
static const double d12   = 0.045;
static const double d13   = 0.08;

static const double a2    = 0.3218;
static const double b2    = 0.045;
static const double eps21 = 0.1403;
static const double eps22 = 0.1047;
static const double d21   = 0.3186;
static const double d22   = 0.045;
static const double d23   = 0.0;



/**
 * Total length of the cylinder (between attachment points) when the rod is
 * fully retracted.
 *
 * Note that it is irrelevant whether the robot can or cannot actually reach the
 * configuration where the cylinder is fully retracted.
 *
 * This value can be used to compute how much the rod is extended given the
 * current total length of the cylinder assembly, which in turn has to be
 * computed with the geometry of the joint.
 */
static const double retractedCylinderLength = 0.281; // [m]

/** Left-Right distance between two HAA axes*/
constexpr double dLR = 0.414;
/** Front-Hind distance between the origins of two HAA frames*/
constexpr double dFH = 0.747;


static const double a1_sqr = a1*a1;
static const double b1_sqr = b1*b1;
static const double a2_sqr = a2*a2;
static const double b2_sqr = b2*b2;



/**
 * The joint angle (in radians) that corresponds to an actuator position state
 * equal to 0, according to the actuator conventions (ie no oil in the positive
 * force chamber).
 * In other words, this is the reading of the LF_HAA status when the leg is raised
 * laterally all the way till the internal end stop (i.e. the end stop of the
 * motor).
*/
const double LF_HAA_JOINT_MOTOR_OFF = -1.2;


static const double BASE2HAAX = 0.3735;  //!< x component of LF_HAA origin, in base frame
static const double BASE2HAAY = 0.207;   //!< y component of LF_HAA origin, in base frame

static const double HAA2HFE  = 0.08;  //!< distance between HFE to HAA, in the xz plane of the HAA frame
//static const double HFE2KFEZ = -0.35;  //!< distance of HFE to KFE in z direction  [is this used? is this at HFE=0rad?]

static const double upleg_length  = 0.35;    //!< length of upper leg
static const double upleg_length_sqr  = upleg_length*upleg_length;



}
}

#endif
