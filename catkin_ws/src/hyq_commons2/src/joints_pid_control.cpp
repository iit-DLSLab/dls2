/*
 * PIDControl.cpp
 *
 *  Created on: May 22, 2013
 *      Author: mfrigerio
 */

#include "iit/robots/hyq/control/joints_pid_control.h"

using namespace iit;
using namespace iit::HyQ::ctrl;


void JointsPIDControl::pid(
        const References& ref,
        const States&     actual)
{
    static iit::dog::JointIdentifiers jid;

    for(int i=0; i<dog::jointsCount; i++)
    {
        jid = dog::orderedJointIDs[i];
        singleJointPID(jid, ref[jid], actual[jid]);

    }
}

