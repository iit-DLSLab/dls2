#ifndef DLS_MSGS__DESIRED_POSE__H
#define DLS_MSGS__DESIRED_POSE__H

#include "bitmask_operators.hpp"

namespace dls_msgs {

enum class DesiredTrajectoryFlag : unsigned long {
    DISABLED = 0,
    X = 1,
    Y = 2,
    Z = 4,
    ROLL = 8,
    PITCH = 16,
    YAW = 32,
    XDOT = 64,
    YDOT = 128,
    ZDOT = 256,
    ROLLDOT = 512,
    PITCHDOT = 1024,
    YAWDOT = 2048,
    XDDOT = 4096,
    YDDOT = 8196,
    ZDDOT = 16384,
    ROLLDDOT = 32768,
    PITCHDDOT = 65536,
    YAWDDOT = 131072,
    Q = 262144,
    QDOT = 524288,
    QDDOT = 1048576,
    MAX
};

enum class DesiredPoseFlag : unsigned {
    DISABLED = 0,
    X = 1,
    Y = 2,
    Z = 4,
    ROLL = 8,
    PITCH = 16,
    YAW = 32,
    Q = 64,
    MAX
};

template<> struct enable_bitmask_operators<DesiredTrajectoryFlag>{static const bool enable=true;};
template<> struct enable_bitmask_operators<DesiredPoseFlag>{static const bool enable=true;};

const static DesiredPoseFlag DESIREDATTRPY = DesiredPoseFlag::ROLL | DesiredPoseFlag::PITCH | DesiredPoseFlag::YAW;
const static DesiredPoseFlag DESIREDPOSITION2D = DesiredPoseFlag::X | DesiredPoseFlag::Y;
const static DesiredPoseFlag DESIREDPOSITION3D = DesiredPoseFlag::X | DesiredPoseFlag::Y | DesiredPoseFlag::Z; 
const static DesiredPoseFlag DESIREDPOSE3D = DESIREDPOSITION2D | DesiredPoseFlag::YAW; 
const static DesiredPoseFlag DESIREDPOSE4D = DESIREDPOSITION3D | DesiredPoseFlag::YAW; 
const static DesiredPoseFlag DESIREDPOSE6D = DESIREDPOSITION3D | DESIREDATTRPY; 
const static DesiredPoseFlag DESIREDPOSE6DQ = DESIREDPOSITION3D | DesiredPoseFlag::Q; 

class DesiredPose {
public:
    DesiredPose() {
	p_[0]=0; p_[1]=0; p_[2]=0;
	eta_[0]=0; eta_[1]=0; eta_[2]=0;
	q_[0]=1; q_[1]=0; q_[2]=0; q_[3]=0;
	flag_ = DesiredPoseFlag::DISABLED;
    }
    
    const double *getP() { return p_; }
    const double *getEta() { return eta_; }
    const double *getQ() { return q_; }
    
    DesiredPoseFlag getDesiredPoseValid() { return flag_; }
    void setDesiredPoseValid(DesiredPoseFlag flag) { flag_=flag;}

   
    void setP(double x, double y, double z) { p_[0]=x; p_[1]=y; p_[2]=z;}
    void setEta(double roll, double pitch, double yaw) { eta_[0]=roll; eta_[1]=pitch; eta_[2]=yaw;}
    void setQ(double q0, double q1, double q2, double q3) { q_[0]=q0; q_[1]=q1; q_[2]=q2; q_[3]=q3;}
    
    const 
    ~DesiredPose() {}
private:
    double p_[3];
    double eta_[3];
    double q_[4];
    DesiredPoseFlag flag_;
};

class DesiredTrajectory {
public:
    DesiredTrajectory() {
	for (int i=0;i<3;i++) {
	    p_[i]=0; pdot_[i]=0; pddot_[i]=0;
	    eta_[i]=0; etadot_[i]=0; etaddot_[i]=0;
	    q_[i+1]=0; qdot_[i]=0;qddot_[i]=0;
	}
	q_[0]=1; qdot_[0]=1; qddot_[0]=1;
	flag_ = DesiredTrajectoryFlag::DISABLED;
    }
    
    const double *getP() { return p_; }
    const double *getPDot() { return pdot_; }
    const double *getPDDot() { return pddot_; }
    const double *getEta() { return eta_; }
    const double *getEtaDot() { return etadot_; }
    const double *getEtaDDot() { return etaddot_; }
    const double *getQ() { return q_; }
    const double *getQDot() { return qdot_; }
    const double *getQDDot() { return qddot_; }
    
    DesiredTrajectoryFlag getDesiredTrajectoryValid() { return flag_; }
    void setDesiredTrajectoryValid(DesiredTrajectoryFlag flag) { flag_=flag;}

   
    void setP(double x, double y, double z) { p_[0]=x; p_[1]=y; p_[2]=z;}
    void setPDot(double xdot, double ydot, double zdot) { pdot_[0]=xdot; pdot_[1]=ydot; pdot_[2]=zdot;}
    void setPDDot(double xddot, double yddot, double zddot) { pddot_[0]=xddot; pddot_[1]=yddot; pddot_[2]=zddot;}
    void setEta(double roll, double pitch, double yaw) { eta_[0]=roll; eta_[1]=pitch; eta_[2]=yaw;}
    void setEtaDot(double rolldot, double pitchdot, double yawdot) { etadot_[0]=rolldot; etadot_[1]=pitchdot; etadot_[2]=yawdot;}
    void setEtaDDot(double rollddot, double pitchddot, double yawddot) { etaddot_[0]=rollddot; etaddot_[1]=pitchddot; etaddot_[2]=yawddot;}
    void setQ(double q0, double q1, double q2, double q3) { q_[0]=q0; q_[1]=q1; q_[2]=q2; q_[3]=q3;}
    void setQDot(double q0dot, double q1dot, double q2dot, double q3dot) { qdot_[0]=q0dot; qdot_[1]=q1dot; qdot_[2]=q2dot; qdot_[3]=q3dot;}
    void setQDDot(double q0ddot, double q1ddot, double q2ddot, double q3ddot) { qddot_[0]=q0ddot; qddot_[1]=q1ddot; qddot_[2]=q2ddot; qddot_[3]=q3ddot;}
    
    const 
    ~DesiredTrajectory() {}
private:
    double p_[3], pdot_[3], pddot_[3];
    double eta_[3],etadot_[3],etaddot_[3];
    double q_[4],qdot_[4],qddot_[4];
    DesiredTrajectoryFlag flag_;
};



} //namespace dls_msgs


#endif
