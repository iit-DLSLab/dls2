
#include <iit/commons/dog/declarations.h>
#include <iit/commons/dog/joint_data_map.h>
#include <iit/robots/hyq/cylinder_lever_arm.h>


using namespace iit;

HyQ::internal::CylinderLeverArm HyQ::internal::CylinderLeverArm::instance;

HyQ::internal::CylinderLeverArm::CylinderLeverArm() :
        length_sign_flip(1)
{
    length_sign_flip[dog::LF_HFE] = -1;
    length_sign_flip[dog::RF_HFE] = -1;
    length_sign_flip[dog::LF_KFE] = -1;
    length_sign_flip[dog::RF_KFE] = -1;

    lengthCalculators[dog::LF_HAA] =
    lengthCalculators[dog::RF_HAA] =
    lengthCalculators[dog::LH_HAA] =
    lengthCalculators[dog::RH_HAA] = & CylinderLeverArm::haa_placeholder;

    lengthCalculators[dog::LF_HFE] =
    lengthCalculators[dog::RF_HFE] =
    lengthCalculators[dog::LH_HFE] =
    lengthCalculators[dog::RH_HFE] = & CylinderLeverArm::hfe_cyl_length;

    lengthCalculators[dog::LF_KFE] =
    lengthCalculators[dog::RF_KFE] =
    lengthCalculators[dog::LH_KFE] =
    lengthCalculators[dog::RH_KFE] = & CylinderLeverArm::kfe_cyl_length;

    leverCalculators[dog::LF_HAA] =
    leverCalculators[dog::RF_HAA] =
    leverCalculators[dog::LH_HAA] =
    leverCalculators[dog::RH_HAA] = & CylinderLeverArm::haa_placeholder;

    leverCalculators[dog::LF_HFE] =
    leverCalculators[dog::RF_HFE] =
    leverCalculators[dog::LH_HFE] =
    leverCalculators[dog::RH_HFE] = & CylinderLeverArm::hfe_lever;

    leverCalculators[dog::LF_KFE] =
    leverCalculators[dog::RF_KFE] =
    leverCalculators[dog::LH_KFE] =
    leverCalculators[dog::RH_KFE] = & CylinderLeverArm::kfe_lever;

    jointStatusCalculators[dog::LF_HAA] =
    jointStatusCalculators[dog::RF_HAA] =
    jointStatusCalculators[dog::LH_HAA] =
    jointStatusCalculators[dog::RH_HAA] = & CylinderLeverArm::haa_placeholder;

    jointStatusCalculators[dog::LF_HFE] =
    jointStatusCalculators[dog::RF_HFE] =
    jointStatusCalculators[dog::LH_HFE] =
    jointStatusCalculators[dog::RH_HFE] = & CylinderLeverArm::hfe_joint_status;

    jointStatusCalculators[dog::LF_KFE] =
    jointStatusCalculators[dog::RF_KFE] =
    jointStatusCalculators[dog::LH_KFE] =
    jointStatusCalculators[dog::RH_KFE] = & CylinderLeverArm::kfe_joint_status;
}








