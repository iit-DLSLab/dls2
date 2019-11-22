/*
 * hydraulics.h
 *
 *  Created on: Apr 29, 2013
 *      Author: phd
 */

#ifndef _IIT_HYQ_COMMONS_HYDRAULICS_H_
#define _IIT_HYQ_COMMONS_HYDRAULICS_H_


#include <iit/commons/dog/declarations.h>
#include <iit/commons/hydraulics.h>
#include <iit/robots/hyq/cylinder_lever_arm.h>
#include <iit/robots/hyq/internals.h>
#include <iit/commons/dog/joint_id_tricks.h>
#include <iit/commons/dog/joint_data_map.h>


namespace iit {
namespace HyQ {
namespace hydr {

typedef iit::hydr::ValvePort Valve;

struct HydraulicsParameters {
        iit::hydr::PipelineProperties pipeline;
        iit::hydr::MotorProperties    HAA_motor;
        iit::hydr::CylinderProperties cylinder;
        dog::JointDataMap<iit::hydr::ValveProperties>    valveParams;
        HydraulicsParameters();
};


typedef dog::JointDataMap<iit::hydr::Pressures>          Pressures;
typedef dog::JointDataMap<iit::hydr::HydDynTerms>        DynamicsTerms;
typedef dog::JointDataMap<iit::hydr::LinearizationTerms> LinearizationTerms;
typedef dog::JointDataMap<iit::hydr::LinearizationGains> LinearizationGains;
typedef dog::JointDataMap<Valve::Port>                   ValvePorts;
typedef dog::JointDataMap<iit::hydr::PortToVoltageSign>  ValvePortToVoltageMap;
typedef dog::JointDataMap<iit::hydr::ForceSignToPort>    ForceToValvePortMap;

class HydraulicsDynamics {

public:
    HydraulicsDynamics();

    HydraulicsDynamics(
    	const HydraulicsParameters&,
    	const ValvePortToVoltageMap&,
    	const ForceToValvePortMap&);

    void setHydraulicsParameters(const HydraulicsParameters&);
    void configure(const ValvePortToVoltageMap&, const ForceToValvePortMap&);

    void setActiveValvePort(dog::JointIdentifiers, Valve::Port);
    //void setGains(const JointDataMap<LinearizationGains>& gains);

    void estimateChamberPressures(
            dog::JointIdentifiers j,
            double tau_force,
            iit::hydr::Pressures& press) const;

    void computeDynamicsTerms(
            const dog::JointState& q,
            const dog::JointState& qd,
            const Pressures& pressures);

    void computeLinearizationTerms(
            const LinearizationGains& linGains);

    void applyLinearizationTerms(
            const dog::JointDataMap<double>& force_ctrl_commands,
            dog::JointDataMap<double>& output_commands) const;

    void applyLinearizationTermsAndUpdateValveGains(
            const dog::JointDataMap<double>& force_ctrl_commands,
            dog::JointDataMap<double>& output_commands);

    const DynamicsTerms& getDynamicsTerms() const;
    const LinearizationTerms& getLinearizationTerms() const;

    double jointStateToMotorActuatorState(const dog::JointIdentifiers& j, const double & q) const;

    double jointStateToCylinderActuatorState(const dog::JointIdentifiers& j,const double & q, const double & cylinder_length) const;

    dog::JointDataMap<iit::hydr::ActuatorState> debugActuator;


    void estimateHAAChamberPressures(
            dog::JointIdentifiers j,
            double tau,
            iit::hydr::Pressures& press) const;

    void estimateCylinderChamberPressures(
            dog::JointIdentifiers j,
            double force,
            iit::hydr::Pressures& press) const;

    void setValveGain(
            dog::JointIdentifiers j,
            double valveGain);

    void setValveDoubleGain(
            dog::JointIdentifiers j,
            double gain_low,
            double gain_high,
            double transition_point,
            double dt);

    double getValveGain(
            dog::JointIdentifiers j) const;
private:
    
    void motor_dynamics_terms(
            const dog::JointIdentifiers& haa,
            const double& q, const double& qd,
            const iit::hydr::Pressures& pressures);

