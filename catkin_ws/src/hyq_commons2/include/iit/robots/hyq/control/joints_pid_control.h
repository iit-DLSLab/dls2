/*
 * PIDControl.h
 *
 *  Created on: May 22, 2013
 *      Author: mfrigerio
 */

#ifndef _IIT_HYQ_COMMONS_PIDCONTROL_H_
#define _IIT_HYQ_COMMONS_PIDCONTROL_H_

#include <iit/commons/dog/declarations.h>
#include <iit/commons/dog/joint_data_map.h>
#include <iit/commons/control.h>

#include <iit/commons/dog/leg_data_map.h>

namespace iit {
namespace HyQ {
namespace ctrl {

typedef iit::commons::ctrl::pid_errors pid_errors; // to cope with a possible change in iit:: ...
typedef iit::commons::ctrl::pid_gains  pid_gains;
typedef iit::commons::ctrl::pid_terms  pid_terms;

typedef iit::dog::JointDataMap<pid_errors> JointsPIDerrors;
typedef iit::dog::JointDataMap<pid_gains>  JointsPIDgains;
typedef iit::dog::JointDataMap<pid_terms>  JointsPIDterms;

typedef iit::dog::JointDataMap<double>  References;
typedef iit::dog::JointDataMap<double>  States;

typedef iit::dog::LegDataMap<pid_errors> HAAsPIDerrors;
typedef iit::dog::LegDataMap<pid_gains>  HAAsPIDgains;
typedef iit::dog::LegDataMap<pid_terms>  HAAsPIDterms;


class JointsPIDControl {
public:
    JointsPIDControl();
    JointsPIDControl(double ts, double tf);

    //void setTs(double ts);
    //void setTf(double tf);
    double getTs() const;
    double getTf() const;

    void resetIntegrators();

    void setGains(const JointsPIDgains& gains);
    const JointsPIDgains& getGains() const;

    const JointsPIDerrors& getErrors() const;
    const JointsPIDterms&  getTerms()  const;

    void pid(
            const References& ref,
            const States&     actual);

private:
    void singleJointPID(iit::dog::JointIdentifiers j, double ref, double actual);
private:
    commons::ctrl::PIDControl controller;
    JointsPIDerrors errors;
    JointsPIDgains  gains;
    JointsPIDterms  terms;
};


inline JointsPIDControl::JointsPIDControl(double ts, double tf)
    : controller(ts, tf), errors(0.0), gains(0.0), terms(0.0)
{
}


inline double JointsPIDControl::getTs() const {
    return controller.getTs();
}
inline double JointsPIDControl::getTf() const {
    return controller.getTf();
}

inline void JointsPIDControl::setGains(const JointsPIDgains& g) {
    gains = g;
}

inline const JointsPIDgains& JointsPIDControl::getGains() const {
    return gains;
}

inline const JointsPIDerrors& JointsPIDControl::getErrors() const {
    return errors;
}

inline const JointsPIDterms& JointsPIDControl::getTerms() const {
    return terms;
}

inline void JointsPIDControl::resetIntegrators()
{
    static iit::dog::JointIdentifiers jid;

    for(int i=0; i<dog::jointsCount; i++)
    {
        jid = dog::orderedJointIDs[i];
        controller.resetIntegrator(terms[jid]);
    }
}


inline void JointsPIDControl::singleJointPID(
        iit::dog::JointIdentifiers j,
        double ref,
        double actual)
{
    errors[j].current = ref - actual;
    controller.pid(gains[j], errors[j], terms[j]);
    errors[j].updatePrevious();
}

}
}
}


#endif
