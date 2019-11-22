#include "iit/robots/hyq/transforms.h"
#include "iit/robots/hyq/jsim.h"

using namespace iit::dog;

//Implementation of default constructor
iit::HyQ::dyn::JSIM::JSIM(InertiaProperties& inertiaProperties, ForceTransforms& forceTransforms) :
    linkInertias(inertiaProperties),
    frcTransf( &forceTransforms ),
    Ic_LF_lowerleg(linkInertias.getTensor_LF_lowerleg()),
    Ic_RF_lowerleg(linkInertias.getTensor_RF_lowerleg()),
    Ic_LH_lowerleg(linkInertias.getTensor_LH_lowerleg()),
    Ic_RH_lowerleg(linkInertias.getTensor_RH_lowerleg())
{
    //Initialize the matrix itself
    this->setZero();
}

#define DATA operator()
#define F(j) (block<6,1>(0,(j)+6))

const iit::HyQ::dyn::JSIM& iit::HyQ::dyn::JSIM::update(const JointState& state) {

    // Precomputes only once the coordinate transforms:
    frcTransf -> fr_RH_upperleg_X_fr_RH_lowerleg(state);
    frcTransf -> fr_RH_hipassembly_X_fr_RH_upperleg(state);
    frcTransf -> fr_trunk_X_fr_RH_hipassembly(state);
    frcTransf -> fr_LH_upperleg_X_fr_LH_lowerleg(state);
    frcTransf -> fr_LH_hipassembly_X_fr_LH_upperleg(state);
    frcTransf -> fr_trunk_X_fr_LH_hipassembly(state);
    frcTransf -> fr_RF_upperleg_X_fr_RF_lowerleg(state);
    frcTransf -> fr_RF_hipassembly_X_fr_RF_upperleg(state);
    frcTransf -> fr_trunk_X_fr_RF_hipassembly(state);
    frcTransf -> fr_LF_upperleg_X_fr_LF_lowerleg(state);
    frcTransf -> fr_LF_hipassembly_X_fr_LF_upperleg(state);
    frcTransf -> fr_trunk_X_fr_LF_hipassembly(state);

    // Initializes the composite inertia tensors
    Ic_trunk = linkInertias.getTensor_trunk();
    Ic_LF_hipassembly = linkInertias.getTensor_LF_hipassembly();
    Ic_LF_upperleg = linkInertias.getTensor_LF_upperleg();
    Ic_RF_hipassembly = linkInertias.getTensor_RF_hipassembly();
    Ic_RF_upperleg = linkInertias.getTensor_RF_upperleg();
    Ic_LH_hipassembly = linkInertias.getTensor_LH_hipassembly();
    Ic_LH_upperleg = linkInertias.getTensor_LH_upperleg();
    Ic_RH_hipassembly = linkInertias.getTensor_RH_hipassembly();
    Ic_RH_upperleg = linkInertias.getTensor_RH_upperleg();

    // "Bottom-up" loop to update the inertia-composite property of each link, for the current configuration

    // Link RH_lowerleg:
    Ic_RH_upperleg = Ic_RH_upperleg + frcTransf -> fr_RH_upperleg_X_fr_RH_lowerleg * Ic_RH_lowerleg * (frcTransf -> fr_RH_upperleg_X_fr_RH_lowerleg).transpose();

    F(RH_KFE) = Ic_RH_lowerleg.col(iit::rbd::AZ);
    DATA(RH_KFE+6, RH_KFE+6) = F(RH_KFE).row(iit::rbd::AZ)(0,0);

    F(RH_KFE) = frcTransf -> fr_RH_upperleg_X_fr_RH_lowerleg * F(RH_KFE);
    DATA(RH_KFE+6, RH_HFE+6) = F(RH_KFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(RH_HFE+6, RH_KFE+6) = DATA(RH_KFE+6, RH_HFE+6);
    F(RH_KFE) = frcTransf -> fr_RH_hipassembly_X_fr_RH_upperleg * F(RH_KFE);
    DATA(RH_KFE+6, RH_HAA+6) = F(RH_KFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(RH_HAA+6, RH_KFE+6) = DATA(RH_KFE+6, RH_HAA+6);
    F(RH_KFE) = frcTransf -> fr_trunk_X_fr_RH_hipassembly * F(RH_KFE);

    // Link RH_upperleg:
    Ic_RH_hipassembly = Ic_RH_hipassembly + frcTransf -> fr_RH_hipassembly_X_fr_RH_upperleg * Ic_RH_upperleg * (frcTransf -> fr_RH_hipassembly_X_fr_RH_upperleg).transpose();

    F(RH_HFE) = Ic_RH_upperleg.col(iit::rbd::AZ);
    DATA(RH_HFE+6, RH_HFE+6) = F(RH_HFE).row(iit::rbd::AZ)(0,0);

    F(RH_HFE) = frcTransf -> fr_RH_hipassembly_X_fr_RH_upperleg * F(RH_HFE);
    DATA(RH_HFE+6, RH_HAA+6) = F(RH_HFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(RH_HAA+6, RH_HFE+6) = DATA(RH_HFE+6, RH_HAA+6);
    F(RH_HFE) = frcTransf -> fr_trunk_X_fr_RH_hipassembly * F(RH_HFE);

    // Link RH_hipassembly:
    Ic_trunk = Ic_trunk + frcTransf -> fr_trunk_X_fr_RH_hipassembly * Ic_RH_hipassembly * (frcTransf -> fr_trunk_X_fr_RH_hipassembly).transpose();

    F(RH_HAA) = Ic_RH_hipassembly.col(iit::rbd::AZ);
    DATA(RH_HAA+6, RH_HAA+6) = F(RH_HAA).row(iit::rbd::AZ)(0,0);

    F(RH_HAA) = frcTransf -> fr_trunk_X_fr_RH_hipassembly * F(RH_HAA);

    // Link LH_lowerleg:
    Ic_LH_upperleg = Ic_LH_upperleg + frcTransf -> fr_LH_upperleg_X_fr_LH_lowerleg * Ic_LH_lowerleg * (frcTransf -> fr_LH_upperleg_X_fr_LH_lowerleg).transpose();

    F(LH_KFE) = Ic_LH_lowerleg.col(iit::rbd::AZ);
    DATA(LH_KFE+6, LH_KFE+6) = F(LH_KFE).row(iit::rbd::AZ)(0,0);

    F(LH_KFE) = frcTransf -> fr_LH_upperleg_X_fr_LH_lowerleg * F(LH_KFE);
    DATA(LH_KFE+6, LH_HFE+6) = F(LH_KFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(LH_HFE+6, LH_KFE+6) = DATA(LH_KFE+6, LH_HFE+6);
    F(LH_KFE) = frcTransf -> fr_LH_hipassembly_X_fr_LH_upperleg * F(LH_KFE);
    DATA(LH_KFE+6, LH_HAA+6) = F(LH_KFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(LH_HAA+6, LH_KFE+6) = DATA(LH_KFE+6, LH_HAA+6);
    F(LH_KFE) = frcTransf -> fr_trunk_X_fr_LH_hipassembly * F(LH_KFE);

    // Link LH_upperleg:
    Ic_LH_hipassembly = Ic_LH_hipassembly + frcTransf -> fr_LH_hipassembly_X_fr_LH_upperleg * Ic_LH_upperleg * (frcTransf -> fr_LH_hipassembly_X_fr_LH_upperleg).transpose();

    F(LH_HFE) = Ic_LH_upperleg.col(iit::rbd::AZ);
    DATA(LH_HFE+6, LH_HFE+6) = F(LH_HFE).row(iit::rbd::AZ)(0,0);

    F(LH_HFE) = frcTransf -> fr_LH_hipassembly_X_fr_LH_upperleg * F(LH_HFE);
    DATA(LH_HFE+6, LH_HAA+6) = F(LH_HFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(LH_HAA+6, LH_HFE+6) = DATA(LH_HFE+6, LH_HAA+6);
    F(LH_HFE) = frcTransf -> fr_trunk_X_fr_LH_hipassembly * F(LH_HFE);

    // Link LH_hipassembly:
    Ic_trunk = Ic_trunk + frcTransf -> fr_trunk_X_fr_LH_hipassembly * Ic_LH_hipassembly * (frcTransf -> fr_trunk_X_fr_LH_hipassembly).transpose();

    F(LH_HAA) = Ic_LH_hipassembly.col(iit::rbd::AZ);
    DATA(LH_HAA+6, LH_HAA+6) = F(LH_HAA).row(iit::rbd::AZ)(0,0);

    F(LH_HAA) = frcTransf -> fr_trunk_X_fr_LH_hipassembly * F(LH_HAA);

    // Link RF_lowerleg:
    Ic_RF_upperleg = Ic_RF_upperleg + frcTransf -> fr_RF_upperleg_X_fr_RF_lowerleg * Ic_RF_lowerleg * (frcTransf -> fr_RF_upperleg_X_fr_RF_lowerleg).transpose();

    F(RF_KFE) = Ic_RF_lowerleg.col(iit::rbd::AZ);
    DATA(RF_KFE+6, RF_KFE+6) = F(RF_KFE).row(iit::rbd::AZ)(0,0);

    F(RF_KFE) = frcTransf -> fr_RF_upperleg_X_fr_RF_lowerleg * F(RF_KFE);
    DATA(RF_KFE+6, RF_HFE+6) = F(RF_KFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(RF_HFE+6, RF_KFE+6) = DATA(RF_KFE+6, RF_HFE+6);
    F(RF_KFE) = frcTransf -> fr_RF_hipassembly_X_fr_RF_upperleg * F(RF_KFE);
    DATA(RF_KFE+6, RF_HAA+6) = F(RF_KFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(RF_HAA+6, RF_KFE+6) = DATA(RF_KFE+6, RF_HAA+6);
    F(RF_KFE) = frcTransf -> fr_trunk_X_fr_RF_hipassembly * F(RF_KFE);

    // Link RF_upperleg:
    Ic_RF_hipassembly = Ic_RF_hipassembly + frcTransf -> fr_RF_hipassembly_X_fr_RF_upperleg * Ic_RF_upperleg * (frcTransf -> fr_RF_hipassembly_X_fr_RF_upperleg).transpose();

    F(RF_HFE) = Ic_RF_upperleg.col(iit::rbd::AZ);
    DATA(RF_HFE+6, RF_HFE+6) = F(RF_HFE).row(iit::rbd::AZ)(0,0);

    F(RF_HFE) = frcTransf -> fr_RF_hipassembly_X_fr_RF_upperleg * F(RF_HFE);
    DATA(RF_HFE+6, RF_HAA+6) = F(RF_HFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(RF_HAA+6, RF_HFE+6) = DATA(RF_HFE+6, RF_HAA+6);
    F(RF_HFE) = frcTransf -> fr_trunk_X_fr_RF_hipassembly * F(RF_HFE);

    // Link RF_hipassembly:
    Ic_trunk = Ic_trunk + frcTransf -> fr_trunk_X_fr_RF_hipassembly * Ic_RF_hipassembly * (frcTransf -> fr_trunk_X_fr_RF_hipassembly).transpose();

    F(RF_HAA) = Ic_RF_hipassembly.col(iit::rbd::AZ);
    DATA(RF_HAA+6, RF_HAA+6) = F(RF_HAA).row(iit::rbd::AZ)(0,0);

    F(RF_HAA) = frcTransf -> fr_trunk_X_fr_RF_hipassembly * F(RF_HAA);

    // Link LF_lowerleg:
    Ic_LF_upperleg = Ic_LF_upperleg + frcTransf -> fr_LF_upperleg_X_fr_LF_lowerleg * Ic_LF_lowerleg * (frcTransf -> fr_LF_upperleg_X_fr_LF_lowerleg).transpose();

    F(LF_KFE) = Ic_LF_lowerleg.col(iit::rbd::AZ);
    DATA(LF_KFE+6, LF_KFE+6) = F(LF_KFE).row(iit::rbd::AZ)(0,0);

    F(LF_KFE) = frcTransf -> fr_LF_upperleg_X_fr_LF_lowerleg * F(LF_KFE);
    DATA(LF_KFE+6, LF_HFE+6) = F(LF_KFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(LF_HFE+6, LF_KFE+6) = DATA(LF_KFE+6, LF_HFE+6);
    F(LF_KFE) = frcTransf -> fr_LF_hipassembly_X_fr_LF_upperleg * F(LF_KFE);
    DATA(LF_KFE+6, LF_HAA+6) = F(LF_KFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(LF_HAA+6, LF_KFE+6) = DATA(LF_KFE+6, LF_HAA+6);
    F(LF_KFE) = frcTransf -> fr_trunk_X_fr_LF_hipassembly * F(LF_KFE);

    // Link LF_upperleg:
    Ic_LF_hipassembly = Ic_LF_hipassembly + frcTransf -> fr_LF_hipassembly_X_fr_LF_upperleg * Ic_LF_upperleg * (frcTransf -> fr_LF_hipassembly_X_fr_LF_upperleg).transpose();

    F(LF_HFE) = Ic_LF_upperleg.col(iit::rbd::AZ);
    DATA(LF_HFE+6, LF_HFE+6) = F(LF_HFE).row(iit::rbd::AZ)(0,0);

    F(LF_HFE) = frcTransf -> fr_LF_hipassembly_X_fr_LF_upperleg * F(LF_HFE);
    DATA(LF_HFE+6, LF_HAA+6) = F(LF_HFE).transpose().col(iit::rbd::AZ)(0,0);
    DATA(LF_HAA+6, LF_HFE+6) = DATA(LF_HFE+6, LF_HAA+6);
    F(LF_HFE) = frcTransf -> fr_trunk_X_fr_LF_hipassembly * F(LF_HFE);

    // Link LF_hipassembly:
    Ic_trunk = Ic_trunk + frcTransf -> fr_trunk_X_fr_LF_hipassembly * Ic_LF_hipassembly * (frcTransf -> fr_trunk_X_fr_LF_hipassembly).transpose();

    F(LF_HAA) = Ic_LF_hipassembly.col(iit::rbd::AZ);
    DATA(LF_HAA+6, LF_HAA+6) = F(LF_HAA).row(iit::rbd::AZ)(0,0);

    F(LF_HAA) = frcTransf -> fr_trunk_X_fr_LF_hipassembly * F(LF_HAA);

    // Copies the upper-right block into the lower-left block, after transposing
    block<12, 6>(6,0) = (block<6, 12>(0,6)).transpose();
    // The composite-inertia of the whole robot is the upper-left quadrant of the JSIM
    block<6,6>(0,0) = Ic_trunk;
    return *this;
}

#undef DATA
#undef F

void iit::HyQ::dyn::JSIM::computeL() {
    L = this -> triangularView<Eigen::Lower>();
    // Joint RH_KFE, index 11 :
    L(11, 11) = std::sqrt(L(11, 11));
    L(11, 10) = L(11, 10) / L(11, 11);
    L(11, 9) = L(11, 9) / L(11, 11);
    L(10, 10) = L(10, 10) - L(11, 10) * L(11, 10);
    L(10, 9) = L(10, 9) - L(11, 10) * L(11, 9);
    L(9, 9) = L(9, 9) - L(11, 9) * L(11, 9);
    
    // Joint RH_HFE, index 10 :
    L(10, 10) = std::sqrt(L(10, 10));
    L(10, 9) = L(10, 9) / L(10, 10);
    L(9, 9) = L(9, 9) - L(10, 9) * L(10, 9);
    
    // Joint RH_HAA, index 9 :
    L(9, 9) = std::sqrt(L(9, 9));
    
    // Joint LH_KFE, index 8 :
    L(8, 8) = std::sqrt(L(8, 8));
    L(8, 7) = L(8, 7) / L(8, 8);
    L(8, 6) = L(8, 6) / L(8, 8);
    L(7, 7) = L(7, 7) - L(8, 7) * L(8, 7);
    L(7, 6) = L(7, 6) - L(8, 7) * L(8, 6);
    L(6, 6) = L(6, 6) - L(8, 6) * L(8, 6);
    
    // Joint LH_HFE, index 7 :
    L(7, 7) = std::sqrt(L(7, 7));
    L(7, 6) = L(7, 6) / L(7, 7);
    L(6, 6) = L(6, 6) - L(7, 6) * L(7, 6);
    
    // Joint LH_HAA, index 6 :
    L(6, 6) = std::sqrt(L(6, 6));
    
    // Joint RF_KFE, index 5 :
    L(5, 5) = std::sqrt(L(5, 5));
    L(5, 4) = L(5, 4) / L(5, 5);
    L(5, 3) = L(5, 3) / L(5, 5);
    L(4, 4) = L(4, 4) - L(5, 4) * L(5, 4);
    L(4, 3) = L(4, 3) - L(5, 4) * L(5, 3);
    L(3, 3) = L(3, 3) - L(5, 3) * L(5, 3);
    
    // Joint RF_HFE, index 4 :
    L(4, 4) = std::sqrt(L(4, 4));
    L(4, 3) = L(4, 3) / L(4, 4);
    L(3, 3) = L(3, 3) - L(4, 3) * L(4, 3);
    
    // Joint RF_HAA, index 3 :
    L(3, 3) = std::sqrt(L(3, 3));
    
    // Joint LF_KFE, index 2 :
    L(2, 2) = std::sqrt(L(2, 2));
    L(2, 1) = L(2, 1) / L(2, 2);
    L(2, 0) = L(2, 0) / L(2, 2);
    L(1, 1) = L(1, 1) - L(2, 1) * L(2, 1);
    L(1, 0) = L(1, 0) - L(2, 1) * L(2, 0);
    L(0, 0) = L(0, 0) - L(2, 0) * L(2, 0);
    
    // Joint LF_HFE, index 1 :
    L(1, 1) = std::sqrt(L(1, 1));
    L(1, 0) = L(1, 0) / L(1, 1);
    L(0, 0) = L(0, 0) - L(1, 0) * L(1, 0);
    
    // Joint LF_HAA, index 0 :
    L(0, 0) = std::sqrt(L(0, 0));
    
}

void iit::HyQ::dyn::JSIM::computeInverse() {
    computeLInverse();

    inverse(0, 0) =  + (Linv(0, 0) * Linv(0, 0));
    inverse(1, 1) =  + (Linv(1, 0) * Linv(1, 0)) + (Linv(1, 1) * Linv(1, 1));
    inverse(1, 0) =  + (Linv(1, 0) * Linv(0, 0));
    inverse(0, 1) = inverse(1, 0);
    inverse(2, 2) =  + (Linv(2, 0) * Linv(2, 0)) + (Linv(2, 1) * Linv(2, 1)) + (Linv(2, 2) * Linv(2, 2));
    inverse(2, 1) =  + (Linv(2, 0) * Linv(1, 0)) + (Linv(2, 1) * Linv(1, 1));
    inverse(1, 2) = inverse(2, 1);
    inverse(2, 0) =  + (Linv(2, 0) * Linv(0, 0));
    inverse(0, 2) = inverse(2, 0);
    inverse(3, 3) =  + (Linv(3, 3) * Linv(3, 3));
    inverse(4, 4) =  + (Linv(4, 3) * Linv(4, 3)) + (Linv(4, 4) * Linv(4, 4));
    inverse(4, 3) =  + (Linv(4, 3) * Linv(3, 3));
    inverse(3, 4) = inverse(4, 3);
    inverse(5, 5) =  + (Linv(5, 3) * Linv(5, 3)) + (Linv(5, 4) * Linv(5, 4)) + (Linv(5, 5) * Linv(5, 5));
    inverse(5, 4) =  + (Linv(5, 3) * Linv(4, 3)) + (Linv(5, 4) * Linv(4, 4));
    inverse(4, 5) = inverse(5, 4);
    inverse(5, 3) =  + (Linv(5, 3) * Linv(3, 3));
    inverse(3, 5) = inverse(5, 3);
    inverse(6, 6) =  + (Linv(6, 6) * Linv(6, 6));
    inverse(7, 7) =  + (Linv(7, 6) * Linv(7, 6)) + (Linv(7, 7) * Linv(7, 7));
    inverse(7, 6) =  + (Linv(7, 6) * Linv(6, 6));
    inverse(6, 7) = inverse(7, 6);
    inverse(8, 8) =  + (Linv(8, 6) * Linv(8, 6)) + (Linv(8, 7) * Linv(8, 7)) + (Linv(8, 8) * Linv(8, 8));
    inverse(8, 7) =  + (Linv(8, 6) * Linv(7, 6)) + (Linv(8, 7) * Linv(7, 7));
    inverse(7, 8) = inverse(8, 7);
    inverse(8, 6) =  + (Linv(8, 6) * Linv(6, 6));
    inverse(6, 8) = inverse(8, 6);
    inverse(9, 9) =  + (Linv(9, 9) * Linv(9, 9));
    inverse(10, 10) =  + (Linv(10, 9) * Linv(10, 9)) + (Linv(10, 10) * Linv(10, 10));
    inverse(10, 9) =  + (Linv(10, 9) * Linv(9, 9));
    inverse(9, 10) = inverse(10, 9);
    inverse(11, 11) =  + (Linv(11, 9) * Linv(11, 9)) + (Linv(11, 10) * Linv(11, 10)) + (Linv(11, 11) * Linv(11, 11));
    inverse(11, 10) =  + (Linv(11, 9) * Linv(10, 9)) + (Linv(11, 10) * Linv(10, 10));
    inverse(10, 11) = inverse(11, 10);
    inverse(11, 9) =  + (Linv(11, 9) * Linv(9, 9));
    inverse(9, 11) = inverse(11, 9);
}

void iit::HyQ::dyn::JSIM::computeLInverse() {
    //assumes L has been computed already
    Linv(0, 0) = 1 / L(0, 0);
    Linv(1, 1) = 1 / L(1, 1);
    Linv(2, 2) = 1 / L(2, 2);
    Linv(3, 3) = 1 / L(3, 3);
    Linv(4, 4) = 1 / L(4, 4);
    Linv(5, 5) = 1 / L(5, 5);
    Linv(6, 6) = 1 / L(6, 6);
    Linv(7, 7) = 1 / L(7, 7);
    Linv(8, 8) = 1 / L(8, 8);
    Linv(9, 9) = 1 / L(9, 9);
    Linv(10, 10) = 1 / L(10, 10);
    Linv(11, 11) = 1 / L(11, 11);
    Linv(1, 0) = - Linv(0, 0) * ((Linv(1, 1) * L(1, 0)) + 0);
    Linv(2, 1) = - Linv(1, 1) * ((Linv(2, 2) * L(2, 1)) + 0);
    Linv(2, 0) = - Linv(0, 0) * ((Linv(2, 1) * L(1, 0)) + (Linv(2, 2) * L(2, 0)) + 0);
    Linv(4, 3) = - Linv(3, 3) * ((Linv(4, 4) * L(4, 3)) + 0);
    Linv(5, 4) = - Linv(4, 4) * ((Linv(5, 5) * L(5, 4)) + 0);
    Linv(5, 3) = - Linv(3, 3) * ((Linv(5, 4) * L(4, 3)) + (Linv(5, 5) * L(5, 3)) + 0);
    Linv(7, 6) = - Linv(6, 6) * ((Linv(7, 7) * L(7, 6)) + 0);
    Linv(8, 7) = - Linv(7, 7) * ((Linv(8, 8) * L(8, 7)) + 0);
    Linv(8, 6) = - Linv(6, 6) * ((Linv(8, 7) * L(7, 6)) + (Linv(8, 8) * L(8, 6)) + 0);
    Linv(10, 9) = - Linv(9, 9) * ((Linv(10, 10) * L(10, 9)) + 0);
    Linv(11, 10) = - Linv(10, 10) * ((Linv(11, 11) * L(11, 10)) + 0);
    Linv(11, 9) = - Linv(9, 9) * ((Linv(11, 10) * L(10, 9)) + (Linv(11, 11) * L(11, 9)) + 0);
}