    void cylinder_dynamics_terms(
            const dog::JointIdentifiers& sagittalj,
            const double& q, const double& qd,
            const iit::hydr::Pressures& pressures);



private:
    static const dog::JointIdentifiers haa_joints_id[4];
    HydraulicsParameters hydrParams;
    DynamicsTerms        dynamicsTerms;
    LinearizationTerms   linearizationTerms;
    ValvePorts           activeValvePorts;
    ValvePortToVoltageMap valvePortToVoltage;
    ForceToValvePortMap  forceToValvePort;
};



inline HydraulicsDynamics::HydraulicsDynamics() :
        hydrParams(), dynamicsTerms(0.0), linearizationTerms(0.0)
{}

inline void HydraulicsDynamics::setHydraulicsParameters(
        const HydraulicsParameters& params)
{
    hydrParams = params;
}

inline void HydraulicsDynamics::configure(
        const ValvePortToVoltageMap& port2Volt, const ForceToValvePortMap& force2Port)
{
    valvePortToVoltage = port2Volt;
    forceToValvePort   = force2Port;
}


inline void HydraulicsDynamics::setActiveValvePort(
        dog::JointIdentifiers j, Valve::Port p)
{
    activeValvePorts[j] = p;
}

inline void HydraulicsDynamics::estimateHAAChamberPressures(
        dog::JointIdentifiers j,
        double tau,
        iit::hydr::Pressures& pressures) const
{
    iit::hydr::motorChamberPressures(
            hydrParams.HAA_motor,
            forceToValvePort[j].getPositiveForcePort(),
            forceToValvePort[j].getNegativeForcePort(),
            tau, pressures);
}

inline void HydraulicsDynamics::estimateCylinderChamberPressures(
        dog::JointIdentifiers j,
        double force,
        iit::hydr::Pressures& pressures) const
{
    iit::hydr::cylinderChamberPressures(
            hydrParams.cylinder,
            forceToValvePort[j].getPositiveForcePort(),
            forceToValvePort[j].getNegativeForcePort(),
            force, pressures);
}


inline void HydraulicsDynamics::estimateChamberPressures(
        dog::JointIdentifiers j,
        double tau_force,
        iit::hydr::Pressures& pressures) const
{
     if( ! dog::isHAA(j) ) {
        // Compute the pressures in the chambers of the cylinders
        estimateCylinderChamberPressures(j, tau_force, pressures);
    }else{
        // Compute the pressures in the chambers of the HAA motors
        estimateHAAChamberPressures(j, tau_force, pressures);
    }
}

inline const DynamicsTerms& HydraulicsDynamics::getDynamicsTerms() const {
    return dynamicsTerms;
}
inline const LinearizationTerms& HydraulicsDynamics::getLinearizationTerms() const {
    return linearizationTerms;
}


/**
 * Returns the position state of the hydraulic motor on the HAA joint.
 * Such a state refers to the position of the internal vane of the motor;
 * by convention, such a state should be zero when the vane is touching
 * one of the internal end-stops (no oil in one chamber).
 * \param q_haa the angle of the HAA joint, in radians, according to the
 *        robot conventions
 */
inline double HydraulicsDynamics::jointStateToMotorActuatorState(const dog::JointIdentifiers& j,const double & q) const
{
    if( ! isHAA(j) ) {
        //TODO print warning / log error / exception!!
        return 0;
    }
    return q - HyQ::internal::misc_cfg.HAAJointToMotorOffset[ dog::toLegID(j) ];
}

inline double HydraulicsDynamics::jointStateToCylinderActuatorState(const dog::JointIdentifiers& j,
        const double & q, const double & cylinder_length) const
{

    double xp;

    if (forceToValvePort[j].getPositiveForcePort() == iit::hydr::ValvePort::A)
        xp = cylinder_length - HyQ::retractedCylinderLength;
    else
        xp = hydrParams.cylinder.stroke - (cylinder_length - HyQ::retractedCylinderLength);

    return xp;

}

inline void HydraulicsDynamics::setValveGain(
        dog::JointIdentifiers j,
        double valveGain)
{
    hydrParams.valveParams[j].gain = valveGain;
    hydrParams.valveParams[j].gain_f = hydrParams.valveParams[j].gain;
}

inline void HydraulicsDynamics::setValveDoubleGain(
        dog::JointIdentifiers j,
        double gain_low,
        double gain_high,
        double transition_point,
        double dt)
{
    hydrParams.valveParams[j].gain_low = gain_low;
    hydrParams.valveParams[j].gain_high = gain_high;
    hydrParams.valveParams[j].transition_point = transition_point;
    hydrParams.valveParams[j].dt = dt;
    hydrParams.valveParams[j].gain_f = hydrParams.valveParams[j].gain;
}

inline double HydraulicsDynamics::getValveGain(
        dog::JointIdentifiers j) const
{
    return hydrParams.valveParams[j].gain;
}

}
}
}



#endif
