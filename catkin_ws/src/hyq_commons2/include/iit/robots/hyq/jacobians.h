#ifndef HYQ_JACOBIANS_H_
#define HYQ_JACOBIANS_H_

#include <iit/rbd/TransformsBase.h>
#include <iit/commons/dog/declarations.h>
#include <iit/commons/dog/kin_dyn_params.h>
#include <iit/commons/dog/leg_data_map.h>

#include "kinematics_parameters.h"

namespace iit {
namespace HyQ {

typedef dog::JointState JointState;

template<int COLS, class M>
class JacobianT : public iit::rbd::JacobianBase<JointState, COLS, M>
{};

/**
 *
 */
class Jacobians {
    public:
        class Type_fr_trunk_J_LF_foot : public JacobianT<3, Type_fr_trunk_J_LF_foot>
        {
        public:
            Type_fr_trunk_J_LF_foot(const Params_lengths& _lengths_values);
            const Type_fr_trunk_J_LF_foot& update(const JointState&);
        protected:
            const Params_lengths* lengths_values;
        };
        
        class Type_fr_trunk_J_RF_foot : public JacobianT<3, Type_fr_trunk_J_RF_foot>
        {
        public:
            Type_fr_trunk_J_RF_foot(const Params_lengths& _lengths_values);
            const Type_fr_trunk_J_RF_foot& update(const JointState&);
        protected:
            const Params_lengths* lengths_values;
        };
        
        class Type_fr_trunk_J_LH_foot : public JacobianT<3, Type_fr_trunk_J_LH_foot>
        {
        public:
            Type_fr_trunk_J_LH_foot(const Params_lengths& _lengths_values);
            const Type_fr_trunk_J_LH_foot& update(const JointState&);
        protected:
            const Params_lengths* lengths_values;
        };
        
        class Type_fr_trunk_J_RH_foot : public JacobianT<3, Type_fr_trunk_J_RH_foot>
        {
        public:
            Type_fr_trunk_J_RH_foot(const Params_lengths& _lengths_values);
            const Type_fr_trunk_J_RH_foot& update(const JointState&);
        protected:
            const Params_lengths* lengths_values;
        };
        
        class Type_fr_trunk_J_LF_shin : public JacobianT<3, Type_fr_trunk_J_LF_shin>
        {
        public:
            Type_fr_trunk_J_LF_shin(const Params_lengths& _lengths_values);
            const Type_fr_trunk_J_LF_shin& update(const JointState&);
        protected:
            const Params_lengths* lengths_values;
        };
        
        class Type_fr_trunk_J_RF_shin : public JacobianT<3, Type_fr_trunk_J_RF_shin>
        {
        public:
            Type_fr_trunk_J_RF_shin(const Params_lengths& _lengths_values);
            const Type_fr_trunk_J_RF_shin& update(const JointState&);
        protected:
            const Params_lengths* lengths_values;
        };
        
        class Type_fr_trunk_J_LH_shin : public JacobianT<3, Type_fr_trunk_J_LH_shin>
        {
        public:
            Type_fr_trunk_J_LH_shin(const Params_lengths& _lengths_values);
            const Type_fr_trunk_J_LH_shin& update(const JointState&);
        protected:
            const Params_lengths* lengths_values;
        };
        
        class Type_fr_trunk_J_RH_shin : public JacobianT<3, Type_fr_trunk_J_RH_shin>
        {
        public:
            Type_fr_trunk_J_RH_shin(const Params_lengths& _lengths_values);
            const Type_fr_trunk_J_RH_shin& update(const JointState&);
        protected:
            const Params_lengths* lengths_values;
        };
        
    public:
        Jacobians(const dog::KinDynParams&);
        void updateParameters();
    public:
        Type_fr_trunk_J_LF_foot fr_trunk_J_LF_foot;
        Type_fr_trunk_J_RF_foot fr_trunk_J_RF_foot;
        Type_fr_trunk_J_LH_foot fr_trunk_J_LH_foot;
        Type_fr_trunk_J_RH_foot fr_trunk_J_RH_foot;
        Type_fr_trunk_J_LF_shin fr_trunk_J_LF_shin;
        Type_fr_trunk_J_RF_shin fr_trunk_J_RF_shin;
        Type_fr_trunk_J_LH_shin fr_trunk_J_LH_shin;
        Type_fr_trunk_J_RH_shin fr_trunk_J_RH_shin;
public:
        dog::FootJac getFootJacobianXY(const JointState & q,
                                       const iit::dog::LegID& leg,
                                       const double& foot_x,
                                       const double& foot_y);
	
	/*dog::FootJac getAngularFootJacobianXY(const JointState & q,
                                       const iit::dog::LegID& leg,
                                       const double& foot_x,
                                       const double& foot_y);*/

    protected:
        Params_lengths lengths_values;
        dog::FootJac foot_jacobian_;


        const dog::KinDynParams& valuesGetter_lengths;
};


}
}

#endif
