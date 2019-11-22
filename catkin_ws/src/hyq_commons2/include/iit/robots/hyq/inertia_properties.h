#ifndef IIT_ROBOT_HYQ_INERTIA_PROPERTIES_H_
#define IIT_ROBOT_HYQ_INERTIA_PROPERTIES_H_

#include <Eigen/Dense>
#include <iit/rbd/rbd.h>
#include <iit/rbd/InertiaMatrix.h>
#include <iit/rbd/utils.h>

#include <iit/commons/dog/declarations.h>
#include "dynamics_parameters.h"
#include <iit/commons/dog/inertia_properties.h>
#include <iit/robots/hyq/transforms.h>


namespace iit {
namespace HyQ {
/**
 * This namespace encloses classes and functions related to the Dynamics
 * of the robot HyQ.
 */
namespace dyn {

typedef iit::rbd::InertiaMatrixDense InertiaMatrix;

class InertiaProperties : public dog::InertiaPropertiesBase {
    public:
        InertiaProperties(const dog::KinDynParams &);
        ~InertiaProperties();
        const InertiaMatrix& getTensor_trunk();
        const InertiaMatrix& getTensor_LF_hipassembly() const;
        const InertiaMatrix& getTensor_LF_upperleg() const;
        const InertiaMatrix& getTensor_LF_lowerleg() const;
        const InertiaMatrix& getTensor_RF_hipassembly() const;
        const InertiaMatrix& getTensor_RF_upperleg() const;
        const InertiaMatrix& getTensor_RF_lowerleg() const;
        const InertiaMatrix& getTensor_LH_hipassembly() const;
        const InertiaMatrix& getTensor_LH_upperleg() const;
        const InertiaMatrix& getTensor_LH_lowerleg() const;
        const InertiaMatrix& getTensor_RH_hipassembly() const;
        const InertiaMatrix& getTensor_RH_upperleg() const;
        const InertiaMatrix& getTensor_RH_lowerleg() const;
        double getMass_trunk() const;
        double getMass_LF_hipassembly() const;
        double getMass_LF_upperleg() const;
        double getMass_LF_lowerleg() const;
        double getMass_RF_hipassembly() const;
        double getMass_RF_upperleg() const;
        double getMass_RF_lowerleg() const;
        double getMass_LH_hipassembly() const;
        double getMass_LH_upperleg() const;
        double getMass_LH_lowerleg() const;
        double getMass_RH_hipassembly() const;
        double getMass_RH_upperleg() const;
        double getMass_RH_lowerleg() const;
        const iit::rbd::Vector3d& getCOM_trunk() const;
        const iit::rbd::Vector3d& getCOM_LF_hipassembly() const;
        const iit::rbd::Vector3d& getCOM_LF_upperleg() const;
        const iit::rbd::Vector3d& getCOM_LF_lowerleg() const;
        const iit::rbd::Vector3d& getCOM_RF_hipassembly() const;
        const iit::rbd::Vector3d& getCOM_RF_upperleg() const;
        const iit::rbd::Vector3d& getCOM_RF_lowerleg() const;
        const iit::rbd::Vector3d& getCOM_LH_hipassembly() const;
        const iit::rbd::Vector3d& getCOM_LH_upperleg() const;
        const iit::rbd::Vector3d& getCOM_LH_lowerleg() const;
        const iit::rbd::Vector3d& getCOM_RH_hipassembly() const;
        const iit::rbd::Vector3d& getCOM_RH_upperleg() const;
        const iit::rbd::Vector3d& getCOM_RH_lowerleg() const;
public:
        double getTotalMass() const;
        const iit::rbd::Vector3d& getTrunkCOM();
        double getTrunkMass();

        double getHipAssemblyMass(const dog::LegID& leg) const;
        double getUpperLegMass(const dog::LegID& leg) const;
        double getLowerLegMass(const dog::LegID& leg) const;

        rbd::Vector3d getHipAssemblyCOM(const dog::LegID& leg) const;
        rbd::Vector3d getUpperLegCOM(const dog::LegID& leg) const;
        rbd::Vector3d getLowerLegCOM(const dog::LegID& leg) const;

        rbd::Vector3d getWholeBodyCOM(const dog::JointState &q);


        /*!
         * Forces the update of the runtime inertia parameters of the robot
         * HyQ.
         *
         * This function uses in turn the getter methods of the RuntimeParamsGetter
         * member of this instance. All the inertia properties contained in
         * this instance that are defined in terms of non-constant parameters,
         * will be updated.
         */
        void updateParameters();
    private:
        const dog::KinDynParams& paramsGetter;

        HomogeneousTransforms homogeneus_transforms;

