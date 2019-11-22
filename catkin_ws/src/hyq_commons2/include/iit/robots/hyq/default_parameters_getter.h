#ifndef _HYQ_DEFAULT_PARAM_GETTER_
#define _HYQ_DEFAULT_PARAM_GETTER_

#include <iit/commons/dog/kin_dyn_params.h>
#include <iit/robots/hyq/kinematics_parameters.h>
#include <iit/robots/hyq/dynamics_parameters.h>

namespace iit {
namespace HyQ {


class DefaultParamsGetter : public dog::KinDynParams
{
    public:
	DefaultParamsGetter() {
            resetDefaults();
        }
        ~DefaultParamsGetter() {}

    public:
        virtual void resetDefaults() {
        	value.foot_x = 0.341;      // [m]
            inertias.trunk_mass = 60.96;
            inertias.trunk_com_x = 0.056;
            inertias.trunk_com_y = 0.0215;
            inertias.trunk_com_z = 0.00358;
            value.LF_shin = 0.341;      // [m]
            value.RF_shin = 0.341;      // [m]
            value.LH_shin = 0.341;      // [m]
            value.RH_shin = 0.341;      // [m]

            inertias.trunk_Ix  =   1.6882;
            inertias.trunk_Iy  =   8.6653;
            inertias.trunk_Iz  =   9.2436;
            inertias.trunk_Ixy =   0.0288;
            inertias.trunk_Ixz =  -0.2777;
            inertias.trunk_Iyz =  -0.0038;
        }

        //getters
        double getValue_trunk_mass() const {
            return inertias.trunk_mass;
        }
        double getValue_trunk_com_x() const {
            return inertias.trunk_com_x;
        }
        double getValue_trunk_com_y() const {
            return inertias.trunk_com_y;
        }
        double getValue_trunk_com_z() const {
            return inertias.trunk_com_z;
        }
        double getValue_trunk_Ix() const {
            return inertias.trunk_Ix;
        }
        double getValue_trunk_Iy() const {
            return inertias.trunk_Iy;
        }
        double getValue_trunk_Iz() const {
            return inertias.trunk_Iz;
        }
        double getValue_trunk_Ixy() const {
            return inertias.trunk_Ixy;
        }
        double getValue_trunk_Ixz() const {
            return inertias.trunk_Ixz;
        }
        double getValue_trunk_Iyz() const {
            return inertias.trunk_Iyz;
        }
        //setters
        // the getters are not virtual (the setters are!) so
        // if you inherit this class, to avoid the use of the default
        // implementation we need to set the functions as virtual!)
        virtual void setValue_trunk_mass(double val)  {
            inertias.trunk_mass =val;
        }
        virtual void setValue_trunk_com_x(double val)  {
            inertias.trunk_com_x =val;
        }
        virtual void setValue_trunk_com_y(double val)  {
            inertias.trunk_com_y =val;
        }
        virtual void setValue_trunk_com_z(double val)  {
            inertias.trunk_com_z =val;
        }
        virtual void setValue_trunk_Ix(double val)  {
            inertias.trunk_Ix =val;
        }
        virtual void setValue_trunk_Iy(double val)  {
            inertias.trunk_Iy =val;
        }
        virtual void setValue_trunk_Iz(double val)  {
            inertias.trunk_Iz =val;
        }
        virtual void setValue_trunk_Ixy(double val)  {
            inertias.trunk_Ixy =val;
        }
        virtual void setValue_trunk_Ixz(double val)  {
            inertias.trunk_Ixz =val;
        }
        virtual void setValue_trunk_Iyz(double val)  {
            inertias.trunk_Iyz =val;
        }
        double getValue_foot_x() const
        {
            return value.foot_x;
        }

        virtual void setValue_foot_x(double val)
        {
            value.foot_x = val;
        }

        double getValue_LF_shin() const {
            return value.LF_shin;
        }
        double getValue_RF_shin() const {
            return value.RF_shin;
        }
        double getValue_LH_shin() const {
            return value.LH_shin;
        }
        double getValue_RH_shin() const {
            return value.RH_shin;
        }

        virtual void setValue_LF_shin(double val)
        {
             value.LF_shin = val;
        }
        virtual void setValue_RF_shin(double val)
        {
             value.RF_shin = val;
        }
        virtual void setValue_LH_shin(double val)
        {
             value.LH_shin = val;
        }
        virtual void setValue_RH_shin(double val)
        {
             value.RH_shin =  val;
        }

        // TODO
        // these are not used in the actual implementation
        // the generated code for HyQ is not yet parametric
        // for these parameters
        virtual double getValue_haa_x() const {
            return 0;
        }
        virtual double getValue_haa_y() const {
            return 0;
        }
        virtual double getValue_haa_z() const {
            return 0;
        }
        virtual double getValue_haa_hfe() const {
            return 0;
        }
        virtual double getValue_upper_leg() const {
            return 0;
        }
        virtual double getValue_lower_leg() const {
            return 0;
        }

        virtual void setValue_haa_x(double val) {}
        virtual void setValue_haa_y(double val) {}
        virtual void setValue_haa_z(double val) {}
        virtual void setValue_haa_hfe(double val) {}
        virtual void setValue_upper_leg(double val) {}
        virtual void setValue_lower_leg(double val) {}

        /**
         * @return the whole set of the current inertias of the inertia parameters
         */
        const dyn::RuntimeInertiaParams& getInertiaParams() const {
            return inertias;
        }
        /**
         * Sets new inertias for all the inertia parameters
         */
        virtual void setInertiaParams(const dyn::RuntimeInertiaParams& newp) {
        	inertias = newp;
        }


    private:
        Params_lengths value;
        dyn::RuntimeInertiaParams inertias;
};


}
}
#endif
