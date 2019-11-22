/*
 * hydraulics.cpp
 *
 *  Created on: May 20, 2013
 *      Author: mfrigerio
 */

#include <iit/commons/hydraulics.h>
#include <iit/commons/control.h>

#include <iit/commons/dog/declarations.h>
#include <iit/commons/dog/joint_data_map.h>


#include <iit/robots/hyq/hydraulics/hydraulics.h>

using namespace iit;

using namespace iit::commons;



HyQ::hydr::HydraulicsParameters::HydraulicsParameters() {
    pipeline.oilBulkMod = 1.8181E9;
    pipeline.volume = 8.17E-6; // tube 0.65 long, internal diameter 0.004 (everything in meters); // TODO this comes from hyl2
    // TODO check the following
    HAA_motor.volumetricDisp = 6.87E-6;
    HAA_motor.totVolume      = 12.0E-6; //range of motion (100°) times vol displacement
	HAA_motor.unusedVolume[iit::hydr::ValvePort::A] = 0.0; // TODO need to update iit commons
	HAA_motor.unusedVolume[iit::hydr::ValvePort::B] = 0.0; // TODO need to update iit commons
    //cylinder
    cylinder.pistonArea[iit::hydr::ValvePort::A] = 2.0106 * 1E-04; //area chamber A [m²]
    cylinder.pistonArea[iit::hydr::ValvePort::B] =  0.6094*cylinder.pistonArea[iit::hydr::ValvePort::A];
    cylinder.stroke = 0.08; //m
    iit::hydr::valveProperties defaultValveProperties;
    defaultValveProperties.gain = 6.12E-8;
    defaultValveProperties.gain_f =  defaultValveProperties.gain;
    defaultValveProperties.gain_high = 6.12E-8;
    defaultValveProperties.gain_low = 6.12E-8;
    defaultValveProperties.transition_point = 0.0;
    //TODO Kv = (1.8*2/60000)/(1.6)/sqrt(68.9476*10^5/2)*10 Kv =    2.019699178641170e-07
    valveParams = defaultValveProperties;
}


HyQ::hydr::HydraulicsDynamics::HydraulicsDynamics(
	const HydraulicsParameters& params,
	const ValvePortToVoltageMap& port2voltage,
   	const ForceToValvePortMap&  force2port)
    
    :
    hydrParams(params),
    dynamicsTerms(0.0),
    linearizationTerms(0.0),
    valvePortToVoltage(port2voltage),
    forceToValvePort(force2port)
{

}


void HyQ::hydr::HydraulicsDynamics::computeDynamicsTerms(
        const dog::JointState& q,
        const dog::JointState& qd,
        const Pressures& pressures)
{
    dog::JointIdentifiers j;
    for (int i = 0; i<dog::jointsCount; i++)
    {
        j = dog::orderedJointIDs[i];
        if( ! dog::isHAA(j) ) {
            cylinder_dynamics_terms(j, q(j), qd(j), pressures[j]);
        }else {
            motor_dynamics_terms(j, q(j), qd(j), pressures[j]);
        }
    }
}


void HyQ::hydr::HydraulicsDynamics::computeLinearizationTerms(const LinearizationGains& gains)
{
    dog::JointIdentifiers j;
    for (int i = 0; i<dog::jointsCount; i++)
    {
        j = dog::orderedJointIDs[i];
        iit::hydr::computeLinearizationTerms(dynamicsTerms[j], gains[j], linearizationTerms[j]);

    }
}

void HyQ::hydr::HydraulicsDynamics::applyLinearizationTerms(
        const dog::JointDataMap<double>& input_commands,
        dog::JointDataMap<double>& output_commands) const
{
    dog::JointIdentifiers j;
    for (int i = 0; i<dog::jointsCount; i++)
    {
        j = dog::orderedJointIDs[i];
        output_commands[j] = iit::hydr::applyLinearizationTerms(linearizationTerms[j], input_commands[j]);
    }
}

void HyQ::hydr::HydraulicsDynamics::applyLinearizationTermsAndUpdateValveGains(
        const dog::JointDataMap<double>& input_commands,
        dog::JointDataMap<double>& output_commands)
{
    dog::JointIdentifiers j;
    for (int i = 0; i<dog::jointsCount; i++)
    {
        j = dog::orderedJointIDs[i];
        output_commands[j] = iit::hydr::applyLinearizationTerms(hydrParams.valveParams[j], linearizationTerms[j], input_commands[j]);
    }
}


void HyQ::hydr::HydraulicsDynamics::motor_dynamics_terms(
        const dog::JointIdentifiers& j,
        const double& q, const double& qd,
        const iit::hydr::Pressures& pressures)
{
    iit::hydr::ActuatorState haa_motorState;

    haa_motorState.q  = jointStateToMotorActuatorState(j,q);
    haa_motorState.qd = qd;
    haa_motorState.activePort = activeValvePorts[j];

    debugActuator[j].qd =  haa_motorState.qd;
    debugActuator[j].q =  haa_motorState.q;

    iit::hydr::feedback_linearization_motor(
            hydrParams.pipeline,
            hydrParams.HAA_motor,
            hydrParams.valveParams[j],
            haa_motorState,
            pressures,
            forceToValvePort[j].getPositiveForcePort(),
            valvePortToVoltage[j],
            dynamicsTerms[j].f, dynamicsTerms[j].g);
}


void HyQ::hydr::HydraulicsDynamics::cylinder_dynamics_terms(
        const dog::JointIdentifiers& j,
        const double& q, const double& qd,
        const iit::hydr::Pressures& pressures)
{
    iit::hydr::ActuatorState cylinderState;

    //actuator state are piston position and velocitys
    cylinderState.q  = jointStateToCylinderActuatorState(j, q, getCylinderLength(j,q)); //piston position
    cylinderState.qd =  qd * getCylinderLeverArm( j, getCylinderLength(j, q) ); //piston velocity is the joint velocity multiplied by lever arm



    cylinderState.activePort = activeValvePorts[j];

    debugActuator[j].qd =  cylinderState.qd;
    debugActuator[j].q =  cylinderState.q;

//    iit::hydr::ValveProperties modifiedValveProp  =   hydrParams.valveParams;
//    if ((j == HyQ::LH_HFE) || (j == HyQ::LH_KFE)){
//        modifiedValveProp.gain = 6.12E-8;
//    }

    iit::hydr::feedback_linearization_cylinder(
            hydrParams.pipeline,
            hydrParams.cylinder,
            hydrParams.valveParams[j],
            cylinderState,
            pressures,
            forceToValvePort[j].getPositiveForcePort(),
            valvePortToVoltage[j],
            dynamicsTerms[j].f, dynamicsTerms[j].g);
}