        InertiaMatrix tensor_trunk;
        InertiaMatrix tensor_LF_hipassembly;
        InertiaMatrix tensor_LF_upperleg;
        InertiaMatrix tensor_LF_lowerleg;
        InertiaMatrix tensor_RF_hipassembly;
        InertiaMatrix tensor_RF_upperleg;
        InertiaMatrix tensor_RF_lowerleg;
        InertiaMatrix tensor_LH_hipassembly;
        InertiaMatrix tensor_LH_upperleg;
        InertiaMatrix tensor_LH_lowerleg;
        InertiaMatrix tensor_RH_hipassembly;
        InertiaMatrix tensor_RH_upperleg;
        InertiaMatrix tensor_RH_lowerleg;
        iit::rbd::Vector3d com_trunk;
        iit::rbd::Vector3d com_LF_hipassembly;
        iit::rbd::Vector3d com_LF_upperleg;
        iit::rbd::Vector3d com_LF_lowerleg;
        iit::rbd::Vector3d com_RF_hipassembly;
        iit::rbd::Vector3d com_RF_upperleg;
        iit::rbd::Vector3d com_RF_lowerleg;
        iit::rbd::Vector3d com_LH_hipassembly;
        iit::rbd::Vector3d com_LH_upperleg;
        iit::rbd::Vector3d com_LH_lowerleg;
        iit::rbd::Vector3d com_RH_hipassembly;
        iit::rbd::Vector3d com_RH_upperleg;
        iit::rbd::Vector3d com_RH_lowerleg;
};


inline InertiaProperties::~InertiaProperties() {}

inline const InertiaMatrix& InertiaProperties::getTensor_trunk()  {
    updateParameters();
    return this->tensor_trunk;
}
inline const InertiaMatrix& InertiaProperties::getTensor_LF_hipassembly() const {
    return this->tensor_LF_hipassembly;
}
inline const InertiaMatrix& InertiaProperties::getTensor_LF_upperleg() const {
    return this->tensor_LF_upperleg;
}
inline const InertiaMatrix& InertiaProperties::getTensor_LF_lowerleg() const {
    return this->tensor_LF_lowerleg;
}
inline const InertiaMatrix& InertiaProperties::getTensor_RF_hipassembly() const {
    return this->tensor_RF_hipassembly;
}
inline const InertiaMatrix& InertiaProperties::getTensor_RF_upperleg() const {
    return this->tensor_RF_upperleg;
}
inline const InertiaMatrix& InertiaProperties::getTensor_RF_lowerleg() const {
    return this->tensor_RF_lowerleg;
}
inline const InertiaMatrix& InertiaProperties::getTensor_LH_hipassembly() const {
    return this->tensor_LH_hipassembly;
}
inline const InertiaMatrix& InertiaProperties::getTensor_LH_upperleg() const {
    return this->tensor_LH_upperleg;
}
inline const InertiaMatrix& InertiaProperties::getTensor_LH_lowerleg() const {
    return this->tensor_LH_lowerleg;
}
inline const InertiaMatrix& InertiaProperties::getTensor_RH_hipassembly() const {
    return this->tensor_RH_hipassembly;
}
inline const InertiaMatrix& InertiaProperties::getTensor_RH_upperleg() const {
    return this->tensor_RH_upperleg;
}
inline const InertiaMatrix& InertiaProperties::getTensor_RH_lowerleg() const {
    return this->tensor_RH_lowerleg;
}
inline double InertiaProperties::getMass_trunk() const {
    return this->tensor_trunk.getMass();
}
inline double InertiaProperties::getMass_LF_hipassembly() const {
    return this->tensor_LF_hipassembly.getMass();
}
inline double InertiaProperties::getMass_LF_upperleg() const {
    return this->tensor_LF_upperleg.getMass();
}
inline double InertiaProperties::getMass_LF_lowerleg() const {
    return this->tensor_LF_lowerleg.getMass();
}
inline double InertiaProperties::getMass_RF_hipassembly() const {
    return this->tensor_RF_hipassembly.getMass();
}
inline double InertiaProperties::getMass_RF_upperleg() const {
    return this->tensor_RF_upperleg.getMass();
}
inline double InertiaProperties::getMass_RF_lowerleg() const {
    return this->tensor_RF_lowerleg.getMass();
}
inline double InertiaProperties::getMass_LH_hipassembly() const {
    return this->tensor_LH_hipassembly.getMass();
}
inline double InertiaProperties::getMass_LH_upperleg() const {
    return this->tensor_LH_upperleg.getMass();
}
inline double InertiaProperties::getMass_LH_lowerleg() const {
    return this->tensor_LH_lowerleg.getMass();
}
inline double InertiaProperties::getMass_RH_hipassembly() const {
    return this->tensor_RH_hipassembly.getMass();
}
inline double InertiaProperties::getMass_RH_upperleg() const {
    return this->tensor_RH_upperleg.getMass();
}
inline double InertiaProperties::getMass_RH_lowerleg() const {
    return this->tensor_RH_lowerleg.getMass();
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_trunk() const {
    return this->com_trunk;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_LF_hipassembly() const {
    return this->com_LF_hipassembly;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_LF_upperleg() const {
    return this->com_LF_upperleg;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_LF_lowerleg() const {
    return this->com_LF_lowerleg;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_RF_hipassembly() const {
    return this->com_RF_hipassembly;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_RF_upperleg() const {
    return this->com_RF_upperleg;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_RF_lowerleg() const {
    return this->com_RF_lowerleg;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_LH_hipassembly() const {
    return this->com_LH_hipassembly;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_LH_upperleg() const {
    return this->com_LH_upperleg;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_LH_lowerleg() const {
    return this->com_LH_lowerleg;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_RH_hipassembly() const {
    return this->com_RH_hipassembly;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_RH_upperleg() const {
    return this->com_RH_upperleg;
}
inline const iit::rbd::Vector3d& InertiaProperties::getCOM_RH_lowerleg() const {
    return this->com_RH_lowerleg;
}

inline double InertiaProperties::getTotalMass() const {
    return paramsGetter.getValue_trunk_mass() + 2.93 + 2.638 + 0.881 + 2.93 + 2.638 + 0.881 + 2.93 + 2.638 + 0.881 + 2.93 + 2.638 + 0.881;
}

}
}
}

#endif
