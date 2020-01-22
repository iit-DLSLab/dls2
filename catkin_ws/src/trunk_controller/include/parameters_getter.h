#ifndef _TEST_PARAM_GETTER_HyQ_
#define _TEST_PARAM_GETTER_HyQ_


#include <doglib/hyq/kinematics_parameters.hpp>
#include <doglib/hyq/dynamics_parameters.hpp>
#include <doglib/hyq/default_parameters_getter.hpp>



namespace dls {
namespace HyQ {


class TestParamsGetter : public dls::dog::HyQ::DefaultParamsGetter
{
    public:
    TestParamsGetter() {
            resetDefaults();
        }
        ~TestParamsGetter() {};

    public:
        void resetDefaults() {
        	value.foot_x = 0.341;      // [m]
            inertias.trunk_mass =  71.758;
            inertias.trunk_com_x = 0.02707;
            inertias.trunk_com_y = 0;
            inertias.trunk_com_z = 0.0329;

            inertias.trunk_Ix  =   1.6882 ;
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
        void setValue_trunk_mass(double val)  {
            inertias.trunk_mass =val;
        }
        void setValue_trunk_com_x(double val)  {
            inertias.trunk_com_x =val;
        }
        void setValue_trunk_com_y(double val)  {
            inertias.trunk_com_y =val;
        }
        void setValue_trunk_com_z(double val)  {
            inertias.trunk_com_z =val;
        }
        void setValue_trunk_Ix(double val)  {
            inertias.trunk_Ix =val;
        }
        void setValue_trunk_Iy(double val)  {
            inertias.trunk_Iy =val;
        }
        void setValue_trunk_Iz(double val)  {
            inertias.trunk_Iz =val;
        }
        void setValue_trunk_Ixy(double val)  {
            inertias.trunk_Ixy =val;
        }
        void setValue_trunk_Ixz(double val)  {
            inertias.trunk_Ixz =val;
        }
        void setValue_trunk_Iyz(double val)  {
            inertias.trunk_Iyz =val;
        }
        double getValue_foot_x() const
        {
            return value.foot_x;
        }

        void setValue_foot_x(double val)
        {
            value.foot_x = val;
        }

        /**
         * @return the whole set of the current inertias of the inertia parameters
         */
        const dls::dog::HyQ::dyn::RuntimeInertiaParams& getInertiaParams() const {
            return inertias;
        }
        /**
         * Sets new inertias for all the inertia parameters
         */
        void setInertiaParams(const dls::dog::HyQ::dyn::RuntimeInertiaParams& newp) {
        	inertias = newp;
        }


    private:
		dls::dog::HyQ::Params_lengths value;
		dls::dog::HyQ::dyn::RuntimeInertiaParams inertias;
};


}
}

#endif
