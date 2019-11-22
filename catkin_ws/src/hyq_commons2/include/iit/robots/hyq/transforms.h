#ifndef HYQ_TRANSFORMS_H_
#define HYQ_TRANSFORMS_H_

#include <Eigen/Dense>
#include <iit/rbd/TransformsBase.h>
#include <iit/commons/dog/declarations.h>
#include <iit/commons/dog/transforms.h>
#include <iit/commons/dog/kin_dyn_params.h>
#include "kinematics_parameters.h"

namespace iit {
namespace HyQ {

// The type of the "vector" with the status of the variables
typedef iit::dog::JointState state_t;

template<class M>
class TransformMotion : public iit::rbd::SpatialTransformBase<state_t, M> {};

template<class M>
class TransformForce : public iit::rbd::SpatialTransformBase<state_t, M> {};

template<class M>
class TransformHomogeneous : public iit::rbd::HomogeneousTransformBase<state_t, M> {};


/**
 * The class for the 6-by-6 coordinates transformation matrices for
 * spatial motion vectors.
 */
class MotionTransforms : public dog::MotionTransformsBase {
public:
    class Dummy {};
    typedef TransformMotion<Dummy>::MatrixType MatrixType;
public:
    dog::SpatialTransformPlain getTransform(const dog::JointState &q, const dog::OriginFrame &orig, const dog::DestFrame &dest);
public:
    class Type_fr_LF_lowerleg_X_LF_foot : public TransformMotion<Type_fr_LF_lowerleg_X_LF_foot>
    {
    public:
        Type_fr_LF_lowerleg_X_LF_foot(const Params_lengths& _lengths_values);
        const Type_fr_LF_lowerleg_X_LF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_RF_lowerleg_X_RF_foot : public TransformMotion<Type_fr_RF_lowerleg_X_RF_foot>
    {
    public:
        Type_fr_RF_lowerleg_X_RF_foot(const Params_lengths& _lengths_values);
        const Type_fr_RF_lowerleg_X_RF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_LH_lowerleg_X_LH_foot : public TransformMotion<Type_fr_LH_lowerleg_X_LH_foot>
    {
    public:
        Type_fr_LH_lowerleg_X_LH_foot(const Params_lengths& _lengths_values);
        const Type_fr_LH_lowerleg_X_LH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_RH_lowerleg_X_RH_foot : public TransformMotion<Type_fr_RH_lowerleg_X_RH_foot>
    {
    public:
        Type_fr_RH_lowerleg_X_RH_foot(const Params_lengths& _lengths_values);
        const Type_fr_RH_lowerleg_X_RH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LF_hipassemblyCOM : public TransformMotion<Type_fr_trunk_X_LF_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_LF_hipassemblyCOM();
        const Type_fr_trunk_X_LF_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RF_hipassemblyCOM : public TransformMotion<Type_fr_trunk_X_RF_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_RF_hipassemblyCOM();
        const Type_fr_trunk_X_RF_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LH_hipassemblyCOM : public TransformMotion<Type_fr_trunk_X_LH_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_LH_hipassemblyCOM();
        const Type_fr_trunk_X_LH_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RH_hipassemblyCOM : public TransformMotion<Type_fr_trunk_X_RH_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_RH_hipassemblyCOM();
        const Type_fr_trunk_X_RH_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LF_upperlegCOM : public TransformMotion<Type_fr_trunk_X_LF_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_LF_upperlegCOM();
        const Type_fr_trunk_X_LF_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RF_upperlegCOM : public TransformMotion<Type_fr_trunk_X_RF_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_RF_upperlegCOM();
        const Type_fr_trunk_X_RF_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LH_upperlegCOM : public TransformMotion<Type_fr_trunk_X_LH_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_LH_upperlegCOM();
        const Type_fr_trunk_X_LH_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RH_upperlegCOM : public TransformMotion<Type_fr_trunk_X_RH_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_RH_upperlegCOM();
        const Type_fr_trunk_X_RH_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LF_lowerlegCOM : public TransformMotion<Type_fr_trunk_X_LF_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_LF_lowerlegCOM();
        const Type_fr_trunk_X_LF_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RF_lowerlegCOM : public TransformMotion<Type_fr_trunk_X_RF_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_RF_lowerlegCOM();
        const Type_fr_trunk_X_RF_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LH_lowerlegCOM : public TransformMotion<Type_fr_trunk_X_LH_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_LH_lowerlegCOM();
        const Type_fr_trunk_X_LH_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RH_lowerlegCOM : public TransformMotion<Type_fr_trunk_X_RH_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_RH_lowerlegCOM();
        const Type_fr_trunk_X_RH_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_LF_foot_X_fr_LF_lowerleg : public TransformMotion<Type_LF_foot_X_fr_LF_lowerleg>
    {
    public:
        Type_LF_foot_X_fr_LF_lowerleg(const Params_lengths& _lengths_values);
        const Type_LF_foot_X_fr_LF_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RF_foot_X_fr_RF_lowerleg : public TransformMotion<Type_RF_foot_X_fr_RF_lowerleg>
    {
    public:
        Type_RF_foot_X_fr_RF_lowerleg(const Params_lengths& _lengths_values);
        const Type_RF_foot_X_fr_RF_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_LH_foot_X_fr_LH_lowerleg : public TransformMotion<Type_LH_foot_X_fr_LH_lowerleg>
    {
    public:
        Type_LH_foot_X_fr_LH_lowerleg(const Params_lengths& _lengths_values);
        const Type_LH_foot_X_fr_LH_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RH_foot_X_fr_RH_lowerleg : public TransformMotion<Type_RH_foot_X_fr_RH_lowerleg>
    {
    public:
        Type_RH_foot_X_fr_RH_lowerleg(const Params_lengths& _lengths_values);
        const Type_RH_foot_X_fr_RH_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LF_foot : public TransformMotion<Type_fr_trunk_X_LF_foot>
    {
    public:
        Type_fr_trunk_X_LF_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RF_foot : public TransformMotion<Type_fr_trunk_X_RF_foot>
    {
    public:
        Type_fr_trunk_X_RF_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LH_foot : public TransformMotion<Type_fr_trunk_X_LH_foot>
    {
    public:
        Type_fr_trunk_X_LH_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RH_foot : public TransformMotion<Type_fr_trunk_X_RH_foot>
    {
    public:
        Type_fr_trunk_X_RH_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_LF_lowerleg_X_fr_trunk : public TransformMotion<Type_fr_LF_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_LF_lowerleg_X_fr_trunk();
        const Type_fr_LF_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_lowerleg_X_fr_trunk : public TransformMotion<Type_fr_RF_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_RF_lowerleg_X_fr_trunk();
        const Type_fr_RF_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_lowerleg_X_fr_trunk : public TransformMotion<Type_fr_LH_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_LH_lowerleg_X_fr_trunk();
        const Type_fr_LH_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_lowerleg_X_fr_trunk : public TransformMotion<Type_fr_RH_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_RH_lowerleg_X_fr_trunk();
        const Type_fr_RH_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_LF_foot_X_fr_trunk : public TransformMotion<Type_LF_foot_X_fr_trunk>
    {
    public:
        Type_LF_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_LF_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RF_foot_X_fr_trunk : public TransformMotion<Type_RF_foot_X_fr_trunk>
    {
    public:
        Type_RF_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_RF_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_LH_foot_X_fr_trunk : public TransformMotion<Type_LH_foot_X_fr_trunk>
    {
    public:
        Type_LH_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_LH_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RH_foot_X_fr_trunk : public TransformMotion<Type_RH_foot_X_fr_trunk>
    {
    public:
        Type_RH_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_RH_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LF_shin : public TransformMotion<Type_fr_trunk_X_LF_shin>
    {
    public:
        Type_fr_trunk_X_LF_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LF_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RF_shin : public TransformMotion<Type_fr_trunk_X_RF_shin>
    {
    public:
        Type_fr_trunk_X_RF_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RF_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LH_shin : public TransformMotion<Type_fr_trunk_X_LH_shin>
    {
    public:
        Type_fr_trunk_X_LH_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LH_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RH_shin : public TransformMotion<Type_fr_trunk_X_RH_shin>
    {
    public:
        Type_fr_trunk_X_RH_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RH_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_fr_LF_HAA : public TransformMotion<Type_fr_trunk_X_fr_LF_HAA>
    {
    public:
        Type_fr_trunk_X_fr_LF_HAA();
        const Type_fr_trunk_X_fr_LF_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LF_HFE : public TransformMotion<Type_fr_trunk_X_fr_LF_HFE>
    {
    public:
        Type_fr_trunk_X_fr_LF_HFE();
        const Type_fr_trunk_X_fr_LF_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LF_KFE : public TransformMotion<Type_fr_trunk_X_fr_LF_KFE>
    {
    public:
        Type_fr_trunk_X_fr_LF_KFE();
        const Type_fr_trunk_X_fr_LF_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_HAA : public TransformMotion<Type_fr_trunk_X_fr_RF_HAA>
    {
    public:
        Type_fr_trunk_X_fr_RF_HAA();
        const Type_fr_trunk_X_fr_RF_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_HFE : public TransformMotion<Type_fr_trunk_X_fr_RF_HFE>
    {
    public:
        Type_fr_trunk_X_fr_RF_HFE();
        const Type_fr_trunk_X_fr_RF_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_KFE : public TransformMotion<Type_fr_trunk_X_fr_RF_KFE>
    {
    public:
        Type_fr_trunk_X_fr_RF_KFE();
        const Type_fr_trunk_X_fr_RF_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_HAA : public TransformMotion<Type_fr_trunk_X_fr_LH_HAA>
    {
    public:
        Type_fr_trunk_X_fr_LH_HAA();
        const Type_fr_trunk_X_fr_LH_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_HFE : public TransformMotion<Type_fr_trunk_X_fr_LH_HFE>
    {
    public:
        Type_fr_trunk_X_fr_LH_HFE();
        const Type_fr_trunk_X_fr_LH_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_KFE : public TransformMotion<Type_fr_trunk_X_fr_LH_KFE>
    {
    public:
        Type_fr_trunk_X_fr_LH_KFE();
        const Type_fr_trunk_X_fr_LH_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_HAA : public TransformMotion<Type_fr_trunk_X_fr_RH_HAA>
    {
    public:
        Type_fr_trunk_X_fr_RH_HAA();
        const Type_fr_trunk_X_fr_RH_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_HFE : public TransformMotion<Type_fr_trunk_X_fr_RH_HFE>
    {
    public:
        Type_fr_trunk_X_fr_RH_HFE();
        const Type_fr_trunk_X_fr_RH_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_KFE : public TransformMotion<Type_fr_trunk_X_fr_RH_KFE>
    {
    public:
        Type_fr_trunk_X_fr_RH_KFE();
        const Type_fr_trunk_X_fr_RH_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_hipassembly_X_fr_trunk : public TransformMotion<Type_fr_LF_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_LF_hipassembly_X_fr_trunk();
        const Type_fr_LF_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LF_hipassembly : public TransformMotion<Type_fr_trunk_X_fr_LF_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_LF_hipassembly();
        const Type_fr_trunk_X_fr_LF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_upperleg_X_fr_LF_hipassembly : public TransformMotion<Type_fr_LF_upperleg_X_fr_LF_hipassembly>
    {
    public:
        Type_fr_LF_upperleg_X_fr_LF_hipassembly();
        const Type_fr_LF_upperleg_X_fr_LF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_hipassembly_X_fr_LF_upperleg : public TransformMotion<Type_fr_LF_hipassembly_X_fr_LF_upperleg>
    {
    public:
        Type_fr_LF_hipassembly_X_fr_LF_upperleg();
        const Type_fr_LF_hipassembly_X_fr_LF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_lowerleg_X_fr_LF_upperleg : public TransformMotion<Type_fr_LF_lowerleg_X_fr_LF_upperleg>
    {
    public:
        Type_fr_LF_lowerleg_X_fr_LF_upperleg();
        const Type_fr_LF_lowerleg_X_fr_LF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_upperleg_X_fr_LF_lowerleg : public TransformMotion<Type_fr_LF_upperleg_X_fr_LF_lowerleg>
    {
    public:
        Type_fr_LF_upperleg_X_fr_LF_lowerleg();
        const Type_fr_LF_upperleg_X_fr_LF_lowerleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_hipassembly_X_fr_trunk : public TransformMotion<Type_fr_RF_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_RF_hipassembly_X_fr_trunk();
        const Type_fr_RF_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_hipassembly : public TransformMotion<Type_fr_trunk_X_fr_RF_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_RF_hipassembly();
        const Type_fr_trunk_X_fr_RF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_upperleg_X_fr_RF_hipassembly : public TransformMotion<Type_fr_RF_upperleg_X_fr_RF_hipassembly>
    {
    public:
        Type_fr_RF_upperleg_X_fr_RF_hipassembly();
        const Type_fr_RF_upperleg_X_fr_RF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_hipassembly_X_fr_RF_upperleg : public TransformMotion<Type_fr_RF_hipassembly_X_fr_RF_upperleg>
    {
    public:
        Type_fr_RF_hipassembly_X_fr_RF_upperleg();
        const Type_fr_RF_hipassembly_X_fr_RF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_lowerleg_X_fr_RF_upperleg : public TransformMotion<Type_fr_RF_lowerleg_X_fr_RF_upperleg>
    {
    public:
        Type_fr_RF_lowerleg_X_fr_RF_upperleg();
        const Type_fr_RF_lowerleg_X_fr_RF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_upperleg_X_fr_RF_lowerleg : public TransformMotion<Type_fr_RF_upperleg_X_fr_RF_lowerleg>
    {
    public:
        Type_fr_RF_upperleg_X_fr_RF_lowerleg();
        const Type_fr_RF_upperleg_X_fr_RF_lowerleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_hipassembly_X_fr_trunk : public TransformMotion<Type_fr_LH_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_LH_hipassembly_X_fr_trunk();
        const Type_fr_LH_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_hipassembly : public TransformMotion<Type_fr_trunk_X_fr_LH_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_LH_hipassembly();
        const Type_fr_trunk_X_fr_LH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_upperleg_X_fr_LH_hipassembly : public TransformMotion<Type_fr_LH_upperleg_X_fr_LH_hipassembly>
    {
    public:
        Type_fr_LH_upperleg_X_fr_LH_hipassembly();
        const Type_fr_LH_upperleg_X_fr_LH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_hipassembly_X_fr_LH_upperleg : public TransformMotion<Type_fr_LH_hipassembly_X_fr_LH_upperleg>
    {
    public:
        Type_fr_LH_hipassembly_X_fr_LH_upperleg();
        const Type_fr_LH_hipassembly_X_fr_LH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_lowerleg_X_fr_LH_upperleg : public TransformMotion<Type_fr_LH_lowerleg_X_fr_LH_upperleg>
    {
    public:
        Type_fr_LH_lowerleg_X_fr_LH_upperleg();
        const Type_fr_LH_lowerleg_X_fr_LH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_upperleg_X_fr_LH_lowerleg : public TransformMotion<Type_fr_LH_upperleg_X_fr_LH_lowerleg>
    {
    public:
        Type_fr_LH_upperleg_X_fr_LH_lowerleg();
        const Type_fr_LH_upperleg_X_fr_LH_lowerleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_hipassembly_X_fr_trunk : public TransformMotion<Type_fr_RH_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_RH_hipassembly_X_fr_trunk();
        const Type_fr_RH_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_hipassembly : public TransformMotion<Type_fr_trunk_X_fr_RH_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_RH_hipassembly();
        const Type_fr_trunk_X_fr_RH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_upperleg_X_fr_RH_hipassembly : public TransformMotion<Type_fr_RH_upperleg_X_fr_RH_hipassembly>
    {
    public:
        Type_fr_RH_upperleg_X_fr_RH_hipassembly();
        const Type_fr_RH_upperleg_X_fr_RH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_hipassembly_X_fr_RH_upperleg : public TransformMotion<Type_fr_RH_hipassembly_X_fr_RH_upperleg>
    {
    public:
        Type_fr_RH_hipassembly_X_fr_RH_upperleg();
        const Type_fr_RH_hipassembly_X_fr_RH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_lowerleg_X_fr_RH_upperleg : public TransformMotion<Type_fr_RH_lowerleg_X_fr_RH_upperleg>
    {
    public:
        Type_fr_RH_lowerleg_X_fr_RH_upperleg();
        const Type_fr_RH_lowerleg_X_fr_RH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_upperleg_X_fr_RH_lowerleg : public TransformMotion<Type_fr_RH_upperleg_X_fr_RH_lowerleg>
    {
    public:
        Type_fr_RH_upperleg_X_fr_RH_lowerleg();
        const Type_fr_RH_upperleg_X_fr_RH_lowerleg& update(const state_t&);
    protected:
    };
    
public:
    MotionTransforms(const dog::KinDynParams&);
    void updateParameters();
    Type_fr_LF_lowerleg_X_LF_foot fr_LF_lowerleg_X_LF_foot;
    Type_fr_RF_lowerleg_X_RF_foot fr_RF_lowerleg_X_RF_foot;
    Type_fr_LH_lowerleg_X_LH_foot fr_LH_lowerleg_X_LH_foot;
    Type_fr_RH_lowerleg_X_RH_foot fr_RH_lowerleg_X_RH_foot;
    Type_fr_trunk_X_LF_hipassemblyCOM fr_trunk_X_LF_hipassemblyCOM;
    Type_fr_trunk_X_RF_hipassemblyCOM fr_trunk_X_RF_hipassemblyCOM;
    Type_fr_trunk_X_LH_hipassemblyCOM fr_trunk_X_LH_hipassemblyCOM;
    Type_fr_trunk_X_RH_hipassemblyCOM fr_trunk_X_RH_hipassemblyCOM;
    Type_fr_trunk_X_LF_upperlegCOM fr_trunk_X_LF_upperlegCOM;
    Type_fr_trunk_X_RF_upperlegCOM fr_trunk_X_RF_upperlegCOM;
    Type_fr_trunk_X_LH_upperlegCOM fr_trunk_X_LH_upperlegCOM;
    Type_fr_trunk_X_RH_upperlegCOM fr_trunk_X_RH_upperlegCOM;
    Type_fr_trunk_X_LF_lowerlegCOM fr_trunk_X_LF_lowerlegCOM;
    Type_fr_trunk_X_RF_lowerlegCOM fr_trunk_X_RF_lowerlegCOM;
    Type_fr_trunk_X_LH_lowerlegCOM fr_trunk_X_LH_lowerlegCOM;
    Type_fr_trunk_X_RH_lowerlegCOM fr_trunk_X_RH_lowerlegCOM;
    Type_LF_foot_X_fr_LF_lowerleg LF_foot_X_fr_LF_lowerleg;
    Type_RF_foot_X_fr_RF_lowerleg RF_foot_X_fr_RF_lowerleg;
    Type_LH_foot_X_fr_LH_lowerleg LH_foot_X_fr_LH_lowerleg;
    Type_RH_foot_X_fr_RH_lowerleg RH_foot_X_fr_RH_lowerleg;
    Type_fr_trunk_X_LF_foot fr_trunk_X_LF_foot;
    Type_fr_trunk_X_RF_foot fr_trunk_X_RF_foot;
    Type_fr_trunk_X_LH_foot fr_trunk_X_LH_foot;
    Type_fr_trunk_X_RH_foot fr_trunk_X_RH_foot;
    Type_fr_LF_lowerleg_X_fr_trunk fr_LF_lowerleg_X_fr_trunk;
    Type_fr_RF_lowerleg_X_fr_trunk fr_RF_lowerleg_X_fr_trunk;
    Type_fr_LH_lowerleg_X_fr_trunk fr_LH_lowerleg_X_fr_trunk;
    Type_fr_RH_lowerleg_X_fr_trunk fr_RH_lowerleg_X_fr_trunk;
    Type_LF_foot_X_fr_trunk LF_foot_X_fr_trunk;
    Type_RF_foot_X_fr_trunk RF_foot_X_fr_trunk;
    Type_LH_foot_X_fr_trunk LH_foot_X_fr_trunk;
    Type_RH_foot_X_fr_trunk RH_foot_X_fr_trunk;
    Type_fr_trunk_X_LF_shin fr_trunk_X_LF_shin;
    Type_fr_trunk_X_RF_shin fr_trunk_X_RF_shin;
    Type_fr_trunk_X_LH_shin fr_trunk_X_LH_shin;
    Type_fr_trunk_X_RH_shin fr_trunk_X_RH_shin;
    Type_fr_trunk_X_fr_LF_HAA fr_trunk_X_fr_LF_HAA;
    Type_fr_trunk_X_fr_LF_HFE fr_trunk_X_fr_LF_HFE;
    Type_fr_trunk_X_fr_LF_KFE fr_trunk_X_fr_LF_KFE;
    Type_fr_trunk_X_fr_RF_HAA fr_trunk_X_fr_RF_HAA;
    Type_fr_trunk_X_fr_RF_HFE fr_trunk_X_fr_RF_HFE;
    Type_fr_trunk_X_fr_RF_KFE fr_trunk_X_fr_RF_KFE;
    Type_fr_trunk_X_fr_LH_HAA fr_trunk_X_fr_LH_HAA;
    Type_fr_trunk_X_fr_LH_HFE fr_trunk_X_fr_LH_HFE;
    Type_fr_trunk_X_fr_LH_KFE fr_trunk_X_fr_LH_KFE;
    Type_fr_trunk_X_fr_RH_HAA fr_trunk_X_fr_RH_HAA;
    Type_fr_trunk_X_fr_RH_HFE fr_trunk_X_fr_RH_HFE;
    Type_fr_trunk_X_fr_RH_KFE fr_trunk_X_fr_RH_KFE;
    Type_fr_LF_hipassembly_X_fr_trunk fr_LF_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_LF_hipassembly fr_trunk_X_fr_LF_hipassembly;
    Type_fr_LF_upperleg_X_fr_LF_hipassembly fr_LF_upperleg_X_fr_LF_hipassembly;
    Type_fr_LF_hipassembly_X_fr_LF_upperleg fr_LF_hipassembly_X_fr_LF_upperleg;
    Type_fr_LF_lowerleg_X_fr_LF_upperleg fr_LF_lowerleg_X_fr_LF_upperleg;
    Type_fr_LF_upperleg_X_fr_LF_lowerleg fr_LF_upperleg_X_fr_LF_lowerleg;
    Type_fr_RF_hipassembly_X_fr_trunk fr_RF_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_RF_hipassembly fr_trunk_X_fr_RF_hipassembly;
    Type_fr_RF_upperleg_X_fr_RF_hipassembly fr_RF_upperleg_X_fr_RF_hipassembly;
    Type_fr_RF_hipassembly_X_fr_RF_upperleg fr_RF_hipassembly_X_fr_RF_upperleg;
    Type_fr_RF_lowerleg_X_fr_RF_upperleg fr_RF_lowerleg_X_fr_RF_upperleg;
    Type_fr_RF_upperleg_X_fr_RF_lowerleg fr_RF_upperleg_X_fr_RF_lowerleg;
    Type_fr_LH_hipassembly_X_fr_trunk fr_LH_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_LH_hipassembly fr_trunk_X_fr_LH_hipassembly;
    Type_fr_LH_upperleg_X_fr_LH_hipassembly fr_LH_upperleg_X_fr_LH_hipassembly;
    Type_fr_LH_hipassembly_X_fr_LH_upperleg fr_LH_hipassembly_X_fr_LH_upperleg;
    Type_fr_LH_lowerleg_X_fr_LH_upperleg fr_LH_lowerleg_X_fr_LH_upperleg;
    Type_fr_LH_upperleg_X_fr_LH_lowerleg fr_LH_upperleg_X_fr_LH_lowerleg;
    Type_fr_RH_hipassembly_X_fr_trunk fr_RH_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_RH_hipassembly fr_trunk_X_fr_RH_hipassembly;
    Type_fr_RH_upperleg_X_fr_RH_hipassembly fr_RH_upperleg_X_fr_RH_hipassembly;
    Type_fr_RH_hipassembly_X_fr_RH_upperleg fr_RH_hipassembly_X_fr_RH_upperleg;
    Type_fr_RH_lowerleg_X_fr_RH_upperleg fr_RH_lowerleg_X_fr_RH_upperleg;
    Type_fr_RH_upperleg_X_fr_RH_lowerleg fr_RH_upperleg_X_fr_RH_lowerleg;

protected:
    Params_lengths values_lengths;

    const dog::KinDynParams* valuesGetter_lengths;
}; //class 'MotionTransforms'

/**
 * The class for the 6-by-6 coordinates transformation matrices for
 * spatial force vectors.
 */
class ForceTransforms : public dog::ForceTransformsBase {
public:
    class Dummy {};
public:
    dog::SpatialTransformPlain getTransform(const dog::JointState &q, const dog::OriginFrame &orig, const dog::DestFrame &dest);
    typedef TransformForce<Dummy>::MatrixType MatrixType;
public:
    class Type_fr_LF_lowerleg_X_LF_foot : public TransformForce<Type_fr_LF_lowerleg_X_LF_foot>
    {
    public:
        Type_fr_LF_lowerleg_X_LF_foot(const Params_lengths& _lengths_values);
        const Type_fr_LF_lowerleg_X_LF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_RF_lowerleg_X_RF_foot : public TransformForce<Type_fr_RF_lowerleg_X_RF_foot>
    {
    public:
        Type_fr_RF_lowerleg_X_RF_foot(const Params_lengths& _lengths_values);
        const Type_fr_RF_lowerleg_X_RF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_LH_lowerleg_X_LH_foot : public TransformForce<Type_fr_LH_lowerleg_X_LH_foot>
    {
    public:
        Type_fr_LH_lowerleg_X_LH_foot(const Params_lengths& _lengths_values);
        const Type_fr_LH_lowerleg_X_LH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_RH_lowerleg_X_RH_foot : public TransformForce<Type_fr_RH_lowerleg_X_RH_foot>
    {
    public:
        Type_fr_RH_lowerleg_X_RH_foot(const Params_lengths& _lengths_values);
        const Type_fr_RH_lowerleg_X_RH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LF_hipassemblyCOM : public TransformForce<Type_fr_trunk_X_LF_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_LF_hipassemblyCOM();
        const Type_fr_trunk_X_LF_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RF_hipassemblyCOM : public TransformForce<Type_fr_trunk_X_RF_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_RF_hipassemblyCOM();
        const Type_fr_trunk_X_RF_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LH_hipassemblyCOM : public TransformForce<Type_fr_trunk_X_LH_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_LH_hipassemblyCOM();
        const Type_fr_trunk_X_LH_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RH_hipassemblyCOM : public TransformForce<Type_fr_trunk_X_RH_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_RH_hipassemblyCOM();
        const Type_fr_trunk_X_RH_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LF_upperlegCOM : public TransformForce<Type_fr_trunk_X_LF_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_LF_upperlegCOM();
        const Type_fr_trunk_X_LF_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RF_upperlegCOM : public TransformForce<Type_fr_trunk_X_RF_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_RF_upperlegCOM();
        const Type_fr_trunk_X_RF_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LH_upperlegCOM : public TransformForce<Type_fr_trunk_X_LH_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_LH_upperlegCOM();
        const Type_fr_trunk_X_LH_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RH_upperlegCOM : public TransformForce<Type_fr_trunk_X_RH_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_RH_upperlegCOM();
        const Type_fr_trunk_X_RH_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LF_lowerlegCOM : public TransformForce<Type_fr_trunk_X_LF_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_LF_lowerlegCOM();
        const Type_fr_trunk_X_LF_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RF_lowerlegCOM : public TransformForce<Type_fr_trunk_X_RF_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_RF_lowerlegCOM();
        const Type_fr_trunk_X_RF_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LH_lowerlegCOM : public TransformForce<Type_fr_trunk_X_LH_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_LH_lowerlegCOM();
        const Type_fr_trunk_X_LH_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RH_lowerlegCOM : public TransformForce<Type_fr_trunk_X_RH_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_RH_lowerlegCOM();
        const Type_fr_trunk_X_RH_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_LF_foot_X_fr_LF_lowerleg : public TransformForce<Type_LF_foot_X_fr_LF_lowerleg>
    {
    public:
        Type_LF_foot_X_fr_LF_lowerleg(const Params_lengths& _lengths_values);
        const Type_LF_foot_X_fr_LF_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RF_foot_X_fr_RF_lowerleg : public TransformForce<Type_RF_foot_X_fr_RF_lowerleg>
    {
    public:
        Type_RF_foot_X_fr_RF_lowerleg(const Params_lengths& _lengths_values);
        const Type_RF_foot_X_fr_RF_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_LH_foot_X_fr_LH_lowerleg : public TransformForce<Type_LH_foot_X_fr_LH_lowerleg>
    {
    public:
        Type_LH_foot_X_fr_LH_lowerleg(const Params_lengths& _lengths_values);
        const Type_LH_foot_X_fr_LH_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RH_foot_X_fr_RH_lowerleg : public TransformForce<Type_RH_foot_X_fr_RH_lowerleg>
    {
    public:
        Type_RH_foot_X_fr_RH_lowerleg(const Params_lengths& _lengths_values);
        const Type_RH_foot_X_fr_RH_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LF_foot : public TransformForce<Type_fr_trunk_X_LF_foot>
    {
    public:
        Type_fr_trunk_X_LF_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RF_foot : public TransformForce<Type_fr_trunk_X_RF_foot>
    {
    public:
        Type_fr_trunk_X_RF_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LH_foot : public TransformForce<Type_fr_trunk_X_LH_foot>
    {
    public:
        Type_fr_trunk_X_LH_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RH_foot : public TransformForce<Type_fr_trunk_X_RH_foot>
    {
    public:
        Type_fr_trunk_X_RH_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_LF_lowerleg_X_fr_trunk : public TransformForce<Type_fr_LF_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_LF_lowerleg_X_fr_trunk();
        const Type_fr_LF_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_lowerleg_X_fr_trunk : public TransformForce<Type_fr_RF_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_RF_lowerleg_X_fr_trunk();
        const Type_fr_RF_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_lowerleg_X_fr_trunk : public TransformForce<Type_fr_LH_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_LH_lowerleg_X_fr_trunk();
        const Type_fr_LH_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_lowerleg_X_fr_trunk : public TransformForce<Type_fr_RH_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_RH_lowerleg_X_fr_trunk();
        const Type_fr_RH_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_LF_foot_X_fr_trunk : public TransformForce<Type_LF_foot_X_fr_trunk>
    {
    public:
        Type_LF_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_LF_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RF_foot_X_fr_trunk : public TransformForce<Type_RF_foot_X_fr_trunk>
    {
    public:
        Type_RF_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_RF_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_LH_foot_X_fr_trunk : public TransformForce<Type_LH_foot_X_fr_trunk>
    {
    public:
        Type_LH_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_LH_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RH_foot_X_fr_trunk : public TransformForce<Type_RH_foot_X_fr_trunk>
    {
    public:
        Type_RH_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_RH_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LF_shin : public TransformForce<Type_fr_trunk_X_LF_shin>
    {
    public:
        Type_fr_trunk_X_LF_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LF_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RF_shin : public TransformForce<Type_fr_trunk_X_RF_shin>
    {
    public:
        Type_fr_trunk_X_RF_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RF_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LH_shin : public TransformForce<Type_fr_trunk_X_LH_shin>
    {
    public:
        Type_fr_trunk_X_LH_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LH_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RH_shin : public TransformForce<Type_fr_trunk_X_RH_shin>
    {
    public:
        Type_fr_trunk_X_RH_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RH_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_fr_LF_HAA : public TransformForce<Type_fr_trunk_X_fr_LF_HAA>
    {
    public:
        Type_fr_trunk_X_fr_LF_HAA();
        const Type_fr_trunk_X_fr_LF_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LF_HFE : public TransformForce<Type_fr_trunk_X_fr_LF_HFE>
    {
    public:
        Type_fr_trunk_X_fr_LF_HFE();
        const Type_fr_trunk_X_fr_LF_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LF_KFE : public TransformForce<Type_fr_trunk_X_fr_LF_KFE>
    {
    public:
        Type_fr_trunk_X_fr_LF_KFE();
        const Type_fr_trunk_X_fr_LF_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_HAA : public TransformForce<Type_fr_trunk_X_fr_RF_HAA>
    {
    public:
        Type_fr_trunk_X_fr_RF_HAA();
        const Type_fr_trunk_X_fr_RF_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_HFE : public TransformForce<Type_fr_trunk_X_fr_RF_HFE>
    {
    public:
        Type_fr_trunk_X_fr_RF_HFE();
        const Type_fr_trunk_X_fr_RF_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_KFE : public TransformForce<Type_fr_trunk_X_fr_RF_KFE>
    {
    public:
        Type_fr_trunk_X_fr_RF_KFE();
        const Type_fr_trunk_X_fr_RF_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_HAA : public TransformForce<Type_fr_trunk_X_fr_LH_HAA>
    {
    public:
        Type_fr_trunk_X_fr_LH_HAA();
        const Type_fr_trunk_X_fr_LH_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_HFE : public TransformForce<Type_fr_trunk_X_fr_LH_HFE>
    {
    public:
        Type_fr_trunk_X_fr_LH_HFE();
        const Type_fr_trunk_X_fr_LH_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_KFE : public TransformForce<Type_fr_trunk_X_fr_LH_KFE>
    {
    public:
        Type_fr_trunk_X_fr_LH_KFE();
        const Type_fr_trunk_X_fr_LH_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_HAA : public TransformForce<Type_fr_trunk_X_fr_RH_HAA>
    {
    public:
        Type_fr_trunk_X_fr_RH_HAA();
        const Type_fr_trunk_X_fr_RH_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_HFE : public TransformForce<Type_fr_trunk_X_fr_RH_HFE>
    {
    public:
        Type_fr_trunk_X_fr_RH_HFE();
        const Type_fr_trunk_X_fr_RH_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_KFE : public TransformForce<Type_fr_trunk_X_fr_RH_KFE>
    {
    public:
        Type_fr_trunk_X_fr_RH_KFE();
        const Type_fr_trunk_X_fr_RH_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_hipassembly_X_fr_trunk : public TransformForce<Type_fr_LF_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_LF_hipassembly_X_fr_trunk();
        const Type_fr_LF_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LF_hipassembly : public TransformForce<Type_fr_trunk_X_fr_LF_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_LF_hipassembly();
        const Type_fr_trunk_X_fr_LF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_upperleg_X_fr_LF_hipassembly : public TransformForce<Type_fr_LF_upperleg_X_fr_LF_hipassembly>
    {
    public:
        Type_fr_LF_upperleg_X_fr_LF_hipassembly();
        const Type_fr_LF_upperleg_X_fr_LF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_hipassembly_X_fr_LF_upperleg : public TransformForce<Type_fr_LF_hipassembly_X_fr_LF_upperleg>
    {
    public:
        Type_fr_LF_hipassembly_X_fr_LF_upperleg();
        const Type_fr_LF_hipassembly_X_fr_LF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_lowerleg_X_fr_LF_upperleg : public TransformForce<Type_fr_LF_lowerleg_X_fr_LF_upperleg>
    {
    public:
        Type_fr_LF_lowerleg_X_fr_LF_upperleg();
        const Type_fr_LF_lowerleg_X_fr_LF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_upperleg_X_fr_LF_lowerleg : public TransformForce<Type_fr_LF_upperleg_X_fr_LF_lowerleg>
    {
    public:
        Type_fr_LF_upperleg_X_fr_LF_lowerleg();
        const Type_fr_LF_upperleg_X_fr_LF_lowerleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_hipassembly_X_fr_trunk : public TransformForce<Type_fr_RF_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_RF_hipassembly_X_fr_trunk();
        const Type_fr_RF_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_hipassembly : public TransformForce<Type_fr_trunk_X_fr_RF_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_RF_hipassembly();
        const Type_fr_trunk_X_fr_RF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_upperleg_X_fr_RF_hipassembly : public TransformForce<Type_fr_RF_upperleg_X_fr_RF_hipassembly>
    {
    public:
        Type_fr_RF_upperleg_X_fr_RF_hipassembly();
        const Type_fr_RF_upperleg_X_fr_RF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_hipassembly_X_fr_RF_upperleg : public TransformForce<Type_fr_RF_hipassembly_X_fr_RF_upperleg>
    {
    public:
        Type_fr_RF_hipassembly_X_fr_RF_upperleg();
        const Type_fr_RF_hipassembly_X_fr_RF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_lowerleg_X_fr_RF_upperleg : public TransformForce<Type_fr_RF_lowerleg_X_fr_RF_upperleg>
    {
    public:
        Type_fr_RF_lowerleg_X_fr_RF_upperleg();
        const Type_fr_RF_lowerleg_X_fr_RF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_upperleg_X_fr_RF_lowerleg : public TransformForce<Type_fr_RF_upperleg_X_fr_RF_lowerleg>
    {
    public:
        Type_fr_RF_upperleg_X_fr_RF_lowerleg();
        const Type_fr_RF_upperleg_X_fr_RF_lowerleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_hipassembly_X_fr_trunk : public TransformForce<Type_fr_LH_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_LH_hipassembly_X_fr_trunk();
        const Type_fr_LH_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_hipassembly : public TransformForce<Type_fr_trunk_X_fr_LH_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_LH_hipassembly();
        const Type_fr_trunk_X_fr_LH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_upperleg_X_fr_LH_hipassembly : public TransformForce<Type_fr_LH_upperleg_X_fr_LH_hipassembly>
    {
    public:
        Type_fr_LH_upperleg_X_fr_LH_hipassembly();
        const Type_fr_LH_upperleg_X_fr_LH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_hipassembly_X_fr_LH_upperleg : public TransformForce<Type_fr_LH_hipassembly_X_fr_LH_upperleg>
    {
    public:
        Type_fr_LH_hipassembly_X_fr_LH_upperleg();
        const Type_fr_LH_hipassembly_X_fr_LH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_lowerleg_X_fr_LH_upperleg : public TransformForce<Type_fr_LH_lowerleg_X_fr_LH_upperleg>
    {
    public:
        Type_fr_LH_lowerleg_X_fr_LH_upperleg();
        const Type_fr_LH_lowerleg_X_fr_LH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_upperleg_X_fr_LH_lowerleg : public TransformForce<Type_fr_LH_upperleg_X_fr_LH_lowerleg>
    {
    public:
        Type_fr_LH_upperleg_X_fr_LH_lowerleg();
        const Type_fr_LH_upperleg_X_fr_LH_lowerleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_hipassembly_X_fr_trunk : public TransformForce<Type_fr_RH_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_RH_hipassembly_X_fr_trunk();
        const Type_fr_RH_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_hipassembly : public TransformForce<Type_fr_trunk_X_fr_RH_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_RH_hipassembly();
        const Type_fr_trunk_X_fr_RH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_upperleg_X_fr_RH_hipassembly : public TransformForce<Type_fr_RH_upperleg_X_fr_RH_hipassembly>
    {
    public:
        Type_fr_RH_upperleg_X_fr_RH_hipassembly();
        const Type_fr_RH_upperleg_X_fr_RH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_hipassembly_X_fr_RH_upperleg : public TransformForce<Type_fr_RH_hipassembly_X_fr_RH_upperleg>
    {
    public:
        Type_fr_RH_hipassembly_X_fr_RH_upperleg();
        const Type_fr_RH_hipassembly_X_fr_RH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_lowerleg_X_fr_RH_upperleg : public TransformForce<Type_fr_RH_lowerleg_X_fr_RH_upperleg>
    {
    public:
        Type_fr_RH_lowerleg_X_fr_RH_upperleg();
        const Type_fr_RH_lowerleg_X_fr_RH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_upperleg_X_fr_RH_lowerleg : public TransformForce<Type_fr_RH_upperleg_X_fr_RH_lowerleg>
    {
    public:
        Type_fr_RH_upperleg_X_fr_RH_lowerleg();
        const Type_fr_RH_upperleg_X_fr_RH_lowerleg& update(const state_t&);
    protected:
    };
    
public:
    ForceTransforms(const dog::KinDynParams&);
    void updateParameters();
    Type_fr_LF_lowerleg_X_LF_foot fr_LF_lowerleg_X_LF_foot;
    Type_fr_RF_lowerleg_X_RF_foot fr_RF_lowerleg_X_RF_foot;
    Type_fr_LH_lowerleg_X_LH_foot fr_LH_lowerleg_X_LH_foot;
    Type_fr_RH_lowerleg_X_RH_foot fr_RH_lowerleg_X_RH_foot;
    Type_fr_trunk_X_LF_hipassemblyCOM fr_trunk_X_LF_hipassemblyCOM;
    Type_fr_trunk_X_RF_hipassemblyCOM fr_trunk_X_RF_hipassemblyCOM;
    Type_fr_trunk_X_LH_hipassemblyCOM fr_trunk_X_LH_hipassemblyCOM;
    Type_fr_trunk_X_RH_hipassemblyCOM fr_trunk_X_RH_hipassemblyCOM;
    Type_fr_trunk_X_LF_upperlegCOM fr_trunk_X_LF_upperlegCOM;
    Type_fr_trunk_X_RF_upperlegCOM fr_trunk_X_RF_upperlegCOM;
    Type_fr_trunk_X_LH_upperlegCOM fr_trunk_X_LH_upperlegCOM;
    Type_fr_trunk_X_RH_upperlegCOM fr_trunk_X_RH_upperlegCOM;
    Type_fr_trunk_X_LF_lowerlegCOM fr_trunk_X_LF_lowerlegCOM;
    Type_fr_trunk_X_RF_lowerlegCOM fr_trunk_X_RF_lowerlegCOM;
    Type_fr_trunk_X_LH_lowerlegCOM fr_trunk_X_LH_lowerlegCOM;
    Type_fr_trunk_X_RH_lowerlegCOM fr_trunk_X_RH_lowerlegCOM;
    Type_LF_foot_X_fr_LF_lowerleg LF_foot_X_fr_LF_lowerleg;
    Type_RF_foot_X_fr_RF_lowerleg RF_foot_X_fr_RF_lowerleg;
    Type_LH_foot_X_fr_LH_lowerleg LH_foot_X_fr_LH_lowerleg;
    Type_RH_foot_X_fr_RH_lowerleg RH_foot_X_fr_RH_lowerleg;
    Type_fr_trunk_X_LF_foot fr_trunk_X_LF_foot;
    Type_fr_trunk_X_RF_foot fr_trunk_X_RF_foot;
    Type_fr_trunk_X_LH_foot fr_trunk_X_LH_foot;
    Type_fr_trunk_X_RH_foot fr_trunk_X_RH_foot;
    Type_fr_LF_lowerleg_X_fr_trunk fr_LF_lowerleg_X_fr_trunk;
    Type_fr_RF_lowerleg_X_fr_trunk fr_RF_lowerleg_X_fr_trunk;
    Type_fr_LH_lowerleg_X_fr_trunk fr_LH_lowerleg_X_fr_trunk;
    Type_fr_RH_lowerleg_X_fr_trunk fr_RH_lowerleg_X_fr_trunk;
    Type_LF_foot_X_fr_trunk LF_foot_X_fr_trunk;
    Type_RF_foot_X_fr_trunk RF_foot_X_fr_trunk;
    Type_LH_foot_X_fr_trunk LH_foot_X_fr_trunk;
    Type_RH_foot_X_fr_trunk RH_foot_X_fr_trunk;
    Type_fr_trunk_X_LF_shin fr_trunk_X_LF_shin;
    Type_fr_trunk_X_RF_shin fr_trunk_X_RF_shin;
    Type_fr_trunk_X_LH_shin fr_trunk_X_LH_shin;
    Type_fr_trunk_X_RH_shin fr_trunk_X_RH_shin;
    Type_fr_trunk_X_fr_LF_HAA fr_trunk_X_fr_LF_HAA;
    Type_fr_trunk_X_fr_LF_HFE fr_trunk_X_fr_LF_HFE;
    Type_fr_trunk_X_fr_LF_KFE fr_trunk_X_fr_LF_KFE;
    Type_fr_trunk_X_fr_RF_HAA fr_trunk_X_fr_RF_HAA;
    Type_fr_trunk_X_fr_RF_HFE fr_trunk_X_fr_RF_HFE;
    Type_fr_trunk_X_fr_RF_KFE fr_trunk_X_fr_RF_KFE;
    Type_fr_trunk_X_fr_LH_HAA fr_trunk_X_fr_LH_HAA;
    Type_fr_trunk_X_fr_LH_HFE fr_trunk_X_fr_LH_HFE;
    Type_fr_trunk_X_fr_LH_KFE fr_trunk_X_fr_LH_KFE;
    Type_fr_trunk_X_fr_RH_HAA fr_trunk_X_fr_RH_HAA;
    Type_fr_trunk_X_fr_RH_HFE fr_trunk_X_fr_RH_HFE;
    Type_fr_trunk_X_fr_RH_KFE fr_trunk_X_fr_RH_KFE;
    Type_fr_LF_hipassembly_X_fr_trunk fr_LF_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_LF_hipassembly fr_trunk_X_fr_LF_hipassembly;
    Type_fr_LF_upperleg_X_fr_LF_hipassembly fr_LF_upperleg_X_fr_LF_hipassembly;
    Type_fr_LF_hipassembly_X_fr_LF_upperleg fr_LF_hipassembly_X_fr_LF_upperleg;
    Type_fr_LF_lowerleg_X_fr_LF_upperleg fr_LF_lowerleg_X_fr_LF_upperleg;
    Type_fr_LF_upperleg_X_fr_LF_lowerleg fr_LF_upperleg_X_fr_LF_lowerleg;
    Type_fr_RF_hipassembly_X_fr_trunk fr_RF_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_RF_hipassembly fr_trunk_X_fr_RF_hipassembly;
    Type_fr_RF_upperleg_X_fr_RF_hipassembly fr_RF_upperleg_X_fr_RF_hipassembly;
    Type_fr_RF_hipassembly_X_fr_RF_upperleg fr_RF_hipassembly_X_fr_RF_upperleg;
    Type_fr_RF_lowerleg_X_fr_RF_upperleg fr_RF_lowerleg_X_fr_RF_upperleg;
    Type_fr_RF_upperleg_X_fr_RF_lowerleg fr_RF_upperleg_X_fr_RF_lowerleg;
    Type_fr_LH_hipassembly_X_fr_trunk fr_LH_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_LH_hipassembly fr_trunk_X_fr_LH_hipassembly;
    Type_fr_LH_upperleg_X_fr_LH_hipassembly fr_LH_upperleg_X_fr_LH_hipassembly;
    Type_fr_LH_hipassembly_X_fr_LH_upperleg fr_LH_hipassembly_X_fr_LH_upperleg;
    Type_fr_LH_lowerleg_X_fr_LH_upperleg fr_LH_lowerleg_X_fr_LH_upperleg;
    Type_fr_LH_upperleg_X_fr_LH_lowerleg fr_LH_upperleg_X_fr_LH_lowerleg;
    Type_fr_RH_hipassembly_X_fr_trunk fr_RH_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_RH_hipassembly fr_trunk_X_fr_RH_hipassembly;
    Type_fr_RH_upperleg_X_fr_RH_hipassembly fr_RH_upperleg_X_fr_RH_hipassembly;
    Type_fr_RH_hipassembly_X_fr_RH_upperleg fr_RH_hipassembly_X_fr_RH_upperleg;
    Type_fr_RH_lowerleg_X_fr_RH_upperleg fr_RH_lowerleg_X_fr_RH_upperleg;
    Type_fr_RH_upperleg_X_fr_RH_lowerleg fr_RH_upperleg_X_fr_RH_lowerleg;

protected:
    Params_lengths values_lengths;

    const dog::KinDynParams* valuesGetter_lengths;
}; //class 'ForceTransforms'

/**
 * The class with the homogeneous (4x4) coordinates transformation
 * matrices.
 */
class HomogeneousTransforms : public dog::HomogeneousTransformsBase {
public:
    class Dummy {};
public:
    dog::HomogeneousTransformPlain getTransform(const dog::JointState &q, const dog::OriginFrame &orig, const dog::DestFrame &dest);
    typedef TransformHomogeneous<Dummy>::MatrixType MatrixType;
public:
    class Type_fr_LF_lowerleg_X_LF_foot : public TransformHomogeneous<Type_fr_LF_lowerleg_X_LF_foot>
    {
    public:
        Type_fr_LF_lowerleg_X_LF_foot(const Params_lengths& _lengths_values);
        const Type_fr_LF_lowerleg_X_LF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_RF_lowerleg_X_RF_foot : public TransformHomogeneous<Type_fr_RF_lowerleg_X_RF_foot>
    {
    public:
        Type_fr_RF_lowerleg_X_RF_foot(const Params_lengths& _lengths_values);
        const Type_fr_RF_lowerleg_X_RF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_LH_lowerleg_X_LH_foot : public TransformHomogeneous<Type_fr_LH_lowerleg_X_LH_foot>
    {
    public:
        Type_fr_LH_lowerleg_X_LH_foot(const Params_lengths& _lengths_values);
        const Type_fr_LH_lowerleg_X_LH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_RH_lowerleg_X_RH_foot : public TransformHomogeneous<Type_fr_RH_lowerleg_X_RH_foot>
    {
    public:
        Type_fr_RH_lowerleg_X_RH_foot(const Params_lengths& _lengths_values);
        const Type_fr_RH_lowerleg_X_RH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LF_hipassemblyCOM : public TransformHomogeneous<Type_fr_trunk_X_LF_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_LF_hipassemblyCOM();
        const Type_fr_trunk_X_LF_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RF_hipassemblyCOM : public TransformHomogeneous<Type_fr_trunk_X_RF_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_RF_hipassemblyCOM();
        const Type_fr_trunk_X_RF_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LH_hipassemblyCOM : public TransformHomogeneous<Type_fr_trunk_X_LH_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_LH_hipassemblyCOM();
        const Type_fr_trunk_X_LH_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RH_hipassemblyCOM : public TransformHomogeneous<Type_fr_trunk_X_RH_hipassemblyCOM>
    {
    public:
        Type_fr_trunk_X_RH_hipassemblyCOM();
        const Type_fr_trunk_X_RH_hipassemblyCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LF_upperlegCOM : public TransformHomogeneous<Type_fr_trunk_X_LF_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_LF_upperlegCOM();
        const Type_fr_trunk_X_LF_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RF_upperlegCOM : public TransformHomogeneous<Type_fr_trunk_X_RF_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_RF_upperlegCOM();
        const Type_fr_trunk_X_RF_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LH_upperlegCOM : public TransformHomogeneous<Type_fr_trunk_X_LH_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_LH_upperlegCOM();
        const Type_fr_trunk_X_LH_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RH_upperlegCOM : public TransformHomogeneous<Type_fr_trunk_X_RH_upperlegCOM>
    {
    public:
        Type_fr_trunk_X_RH_upperlegCOM();
        const Type_fr_trunk_X_RH_upperlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LF_lowerlegCOM : public TransformHomogeneous<Type_fr_trunk_X_LF_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_LF_lowerlegCOM();
        const Type_fr_trunk_X_LF_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RF_lowerlegCOM : public TransformHomogeneous<Type_fr_trunk_X_RF_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_RF_lowerlegCOM();
        const Type_fr_trunk_X_RF_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_LH_lowerlegCOM : public TransformHomogeneous<Type_fr_trunk_X_LH_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_LH_lowerlegCOM();
        const Type_fr_trunk_X_LH_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_RH_lowerlegCOM : public TransformHomogeneous<Type_fr_trunk_X_RH_lowerlegCOM>
    {
    public:
        Type_fr_trunk_X_RH_lowerlegCOM();
        const Type_fr_trunk_X_RH_lowerlegCOM& update(const state_t&);
    protected:
    };
    
    class Type_LF_foot_X_fr_LF_lowerleg : public TransformHomogeneous<Type_LF_foot_X_fr_LF_lowerleg>
    {
    public:
        Type_LF_foot_X_fr_LF_lowerleg(const Params_lengths& _lengths_values);
        const Type_LF_foot_X_fr_LF_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RF_foot_X_fr_RF_lowerleg : public TransformHomogeneous<Type_RF_foot_X_fr_RF_lowerleg>
    {
    public:
        Type_RF_foot_X_fr_RF_lowerleg(const Params_lengths& _lengths_values);
        const Type_RF_foot_X_fr_RF_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_LH_foot_X_fr_LH_lowerleg : public TransformHomogeneous<Type_LH_foot_X_fr_LH_lowerleg>
    {
    public:
        Type_LH_foot_X_fr_LH_lowerleg(const Params_lengths& _lengths_values);
        const Type_LH_foot_X_fr_LH_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RH_foot_X_fr_RH_lowerleg : public TransformHomogeneous<Type_RH_foot_X_fr_RH_lowerleg>
    {
    public:
        Type_RH_foot_X_fr_RH_lowerleg(const Params_lengths& _lengths_values);
        const Type_RH_foot_X_fr_RH_lowerleg& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LF_foot : public TransformHomogeneous<Type_fr_trunk_X_LF_foot>
    {
    public:
        Type_fr_trunk_X_LF_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RF_foot : public TransformHomogeneous<Type_fr_trunk_X_RF_foot>
    {
    public:
        Type_fr_trunk_X_RF_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RF_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LH_foot : public TransformHomogeneous<Type_fr_trunk_X_LH_foot>
    {
    public:
        Type_fr_trunk_X_LH_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RH_foot : public TransformHomogeneous<Type_fr_trunk_X_RH_foot>
    {
    public:
        Type_fr_trunk_X_RH_foot(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RH_foot& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_LF_lowerleg_X_fr_trunk : public TransformHomogeneous<Type_fr_LF_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_LF_lowerleg_X_fr_trunk();
        const Type_fr_LF_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_lowerleg_X_fr_trunk : public TransformHomogeneous<Type_fr_RF_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_RF_lowerleg_X_fr_trunk();
        const Type_fr_RF_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_lowerleg_X_fr_trunk : public TransformHomogeneous<Type_fr_LH_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_LH_lowerleg_X_fr_trunk();
        const Type_fr_LH_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_lowerleg_X_fr_trunk : public TransformHomogeneous<Type_fr_RH_lowerleg_X_fr_trunk>
    {
    public:
        Type_fr_RH_lowerleg_X_fr_trunk();
        const Type_fr_RH_lowerleg_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_LF_foot_X_fr_trunk : public TransformHomogeneous<Type_LF_foot_X_fr_trunk>
    {
    public:
        Type_LF_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_LF_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RF_foot_X_fr_trunk : public TransformHomogeneous<Type_RF_foot_X_fr_trunk>
    {
    public:
        Type_RF_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_RF_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_LH_foot_X_fr_trunk : public TransformHomogeneous<Type_LH_foot_X_fr_trunk>
    {
    public:
        Type_LH_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_LH_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_RH_foot_X_fr_trunk : public TransformHomogeneous<Type_RH_foot_X_fr_trunk>
    {
    public:
        Type_RH_foot_X_fr_trunk(const Params_lengths& _lengths_values);
        const Type_RH_foot_X_fr_trunk& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LF_shin : public TransformHomogeneous<Type_fr_trunk_X_LF_shin>
    {
    public:
        Type_fr_trunk_X_LF_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LF_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RF_shin : public TransformHomogeneous<Type_fr_trunk_X_RF_shin>
    {
    public:
        Type_fr_trunk_X_RF_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RF_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_LH_shin : public TransformHomogeneous<Type_fr_trunk_X_LH_shin>
    {
    public:
        Type_fr_trunk_X_LH_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_LH_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_RH_shin : public TransformHomogeneous<Type_fr_trunk_X_RH_shin>
    {
    public:
        Type_fr_trunk_X_RH_shin(const Params_lengths& _lengths_values);
        const Type_fr_trunk_X_RH_shin& update(const state_t&);
    protected:
        const Params_lengths* lengths_values;
    };
    
    class Type_fr_trunk_X_fr_LF_HAA : public TransformHomogeneous<Type_fr_trunk_X_fr_LF_HAA>
    {
    public:
        Type_fr_trunk_X_fr_LF_HAA();
        const Type_fr_trunk_X_fr_LF_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LF_HFE : public TransformHomogeneous<Type_fr_trunk_X_fr_LF_HFE>
    {
    public:
        Type_fr_trunk_X_fr_LF_HFE();
        const Type_fr_trunk_X_fr_LF_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LF_KFE : public TransformHomogeneous<Type_fr_trunk_X_fr_LF_KFE>
    {
    public:
        Type_fr_trunk_X_fr_LF_KFE();
        const Type_fr_trunk_X_fr_LF_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_HAA : public TransformHomogeneous<Type_fr_trunk_X_fr_RF_HAA>
    {
    public:
        Type_fr_trunk_X_fr_RF_HAA();
        const Type_fr_trunk_X_fr_RF_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_HFE : public TransformHomogeneous<Type_fr_trunk_X_fr_RF_HFE>
    {
    public:
        Type_fr_trunk_X_fr_RF_HFE();
        const Type_fr_trunk_X_fr_RF_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_KFE : public TransformHomogeneous<Type_fr_trunk_X_fr_RF_KFE>
    {
    public:
        Type_fr_trunk_X_fr_RF_KFE();
        const Type_fr_trunk_X_fr_RF_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_HAA : public TransformHomogeneous<Type_fr_trunk_X_fr_LH_HAA>
    {
    public:
        Type_fr_trunk_X_fr_LH_HAA();
        const Type_fr_trunk_X_fr_LH_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_HFE : public TransformHomogeneous<Type_fr_trunk_X_fr_LH_HFE>
    {
    public:
        Type_fr_trunk_X_fr_LH_HFE();
        const Type_fr_trunk_X_fr_LH_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_KFE : public TransformHomogeneous<Type_fr_trunk_X_fr_LH_KFE>
    {
    public:
        Type_fr_trunk_X_fr_LH_KFE();
        const Type_fr_trunk_X_fr_LH_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_HAA : public TransformHomogeneous<Type_fr_trunk_X_fr_RH_HAA>
    {
    public:
        Type_fr_trunk_X_fr_RH_HAA();
        const Type_fr_trunk_X_fr_RH_HAA& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_HFE : public TransformHomogeneous<Type_fr_trunk_X_fr_RH_HFE>
    {
    public:
        Type_fr_trunk_X_fr_RH_HFE();
        const Type_fr_trunk_X_fr_RH_HFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_KFE : public TransformHomogeneous<Type_fr_trunk_X_fr_RH_KFE>
    {
    public:
        Type_fr_trunk_X_fr_RH_KFE();
        const Type_fr_trunk_X_fr_RH_KFE& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_hipassembly_X_fr_trunk : public TransformHomogeneous<Type_fr_LF_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_LF_hipassembly_X_fr_trunk();
        const Type_fr_LF_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LF_hipassembly : public TransformHomogeneous<Type_fr_trunk_X_fr_LF_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_LF_hipassembly();
        const Type_fr_trunk_X_fr_LF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_upperleg_X_fr_LF_hipassembly : public TransformHomogeneous<Type_fr_LF_upperleg_X_fr_LF_hipassembly>
    {
    public:
        Type_fr_LF_upperleg_X_fr_LF_hipassembly();
        const Type_fr_LF_upperleg_X_fr_LF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_hipassembly_X_fr_LF_upperleg : public TransformHomogeneous<Type_fr_LF_hipassembly_X_fr_LF_upperleg>
    {
    public:
        Type_fr_LF_hipassembly_X_fr_LF_upperleg();
        const Type_fr_LF_hipassembly_X_fr_LF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_lowerleg_X_fr_LF_upperleg : public TransformHomogeneous<Type_fr_LF_lowerleg_X_fr_LF_upperleg>
    {
    public:
        Type_fr_LF_lowerleg_X_fr_LF_upperleg();
        const Type_fr_LF_lowerleg_X_fr_LF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LF_upperleg_X_fr_LF_lowerleg : public TransformHomogeneous<Type_fr_LF_upperleg_X_fr_LF_lowerleg>
    {
    public:
        Type_fr_LF_upperleg_X_fr_LF_lowerleg();
        const Type_fr_LF_upperleg_X_fr_LF_lowerleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_hipassembly_X_fr_trunk : public TransformHomogeneous<Type_fr_RF_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_RF_hipassembly_X_fr_trunk();
        const Type_fr_RF_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RF_hipassembly : public TransformHomogeneous<Type_fr_trunk_X_fr_RF_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_RF_hipassembly();
        const Type_fr_trunk_X_fr_RF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_upperleg_X_fr_RF_hipassembly : public TransformHomogeneous<Type_fr_RF_upperleg_X_fr_RF_hipassembly>
    {
    public:
        Type_fr_RF_upperleg_X_fr_RF_hipassembly();
        const Type_fr_RF_upperleg_X_fr_RF_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_hipassembly_X_fr_RF_upperleg : public TransformHomogeneous<Type_fr_RF_hipassembly_X_fr_RF_upperleg>
    {
    public:
        Type_fr_RF_hipassembly_X_fr_RF_upperleg();
        const Type_fr_RF_hipassembly_X_fr_RF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_lowerleg_X_fr_RF_upperleg : public TransformHomogeneous<Type_fr_RF_lowerleg_X_fr_RF_upperleg>
    {
    public:
        Type_fr_RF_lowerleg_X_fr_RF_upperleg();
        const Type_fr_RF_lowerleg_X_fr_RF_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RF_upperleg_X_fr_RF_lowerleg : public TransformHomogeneous<Type_fr_RF_upperleg_X_fr_RF_lowerleg>
    {
    public:
        Type_fr_RF_upperleg_X_fr_RF_lowerleg();
        const Type_fr_RF_upperleg_X_fr_RF_lowerleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_hipassembly_X_fr_trunk : public TransformHomogeneous<Type_fr_LH_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_LH_hipassembly_X_fr_trunk();
        const Type_fr_LH_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_LH_hipassembly : public TransformHomogeneous<Type_fr_trunk_X_fr_LH_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_LH_hipassembly();
        const Type_fr_trunk_X_fr_LH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_upperleg_X_fr_LH_hipassembly : public TransformHomogeneous<Type_fr_LH_upperleg_X_fr_LH_hipassembly>
    {
    public:
        Type_fr_LH_upperleg_X_fr_LH_hipassembly();
        const Type_fr_LH_upperleg_X_fr_LH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_hipassembly_X_fr_LH_upperleg : public TransformHomogeneous<Type_fr_LH_hipassembly_X_fr_LH_upperleg>
    {
    public:
        Type_fr_LH_hipassembly_X_fr_LH_upperleg();
        const Type_fr_LH_hipassembly_X_fr_LH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_lowerleg_X_fr_LH_upperleg : public TransformHomogeneous<Type_fr_LH_lowerleg_X_fr_LH_upperleg>
    {
    public:
        Type_fr_LH_lowerleg_X_fr_LH_upperleg();
        const Type_fr_LH_lowerleg_X_fr_LH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_LH_upperleg_X_fr_LH_lowerleg : public TransformHomogeneous<Type_fr_LH_upperleg_X_fr_LH_lowerleg>
    {
    public:
        Type_fr_LH_upperleg_X_fr_LH_lowerleg();
        const Type_fr_LH_upperleg_X_fr_LH_lowerleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_hipassembly_X_fr_trunk : public TransformHomogeneous<Type_fr_RH_hipassembly_X_fr_trunk>
    {
    public:
        Type_fr_RH_hipassembly_X_fr_trunk();
        const Type_fr_RH_hipassembly_X_fr_trunk& update(const state_t&);
    protected:
    };
    
    class Type_fr_trunk_X_fr_RH_hipassembly : public TransformHomogeneous<Type_fr_trunk_X_fr_RH_hipassembly>
    {
    public:
        Type_fr_trunk_X_fr_RH_hipassembly();
        const Type_fr_trunk_X_fr_RH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_upperleg_X_fr_RH_hipassembly : public TransformHomogeneous<Type_fr_RH_upperleg_X_fr_RH_hipassembly>
    {
    public:
        Type_fr_RH_upperleg_X_fr_RH_hipassembly();
        const Type_fr_RH_upperleg_X_fr_RH_hipassembly& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_hipassembly_X_fr_RH_upperleg : public TransformHomogeneous<Type_fr_RH_hipassembly_X_fr_RH_upperleg>
    {
    public:
        Type_fr_RH_hipassembly_X_fr_RH_upperleg();
        const Type_fr_RH_hipassembly_X_fr_RH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_lowerleg_X_fr_RH_upperleg : public TransformHomogeneous<Type_fr_RH_lowerleg_X_fr_RH_upperleg>
    {
    public:
        Type_fr_RH_lowerleg_X_fr_RH_upperleg();
        const Type_fr_RH_lowerleg_X_fr_RH_upperleg& update(const state_t&);
    protected:
    };
    
    class Type_fr_RH_upperleg_X_fr_RH_lowerleg : public TransformHomogeneous<Type_fr_RH_upperleg_X_fr_RH_lowerleg>
    {
    public:
        Type_fr_RH_upperleg_X_fr_RH_lowerleg();
        const Type_fr_RH_upperleg_X_fr_RH_lowerleg& update(const state_t&);
    protected:
    };
    
public:
    HomogeneousTransforms(const dog::KinDynParams&);
    void updateParameters();
    Type_fr_LF_lowerleg_X_LF_foot fr_LF_lowerleg_X_LF_foot;
    Type_fr_RF_lowerleg_X_RF_foot fr_RF_lowerleg_X_RF_foot;
    Type_fr_LH_lowerleg_X_LH_foot fr_LH_lowerleg_X_LH_foot;
    Type_fr_RH_lowerleg_X_RH_foot fr_RH_lowerleg_X_RH_foot;
    Type_fr_trunk_X_LF_hipassemblyCOM fr_trunk_X_LF_hipassemblyCOM;
    Type_fr_trunk_X_RF_hipassemblyCOM fr_trunk_X_RF_hipassemblyCOM;
    Type_fr_trunk_X_LH_hipassemblyCOM fr_trunk_X_LH_hipassemblyCOM;
    Type_fr_trunk_X_RH_hipassemblyCOM fr_trunk_X_RH_hipassemblyCOM;
    Type_fr_trunk_X_LF_upperlegCOM fr_trunk_X_LF_upperlegCOM;
    Type_fr_trunk_X_RF_upperlegCOM fr_trunk_X_RF_upperlegCOM;
    Type_fr_trunk_X_LH_upperlegCOM fr_trunk_X_LH_upperlegCOM;
    Type_fr_trunk_X_RH_upperlegCOM fr_trunk_X_RH_upperlegCOM;
    Type_fr_trunk_X_LF_lowerlegCOM fr_trunk_X_LF_lowerlegCOM;
    Type_fr_trunk_X_RF_lowerlegCOM fr_trunk_X_RF_lowerlegCOM;
    Type_fr_trunk_X_LH_lowerlegCOM fr_trunk_X_LH_lowerlegCOM;
    Type_fr_trunk_X_RH_lowerlegCOM fr_trunk_X_RH_lowerlegCOM;
    Type_LF_foot_X_fr_LF_lowerleg LF_foot_X_fr_LF_lowerleg;
    Type_RF_foot_X_fr_RF_lowerleg RF_foot_X_fr_RF_lowerleg;
    Type_LH_foot_X_fr_LH_lowerleg LH_foot_X_fr_LH_lowerleg;
    Type_RH_foot_X_fr_RH_lowerleg RH_foot_X_fr_RH_lowerleg;
    Type_fr_trunk_X_LF_foot fr_trunk_X_LF_foot;
    Type_fr_trunk_X_RF_foot fr_trunk_X_RF_foot;
    Type_fr_trunk_X_LH_foot fr_trunk_X_LH_foot;
    Type_fr_trunk_X_RH_foot fr_trunk_X_RH_foot;
    Type_fr_LF_lowerleg_X_fr_trunk fr_LF_lowerleg_X_fr_trunk;
    Type_fr_RF_lowerleg_X_fr_trunk fr_RF_lowerleg_X_fr_trunk;
    Type_fr_LH_lowerleg_X_fr_trunk fr_LH_lowerleg_X_fr_trunk;
    Type_fr_RH_lowerleg_X_fr_trunk fr_RH_lowerleg_X_fr_trunk;
    Type_LF_foot_X_fr_trunk LF_foot_X_fr_trunk;
    Type_RF_foot_X_fr_trunk RF_foot_X_fr_trunk;
    Type_LH_foot_X_fr_trunk LH_foot_X_fr_trunk;
    Type_RH_foot_X_fr_trunk RH_foot_X_fr_trunk;
    Type_fr_trunk_X_LF_shin fr_trunk_X_LF_shin;
    Type_fr_trunk_X_RF_shin fr_trunk_X_RF_shin;
    Type_fr_trunk_X_LH_shin fr_trunk_X_LH_shin;
    Type_fr_trunk_X_RH_shin fr_trunk_X_RH_shin;

    Type_fr_trunk_X_fr_LF_HAA fr_trunk_X_fr_LF_HAA;
    Type_fr_trunk_X_fr_LF_HFE fr_trunk_X_fr_LF_HFE;
    Type_fr_trunk_X_fr_LF_KFE fr_trunk_X_fr_LF_KFE;
    Type_fr_trunk_X_fr_RF_HAA fr_trunk_X_fr_RF_HAA;
    Type_fr_trunk_X_fr_RF_HFE fr_trunk_X_fr_RF_HFE;
    Type_fr_trunk_X_fr_RF_KFE fr_trunk_X_fr_RF_KFE;
    Type_fr_trunk_X_fr_LH_HAA fr_trunk_X_fr_LH_HAA;
    Type_fr_trunk_X_fr_LH_HFE fr_trunk_X_fr_LH_HFE;
    Type_fr_trunk_X_fr_LH_KFE fr_trunk_X_fr_LH_KFE;
    Type_fr_trunk_X_fr_RH_HAA fr_trunk_X_fr_RH_HAA;
    Type_fr_trunk_X_fr_RH_HFE fr_trunk_X_fr_RH_HFE;
    Type_fr_trunk_X_fr_RH_KFE fr_trunk_X_fr_RH_KFE;

    Type_fr_LF_hipassembly_X_fr_trunk fr_LF_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_LF_hipassembly fr_trunk_X_fr_LF_hipassembly;
    Type_fr_LF_upperleg_X_fr_LF_hipassembly fr_LF_upperleg_X_fr_LF_hipassembly;
    Type_fr_LF_hipassembly_X_fr_LF_upperleg fr_LF_hipassembly_X_fr_LF_upperleg;
    Type_fr_LF_lowerleg_X_fr_LF_upperleg fr_LF_lowerleg_X_fr_LF_upperleg;
    Type_fr_LF_upperleg_X_fr_LF_lowerleg fr_LF_upperleg_X_fr_LF_lowerleg;
    Type_fr_RF_hipassembly_X_fr_trunk fr_RF_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_RF_hipassembly fr_trunk_X_fr_RF_hipassembly;
    Type_fr_RF_upperleg_X_fr_RF_hipassembly fr_RF_upperleg_X_fr_RF_hipassembly;
    Type_fr_RF_hipassembly_X_fr_RF_upperleg fr_RF_hipassembly_X_fr_RF_upperleg;
    Type_fr_RF_lowerleg_X_fr_RF_upperleg fr_RF_lowerleg_X_fr_RF_upperleg;
    Type_fr_RF_upperleg_X_fr_RF_lowerleg fr_RF_upperleg_X_fr_RF_lowerleg;
    Type_fr_LH_hipassembly_X_fr_trunk fr_LH_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_LH_hipassembly fr_trunk_X_fr_LH_hipassembly;
    Type_fr_LH_upperleg_X_fr_LH_hipassembly fr_LH_upperleg_X_fr_LH_hipassembly;
    Type_fr_LH_hipassembly_X_fr_LH_upperleg fr_LH_hipassembly_X_fr_LH_upperleg;
    Type_fr_LH_lowerleg_X_fr_LH_upperleg fr_LH_lowerleg_X_fr_LH_upperleg;
    Type_fr_LH_upperleg_X_fr_LH_lowerleg fr_LH_upperleg_X_fr_LH_lowerleg;
    Type_fr_RH_hipassembly_X_fr_trunk fr_RH_hipassembly_X_fr_trunk;
    Type_fr_trunk_X_fr_RH_hipassembly fr_trunk_X_fr_RH_hipassembly;
    Type_fr_RH_upperleg_X_fr_RH_hipassembly fr_RH_upperleg_X_fr_RH_hipassembly;
    Type_fr_RH_hipassembly_X_fr_RH_upperleg fr_RH_hipassembly_X_fr_RH_upperleg;
    Type_fr_RH_lowerleg_X_fr_RH_upperleg fr_RH_lowerleg_X_fr_RH_upperleg;
    Type_fr_RH_upperleg_X_fr_RH_lowerleg fr_RH_upperleg_X_fr_RH_lowerleg;

protected:
    Params_lengths values_lengths;

    const dog::KinDynParams* valuesGetter_lengths;
}; //class 'HomogeneousTransforms'

}
}

#endif
