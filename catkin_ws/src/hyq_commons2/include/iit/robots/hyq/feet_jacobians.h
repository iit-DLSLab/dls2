#ifndef IIT_HYQ_FEETJACOBIANS_H_
#define IIT_HYQ_FEETJACOBIANS_H_

#include <iit/commons/dog/feet_jacobians.h>
#include <iit/commons/dog/leg_data_map.h>

#include <iit/commons/dog/declarations.h>
#include <iit/commons/dog/joint_id_tricks.h>
#include <iit/robots/hyq/jacobians.h>

#include <iit/rbd/utils.h>

namespace iit {
namespace HyQ {



class FeetJacobians : public dog::FeetJacobians
{
typedef dog::JointState JointState;


public:
    FeetJacobians(HyQ::Jacobians& jacobians) : jacs(jacobians)
    {
        lin_myGetters[dog::LF] = & FeetJacobians::getFootJacobianLF;
        lin_myGetters[dog::RF] = & FeetJacobians::getFootJacobianRF;
        lin_myGetters[dog::LH] = & FeetJacobians::getFootJacobianLH;
        lin_myGetters[dog::RH] = & FeetJacobians::getFootJacobianRH;
	ang_myGetters[dog::LF] = & FeetJacobians::getAngularFootJacobianLF;
        ang_myGetters[dog::RF] = & FeetJacobians::getAngularFootJacobianRF;
        ang_myGetters[dog::LH] = & FeetJacobians::getAngularFootJacobianLH;
        ang_myGetters[dog::RH] = & FeetJacobians::getAngularFootJacobianRH;

    }
    ~FeetJacobians() {}

    dog::FootJac getFootJacobian(const JointState& q, const dog::LegID& leg) {
        return ((*this).*(lin_myGetters[leg]))(q);
    }

    dog::FootJac getFootJacobianLF(const JointState& q) {
        jacs.updateParameters();
        return jacs.fr_trunk_J_LF_foot(q).block<3,3>(rbd::LX,0);
    }

    dog::FootJac getFootJacobianRF(const JointState& q) {
        jacs.updateParameters();
        return jacs.fr_trunk_J_RF_foot(q).block<3,3>(rbd::LX,0);
    }

    dog::FootJac getFootJacobianLH(const JointState& q)  {
        jacs.updateParameters();
        return jacs.fr_trunk_J_LH_foot(q).block<3,3>(rbd::LX,0);
    }

    dog::FootJac getFootJacobianRH(const JointState& q)  {
        jacs.updateParameters();
        return jacs.fr_trunk_J_RH_foot(q).block<3,3>(rbd::LX,0);
    }


   dog::FootJac getFootJacobian(const JointState &q, const dog::LegID &leg,
                                const double& foot_x, const double& foot_y)
   {
       jacs.updateParameters();
       return jacs.getFootJacobianXY(q, leg, foot_x, foot_y);
   }
   
   
   dog::FootJac getAngularFootJacobian(const JointState& q, const dog::LegID& leg) {
       return ((*this).*(ang_myGetters[leg]))(q);
   }

   dog::FootJac getAngularFootJacobianLF(const JointState& q) {
       jacs.updateParameters();
       return jacs.fr_trunk_J_LF_foot(q).block<3,3>(rbd::AX,0);
   }

   dog::FootJac getAngularFootJacobianRF(const JointState& q) {
       jacs.updateParameters();
       return jacs.fr_trunk_J_RF_foot(q).block<3,3>(rbd::AX,0);
   }

   dog::FootJac getAngularFootJacobianLH(const JointState& q)  {
       jacs.updateParameters();
       return jacs.fr_trunk_J_LH_foot(q).block<3,3>(rbd::AX,0);
   }

   dog::FootJac getAngularFootJacobianRH(const JointState& q)  {
       jacs.updateParameters();
       return jacs.fr_trunk_J_RH_foot(q).block<3,3>(rbd::AX,0);
   }


  /*dog::FootJac getAngularFootJacobian(const JointState &q, const dog::LegID &leg,
			       const double& foot_x, const double& foot_y)
  {
      jacs.updateParameters();
      return jacs.getAngularFootJacobianXY(q, leg, foot_x, foot_y);
  }*/
   
   
private:
    HyQ::Jacobians& jacs;

    typedef  dog::FootJac(FeetJacobians::*getter)(const JointState&);

    dog::LegDataMap< getter > lin_myGetters, ang_myGetters;
};


}
}


#endif
