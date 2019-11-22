#include "iit/robots/hyq/transforms.h"

// Constructors
using namespace iit::dog;
using namespace iit::HyQ;

SpatialTransformPlain MotionTransforms::getTransform(const dog::JointState &q,
                                                     const dog::OriginFrame &orig,
                                                     const dog::DestFrame &dest){
    this->updateParameters();
    switch(dest){
    case DestFrame::TRUNK:
        switch(orig){
        case OriginFrame::LF_HIPASSEMBLY_COM:
            return this->fr_trunk_X_LF_hipassemblyCOM(q);
        case OriginFrame::RF_HIPASSEMBLY_COM:
            return this->fr_trunk_X_RF_hipassemblyCOM(q);
        case OriginFrame::LH_HIPASSEMBLY_COM:
            return this->fr_trunk_X_LH_hipassemblyCOM(q);
        case OriginFrame::RH_HIPASSEMBLY_COM:
            return this->fr_trunk_X_RH_hipassemblyCOM(q);

        case OriginFrame::LF_HIPASSEMBLY:
            return this->fr_trunk_X_fr_LF_hipassembly(q);
        case OriginFrame::RF_HIPASSEMBLY:
            return this->fr_trunk_X_fr_RF_hipassembly(q);
        case OriginFrame::LH_HIPASSEMBLY:
            return this->fr_trunk_X_fr_LH_hipassembly(q);
        case OriginFrame::RH_HIPASSEMBLY:
            return this->fr_trunk_X_fr_RH_hipassembly(q);

        case OriginFrame::LF_UPPERLEG_COM:
            return this->fr_trunk_X_LF_upperlegCOM(q);
        case OriginFrame::RF_UPPERLEG_COM:
            return this->fr_trunk_X_RF_upperlegCOM(q);
        case OriginFrame::LH_UPPERLEG_COM:
            return this->fr_trunk_X_LH_upperlegCOM(q);
        case OriginFrame::RH_UPPERLEG_COM:
            return this->fr_trunk_X_RH_upperlegCOM(q);

        case OriginFrame::LF_LOWERLEG_COM:
            return this->fr_trunk_X_LF_lowerlegCOM(q);
        case OriginFrame::RF_LOWERLEG_COM:
            return this->fr_trunk_X_RF_lowerlegCOM(q);
        case OriginFrame::LH_LOWERLEG_COM:
            return this->fr_trunk_X_LH_lowerlegCOM(q);
        case OriginFrame::RH_LOWERLEG_COM:
            return this->fr_trunk_X_RH_lowerlegCOM(q);

        case OriginFrame::LF_FOOT:
            return this->fr_trunk_X_LF_foot(q);
        case OriginFrame::RF_FOOT:
            return this->fr_trunk_X_RF_foot(q);
        case OriginFrame::LH_FOOT:
            return this->fr_trunk_X_LH_foot(q);
        case OriginFrame::RH_FOOT:
            return this->fr_trunk_X_RH_foot(q);

        case OriginFrame::LF_SHIN:
            return this->fr_trunk_X_LF_shin(q);
        case OriginFrame::RF_SHIN:
            return this->fr_trunk_X_RF_shin(q);
        case OriginFrame::LH_SHIN:
            return this->fr_trunk_X_LH_shin(q);
        case OriginFrame::RH_SHIN:
            return this->fr_trunk_X_RH_shin(q);

        case OriginFrame::LF_HAA:
            return this->fr_trunk_X_fr_LF_HAA(q);
        case OriginFrame::RF_HAA:
            return this->fr_trunk_X_fr_RF_HAA(q);
        case OriginFrame::LH_HAA:
            return this->fr_trunk_X_fr_LH_HAA(q);
        case OriginFrame::RH_HAA:
            return this->fr_trunk_X_fr_RH_HAA(q);

        case OriginFrame::LF_HFE:
            return this->fr_trunk_X_fr_LF_HFE(q);
        case OriginFrame::RF_HFE:
            return this->fr_trunk_X_fr_RF_HFE(q);
        case OriginFrame::LH_HFE:
            return this->fr_trunk_X_fr_LH_HFE(q);
        case OriginFrame::RH_HFE:
            return this->fr_trunk_X_fr_RH_HFE(q);

        case OriginFrame::LF_KFE:
            return this->fr_trunk_X_fr_LF_KFE(q);
        case OriginFrame::RF_KFE:
            return this->fr_trunk_X_fr_RF_KFE(q);
        case OriginFrame::LH_KFE:
            return this->fr_trunk_X_fr_LH_KFE(q);
        case OriginFrame::RH_KFE:
            return this->fr_trunk_X_fr_RH_KFE(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LF_FOOT:
        switch(orig){
        case OriginFrame::LF_LOWERLEG:
            return this->LF_foot_X_fr_LF_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->LF_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
            break;
        }
        break;

    case DestFrame::RF_FOOT:
        switch(orig){
        case OriginFrame::RF_LOWERLEG:
            return this->RF_foot_X_fr_RF_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->RF_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LH_FOOT:
        switch(orig){
        case OriginFrame::LH_LOWERLEG:
            return this->LH_foot_X_fr_LH_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->LH_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RH_FOOT:
        switch(orig){
        case OriginFrame::RH_LOWERLEG:
            return this->RH_foot_X_fr_RH_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->RH_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LF_LOWERLEG:
        switch(orig){
        case OriginFrame::LF_FOOT:
            return this->fr_LF_lowerleg_X_LF_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_LF_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RF_LOWERLEG:
        switch(orig){
        case OriginFrame::RF_FOOT:
            return this->fr_RF_lowerleg_X_RF_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_RF_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LH_LOWERLEG:
        switch(orig){
        case OriginFrame::LH_FOOT:
            return this->fr_LH_lowerleg_X_LH_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_LH_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RH_LOWERLEG:
        switch(orig){
        case OriginFrame::RH_FOOT:
            return this->fr_RH_lowerleg_X_RH_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_RH_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::LF_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::LF_UPPERLEG:
            return this->fr_LF_hipassembly_X_fr_LF_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::RF_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::RF_UPPERLEG:
            return this->fr_RF_hipassembly_X_fr_RF_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LH_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::LH_UPPERLEG:
            return this->fr_LH_hipassembly_X_fr_LH_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RH_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::RH_UPPERLEG:
            return this->fr_RH_hipassembly_X_fr_RH_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::LF_UPPERLEG:
        switch(orig){
        case OriginFrame::LF_LOWERLEG:
            return this->fr_LF_upperleg_X_fr_LF_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::RF_UPPERLEG:
        switch(orig){
        case OriginFrame::RF_LOWERLEG:
            return this->fr_RF_upperleg_X_fr_RF_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::LH_UPPERLEG:
        switch(orig){
        case OriginFrame::LH_LOWERLEG:
            return this->fr_LH_upperleg_X_fr_LH_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::RH_UPPERLEG:
        switch(orig){
        case OriginFrame::RH_LOWERLEG:
            return this->fr_RH_upperleg_X_fr_RH_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    default:
        std::cerr<<"getTransform ERROR: transform not available" << std::endl;
        break;
    }
}





iit::HyQ::MotionTransforms::MotionTransforms
    (const dog::KinDynParams& getter_lengths)
     :
    fr_LF_lowerleg_X_LF_foot(values_lengths),
    fr_RF_lowerleg_X_RF_foot(values_lengths),
    fr_LH_lowerleg_X_LH_foot(values_lengths),
    fr_RH_lowerleg_X_RH_foot(values_lengths),
    fr_trunk_X_LF_hipassemblyCOM(),
    fr_trunk_X_RF_hipassemblyCOM(),
    fr_trunk_X_LH_hipassemblyCOM(),
    fr_trunk_X_RH_hipassemblyCOM(),
    fr_trunk_X_LF_upperlegCOM(),
    fr_trunk_X_RF_upperlegCOM(),
    fr_trunk_X_LH_upperlegCOM(),
    fr_trunk_X_RH_upperlegCOM(),
    fr_trunk_X_LF_lowerlegCOM(),
    fr_trunk_X_RF_lowerlegCOM(),
    fr_trunk_X_LH_lowerlegCOM(),
    fr_trunk_X_RH_lowerlegCOM(),
    LF_foot_X_fr_LF_lowerleg(values_lengths),
    RF_foot_X_fr_RF_lowerleg(values_lengths),
    LH_foot_X_fr_LH_lowerleg(values_lengths),
    RH_foot_X_fr_RH_lowerleg(values_lengths),
    fr_trunk_X_LF_foot(values_lengths),
    fr_trunk_X_RF_foot(values_lengths),
    fr_trunk_X_LH_foot(values_lengths),
    fr_trunk_X_RH_foot(values_lengths),
    fr_LF_lowerleg_X_fr_trunk(),
    fr_RF_lowerleg_X_fr_trunk(),
    fr_LH_lowerleg_X_fr_trunk(),
    fr_RH_lowerleg_X_fr_trunk(),
    LF_foot_X_fr_trunk(values_lengths),
    RF_foot_X_fr_trunk(values_lengths),
    LH_foot_X_fr_trunk(values_lengths),
    RH_foot_X_fr_trunk(values_lengths),
    fr_trunk_X_LF_shin(values_lengths),
    fr_trunk_X_RF_shin(values_lengths),
    fr_trunk_X_LH_shin(values_lengths),
    fr_trunk_X_RH_shin(values_lengths),
    fr_trunk_X_fr_LF_HAA(),
    fr_trunk_X_fr_LF_HFE(),
    fr_trunk_X_fr_LF_KFE(),
    fr_trunk_X_fr_RF_HAA(),
    fr_trunk_X_fr_RF_HFE(),
    fr_trunk_X_fr_RF_KFE(),
    fr_trunk_X_fr_LH_HAA(),
    fr_trunk_X_fr_LH_HFE(),
    fr_trunk_X_fr_LH_KFE(),
    fr_trunk_X_fr_RH_HAA(),
    fr_trunk_X_fr_RH_HFE(),
    fr_trunk_X_fr_RH_KFE(),
    fr_LF_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_LF_hipassembly(),
    fr_LF_upperleg_X_fr_LF_hipassembly(),
    fr_LF_hipassembly_X_fr_LF_upperleg(),
    fr_LF_lowerleg_X_fr_LF_upperleg(),
    fr_LF_upperleg_X_fr_LF_lowerleg(),
    fr_RF_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_RF_hipassembly(),
    fr_RF_upperleg_X_fr_RF_hipassembly(),
    fr_RF_hipassembly_X_fr_RF_upperleg(),
    fr_RF_lowerleg_X_fr_RF_upperleg(),
    fr_RF_upperleg_X_fr_RF_lowerleg(),
    fr_LH_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_LH_hipassembly(),
    fr_LH_upperleg_X_fr_LH_hipassembly(),
    fr_LH_hipassembly_X_fr_LH_upperleg(),
    fr_LH_lowerleg_X_fr_LH_upperleg(),
    fr_LH_upperleg_X_fr_LH_lowerleg(),
    fr_RH_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_RH_hipassembly(),
    fr_RH_upperleg_X_fr_RH_hipassembly(),
    fr_RH_hipassembly_X_fr_RH_upperleg(),
    fr_RH_lowerleg_X_fr_RH_upperleg(),
    fr_RH_upperleg_X_fr_RH_lowerleg()
    ,
    valuesGetter_lengths(& getter_lengths)
{
    updateParameters();
}



void iit::HyQ::MotionTransforms::updateParameters() {
    values_lengths.foot_x = valuesGetter_lengths -> getValue_foot_x();
    values_lengths.LF_shin = valuesGetter_lengths -> getValue_LF_shin();
    values_lengths.RF_shin = valuesGetter_lengths -> getValue_RF_shin();
    values_lengths.LH_shin = valuesGetter_lengths -> getValue_LH_shin();
    values_lengths.RH_shin = valuesGetter_lengths -> getValue_RH_shin();
}

SpatialTransformPlain ForceTransforms::getTransform(const dog::JointState &q,
                                                     const dog::OriginFrame &orig,
                                                     const dog::DestFrame &dest){
    this->updateParameters();
    switch(dest){
    case DestFrame::TRUNK:
        switch(orig){
        case OriginFrame::LF_HIPASSEMBLY_COM:
            return this->fr_trunk_X_LF_hipassemblyCOM(q);
        case OriginFrame::RF_HIPASSEMBLY_COM:
            return this->fr_trunk_X_RF_hipassemblyCOM(q);
        case OriginFrame::LH_HIPASSEMBLY_COM:
            return this->fr_trunk_X_LH_hipassemblyCOM(q);
        case OriginFrame::RH_HIPASSEMBLY_COM:
            return this->fr_trunk_X_RH_hipassemblyCOM(q);
        case OriginFrame::LF_HIPASSEMBLY:
            return this->fr_trunk_X_fr_LF_hipassembly(q);
        case OriginFrame::RF_HIPASSEMBLY:
            return this->fr_trunk_X_fr_RF_hipassembly(q);
        case OriginFrame::LH_HIPASSEMBLY:
            return this->fr_trunk_X_fr_LH_hipassembly(q);
        case OriginFrame::RH_HIPASSEMBLY:
            return this->fr_trunk_X_fr_RH_hipassembly(q);
        case OriginFrame::LF_UPPERLEG_COM:
            return this->fr_trunk_X_LF_upperlegCOM(q);
        case OriginFrame::RF_UPPERLEG_COM:
            return this->fr_trunk_X_RF_upperlegCOM(q);
        case OriginFrame::LH_UPPERLEG_COM:
            return this->fr_trunk_X_LH_upperlegCOM(q);
        case OriginFrame::RH_UPPERLEG_COM:
            return this->fr_trunk_X_RH_upperlegCOM(q);

        case OriginFrame::LF_LOWERLEG_COM:
            return this->fr_trunk_X_LF_lowerlegCOM(q);
        case OriginFrame::RF_LOWERLEG_COM:
            return this->fr_trunk_X_RF_lowerlegCOM(q);
        case OriginFrame::LH_LOWERLEG_COM:
            return this->fr_trunk_X_LH_lowerlegCOM(q);
        case OriginFrame::RH_LOWERLEG_COM:
            return this->fr_trunk_X_RH_lowerlegCOM(q);

        case OriginFrame::LF_FOOT:
            return this->fr_trunk_X_LF_foot(q);
        case OriginFrame::RF_FOOT:
            return this->fr_trunk_X_RF_foot(q);
        case OriginFrame::LH_FOOT:
            return this->fr_trunk_X_LH_foot(q);
        case OriginFrame::RH_FOOT:
            return this->fr_trunk_X_RH_foot(q);

        case OriginFrame::LF_SHIN:
            return this->fr_trunk_X_LF_shin(q);
        case OriginFrame::RF_SHIN:
            return this->fr_trunk_X_RF_shin(q);
        case OriginFrame::LH_SHIN:
            return this->fr_trunk_X_LH_shin(q);
        case OriginFrame::RH_SHIN:
            return this->fr_trunk_X_RH_shin(q);

        case OriginFrame::LF_HAA:
            return this->fr_trunk_X_fr_LF_HAA(q);
        case OriginFrame::RF_HAA:
            return this->fr_trunk_X_fr_RF_HAA(q);
        case OriginFrame::LH_HAA:
            return this->fr_trunk_X_fr_LH_HAA(q);
        case OriginFrame::RH_HAA:
            return this->fr_trunk_X_fr_RH_HAA(q);

        case OriginFrame::LF_HFE:
            return this->fr_trunk_X_fr_LF_HFE(q);
        case OriginFrame::RF_HFE:
            return this->fr_trunk_X_fr_RF_HFE(q);
        case OriginFrame::LH_HFE:
            return this->fr_trunk_X_fr_LH_HFE(q);
        case OriginFrame::RH_HFE:
            return this->fr_trunk_X_fr_RH_HFE(q);

        case OriginFrame::LF_KFE:
            return this->fr_trunk_X_fr_LF_KFE(q);
        case OriginFrame::RF_KFE:
            return this->fr_trunk_X_fr_RF_KFE(q);
        case OriginFrame::LH_KFE:
            return this->fr_trunk_X_fr_LH_KFE(q);
        case OriginFrame::RH_KFE:
            return this->fr_trunk_X_fr_RH_KFE(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LF_FOOT:
        switch(orig){
        case OriginFrame::LF_LOWERLEG:
            return this->LF_foot_X_fr_LF_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->LF_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
            break;
        }
        break;

    case DestFrame::RF_FOOT:
        switch(orig){
        case OriginFrame::RF_LOWERLEG:
            return this->RF_foot_X_fr_RF_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->RF_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LH_FOOT:
        switch(orig){
        case OriginFrame::LH_LOWERLEG:
            return this->LH_foot_X_fr_LH_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->LH_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RH_FOOT:
        switch(orig){
        case OriginFrame::RH_LOWERLEG:
            return this->RH_foot_X_fr_RH_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->RH_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LF_LOWERLEG:
        switch(orig){
        case OriginFrame::LF_FOOT:
            return this->fr_LF_lowerleg_X_LF_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_LF_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RF_LOWERLEG:
        switch(orig){
        case OriginFrame::RF_FOOT:
            return this->fr_RF_lowerleg_X_RF_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_RF_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LH_LOWERLEG:
        switch(orig){
        case OriginFrame::LH_FOOT:
            return this->fr_LH_lowerleg_X_LH_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_LH_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RH_LOWERLEG:
        switch(orig){
        case OriginFrame::RH_FOOT:
            return this->fr_RH_lowerleg_X_RH_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_RH_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::LF_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::LF_UPPERLEG:
            return this->fr_LF_hipassembly_X_fr_LF_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::RF_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::RF_UPPERLEG:
            return this->fr_RF_hipassembly_X_fr_RF_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LH_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::LH_UPPERLEG:
            return this->fr_LH_hipassembly_X_fr_LH_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RH_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::RH_UPPERLEG:
            return this->fr_RH_hipassembly_X_fr_RH_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::LF_UPPERLEG:
        switch(orig){
        case OriginFrame::LF_LOWERLEG:
            return this->fr_LF_upperleg_X_fr_LF_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::RF_UPPERLEG:
        switch(orig){
        case OriginFrame::RF_LOWERLEG:
            return this->fr_RF_upperleg_X_fr_RF_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::LH_UPPERLEG:
        switch(orig){
        case OriginFrame::LH_LOWERLEG:
            return this->fr_LH_upperleg_X_fr_LH_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::RH_UPPERLEG:
        switch(orig){
        case OriginFrame::RH_LOWERLEG:
            return this->fr_RH_upperleg_X_fr_RH_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    default:
        std::cerr<<"getTransform ERROR: transform not available" << std::endl;
        break;
    }
}


iit::HyQ::ForceTransforms::ForceTransforms
    (const dog::KinDynParams& getter_lengths)
     :
    fr_LF_lowerleg_X_LF_foot(values_lengths),
    fr_RF_lowerleg_X_RF_foot(values_lengths),
    fr_LH_lowerleg_X_LH_foot(values_lengths),
    fr_RH_lowerleg_X_RH_foot(values_lengths),
    fr_trunk_X_LF_hipassemblyCOM(),
    fr_trunk_X_RF_hipassemblyCOM(),
    fr_trunk_X_LH_hipassemblyCOM(),
    fr_trunk_X_RH_hipassemblyCOM(),
    fr_trunk_X_LF_upperlegCOM(),
    fr_trunk_X_RF_upperlegCOM(),
    fr_trunk_X_LH_upperlegCOM(),
    fr_trunk_X_RH_upperlegCOM(),
    fr_trunk_X_LF_lowerlegCOM(),
    fr_trunk_X_RF_lowerlegCOM(),
    fr_trunk_X_LH_lowerlegCOM(),
    fr_trunk_X_RH_lowerlegCOM(),
    LF_foot_X_fr_LF_lowerleg(values_lengths),
    RF_foot_X_fr_RF_lowerleg(values_lengths),
    LH_foot_X_fr_LH_lowerleg(values_lengths),
    RH_foot_X_fr_RH_lowerleg(values_lengths),
    fr_trunk_X_LF_foot(values_lengths),
    fr_trunk_X_RF_foot(values_lengths),
    fr_trunk_X_LH_foot(values_lengths),
    fr_trunk_X_RH_foot(values_lengths),
    fr_LF_lowerleg_X_fr_trunk(),
    fr_RF_lowerleg_X_fr_trunk(),
    fr_LH_lowerleg_X_fr_trunk(),
    fr_RH_lowerleg_X_fr_trunk(),
    LF_foot_X_fr_trunk(values_lengths),
    RF_foot_X_fr_trunk(values_lengths),
    LH_foot_X_fr_trunk(values_lengths),
    RH_foot_X_fr_trunk(values_lengths),
    fr_trunk_X_LF_shin(values_lengths),
    fr_trunk_X_RF_shin(values_lengths),
    fr_trunk_X_LH_shin(values_lengths),
    fr_trunk_X_RH_shin(values_lengths),
    fr_trunk_X_fr_LF_HAA(),
    fr_trunk_X_fr_LF_HFE(),
    fr_trunk_X_fr_LF_KFE(),
    fr_trunk_X_fr_RF_HAA(),
    fr_trunk_X_fr_RF_HFE(),
    fr_trunk_X_fr_RF_KFE(),
    fr_trunk_X_fr_LH_HAA(),
    fr_trunk_X_fr_LH_HFE(),
    fr_trunk_X_fr_LH_KFE(),
    fr_trunk_X_fr_RH_HAA(),
    fr_trunk_X_fr_RH_HFE(),
    fr_trunk_X_fr_RH_KFE(),
    fr_LF_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_LF_hipassembly(),
    fr_LF_upperleg_X_fr_LF_hipassembly(),
    fr_LF_hipassembly_X_fr_LF_upperleg(),
    fr_LF_lowerleg_X_fr_LF_upperleg(),
    fr_LF_upperleg_X_fr_LF_lowerleg(),
    fr_RF_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_RF_hipassembly(),
    fr_RF_upperleg_X_fr_RF_hipassembly(),
    fr_RF_hipassembly_X_fr_RF_upperleg(),
    fr_RF_lowerleg_X_fr_RF_upperleg(),
    fr_RF_upperleg_X_fr_RF_lowerleg(),
    fr_LH_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_LH_hipassembly(),
    fr_LH_upperleg_X_fr_LH_hipassembly(),
    fr_LH_hipassembly_X_fr_LH_upperleg(),
    fr_LH_lowerleg_X_fr_LH_upperleg(),
    fr_LH_upperleg_X_fr_LH_lowerleg(),
    fr_RH_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_RH_hipassembly(),
    fr_RH_upperleg_X_fr_RH_hipassembly(),
    fr_RH_hipassembly_X_fr_RH_upperleg(),
    fr_RH_lowerleg_X_fr_RH_upperleg(),
    fr_RH_upperleg_X_fr_RH_lowerleg()
    ,
    valuesGetter_lengths(& getter_lengths)
{
    updateParameters();
}
void iit::HyQ::ForceTransforms::updateParameters() {
    values_lengths.foot_x = valuesGetter_lengths -> getValue_foot_x();
    values_lengths.LF_shin = valuesGetter_lengths -> getValue_LF_shin();
    values_lengths.RF_shin = valuesGetter_lengths -> getValue_RF_shin();
    values_lengths.LH_shin = valuesGetter_lengths -> getValue_LH_shin();
    values_lengths.RH_shin = valuesGetter_lengths -> getValue_RH_shin();
}


HomogeneousTransformPlain HomogeneousTransforms::getTransform(const dog::JointState &q,
                                                     const dog::OriginFrame &orig,
                                                     const dog::DestFrame &dest){
    this->updateParameters();
    switch(dest){
    case DestFrame::TRUNK:
        switch(orig){
        case OriginFrame::LF_HIPASSEMBLY_COM:
            return this->fr_trunk_X_LF_hipassemblyCOM(q);
        case OriginFrame::RF_HIPASSEMBLY_COM:
            return this->fr_trunk_X_RF_hipassemblyCOM(q);
        case OriginFrame::LH_HIPASSEMBLY_COM:
            return this->fr_trunk_X_LH_hipassemblyCOM(q);
        case OriginFrame::RH_HIPASSEMBLY_COM:
            return this->fr_trunk_X_RH_hipassemblyCOM(q);

        case OriginFrame::LF_HIPASSEMBLY:
            return this->fr_trunk_X_fr_LF_hipassembly(q);
        case OriginFrame::RF_HIPASSEMBLY:
            return this->fr_trunk_X_fr_RF_hipassembly(q);
        case OriginFrame::LH_HIPASSEMBLY:
            return this->fr_trunk_X_fr_LH_hipassembly(q);
        case OriginFrame::RH_HIPASSEMBLY:
            return this->fr_trunk_X_fr_RH_hipassembly(q);


        case OriginFrame::LF_UPPERLEG_COM:
            return this->fr_trunk_X_LF_upperlegCOM(q);
        case OriginFrame::RF_UPPERLEG_COM:
            return this->fr_trunk_X_RF_upperlegCOM(q);
        case OriginFrame::LH_UPPERLEG_COM:
            return this->fr_trunk_X_LH_upperlegCOM(q);
        case OriginFrame::RH_UPPERLEG_COM:
            return this->fr_trunk_X_RH_upperlegCOM(q);

        case OriginFrame::LF_LOWERLEG_COM:
            return this->fr_trunk_X_LF_lowerlegCOM(q);
        case OriginFrame::RF_LOWERLEG_COM:
            return this->fr_trunk_X_RF_lowerlegCOM(q);
        case OriginFrame::LH_LOWERLEG_COM:
            return this->fr_trunk_X_LH_lowerlegCOM(q);
        case OriginFrame::RH_LOWERLEG_COM:
            return this->fr_trunk_X_RH_lowerlegCOM(q);

        case OriginFrame::LF_FOOT:
            return this->fr_trunk_X_LF_foot(q);
        case OriginFrame::RF_FOOT:
            return this->fr_trunk_X_RF_foot(q);
        case OriginFrame::LH_FOOT:
            return this->fr_trunk_X_LH_foot(q);
        case OriginFrame::RH_FOOT:
            return this->fr_trunk_X_RH_foot(q);

        case OriginFrame::LF_SHIN:
            return this->fr_trunk_X_LF_shin(q);
        case OriginFrame::RF_SHIN:
            return this->fr_trunk_X_RF_shin(q);
        case OriginFrame::LH_SHIN:
            return this->fr_trunk_X_LH_shin(q);
        case OriginFrame::RH_SHIN:
            return this->fr_trunk_X_RH_shin(q);

        case OriginFrame::LF_HAA:
            return this->fr_trunk_X_fr_LF_HAA(q);
        case OriginFrame::RF_HAA:
            return this->fr_trunk_X_fr_RF_HAA(q);
        case OriginFrame::LH_HAA:
            return this->fr_trunk_X_fr_LH_HAA(q);
        case OriginFrame::RH_HAA:
            return this->fr_trunk_X_fr_RH_HAA(q);

        case OriginFrame::LF_HFE:
            return this->fr_trunk_X_fr_LF_HFE(q);
        case OriginFrame::RF_HFE:
            return this->fr_trunk_X_fr_RF_HFE(q);
        case OriginFrame::LH_HFE:
            return this->fr_trunk_X_fr_LH_HFE(q);
        case OriginFrame::RH_HFE:
            return this->fr_trunk_X_fr_RH_HFE(q);

        case OriginFrame::LF_KFE:
            return this->fr_trunk_X_fr_LF_KFE(q);
        case OriginFrame::RF_KFE:
            return this->fr_trunk_X_fr_RF_KFE(q);
        case OriginFrame::LH_KFE:
            return this->fr_trunk_X_fr_LH_KFE(q);
        case OriginFrame::RH_KFE:
            return this->fr_trunk_X_fr_RH_KFE(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LF_FOOT:
        switch(orig){
        case OriginFrame::LF_LOWERLEG:
            return this->LF_foot_X_fr_LF_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->LF_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
            break;
        }
        break;

    case DestFrame::RF_FOOT:
        switch(orig){
        case OriginFrame::RF_LOWERLEG:
            return this->RF_foot_X_fr_RF_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->RF_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LH_FOOT:
        switch(orig){
        case OriginFrame::LH_LOWERLEG:
            return this->LH_foot_X_fr_LH_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->LH_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RH_FOOT:
        switch(orig){
        case OriginFrame::RH_LOWERLEG:
            return this->RH_foot_X_fr_RH_lowerleg(q);
        case OriginFrame::TRUNK:
            return this->RH_foot_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LF_LOWERLEG:
        switch(orig){
        case OriginFrame::LF_FOOT:
            return this->fr_LF_lowerleg_X_LF_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_LF_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RF_LOWERLEG:
        switch(orig){
        case OriginFrame::RF_FOOT:
            return this->fr_RF_lowerleg_X_RF_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_RF_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LH_LOWERLEG:
        switch(orig){
        case OriginFrame::LH_FOOT:
            return this->fr_LH_lowerleg_X_LH_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_LH_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RH_LOWERLEG:
        switch(orig){
        case OriginFrame::RH_FOOT:
            return this->fr_RH_lowerleg_X_RH_foot(q);
        case OriginFrame::TRUNK:
            return this->fr_RH_lowerleg_X_fr_trunk(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::LF_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::LF_UPPERLEG:
            return this->fr_LF_hipassembly_X_fr_LF_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::RF_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::RF_UPPERLEG:
            return this->fr_RF_hipassembly_X_fr_RF_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::LH_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::LH_UPPERLEG:
            return this->fr_LH_hipassembly_X_fr_LH_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;

    case DestFrame::RH_HIPASSEMBLY:
        switch(orig){
        case OriginFrame::RH_UPPERLEG:
            return this->fr_RH_hipassembly_X_fr_RH_upperleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::LF_UPPERLEG:
        switch(orig){
        case OriginFrame::LF_LOWERLEG:
            return this->fr_LF_upperleg_X_fr_LF_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::RF_UPPERLEG:
        switch(orig){
        case OriginFrame::RF_LOWERLEG:
            return this->fr_RF_upperleg_X_fr_RF_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::LH_UPPERLEG:
        switch(orig){
        case OriginFrame::LH_LOWERLEG:
            return this->fr_LH_upperleg_X_fr_LH_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    case DestFrame::RH_UPPERLEG:
        switch(orig){
        case OriginFrame::RH_LOWERLEG:
            return this->fr_RH_upperleg_X_fr_RH_lowerleg(q);
        default:
            std::cerr<<"getTransform ERROR: transform not available" << std::endl;
           break;
        }
        break;
    default:
       std::cerr<<"getTransform ERROR: transform not available" << std::endl;
        break;
    }
}



iit::HyQ::HomogeneousTransforms::HomogeneousTransforms
    (const KinDynParams &getter_lengths)
     :
    fr_LF_lowerleg_X_LF_foot(values_lengths),
    fr_RF_lowerleg_X_RF_foot(values_lengths),
    fr_LH_lowerleg_X_LH_foot(values_lengths),
    fr_RH_lowerleg_X_RH_foot(values_lengths),
    fr_trunk_X_LF_hipassemblyCOM(),
    fr_trunk_X_RF_hipassemblyCOM(),
    fr_trunk_X_LH_hipassemblyCOM(),
    fr_trunk_X_RH_hipassemblyCOM(),
    fr_trunk_X_LF_upperlegCOM(),
    fr_trunk_X_RF_upperlegCOM(),
    fr_trunk_X_LH_upperlegCOM(),
    fr_trunk_X_RH_upperlegCOM(),
    fr_trunk_X_LF_lowerlegCOM(),
    fr_trunk_X_RF_lowerlegCOM(),
    fr_trunk_X_LH_lowerlegCOM(),
    fr_trunk_X_RH_lowerlegCOM(),
    LF_foot_X_fr_LF_lowerleg(values_lengths),
    RF_foot_X_fr_RF_lowerleg(values_lengths),
    LH_foot_X_fr_LH_lowerleg(values_lengths),
    RH_foot_X_fr_RH_lowerleg(values_lengths),
    fr_trunk_X_LF_foot(values_lengths),
    fr_trunk_X_RF_foot(values_lengths),
    fr_trunk_X_LH_foot(values_lengths),
    fr_trunk_X_RH_foot(values_lengths),
    fr_LF_lowerleg_X_fr_trunk(),
    fr_RF_lowerleg_X_fr_trunk(),
    fr_LH_lowerleg_X_fr_trunk(),
    fr_RH_lowerleg_X_fr_trunk(),
    LF_foot_X_fr_trunk(values_lengths),
    RF_foot_X_fr_trunk(values_lengths),
    LH_foot_X_fr_trunk(values_lengths),
    RH_foot_X_fr_trunk(values_lengths),
    fr_trunk_X_LF_shin(values_lengths),
    fr_trunk_X_RF_shin(values_lengths),
    fr_trunk_X_LH_shin(values_lengths),
    fr_trunk_X_RH_shin(values_lengths),
    fr_trunk_X_fr_LF_HAA(),
    fr_trunk_X_fr_LF_HFE(),
    fr_trunk_X_fr_LF_KFE(),
    fr_trunk_X_fr_RF_HAA(),
    fr_trunk_X_fr_RF_HFE(),
    fr_trunk_X_fr_RF_KFE(),
    fr_trunk_X_fr_LH_HAA(),
    fr_trunk_X_fr_LH_HFE(),
    fr_trunk_X_fr_LH_KFE(),
    fr_trunk_X_fr_RH_HAA(),
    fr_trunk_X_fr_RH_HFE(),
    fr_trunk_X_fr_RH_KFE(),
    fr_LF_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_LF_hipassembly(),
    fr_LF_upperleg_X_fr_LF_hipassembly(),
    fr_LF_hipassembly_X_fr_LF_upperleg(),
    fr_LF_lowerleg_X_fr_LF_upperleg(),
    fr_LF_upperleg_X_fr_LF_lowerleg(),
    fr_RF_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_RF_hipassembly(),
    fr_RF_upperleg_X_fr_RF_hipassembly(),
    fr_RF_hipassembly_X_fr_RF_upperleg(),
    fr_RF_lowerleg_X_fr_RF_upperleg(),
    fr_RF_upperleg_X_fr_RF_lowerleg(),
    fr_LH_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_LH_hipassembly(),
    fr_LH_upperleg_X_fr_LH_hipassembly(),
    fr_LH_hipassembly_X_fr_LH_upperleg(),
    fr_LH_lowerleg_X_fr_LH_upperleg(),
    fr_LH_upperleg_X_fr_LH_lowerleg(),
    fr_RH_hipassembly_X_fr_trunk(),
    fr_trunk_X_fr_RH_hipassembly(),
    fr_RH_upperleg_X_fr_RH_hipassembly(),
    fr_RH_hipassembly_X_fr_RH_upperleg(),
    fr_RH_lowerleg_X_fr_RH_upperleg(),
    fr_RH_upperleg_X_fr_RH_lowerleg()
    ,
    valuesGetter_lengths(& getter_lengths)
{
    updateParameters();
}
void iit::HyQ::HomogeneousTransforms::updateParameters() {
    values_lengths.foot_x = valuesGetter_lengths -> getValue_foot_x();
    values_lengths.LF_shin = valuesGetter_lengths -> getValue_LF_shin();
    values_lengths.RF_shin = valuesGetter_lengths -> getValue_RF_shin();
    values_lengths.LH_shin = valuesGetter_lengths -> getValue_LH_shin();
    values_lengths.RH_shin = valuesGetter_lengths -> getValue_RH_shin();
}

iit::HyQ::MotionTransforms::Type_fr_LF_lowerleg_X_LF_foot::Type_fr_LF_lowerleg_X_LF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = - 1;
    (*this)(4,4) = 0;
    (*this)(4,5) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LF_lowerleg_X_LF_foot& iit::HyQ::MotionTransforms::Type_fr_LF_lowerleg_X_LF_foot::update(const state_t& q) {
    
    
    (*this)(4,1) = - lengths_values -> foot_x;
    (*this)(5,0) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RF_lowerleg_X_RF_foot::Type_fr_RF_lowerleg_X_RF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = - 1;
    (*this)(4,4) = 0;
    (*this)(4,5) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RF_lowerleg_X_RF_foot& iit::HyQ::MotionTransforms::Type_fr_RF_lowerleg_X_RF_foot::update(const state_t& q) {
    
    
    (*this)(4,1) = - lengths_values -> foot_x;
    (*this)(5,0) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LH_lowerleg_X_LH_foot::Type_fr_LH_lowerleg_X_LH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = - 1;
    (*this)(4,4) = 0;
    (*this)(4,5) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LH_lowerleg_X_LH_foot& iit::HyQ::MotionTransforms::Type_fr_LH_lowerleg_X_LH_foot::update(const state_t& q) {
    
    
    (*this)(4,1) = - lengths_values -> foot_x;
    (*this)(5,0) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RH_lowerleg_X_RH_foot::Type_fr_RH_lowerleg_X_RH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = - 1;
    (*this)(4,4) = 0;
    (*this)(4,5) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RH_lowerleg_X_RH_foot& iit::HyQ::MotionTransforms::Type_fr_RH_lowerleg_X_RH_foot::update(const state_t& q) {
    
    
    (*this)(4,1) = - lengths_values -> foot_x;
    (*this)(5,0) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_hipassemblyCOM::Type_fr_trunk_X_LF_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,5) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_hipassemblyCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(1,0) = - sin__q_LF_HAA__;
    (*this)(1,1) = - cos__q_LF_HAA__;
    (*this)(2,0) = - cos__q_LF_HAA__;
    (*this)(2,1) =  sin__q_LF_HAA__;
    (*this)(3,0) = (- 0.207 *  cos__q_LF_HAA__);
    (*this)(3,1) = (( 0.207 *  sin__q_LF_HAA__) -  0.04263);
    (*this)(4,0) = ( 0.2041 *  cos__q_LF_HAA__);
    (*this)(4,1) = (- 0.2041 *  sin__q_LF_HAA__);
    (*this)(4,2) = ( 0.04263 *  cos__q_LF_HAA__);
    (*this)(4,3) = - sin__q_LF_HAA__;
    (*this)(4,4) = - cos__q_LF_HAA__;
    (*this)(5,0) = (- 0.2041 *  sin__q_LF_HAA__);
    (*this)(5,1) = (- 0.2041 *  cos__q_LF_HAA__);
    (*this)(5,2) = ( 0.207 - ( 0.04263 *  sin__q_LF_HAA__));
    (*this)(5,3) = - cos__q_LF_HAA__;
    (*this)(5,4) =  sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_hipassemblyCOM::Type_fr_trunk_X_RF_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,5) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_hipassemblyCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(1,0) =  sin__q_RF_HAA__;
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(2,0) = - cos__q_RF_HAA__;
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(3,0) = ( 0.207 *  cos__q_RF_HAA__);
    (*this)(3,1) = ( 0.04263 - ( 0.207 *  sin__q_RF_HAA__));
    (*this)(4,0) = ( 0.2041 *  cos__q_RF_HAA__);
    (*this)(4,1) = (- 0.2041 *  sin__q_RF_HAA__);
    (*this)(4,2) = (- 0.04263 *  cos__q_RF_HAA__);
    (*this)(4,3) =  sin__q_RF_HAA__;
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(5,0) = ( 0.2041 *  sin__q_RF_HAA__);
    (*this)(5,1) = ( 0.2041 *  cos__q_RF_HAA__);
    (*this)(5,2) = ( 0.207 - ( 0.04263 *  sin__q_RF_HAA__));
    (*this)(5,3) = - cos__q_RF_HAA__;
    (*this)(5,4) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_hipassemblyCOM::Type_fr_trunk_X_LH_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,5) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_hipassemblyCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(1,0) = - sin__q_LH_HAA__;
    (*this)(1,1) = - cos__q_LH_HAA__;
    (*this)(2,0) = - cos__q_LH_HAA__;
    (*this)(2,1) =  sin__q_LH_HAA__;
    (*this)(3,0) = (- 0.207 *  cos__q_LH_HAA__);
    (*this)(3,1) = (( 0.207 *  sin__q_LH_HAA__) -  0.04263);
    (*this)(4,0) = (- 0.2041 *  cos__q_LH_HAA__);
    (*this)(4,1) = ( 0.2041 *  sin__q_LH_HAA__);
    (*this)(4,2) = ( 0.04263 *  cos__q_LH_HAA__);
    (*this)(4,3) = - sin__q_LH_HAA__;
    (*this)(4,4) = - cos__q_LH_HAA__;
    (*this)(5,0) = ( 0.2041 *  sin__q_LH_HAA__);
    (*this)(5,1) = ( 0.2041 *  cos__q_LH_HAA__);
    (*this)(5,2) = ( 0.207 - ( 0.04263 *  sin__q_LH_HAA__));
    (*this)(5,3) = - cos__q_LH_HAA__;
    (*this)(5,4) =  sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_hipassemblyCOM::Type_fr_trunk_X_RH_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,5) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_hipassemblyCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(1,0) =  sin__q_RH_HAA__;
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(2,0) = - cos__q_RH_HAA__;
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(3,0) = ( 0.207 *  cos__q_RH_HAA__);
    (*this)(3,1) = ( 0.04263 - ( 0.207 *  sin__q_RH_HAA__));
    (*this)(4,0) = (- 0.2041 *  cos__q_RH_HAA__);
    (*this)(4,1) = ( 0.2041 *  sin__q_RH_HAA__);
    (*this)(4,2) = (- 0.04263 *  cos__q_RH_HAA__);
    (*this)(4,3) =  sin__q_RH_HAA__;
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(5,0) = (- 0.2041 *  sin__q_RH_HAA__);
    (*this)(5,1) = (- 0.2041 *  cos__q_RH_HAA__);
    (*this)(5,2) = ( 0.207 - ( 0.04263 *  sin__q_RH_HAA__));
    (*this)(5,3) = - cos__q_RH_HAA__;
    (*this)(5,4) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_upperlegCOM::Type_fr_trunk_X_LF_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_upperlegCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_upperlegCOM::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = - sin__q_LF_HFE__;
    (*this)(0,1) = - cos__q_LF_HFE__;
    (*this)(1,0) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(1,1) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(2,0) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(2,1) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(3,0) = ((- 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__);
    (*this)(3,1) = (( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__);
    (*this)(3,2) = (((( 0.02625 *  sin__q_LF_HFE__) + ( 0.1507 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(3,3) = - sin__q_LF_HFE__;
    (*this)(3,4) = - cos__q_LF_HFE__;
    (*this)(4,0) = (((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.02625 *  cos__q_LF_HAA__));
    (*this)(4,1) = ((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.1507 *  cos__q_LF_HAA__));
    (*this)(4,2) = ((((- 0.1507 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.02625 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(4,3) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(4,4) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,0) = (((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.02625 *  sin__q_LF_HAA__));
    (*this)(5,1) = (((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.1507 *  sin__q_LF_HAA__));
    (*this)(5,2) = ((((- 0.1507 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.02625 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(5,3) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(5,4) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_upperlegCOM::Type_fr_trunk_X_RF_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_upperlegCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_upperlegCOM::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = - sin__q_RF_HFE__;
    (*this)(0,1) = - cos__q_RF_HFE__;
    (*this)(1,0) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(1,1) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(2,0) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(2,1) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(3,0) = (( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__);
    (*this)(3,1) = ((- 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__);
    (*this)(3,2) = (((( 0.02625 *  sin__q_RF_HFE__) + ( 0.1507 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(3,3) = - sin__q_RF_HFE__;
    (*this)(3,4) = - cos__q_RF_HFE__;
    (*this)(4,0) = (((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.02625 *  cos__q_RF_HAA__));
    (*this)(4,1) = ((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.1507 *  cos__q_RF_HAA__));
    (*this)(4,2) = (((( 0.1507 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.02625 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(4,3) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(4,4) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,0) = ((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.02625 *  sin__q_RF_HAA__));
    (*this)(5,1) = ((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.1507 *  sin__q_RF_HAA__));
    (*this)(5,2) = ((((- 0.1507 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.02625 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(5,3) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(5,4) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_upperlegCOM::Type_fr_trunk_X_LH_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_upperlegCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_upperlegCOM::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = - sin__q_LH_HFE__;
    (*this)(0,1) = - cos__q_LH_HFE__;
    (*this)(1,0) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(1,1) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(2,0) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(2,1) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(3,0) = ((- 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__);
    (*this)(3,1) = (( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__);
    (*this)(3,2) = ((((- 0.02625 *  sin__q_LH_HFE__) + ( 0.1507 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(3,3) = - sin__q_LH_HFE__;
    (*this)(3,4) = - cos__q_LH_HFE__;
    (*this)(4,0) = (((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.02625 *  cos__q_LH_HAA__));
    (*this)(4,1) = (((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.1507 *  cos__q_LH_HAA__));
    (*this)(4,2) = ((((- 0.1507 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.02625 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(4,3) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(4,4) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,0) = (((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.02625 *  sin__q_LH_HAA__));
    (*this)(5,1) = ((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.1507 *  sin__q_LH_HAA__));
    (*this)(5,2) = ((((- 0.1507 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.02625 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(5,3) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(5,4) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_upperlegCOM::Type_fr_trunk_X_RH_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_upperlegCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_upperlegCOM::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = - sin__q_RH_HFE__;
    (*this)(0,1) = - cos__q_RH_HFE__;
    (*this)(1,0) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(1,1) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(2,0) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(2,1) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(3,0) = (( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__);
    (*this)(3,1) = ((- 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__);
    (*this)(3,2) = ((((- 0.02625 *  sin__q_RH_HFE__) + ( 0.1507 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(3,3) = - sin__q_RH_HFE__;
    (*this)(3,4) = - cos__q_RH_HFE__;
    (*this)(4,0) = (((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.02625 *  cos__q_RH_HAA__));
    (*this)(4,1) = (((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.1507 *  cos__q_RH_HAA__));
    (*this)(4,2) = (((( 0.1507 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.02625 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(4,3) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(4,4) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,0) = ((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.02625 *  sin__q_RH_HAA__));
    (*this)(5,1) = (((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.1507 *  sin__q_RH_HAA__));
    (*this)(5,2) = ((((- 0.1507 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.02625 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(5,3) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(5,4) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_lowerlegCOM::Type_fr_trunk_X_LF_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_lowerlegCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_lowerlegCOM::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,1) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(1,0) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(2,0) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,1) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,2) = ((((((- 0.1254 *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + (( 0.1254 *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + ( 0.35 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(3,3) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(3,4) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(4,0) = ((((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(4,1) = ((((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__)) + ( 0.1254 *  cos__q_LF_HAA__));
    (*this)(4,2) = ((((((- 0.1254 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.1254 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(4,3) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,4) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,0) = (((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(5,1) = ((((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__)) - ( 0.1254 *  sin__q_LF_HAA__));
    (*this)(5,2) = ((((((- 0.1254 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.1254 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(5,3) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,4) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_lowerlegCOM::Type_fr_trunk_X_RF_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_lowerlegCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_lowerlegCOM::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,1) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(1,0) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(1,1) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(2,0) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(3,1) = ((((- 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(3,2) = ((((((- 0.1254 *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + (( 0.1254 *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + ( 0.35 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(3,3) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(3,4) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(4,0) = ((((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(4,1) = ((((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__)) + ( 0.1254 *  cos__q_RF_HAA__));
    (*this)(4,2) = (((((( 0.1254 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( 0.1254 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(4,3) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(4,4) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,0) = ((((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(5,1) = (((((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__)) + ( 0.1254 *  sin__q_RF_HAA__));
    (*this)(5,2) = ((((((- 0.1254 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.1254 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(5,3) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,4) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_lowerlegCOM::Type_fr_trunk_X_LH_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_lowerlegCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_lowerlegCOM::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,1) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(1,0) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(2,0) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,1) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,2) = ((((((- 0.1254 *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + (( 0.1254 *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + ( 0.35 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(3,3) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(3,4) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(4,0) = (((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(4,1) = (((((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__)) + ( 0.1254 *  cos__q_LH_HAA__));
    (*this)(4,2) = ((((((- 0.1254 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.1254 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(4,3) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,4) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,0) = ((((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(5,1) = ((((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__)) - ( 0.1254 *  sin__q_LH_HAA__));
    (*this)(5,2) = ((((((- 0.1254 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.1254 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(5,3) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,4) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_lowerlegCOM::Type_fr_trunk_X_RH_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_lowerlegCOM& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_lowerlegCOM::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,1) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(1,0) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(1,1) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(2,0) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(3,1) = ((((- 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(3,2) = ((((((- 0.1254 *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + (( 0.1254 *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + ( 0.35 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(3,3) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(3,4) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(4,0) = (((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(4,1) = (((((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__)) + ( 0.1254 *  cos__q_RH_HAA__));
    (*this)(4,2) = (((((( 0.1254 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( 0.1254 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(4,3) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(4,4) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,0) = (((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + ((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(5,1) = (((((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__)) + ( 0.1254 *  sin__q_RH_HAA__));
    (*this)(5,2) = ((((((- 0.1254 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.1254 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(5,3) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,4) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_LF_foot_X_fr_LF_lowerleg::Type_LF_foot_X_fr_LF_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_LF_foot_X_fr_LF_lowerleg& iit::HyQ::MotionTransforms::Type_LF_foot_X_fr_LF_lowerleg::update(const state_t& q) {
    
    
    (*this)(3,2) = - lengths_values -> foot_x;
    (*this)(4,1) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::MotionTransforms::Type_RF_foot_X_fr_RF_lowerleg::Type_RF_foot_X_fr_RF_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_RF_foot_X_fr_RF_lowerleg& iit::HyQ::MotionTransforms::Type_RF_foot_X_fr_RF_lowerleg::update(const state_t& q) {
    
    
    (*this)(3,2) = - lengths_values -> foot_x;
    (*this)(4,1) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::MotionTransforms::Type_LH_foot_X_fr_LH_lowerleg::Type_LH_foot_X_fr_LH_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_LH_foot_X_fr_LH_lowerleg& iit::HyQ::MotionTransforms::Type_LH_foot_X_fr_LH_lowerleg::update(const state_t& q) {
    
    
    (*this)(3,2) = - lengths_values -> foot_x;
    (*this)(4,1) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::MotionTransforms::Type_RH_foot_X_fr_RH_lowerleg::Type_RH_foot_X_fr_RH_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_RH_foot_X_fr_RH_lowerleg& iit::HyQ::MotionTransforms::Type_RH_foot_X_fr_RH_lowerleg::update(const state_t& q) {
    
    
    (*this)(3,2) = - lengths_values -> foot_x;
    (*this)(4,1) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_foot::Type_fr_trunk_X_LF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,4) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_foot& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_foot::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(0,2) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(1,0) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) =  cos__q_LF_HAA__;
    (*this)(1,2) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(2,0) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) = - sin__q_LF_HAA__;
    (*this)(2,2) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(3,0) = ((((- 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,1) = ((((((- lengths_values -> foot_x *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + (( lengths_values -> foot_x *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + ( 0.35 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(3,2) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(3,3) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(3,5) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(4,0) = (((((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__)) - ( lengths_values -> foot_x *  cos__q_LF_HAA__));
    (*this)(4,1) = ((((((- lengths_values -> foot_x *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(4,2) = (((((( 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + ((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(4,3) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,4) =  cos__q_LF_HAA__;
    (*this)(4,5) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(5,0) = (((((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + ((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  cos__q_LF_HFE__)) + ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__)) + ( lengths_values -> foot_x *  sin__q_LF_HAA__));
    (*this)(5,1) = ((((((- lengths_values -> foot_x *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(5,2) = ((((((- 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + ((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  cos__q_LF_HFE__)) + ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + ((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(5,3) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,4) = - sin__q_LF_HAA__;
    (*this)(5,5) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_foot::Type_fr_trunk_X_RF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,4) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_foot& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_foot::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(0,2) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(1,0) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,0) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(2,2) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(3,0) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(3,1) = ((((((- lengths_values -> foot_x *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + (( lengths_values -> foot_x *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + ( 0.35 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(3,2) = (((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(3,3) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(3,5) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(4,0) = (((((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + ((((( 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__)) - ( lengths_values -> foot_x *  cos__q_RF_HAA__));
    (*this)(4,1) = (((((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(4,2) = (((((( 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(4,3) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(4,5) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,0) = ((((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + ((((( 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) - ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__)) - ( lengths_values -> foot_x *  sin__q_RF_HAA__));
    (*this)(5,1) = ((((((- lengths_values -> foot_x *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(5,2) = (((((( 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) - ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(5,3) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,4) =  sin__q_RF_HAA__;
    (*this)(5,5) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_foot::Type_fr_trunk_X_LH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,4) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_foot& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_foot::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(0,2) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(1,0) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) =  cos__q_LH_HAA__;
    (*this)(1,2) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(2,0) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) = - sin__q_LH_HAA__;
    (*this)(2,2) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(3,0) = ((((- 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,1) = ((((((- lengths_values -> foot_x *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + (( lengths_values -> foot_x *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + ( 0.35 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(3,2) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(3,3) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(3,5) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(4,0) = (((((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__)) - ( lengths_values -> foot_x *  cos__q_LH_HAA__));
    (*this)(4,1) = ((((((- lengths_values -> foot_x *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(4,2) = ((((((- 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(4,3) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,4) =  cos__q_LH_HAA__;
    (*this)(4,5) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(5,0) = (((((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + ((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  cos__q_LH_HFE__)) + ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__)) + ( lengths_values -> foot_x *  sin__q_LH_HAA__));
    (*this)(5,1) = ((((((- lengths_values -> foot_x *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(5,2) = (((((( 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + ((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  cos__q_LH_HFE__)) + ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + ((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(5,3) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,4) = - sin__q_LH_HAA__;
    (*this)(5,5) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_foot::Type_fr_trunk_X_RH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,4) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_foot& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_foot::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(0,2) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(1,0) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,0) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(2,2) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(3,0) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(3,1) = ((((((- lengths_values -> foot_x *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + (( lengths_values -> foot_x *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + ( 0.35 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(3,2) = (((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(3,3) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(3,5) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(4,0) = (((((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + (((((- 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__)) - ( lengths_values -> foot_x *  cos__q_RH_HAA__));
    (*this)(4,1) = (((((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(4,2) = ((((((- 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(4,3) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(4,5) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,0) = ((((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + (((((- 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) - ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__)) - ( lengths_values -> foot_x *  sin__q_RH_HAA__));
    (*this)(5,1) = ((((((- lengths_values -> foot_x *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(5,2) = ((((((- 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) - ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(5,3) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,4) =  sin__q_RH_HAA__;
    (*this)(5,5) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LF_lowerleg_X_fr_trunk::Type_fr_LF_lowerleg_X_fr_trunk()
{
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(5,3) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LF_lowerleg_X_fr_trunk& iit::HyQ::MotionTransforms::Type_fr_LF_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,1) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,2) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,0) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(1,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,2) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) =  cos__q_LF_HAA__;
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,1) = ((((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(3,2) = (((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(3,3) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(3,4) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,5) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,0) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,1) = (((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__));
    (*this)(4,2) = (((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__));
    (*this)(4,3) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(4,4) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,5) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,0) = ((( 0.35 *  cos__q_LF_HFE__) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(5,1) = (( 0.3735 *  sin__q_LF_HAA__) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(5,2) = (( 0.3735 *  cos__q_LF_HAA__) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(5,4) =  cos__q_LF_HAA__;
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RF_lowerleg_X_fr_trunk::Type_fr_RF_lowerleg_X_fr_trunk()
{
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(5,3) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RF_lowerleg_X_fr_trunk& iit::HyQ::MotionTransforms::Type_fr_RF_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,1) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(0,2) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,0) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(1,1) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,2) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) =  cos__q_RF_HAA__;
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(3,1) = ((((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(3,2) = ((((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(3,3) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(3,4) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(3,5) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,0) = ((((- 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,1) = (((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__));
    (*this)(4,2) = ((((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__));
    (*this)(4,3) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(4,4) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,5) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,0) = ((( 0.35 *  cos__q_RF_HFE__) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(5,1) = ((( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(5,2) = (( 0.3735 *  cos__q_RF_HAA__) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__));
    (*this)(5,4) =  cos__q_RF_HAA__;
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LH_lowerleg_X_fr_trunk::Type_fr_LH_lowerleg_X_fr_trunk()
{
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(5,3) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LH_lowerleg_X_fr_trunk& iit::HyQ::MotionTransforms::Type_fr_LH_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,1) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,2) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,0) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(1,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,2) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) =  cos__q_LH_HAA__;
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,1) = (((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(3,2) = ((((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(3,3) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(3,4) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,5) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,0) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,1) = ((((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__));
    (*this)(4,2) = (((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__));
    (*this)(4,3) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(4,4) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,5) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,0) = ((( 0.35 *  cos__q_LH_HFE__) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(5,1) = (((- 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(5,2) = (((- 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(5,4) =  cos__q_LH_HAA__;
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RH_lowerleg_X_fr_trunk::Type_fr_RH_lowerleg_X_fr_trunk()
{
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(5,3) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RH_lowerleg_X_fr_trunk& iit::HyQ::MotionTransforms::Type_fr_RH_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,1) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(0,2) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,0) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(1,1) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,2) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) =  cos__q_RH_HAA__;
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(3,1) = (((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(3,2) = (((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + ((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(3,3) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(3,4) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(3,5) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,0) = ((((- 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,1) = ((((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__));
    (*this)(4,2) = ((((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__));
    (*this)(4,3) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(4,4) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,5) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,0) = ((( 0.35 *  cos__q_RH_HFE__) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(5,1) = ((( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(5,2) = (((- 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(5,4) =  cos__q_RH_HAA__;
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_LF_foot_X_fr_trunk::Type_LF_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(4,3) = 0;
}
const iit::HyQ::MotionTransforms::Type_LF_foot_X_fr_trunk& iit::HyQ::MotionTransforms::Type_LF_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(0,1) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,2) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) =  cos__q_LF_HAA__;
    (*this)(1,2) = - sin__q_LF_HAA__;
    (*this)(2,0) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(2,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(2,2) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(3,0) = ((((- 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,1) = (((((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__)) - ( lengths_values -> foot_x *  cos__q_LF_HAA__));
    (*this)(3,2) = (((((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + ((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  cos__q_LF_HFE__)) + ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__)) + ( lengths_values -> foot_x *  sin__q_LF_HAA__));
    (*this)(3,3) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(3,4) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,5) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,0) = ((((((- lengths_values -> foot_x *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + (( lengths_values -> foot_x *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + ( 0.35 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(4,1) = ((((((- lengths_values -> foot_x *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(4,2) = ((((((- lengths_values -> foot_x *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(4,4) =  cos__q_LF_HAA__;
    (*this)(4,5) = - sin__q_LF_HAA__;
    (*this)(5,0) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(5,1) = (((((( 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + ((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(5,2) = ((((((- 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + ((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  cos__q_LF_HFE__)) + ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + ((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(5,3) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(5,4) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(5,5) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    return *this;
}
iit::HyQ::MotionTransforms::Type_RF_foot_X_fr_trunk::Type_RF_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(4,3) = 0;
}
const iit::HyQ::MotionTransforms::Type_RF_foot_X_fr_trunk& iit::HyQ::MotionTransforms::Type_RF_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(0,1) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,2) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) =  sin__q_RF_HAA__;
    (*this)(2,0) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(2,1) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,2) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(3,0) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(3,1) = (((((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + ((((( 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__)) - ( lengths_values -> foot_x *  cos__q_RF_HAA__));
    (*this)(3,2) = ((((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + ((((( 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) - ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__)) - ( lengths_values -> foot_x *  sin__q_RF_HAA__));
    (*this)(3,3) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(3,4) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(3,5) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,0) = ((((((- lengths_values -> foot_x *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + (( lengths_values -> foot_x *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + ( 0.35 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(4,1) = (((((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(4,2) = ((((((- lengths_values -> foot_x *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(4,5) =  sin__q_RF_HAA__;
    (*this)(5,0) = (((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,1) = (((((( 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(5,2) = (((((( 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) - ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(5,3) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(5,4) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,5) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    return *this;
}
iit::HyQ::MotionTransforms::Type_LH_foot_X_fr_trunk::Type_LH_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(4,3) = 0;
}
const iit::HyQ::MotionTransforms::Type_LH_foot_X_fr_trunk& iit::HyQ::MotionTransforms::Type_LH_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(0,1) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,2) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) =  cos__q_LH_HAA__;
    (*this)(1,2) = - sin__q_LH_HAA__;
    (*this)(2,0) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(2,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(2,2) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(3,0) = ((((- 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,1) = (((((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__)) - ( lengths_values -> foot_x *  cos__q_LH_HAA__));
    (*this)(3,2) = (((((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + ((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  cos__q_LH_HFE__)) + ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__)) + ( lengths_values -> foot_x *  sin__q_LH_HAA__));
    (*this)(3,3) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(3,4) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,5) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,0) = ((((((- lengths_values -> foot_x *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + (( lengths_values -> foot_x *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + ( 0.35 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(4,1) = ((((((- lengths_values -> foot_x *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(4,2) = ((((((- lengths_values -> foot_x *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(4,4) =  cos__q_LH_HAA__;
    (*this)(4,5) = - sin__q_LH_HAA__;
    (*this)(5,0) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(5,1) = ((((((- 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(5,2) = (((((( 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + ((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  cos__q_LH_HFE__)) + ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + ((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(5,3) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(5,4) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(5,5) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    return *this;
}
iit::HyQ::MotionTransforms::Type_RH_foot_X_fr_trunk::Type_RH_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(4,3) = 0;
}
const iit::HyQ::MotionTransforms::Type_RH_foot_X_fr_trunk& iit::HyQ::MotionTransforms::Type_RH_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(0,1) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,2) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) =  sin__q_RH_HAA__;
    (*this)(2,0) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(2,1) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,2) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(3,0) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(3,1) = (((((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + (((((- 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__)) - ( lengths_values -> foot_x *  cos__q_RH_HAA__));
    (*this)(3,2) = ((((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + (((((- 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) - ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__)) - ( lengths_values -> foot_x *  sin__q_RH_HAA__));
    (*this)(3,3) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(3,4) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(3,5) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,0) = ((((((- lengths_values -> foot_x *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + (( lengths_values -> foot_x *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + ( 0.35 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(4,1) = (((((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(4,2) = ((((((- lengths_values -> foot_x *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(4,5) =  sin__q_RH_HAA__;
    (*this)(5,0) = (((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,1) = ((((((- 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(5,2) = ((((((- 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) - ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(5,3) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(5,4) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,5) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_shin::Type_fr_trunk_X_LF_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_shin& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LF_shin::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,1) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(1,0) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(2,0) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,1) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,2) = ((((((- lengths_values -> LF_shin *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + (( lengths_values -> LF_shin *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + ( 0.35 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(3,3) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(3,4) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(4,0) = ((((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(4,1) = ((((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__)) + ( lengths_values -> LF_shin *  cos__q_LF_HAA__));
    (*this)(4,2) = ((((((- lengths_values -> LF_shin *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> LF_shin *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(4,3) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,4) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,0) = (((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(5,1) = ((((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__)) - ( lengths_values -> LF_shin *  sin__q_LF_HAA__));
    (*this)(5,2) = ((((((- lengths_values -> LF_shin *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(5,3) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,4) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_shin::Type_fr_trunk_X_RF_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_shin& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RF_shin::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,1) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(1,0) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(1,1) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(2,0) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(3,1) = ((((- 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(3,2) = ((((((- lengths_values -> RF_shin *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + (( lengths_values -> RF_shin *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + ( 0.35 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(3,3) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(3,4) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(4,0) = ((((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(4,1) = ((((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__)) + ( lengths_values -> RF_shin *  cos__q_RF_HAA__));
    (*this)(4,2) = (((((( lengths_values -> RF_shin *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> RF_shin *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(4,3) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(4,4) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,0) = ((((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(5,1) = (((((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__)) + ( lengths_values -> RF_shin *  sin__q_RF_HAA__));
    (*this)(5,2) = ((((((- lengths_values -> RF_shin *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> RF_shin *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(5,3) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,4) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_shin::Type_fr_trunk_X_LH_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_shin& iit::HyQ::MotionTransforms::Type_fr_trunk_X_LH_shin::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,1) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(1,0) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(2,0) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,1) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,2) = ((((((- lengths_values -> LH_shin *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + (( lengths_values -> LH_shin *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + ( 0.35 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(3,3) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(3,4) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(4,0) = (((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(4,1) = (((((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__)) + ( lengths_values -> LH_shin *  cos__q_LH_HAA__));
    (*this)(4,2) = ((((((- lengths_values -> LH_shin *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> LH_shin *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(4,3) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,4) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,0) = ((((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(5,1) = ((((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__)) - ( lengths_values -> LH_shin *  sin__q_LH_HAA__));
    (*this)(5,2) = ((((((- lengths_values -> LH_shin *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(5,3) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,4) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_shin::Type_fr_trunk_X_RH_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_shin& iit::HyQ::MotionTransforms::Type_fr_trunk_X_RH_shin::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,1) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(1,0) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(1,1) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(2,0) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(3,0) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(3,1) = ((((- 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(3,2) = ((((((- lengths_values -> RH_shin *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + (( lengths_values -> RH_shin *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + ( 0.35 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(3,3) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(3,4) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(4,0) = (((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(4,1) = (((((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__)) + ( lengths_values -> RH_shin *  cos__q_RH_HAA__));
    (*this)(4,2) = (((((( lengths_values -> RH_shin *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> RH_shin *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(4,3) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(4,4) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,0) = (((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + ((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(5,1) = (((((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__)) + ( lengths_values -> RH_shin *  sin__q_RH_HAA__));
    (*this)(5,2) = ((((((- lengths_values -> RH_shin *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> RH_shin *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(5,3) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,4) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_HAA::Type_fr_trunk_X_fr_LF_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = - 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = - 0.207;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,0) = 0.3735;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = - 1.0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = - 0.3735;
    (*this)(5,2) = 0.207;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_HAA& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_HFE::Type_fr_trunk_X_fr_LF_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,1) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,1) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1.0;
    (*this)(3,5) = 0;
    (*this)(4,4) = 0;
    (*this)(5,4) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_HFE& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_HFE::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(1,0) = - sin__q_LF_HAA__;
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(2,0) = - cos__q_LF_HAA__;
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(3,0) = (- 0.207 *  cos__q_LF_HAA__);
    (*this)(3,2) = ( 0.08 - ( 0.207 *  sin__q_LF_HAA__));
    (*this)(4,0) = ( 0.3735 *  cos__q_LF_HAA__);
    (*this)(4,1) = ( 0.08 *  cos__q_LF_HAA__);
    (*this)(4,2) = ( 0.3735 *  sin__q_LF_HAA__);
    (*this)(4,3) = - sin__q_LF_HAA__;
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,0) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(5,1) = ( 0.207 - ( 0.08 *  sin__q_LF_HAA__));
    (*this)(5,2) = ( 0.3735 *  cos__q_LF_HAA__);
    (*this)(5,3) = - cos__q_LF_HAA__;
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_KFE::Type_fr_trunk_X_fr_LF_KFE()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_KFE& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_KFE::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = - sin__q_LF_HFE__;
    (*this)(0,1) = - cos__q_LF_HFE__;
    (*this)(1,0) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(1,1) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(2,0) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(2,1) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(3,0) = ((- 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__);
    (*this)(3,1) = (( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__);
    (*this)(3,2) = ((( 0.35 *  cos__q_LF_HFE__) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(3,3) = - sin__q_LF_HFE__;
    (*this)(3,4) = - cos__q_LF_HFE__;
    (*this)(4,0) = ((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__));
    (*this)(4,1) = ((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__));
    (*this)(4,2) = (( 0.3735 *  sin__q_LF_HAA__) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(4,3) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(4,4) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,0) = ((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__));
    (*this)(5,1) = (((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__));
    (*this)(5,2) = (( 0.3735 *  cos__q_LF_HAA__) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(5,3) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(5,4) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_HAA::Type_fr_trunk_X_fr_RF_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0.207;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,0) = 0.3735;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 1.0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0.3735;
    (*this)(5,2) = 0.207;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_HAA& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_HFE::Type_fr_trunk_X_fr_RF_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,1) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,1) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1.0;
    (*this)(3,5) = 0;
    (*this)(4,4) = 0;
    (*this)(5,4) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_HFE& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_HFE::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(1,0) =  sin__q_RF_HAA__;
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(2,0) = - cos__q_RF_HAA__;
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(3,0) = ( 0.207 *  cos__q_RF_HAA__);
    (*this)(3,2) = ( 0.08 - ( 0.207 *  sin__q_RF_HAA__));
    (*this)(4,0) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(4,1) = ( 0.08 *  cos__q_RF_HAA__);
    (*this)(4,2) = (- 0.3735 *  sin__q_RF_HAA__);
    (*this)(4,3) =  sin__q_RF_HAA__;
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,0) = ( 0.3735 *  sin__q_RF_HAA__);
    (*this)(5,1) = (( 0.08 *  sin__q_RF_HAA__) -  0.207);
    (*this)(5,2) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(5,3) = - cos__q_RF_HAA__;
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_KFE::Type_fr_trunk_X_fr_RF_KFE()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_KFE& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_KFE::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = - sin__q_RF_HFE__;
    (*this)(0,1) = - cos__q_RF_HFE__;
    (*this)(1,0) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(1,1) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(2,0) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(2,1) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(3,0) = (( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__);
    (*this)(3,1) = ((- 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__);
    (*this)(3,2) = ((( 0.35 *  cos__q_RF_HFE__) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(3,3) = - sin__q_RF_HFE__;
    (*this)(3,4) = - cos__q_RF_HFE__;
    (*this)(4,0) = ((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__));
    (*this)(4,1) = ((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__));
    (*this)(4,2) = ((( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(4,3) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(4,4) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,0) = (((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__));
    (*this)(5,1) = ((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__));
    (*this)(5,2) = (( 0.3735 *  cos__q_RF_HAA__) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__));
    (*this)(5,3) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(5,4) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_HAA::Type_fr_trunk_X_fr_LH_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = - 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = - 0.207;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,0) = - 0.3735;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = - 1.0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0.3735;
    (*this)(5,2) = 0.207;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_HAA& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_HFE::Type_fr_trunk_X_fr_LH_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,1) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,1) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1.0;
    (*this)(3,5) = 0;
    (*this)(4,4) = 0;
    (*this)(5,4) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_HFE& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_HFE::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(1,0) = - sin__q_LH_HAA__;
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(2,0) = - cos__q_LH_HAA__;
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(3,0) = (- 0.207 *  cos__q_LH_HAA__);
    (*this)(3,2) = ( 0.08 - ( 0.207 *  sin__q_LH_HAA__));
    (*this)(4,0) = (- 0.3735 *  cos__q_LH_HAA__);
    (*this)(4,1) = ( 0.08 *  cos__q_LH_HAA__);
    (*this)(4,2) = (- 0.3735 *  sin__q_LH_HAA__);
    (*this)(4,3) = - sin__q_LH_HAA__;
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,0) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(5,1) = ( 0.207 - ( 0.08 *  sin__q_LH_HAA__));
    (*this)(5,2) = (- 0.3735 *  cos__q_LH_HAA__);
    (*this)(5,3) = - cos__q_LH_HAA__;
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_KFE::Type_fr_trunk_X_fr_LH_KFE()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_KFE& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_KFE::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = - sin__q_LH_HFE__;
    (*this)(0,1) = - cos__q_LH_HFE__;
    (*this)(1,0) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(1,1) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(2,0) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(2,1) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(3,0) = ((- 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__);
    (*this)(3,1) = (( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__);
    (*this)(3,2) = ((( 0.35 *  cos__q_LH_HFE__) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(3,3) = - sin__q_LH_HFE__;
    (*this)(3,4) = - cos__q_LH_HFE__;
    (*this)(4,0) = ((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__));
    (*this)(4,1) = (((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__));
    (*this)(4,2) = (((- 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(4,3) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(4,4) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,0) = ((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__));
    (*this)(5,1) = ((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__));
    (*this)(5,2) = (((- 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(5,3) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(5,4) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_HAA::Type_fr_trunk_X_fr_RH_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0.207;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,0) = - 0.3735;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 1.0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = - 0.3735;
    (*this)(5,2) = 0.207;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_HAA& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_HFE::Type_fr_trunk_X_fr_RH_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,1) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,1) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1.0;
    (*this)(3,5) = 0;
    (*this)(4,4) = 0;
    (*this)(5,4) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_HFE& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_HFE::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(1,0) =  sin__q_RH_HAA__;
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(2,0) = - cos__q_RH_HAA__;
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(3,0) = ( 0.207 *  cos__q_RH_HAA__);
    (*this)(3,2) = ( 0.08 - ( 0.207 *  sin__q_RH_HAA__));
    (*this)(4,0) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(4,1) = ( 0.08 *  cos__q_RH_HAA__);
    (*this)(4,2) = ( 0.3735 *  sin__q_RH_HAA__);
    (*this)(4,3) =  sin__q_RH_HAA__;
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,0) = (- 0.3735 *  sin__q_RH_HAA__);
    (*this)(5,1) = (( 0.08 *  sin__q_RH_HAA__) -  0.207);
    (*this)(5,2) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(5,3) = - cos__q_RH_HAA__;
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_KFE::Type_fr_trunk_X_fr_RH_KFE()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_KFE& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_KFE::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = - sin__q_RH_HFE__;
    (*this)(0,1) = - cos__q_RH_HFE__;
    (*this)(1,0) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(1,1) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(2,0) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(2,1) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(3,0) = (( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__);
    (*this)(3,1) = ((- 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__);
    (*this)(3,2) = ((( 0.35 *  cos__q_RH_HFE__) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(3,3) = - sin__q_RH_HFE__;
    (*this)(3,4) = - cos__q_RH_HFE__;
    (*this)(4,0) = ((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__));
    (*this)(4,1) = (((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__));
    (*this)(4,2) = ((( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(4,3) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(4,4) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,0) = (((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__));
    (*this)(5,1) = (((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__));
    (*this)(5,2) = (((- 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(5,3) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(5,4) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LF_hipassembly_X_fr_trunk::Type_fr_LF_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,3) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0.207;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LF_hipassembly_X_fr_trunk& iit::HyQ::MotionTransforms::Type_fr_LF_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,1) = - sin__q_LF_HAA__;
    (*this)(0,2) = - cos__q_LF_HAA__;
    (*this)(1,1) = - cos__q_LF_HAA__;
    (*this)(1,2) =  sin__q_LF_HAA__;
    (*this)(3,0) = (- 0.207 *  cos__q_LF_HAA__);
    (*this)(3,1) = ( 0.3735 *  cos__q_LF_HAA__);
    (*this)(3,2) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(3,4) = - sin__q_LF_HAA__;
    (*this)(3,5) = - cos__q_LF_HAA__;
    (*this)(4,0) = ( 0.207 *  sin__q_LF_HAA__);
    (*this)(4,1) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(4,2) = (- 0.3735 *  cos__q_LF_HAA__);
    (*this)(4,4) = - cos__q_LF_HAA__;
    (*this)(4,5) =  sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_hipassembly::Type_fr_trunk_X_fr_LF_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,2) = 0.207;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_hipassembly& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LF_hipassembly::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(1,0) = - sin__q_LF_HAA__;
    (*this)(1,1) = - cos__q_LF_HAA__;
    (*this)(2,0) = - cos__q_LF_HAA__;
    (*this)(2,1) =  sin__q_LF_HAA__;
    (*this)(3,0) = (- 0.207 *  cos__q_LF_HAA__);
    (*this)(3,1) = ( 0.207 *  sin__q_LF_HAA__);
    (*this)(4,0) = ( 0.3735 *  cos__q_LF_HAA__);
    (*this)(4,1) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(4,3) = - sin__q_LF_HAA__;
    (*this)(4,4) = - cos__q_LF_HAA__;
    (*this)(5,0) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(5,1) = (- 0.3735 *  cos__q_LF_HAA__);
    (*this)(5,3) = - cos__q_LF_HAA__;
    (*this)(5,4) =  sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LF_upperleg_X_fr_LF_hipassembly::Type_fr_LF_upperleg_X_fr_LF_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = - 1.0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = - 0.08;
    (*this)(5,3) = 0;
    (*this)(5,4) = - 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LF_upperleg_X_fr_LF_hipassembly& iit::HyQ::MotionTransforms::Type_fr_LF_upperleg_X_fr_LF_hipassembly::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double cos__q_LF_HFE__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    
    (*this)(0,0) =  cos__q_LF_HFE__;
    (*this)(0,2) =  sin__q_LF_HFE__;
    (*this)(1,0) = - sin__q_LF_HFE__;
    (*this)(1,2) =  cos__q_LF_HFE__;
    (*this)(3,1) = (- 0.08 *  sin__q_LF_HFE__);
    (*this)(3,3) =  cos__q_LF_HFE__;
    (*this)(3,5) =  sin__q_LF_HFE__;
    (*this)(4,1) = (- 0.08 *  cos__q_LF_HFE__);
    (*this)(4,3) = - sin__q_LF_HFE__;
    (*this)(4,5) =  cos__q_LF_HFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LF_hipassembly_X_fr_LF_upperleg::Type_fr_LF_hipassembly_X_fr_LF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = - 1;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = - 1.0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = - 0.08;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LF_hipassembly_X_fr_LF_upperleg& iit::HyQ::MotionTransforms::Type_fr_LF_hipassembly_X_fr_LF_upperleg::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double cos__q_LF_HFE__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    
    (*this)(0,0) =  cos__q_LF_HFE__;
    (*this)(0,1) = - sin__q_LF_HFE__;
    (*this)(2,0) =  sin__q_LF_HFE__;
    (*this)(2,1) =  cos__q_LF_HFE__;
    (*this)(3,3) =  cos__q_LF_HFE__;
    (*this)(3,4) = - sin__q_LF_HFE__;
    (*this)(4,0) = (- 0.08 *  sin__q_LF_HFE__);
    (*this)(4,1) = (- 0.08 *  cos__q_LF_HFE__);
    (*this)(5,3) =  sin__q_LF_HFE__;
    (*this)(5,4) =  cos__q_LF_HFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LF_lowerleg_X_fr_LF_upperleg::Type_fr_LF_lowerleg_X_fr_LF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1.0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = - 0.35;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1;
}
const iit::HyQ::MotionTransforms::Type_fr_LF_lowerleg_X_fr_LF_upperleg& iit::HyQ::MotionTransforms::Type_fr_LF_lowerleg_X_fr_LF_upperleg::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double cos__q_LF_KFE__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    
    (*this)(0,0) =  cos__q_LF_KFE__;
    (*this)(0,1) =  sin__q_LF_KFE__;
    (*this)(1,0) = - sin__q_LF_KFE__;
    (*this)(1,1) =  cos__q_LF_KFE__;
    (*this)(3,2) = ( 0.35 *  sin__q_LF_KFE__);
    (*this)(3,3) =  cos__q_LF_KFE__;
    (*this)(3,4) =  sin__q_LF_KFE__;
    (*this)(4,2) = ( 0.35 *  cos__q_LF_KFE__);
    (*this)(4,3) = - sin__q_LF_KFE__;
    (*this)(4,4) =  cos__q_LF_KFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LF_upperleg_X_fr_LF_lowerleg::Type_fr_LF_upperleg_X_fr_LF_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = - 0.35;
    (*this)(4,5) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1.0;
}
const iit::HyQ::MotionTransforms::Type_fr_LF_upperleg_X_fr_LF_lowerleg& iit::HyQ::MotionTransforms::Type_fr_LF_upperleg_X_fr_LF_lowerleg::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double cos__q_LF_KFE__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    
    (*this)(0,0) =  cos__q_LF_KFE__;
    (*this)(0,1) = - sin__q_LF_KFE__;
    (*this)(1,0) =  sin__q_LF_KFE__;
    (*this)(1,1) =  cos__q_LF_KFE__;
    (*this)(3,3) =  cos__q_LF_KFE__;
    (*this)(3,4) = - sin__q_LF_KFE__;
    (*this)(4,3) =  sin__q_LF_KFE__;
    (*this)(4,4) =  cos__q_LF_KFE__;
    (*this)(5,0) = ( 0.35 *  sin__q_LF_KFE__);
    (*this)(5,1) = ( 0.35 *  cos__q_LF_KFE__);
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RF_hipassembly_X_fr_trunk::Type_fr_RF_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,3) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0.207;
    (*this)(5,3) = 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RF_hipassembly_X_fr_trunk& iit::HyQ::MotionTransforms::Type_fr_RF_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,1) =  sin__q_RF_HAA__;
    (*this)(0,2) = - cos__q_RF_HAA__;
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) =  sin__q_RF_HAA__;
    (*this)(3,0) = ( 0.207 *  cos__q_RF_HAA__);
    (*this)(3,1) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(3,2) = ( 0.3735 *  sin__q_RF_HAA__);
    (*this)(3,4) =  sin__q_RF_HAA__;
    (*this)(3,5) = - cos__q_RF_HAA__;
    (*this)(4,0) = (- 0.207 *  sin__q_RF_HAA__);
    (*this)(4,1) = (- 0.3735 *  sin__q_RF_HAA__);
    (*this)(4,2) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(4,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_hipassembly::Type_fr_trunk_X_fr_RF_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,2) = 0.207;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_hipassembly& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RF_hipassembly::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(1,0) =  sin__q_RF_HAA__;
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(2,0) = - cos__q_RF_HAA__;
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(3,0) = ( 0.207 *  cos__q_RF_HAA__);
    (*this)(3,1) = (- 0.207 *  sin__q_RF_HAA__);
    (*this)(4,0) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(4,1) = (- 0.3735 *  sin__q_RF_HAA__);
    (*this)(4,3) =  sin__q_RF_HAA__;
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(5,0) = ( 0.3735 *  sin__q_RF_HAA__);
    (*this)(5,1) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(5,3) = - cos__q_RF_HAA__;
    (*this)(5,4) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RF_upperleg_X_fr_RF_hipassembly::Type_fr_RF_upperleg_X_fr_RF_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1.0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0.08;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RF_upperleg_X_fr_RF_hipassembly& iit::HyQ::MotionTransforms::Type_fr_RF_upperleg_X_fr_RF_hipassembly::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double cos__q_RF_HFE__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    
    (*this)(0,0) =  cos__q_RF_HFE__;
    (*this)(0,2) = - sin__q_RF_HFE__;
    (*this)(1,0) = - sin__q_RF_HFE__;
    (*this)(1,2) = - cos__q_RF_HFE__;
    (*this)(3,1) = ( 0.08 *  sin__q_RF_HFE__);
    (*this)(3,3) =  cos__q_RF_HFE__;
    (*this)(3,5) = - sin__q_RF_HFE__;
    (*this)(4,1) = ( 0.08 *  cos__q_RF_HFE__);
    (*this)(4,3) = - sin__q_RF_HFE__;
    (*this)(4,5) = - cos__q_RF_HFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RF_hipassembly_X_fr_RF_upperleg::Type_fr_RF_hipassembly_X_fr_RF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1.0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0.08;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RF_hipassembly_X_fr_RF_upperleg& iit::HyQ::MotionTransforms::Type_fr_RF_hipassembly_X_fr_RF_upperleg::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double cos__q_RF_HFE__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    
    (*this)(0,0) =  cos__q_RF_HFE__;
    (*this)(0,1) = - sin__q_RF_HFE__;
    (*this)(2,0) = - sin__q_RF_HFE__;
    (*this)(2,1) = - cos__q_RF_HFE__;
    (*this)(3,3) =  cos__q_RF_HFE__;
    (*this)(3,4) = - sin__q_RF_HFE__;
    (*this)(4,0) = ( 0.08 *  sin__q_RF_HFE__);
    (*this)(4,1) = ( 0.08 *  cos__q_RF_HFE__);
    (*this)(5,3) = - sin__q_RF_HFE__;
    (*this)(5,4) = - cos__q_RF_HFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RF_lowerleg_X_fr_RF_upperleg::Type_fr_RF_lowerleg_X_fr_RF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1.0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = - 0.35;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1;
}
const iit::HyQ::MotionTransforms::Type_fr_RF_lowerleg_X_fr_RF_upperleg& iit::HyQ::MotionTransforms::Type_fr_RF_lowerleg_X_fr_RF_upperleg::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double cos__q_RF_KFE__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    
    (*this)(0,0) =  cos__q_RF_KFE__;
    (*this)(0,1) =  sin__q_RF_KFE__;
    (*this)(1,0) = - sin__q_RF_KFE__;
    (*this)(1,1) =  cos__q_RF_KFE__;
    (*this)(3,2) = ( 0.35 *  sin__q_RF_KFE__);
    (*this)(3,3) =  cos__q_RF_KFE__;
    (*this)(3,4) =  sin__q_RF_KFE__;
    (*this)(4,2) = ( 0.35 *  cos__q_RF_KFE__);
    (*this)(4,3) = - sin__q_RF_KFE__;
    (*this)(4,4) =  cos__q_RF_KFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RF_upperleg_X_fr_RF_lowerleg::Type_fr_RF_upperleg_X_fr_RF_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = - 0.35;
    (*this)(4,5) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1.0;
}
const iit::HyQ::MotionTransforms::Type_fr_RF_upperleg_X_fr_RF_lowerleg& iit::HyQ::MotionTransforms::Type_fr_RF_upperleg_X_fr_RF_lowerleg::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double cos__q_RF_KFE__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    
    (*this)(0,0) =  cos__q_RF_KFE__;
    (*this)(0,1) = - sin__q_RF_KFE__;
    (*this)(1,0) =  sin__q_RF_KFE__;
    (*this)(1,1) =  cos__q_RF_KFE__;
    (*this)(3,3) =  cos__q_RF_KFE__;
    (*this)(3,4) = - sin__q_RF_KFE__;
    (*this)(4,3) =  sin__q_RF_KFE__;
    (*this)(4,4) =  cos__q_RF_KFE__;
    (*this)(5,0) = ( 0.35 *  sin__q_RF_KFE__);
    (*this)(5,1) = ( 0.35 *  cos__q_RF_KFE__);
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LH_hipassembly_X_fr_trunk::Type_fr_LH_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,3) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0.207;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LH_hipassembly_X_fr_trunk& iit::HyQ::MotionTransforms::Type_fr_LH_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,1) = - sin__q_LH_HAA__;
    (*this)(0,2) = - cos__q_LH_HAA__;
    (*this)(1,1) = - cos__q_LH_HAA__;
    (*this)(1,2) =  sin__q_LH_HAA__;
    (*this)(3,0) = (- 0.207 *  cos__q_LH_HAA__);
    (*this)(3,1) = (- 0.3735 *  cos__q_LH_HAA__);
    (*this)(3,2) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(3,4) = - sin__q_LH_HAA__;
    (*this)(3,5) = - cos__q_LH_HAA__;
    (*this)(4,0) = ( 0.207 *  sin__q_LH_HAA__);
    (*this)(4,1) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(4,2) = ( 0.3735 *  cos__q_LH_HAA__);
    (*this)(4,4) = - cos__q_LH_HAA__;
    (*this)(4,5) =  sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_hipassembly::Type_fr_trunk_X_fr_LH_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,2) = 0.207;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_hipassembly& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_LH_hipassembly::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(1,0) = - sin__q_LH_HAA__;
    (*this)(1,1) = - cos__q_LH_HAA__;
    (*this)(2,0) = - cos__q_LH_HAA__;
    (*this)(2,1) =  sin__q_LH_HAA__;
    (*this)(3,0) = (- 0.207 *  cos__q_LH_HAA__);
    (*this)(3,1) = ( 0.207 *  sin__q_LH_HAA__);
    (*this)(4,0) = (- 0.3735 *  cos__q_LH_HAA__);
    (*this)(4,1) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(4,3) = - sin__q_LH_HAA__;
    (*this)(4,4) = - cos__q_LH_HAA__;
    (*this)(5,0) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(5,1) = ( 0.3735 *  cos__q_LH_HAA__);
    (*this)(5,3) = - cos__q_LH_HAA__;
    (*this)(5,4) =  sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LH_upperleg_X_fr_LH_hipassembly::Type_fr_LH_upperleg_X_fr_LH_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = - 1.0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = - 0.08;
    (*this)(5,3) = 0;
    (*this)(5,4) = - 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LH_upperleg_X_fr_LH_hipassembly& iit::HyQ::MotionTransforms::Type_fr_LH_upperleg_X_fr_LH_hipassembly::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double cos__q_LH_HFE__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    
    (*this)(0,0) =  cos__q_LH_HFE__;
    (*this)(0,2) =  sin__q_LH_HFE__;
    (*this)(1,0) = - sin__q_LH_HFE__;
    (*this)(1,2) =  cos__q_LH_HFE__;
    (*this)(3,1) = (- 0.08 *  sin__q_LH_HFE__);
    (*this)(3,3) =  cos__q_LH_HFE__;
    (*this)(3,5) =  sin__q_LH_HFE__;
    (*this)(4,1) = (- 0.08 *  cos__q_LH_HFE__);
    (*this)(4,3) = - sin__q_LH_HFE__;
    (*this)(4,5) =  cos__q_LH_HFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LH_hipassembly_X_fr_LH_upperleg::Type_fr_LH_hipassembly_X_fr_LH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = - 1;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = - 1.0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = - 0.08;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_LH_hipassembly_X_fr_LH_upperleg& iit::HyQ::MotionTransforms::Type_fr_LH_hipassembly_X_fr_LH_upperleg::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double cos__q_LH_HFE__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    
    (*this)(0,0) =  cos__q_LH_HFE__;
    (*this)(0,1) = - sin__q_LH_HFE__;
    (*this)(2,0) =  sin__q_LH_HFE__;
    (*this)(2,1) =  cos__q_LH_HFE__;
    (*this)(3,3) =  cos__q_LH_HFE__;
    (*this)(3,4) = - sin__q_LH_HFE__;
    (*this)(4,0) = (- 0.08 *  sin__q_LH_HFE__);
    (*this)(4,1) = (- 0.08 *  cos__q_LH_HFE__);
    (*this)(5,3) =  sin__q_LH_HFE__;
    (*this)(5,4) =  cos__q_LH_HFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LH_lowerleg_X_fr_LH_upperleg::Type_fr_LH_lowerleg_X_fr_LH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1.0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = - 0.35;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1;
}
const iit::HyQ::MotionTransforms::Type_fr_LH_lowerleg_X_fr_LH_upperleg& iit::HyQ::MotionTransforms::Type_fr_LH_lowerleg_X_fr_LH_upperleg::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double cos__q_LH_KFE__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    
    (*this)(0,0) =  cos__q_LH_KFE__;
    (*this)(0,1) =  sin__q_LH_KFE__;
    (*this)(1,0) = - sin__q_LH_KFE__;
    (*this)(1,1) =  cos__q_LH_KFE__;
    (*this)(3,2) = ( 0.35 *  sin__q_LH_KFE__);
    (*this)(3,3) =  cos__q_LH_KFE__;
    (*this)(3,4) =  sin__q_LH_KFE__;
    (*this)(4,2) = ( 0.35 *  cos__q_LH_KFE__);
    (*this)(4,3) = - sin__q_LH_KFE__;
    (*this)(4,4) =  cos__q_LH_KFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_LH_upperleg_X_fr_LH_lowerleg::Type_fr_LH_upperleg_X_fr_LH_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = - 0.35;
    (*this)(4,5) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1.0;
}
const iit::HyQ::MotionTransforms::Type_fr_LH_upperleg_X_fr_LH_lowerleg& iit::HyQ::MotionTransforms::Type_fr_LH_upperleg_X_fr_LH_lowerleg::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double cos__q_LH_KFE__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    
    (*this)(0,0) =  cos__q_LH_KFE__;
    (*this)(0,1) = - sin__q_LH_KFE__;
    (*this)(1,0) =  sin__q_LH_KFE__;
    (*this)(1,1) =  cos__q_LH_KFE__;
    (*this)(3,3) =  cos__q_LH_KFE__;
    (*this)(3,4) = - sin__q_LH_KFE__;
    (*this)(4,3) =  sin__q_LH_KFE__;
    (*this)(4,4) =  cos__q_LH_KFE__;
    (*this)(5,0) = ( 0.35 *  sin__q_LH_KFE__);
    (*this)(5,1) = ( 0.35 *  cos__q_LH_KFE__);
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RH_hipassembly_X_fr_trunk::Type_fr_RH_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,3) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0.207;
    (*this)(5,3) = 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RH_hipassembly_X_fr_trunk& iit::HyQ::MotionTransforms::Type_fr_RH_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,1) =  sin__q_RH_HAA__;
    (*this)(0,2) = - cos__q_RH_HAA__;
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) =  sin__q_RH_HAA__;
    (*this)(3,0) = ( 0.207 *  cos__q_RH_HAA__);
    (*this)(3,1) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(3,2) = (- 0.3735 *  sin__q_RH_HAA__);
    (*this)(3,4) =  sin__q_RH_HAA__;
    (*this)(3,5) = - cos__q_RH_HAA__;
    (*this)(4,0) = (- 0.207 *  sin__q_RH_HAA__);
    (*this)(4,1) = ( 0.3735 *  sin__q_RH_HAA__);
    (*this)(4,2) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(4,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_hipassembly::Type_fr_trunk_X_fr_RH_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,2) = 0.207;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_hipassembly& iit::HyQ::MotionTransforms::Type_fr_trunk_X_fr_RH_hipassembly::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(1,0) =  sin__q_RH_HAA__;
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(2,0) = - cos__q_RH_HAA__;
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(3,0) = ( 0.207 *  cos__q_RH_HAA__);
    (*this)(3,1) = (- 0.207 *  sin__q_RH_HAA__);
    (*this)(4,0) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(4,1) = ( 0.3735 *  sin__q_RH_HAA__);
    (*this)(4,3) =  sin__q_RH_HAA__;
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(5,0) = (- 0.3735 *  sin__q_RH_HAA__);
    (*this)(5,1) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(5,3) = - cos__q_RH_HAA__;
    (*this)(5,4) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RH_upperleg_X_fr_RH_hipassembly::Type_fr_RH_upperleg_X_fr_RH_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1.0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0.08;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RH_upperleg_X_fr_RH_hipassembly& iit::HyQ::MotionTransforms::Type_fr_RH_upperleg_X_fr_RH_hipassembly::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double cos__q_RH_HFE__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    
    (*this)(0,0) =  cos__q_RH_HFE__;
    (*this)(0,2) = - sin__q_RH_HFE__;
    (*this)(1,0) = - sin__q_RH_HFE__;
    (*this)(1,2) = - cos__q_RH_HFE__;
    (*this)(3,1) = ( 0.08 *  sin__q_RH_HFE__);
    (*this)(3,3) =  cos__q_RH_HFE__;
    (*this)(3,5) = - sin__q_RH_HFE__;
    (*this)(4,1) = ( 0.08 *  cos__q_RH_HFE__);
    (*this)(4,3) = - sin__q_RH_HFE__;
    (*this)(4,5) = - cos__q_RH_HFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RH_hipassembly_X_fr_RH_upperleg::Type_fr_RH_hipassembly_X_fr_RH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1.0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0.08;
    (*this)(5,5) = 0;
}
const iit::HyQ::MotionTransforms::Type_fr_RH_hipassembly_X_fr_RH_upperleg& iit::HyQ::MotionTransforms::Type_fr_RH_hipassembly_X_fr_RH_upperleg::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double cos__q_RH_HFE__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    
    (*this)(0,0) =  cos__q_RH_HFE__;
    (*this)(0,1) = - sin__q_RH_HFE__;
    (*this)(2,0) = - sin__q_RH_HFE__;
    (*this)(2,1) = - cos__q_RH_HFE__;
    (*this)(3,3) =  cos__q_RH_HFE__;
    (*this)(3,4) = - sin__q_RH_HFE__;
    (*this)(4,0) = ( 0.08 *  sin__q_RH_HFE__);
    (*this)(4,1) = ( 0.08 *  cos__q_RH_HFE__);
    (*this)(5,3) = - sin__q_RH_HFE__;
    (*this)(5,4) = - cos__q_RH_HFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RH_lowerleg_X_fr_RH_upperleg::Type_fr_RH_lowerleg_X_fr_RH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1.0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = - 0.35;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1;
}
const iit::HyQ::MotionTransforms::Type_fr_RH_lowerleg_X_fr_RH_upperleg& iit::HyQ::MotionTransforms::Type_fr_RH_lowerleg_X_fr_RH_upperleg::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double cos__q_RH_KFE__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    
    (*this)(0,0) =  cos__q_RH_KFE__;
    (*this)(0,1) =  sin__q_RH_KFE__;
    (*this)(1,0) = - sin__q_RH_KFE__;
    (*this)(1,1) =  cos__q_RH_KFE__;
    (*this)(3,2) = ( 0.35 *  sin__q_RH_KFE__);
    (*this)(3,3) =  cos__q_RH_KFE__;
    (*this)(3,4) =  sin__q_RH_KFE__;
    (*this)(4,2) = ( 0.35 *  cos__q_RH_KFE__);
    (*this)(4,3) = - sin__q_RH_KFE__;
    (*this)(4,4) =  cos__q_RH_KFE__;
    return *this;
}
iit::HyQ::MotionTransforms::Type_fr_RH_upperleg_X_fr_RH_lowerleg::Type_fr_RH_upperleg_X_fr_RH_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = - 0.35;
    (*this)(4,5) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1.0;
}
const iit::HyQ::MotionTransforms::Type_fr_RH_upperleg_X_fr_RH_lowerleg& iit::HyQ::MotionTransforms::Type_fr_RH_upperleg_X_fr_RH_lowerleg::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double cos__q_RH_KFE__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    
    (*this)(0,0) =  cos__q_RH_KFE__;
    (*this)(0,1) = - sin__q_RH_KFE__;
    (*this)(1,0) =  sin__q_RH_KFE__;
    (*this)(1,1) =  cos__q_RH_KFE__;
    (*this)(3,3) =  cos__q_RH_KFE__;
    (*this)(3,4) = - sin__q_RH_KFE__;
    (*this)(4,3) =  sin__q_RH_KFE__;
    (*this)(4,4) =  cos__q_RH_KFE__;
    (*this)(5,0) = ( 0.35 *  sin__q_RH_KFE__);
    (*this)(5,1) = ( 0.35 *  cos__q_RH_KFE__);
    return *this;
}

iit::HyQ::ForceTransforms::Type_fr_LF_lowerleg_X_LF_foot::Type_fr_LF_lowerleg_X_LF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = - 1;
    (*this)(4,4) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LF_lowerleg_X_LF_foot& iit::HyQ::ForceTransforms::Type_fr_LF_lowerleg_X_LF_foot::update(const state_t& q) {
    
    
    (*this)(1,4) = - lengths_values -> foot_x;
    (*this)(2,3) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RF_lowerleg_X_RF_foot::Type_fr_RF_lowerleg_X_RF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = - 1;
    (*this)(4,4) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RF_lowerleg_X_RF_foot& iit::HyQ::ForceTransforms::Type_fr_RF_lowerleg_X_RF_foot::update(const state_t& q) {
    
    
    (*this)(1,4) = - lengths_values -> foot_x;
    (*this)(2,3) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LH_lowerleg_X_LH_foot::Type_fr_LH_lowerleg_X_LH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = - 1;
    (*this)(4,4) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LH_lowerleg_X_LH_foot& iit::HyQ::ForceTransforms::Type_fr_LH_lowerleg_X_LH_foot::update(const state_t& q) {
    
    
    (*this)(1,4) = - lengths_values -> foot_x;
    (*this)(2,3) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RH_lowerleg_X_RH_foot::Type_fr_RH_lowerleg_X_RH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = - 1;
    (*this)(4,4) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RH_lowerleg_X_RH_foot& iit::HyQ::ForceTransforms::Type_fr_RH_lowerleg_X_RH_foot::update(const state_t& q) {
    
    
    (*this)(1,4) = - lengths_values -> foot_x;
    (*this)(2,3) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_hipassemblyCOM::Type_fr_trunk_X_LF_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_hipassemblyCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,3) = (- 0.207 *  cos__q_LF_HAA__);
    (*this)(0,4) = (( 0.207 *  sin__q_LF_HAA__) -  0.04263);
    (*this)(1,0) = - sin__q_LF_HAA__;
    (*this)(1,1) = - cos__q_LF_HAA__;
    (*this)(1,3) = ( 0.2041 *  cos__q_LF_HAA__);
    (*this)(1,4) = (- 0.2041 *  sin__q_LF_HAA__);
    (*this)(1,5) = ( 0.04263 *  cos__q_LF_HAA__);
    (*this)(2,0) = - cos__q_LF_HAA__;
    (*this)(2,1) =  sin__q_LF_HAA__;
    (*this)(2,3) = (- 0.2041 *  sin__q_LF_HAA__);
    (*this)(2,4) = (- 0.2041 *  cos__q_LF_HAA__);
    (*this)(2,5) = ( 0.207 - ( 0.04263 *  sin__q_LF_HAA__));
    (*this)(4,3) = - sin__q_LF_HAA__;
    (*this)(4,4) = - cos__q_LF_HAA__;
    (*this)(5,3) = - cos__q_LF_HAA__;
    (*this)(5,4) =  sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_hipassemblyCOM::Type_fr_trunk_X_RF_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_hipassemblyCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,3) = ( 0.207 *  cos__q_RF_HAA__);
    (*this)(0,4) = ( 0.04263 - ( 0.207 *  sin__q_RF_HAA__));
    (*this)(1,0) =  sin__q_RF_HAA__;
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,3) = ( 0.2041 *  cos__q_RF_HAA__);
    (*this)(1,4) = (- 0.2041 *  sin__q_RF_HAA__);
    (*this)(1,5) = (- 0.04263 *  cos__q_RF_HAA__);
    (*this)(2,0) = - cos__q_RF_HAA__;
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(2,3) = ( 0.2041 *  sin__q_RF_HAA__);
    (*this)(2,4) = ( 0.2041 *  cos__q_RF_HAA__);
    (*this)(2,5) = ( 0.207 - ( 0.04263 *  sin__q_RF_HAA__));
    (*this)(4,3) =  sin__q_RF_HAA__;
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(5,3) = - cos__q_RF_HAA__;
    (*this)(5,4) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_hipassemblyCOM::Type_fr_trunk_X_LH_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_hipassemblyCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,3) = (- 0.207 *  cos__q_LH_HAA__);
    (*this)(0,4) = (( 0.207 *  sin__q_LH_HAA__) -  0.04263);
    (*this)(1,0) = - sin__q_LH_HAA__;
    (*this)(1,1) = - cos__q_LH_HAA__;
    (*this)(1,3) = (- 0.2041 *  cos__q_LH_HAA__);
    (*this)(1,4) = ( 0.2041 *  sin__q_LH_HAA__);
    (*this)(1,5) = ( 0.04263 *  cos__q_LH_HAA__);
    (*this)(2,0) = - cos__q_LH_HAA__;
    (*this)(2,1) =  sin__q_LH_HAA__;
    (*this)(2,3) = ( 0.2041 *  sin__q_LH_HAA__);
    (*this)(2,4) = ( 0.2041 *  cos__q_LH_HAA__);
    (*this)(2,5) = ( 0.207 - ( 0.04263 *  sin__q_LH_HAA__));
    (*this)(4,3) = - sin__q_LH_HAA__;
    (*this)(4,4) = - cos__q_LH_HAA__;
    (*this)(5,3) = - cos__q_LH_HAA__;
    (*this)(5,4) =  sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_hipassemblyCOM::Type_fr_trunk_X_RH_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_hipassemblyCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,3) = ( 0.207 *  cos__q_RH_HAA__);
    (*this)(0,4) = ( 0.04263 - ( 0.207 *  sin__q_RH_HAA__));
    (*this)(1,0) =  sin__q_RH_HAA__;
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,3) = (- 0.2041 *  cos__q_RH_HAA__);
    (*this)(1,4) = ( 0.2041 *  sin__q_RH_HAA__);
    (*this)(1,5) = (- 0.04263 *  cos__q_RH_HAA__);
    (*this)(2,0) = - cos__q_RH_HAA__;
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(2,3) = (- 0.2041 *  sin__q_RH_HAA__);
    (*this)(2,4) = (- 0.2041 *  cos__q_RH_HAA__);
    (*this)(2,5) = ( 0.207 - ( 0.04263 *  sin__q_RH_HAA__));
    (*this)(4,3) =  sin__q_RH_HAA__;
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(5,3) = - cos__q_RH_HAA__;
    (*this)(5,4) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_upperlegCOM::Type_fr_trunk_X_LF_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_upperlegCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_upperlegCOM::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = - sin__q_LF_HFE__;
    (*this)(0,1) = - cos__q_LF_HFE__;
    (*this)(0,3) = ((- 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__);
    (*this)(0,4) = (( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__);
    (*this)(0,5) = (((( 0.02625 *  sin__q_LF_HFE__) + ( 0.1507 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(1,0) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(1,1) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = (((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.02625 *  cos__q_LF_HAA__));
    (*this)(1,4) = ((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.1507 *  cos__q_LF_HAA__));
    (*this)(1,5) = ((((- 0.1507 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.02625 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(2,0) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(2,1) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = (((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.02625 *  sin__q_LF_HAA__));
    (*this)(2,4) = (((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.1507 *  sin__q_LF_HAA__));
    (*this)(2,5) = ((((- 0.1507 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.02625 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(3,3) = - sin__q_LF_HFE__;
    (*this)(3,4) = - cos__q_LF_HFE__;
    (*this)(4,3) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(4,4) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,3) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(5,4) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_upperlegCOM::Type_fr_trunk_X_RF_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_upperlegCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_upperlegCOM::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = - sin__q_RF_HFE__;
    (*this)(0,1) = - cos__q_RF_HFE__;
    (*this)(0,3) = (( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__);
    (*this)(0,4) = ((- 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__);
    (*this)(0,5) = (((( 0.02625 *  sin__q_RF_HFE__) + ( 0.1507 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(1,0) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(1,1) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = (((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.02625 *  cos__q_RF_HAA__));
    (*this)(1,4) = ((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.1507 *  cos__q_RF_HAA__));
    (*this)(1,5) = (((( 0.1507 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.02625 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(2,0) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(2,1) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = ((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.02625 *  sin__q_RF_HAA__));
    (*this)(2,4) = ((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.1507 *  sin__q_RF_HAA__));
    (*this)(2,5) = ((((- 0.1507 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.02625 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(3,3) = - sin__q_RF_HFE__;
    (*this)(3,4) = - cos__q_RF_HFE__;
    (*this)(4,3) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(4,4) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,3) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(5,4) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_upperlegCOM::Type_fr_trunk_X_LH_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_upperlegCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_upperlegCOM::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = - sin__q_LH_HFE__;
    (*this)(0,1) = - cos__q_LH_HFE__;
    (*this)(0,3) = ((- 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__);
    (*this)(0,4) = (( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__);
    (*this)(0,5) = ((((- 0.02625 *  sin__q_LH_HFE__) + ( 0.1507 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(1,0) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(1,1) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = (((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.02625 *  cos__q_LH_HAA__));
    (*this)(1,4) = (((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.1507 *  cos__q_LH_HAA__));
    (*this)(1,5) = ((((- 0.1507 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.02625 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(2,0) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(2,1) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = (((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.02625 *  sin__q_LH_HAA__));
    (*this)(2,4) = ((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.1507 *  sin__q_LH_HAA__));
    (*this)(2,5) = ((((- 0.1507 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.02625 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(3,3) = - sin__q_LH_HFE__;
    (*this)(3,4) = - cos__q_LH_HFE__;
    (*this)(4,3) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(4,4) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,3) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(5,4) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_upperlegCOM::Type_fr_trunk_X_RH_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_upperlegCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_upperlegCOM::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = - sin__q_RH_HFE__;
    (*this)(0,1) = - cos__q_RH_HFE__;
    (*this)(0,3) = (( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__);
    (*this)(0,4) = ((- 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__);
    (*this)(0,5) = ((((- 0.02625 *  sin__q_RH_HFE__) + ( 0.1507 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(1,0) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(1,1) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = (((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.02625 *  cos__q_RH_HAA__));
    (*this)(1,4) = (((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.1507 *  cos__q_RH_HAA__));
    (*this)(1,5) = (((( 0.1507 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.02625 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(2,0) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(2,1) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = ((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.02625 *  sin__q_RH_HAA__));
    (*this)(2,4) = (((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.1507 *  sin__q_RH_HAA__));
    (*this)(2,5) = ((((- 0.1507 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.02625 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(3,3) = - sin__q_RH_HFE__;
    (*this)(3,4) = - cos__q_RH_HFE__;
    (*this)(4,3) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(4,4) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,3) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(5,4) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_lowerlegCOM::Type_fr_trunk_X_LF_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_lowerlegCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_lowerlegCOM::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,1) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,4) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,5) = ((((((- 0.1254 *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + (( 0.1254 *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + ( 0.35 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(1,0) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = ((((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(1,4) = ((((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__)) + ( 0.1254 *  cos__q_LF_HAA__));
    (*this)(1,5) = ((((((- 0.1254 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.1254 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(2,0) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = (((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(2,4) = ((((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__)) - ( 0.1254 *  sin__q_LF_HAA__));
    (*this)(2,5) = ((((((- 0.1254 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.1254 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(3,3) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(3,4) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(4,3) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,4) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,3) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,4) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_lowerlegCOM::Type_fr_trunk_X_RF_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_lowerlegCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_lowerlegCOM::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,1) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(0,4) = ((((- 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,5) = ((((((- 0.1254 *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + (( 0.1254 *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + ( 0.35 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(1,0) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(1,1) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = ((((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(1,4) = ((((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__)) + ( 0.1254 *  cos__q_RF_HAA__));
    (*this)(1,5) = (((((( 0.1254 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( 0.1254 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(2,0) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = ((((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(2,4) = (((((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__)) + ( 0.1254 *  sin__q_RF_HAA__));
    (*this)(2,5) = ((((((- 0.1254 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.1254 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(3,3) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(3,4) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(4,3) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(4,4) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,3) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,4) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_lowerlegCOM::Type_fr_trunk_X_LH_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_lowerlegCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_lowerlegCOM::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,1) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,4) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,5) = ((((((- 0.1254 *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + (( 0.1254 *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + ( 0.35 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(1,0) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = (((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(1,4) = (((((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__)) + ( 0.1254 *  cos__q_LH_HAA__));
    (*this)(1,5) = ((((((- 0.1254 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.1254 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(2,0) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = ((((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(2,4) = ((((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__)) - ( 0.1254 *  sin__q_LH_HAA__));
    (*this)(2,5) = ((((((- 0.1254 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.1254 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(3,3) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(3,4) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(4,3) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,4) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,3) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,4) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_lowerlegCOM::Type_fr_trunk_X_RH_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_lowerlegCOM& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_lowerlegCOM::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,1) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(0,4) = ((((- 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,5) = ((((((- 0.1254 *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + (( 0.1254 *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + ( 0.35 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(1,0) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(1,1) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = (((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(1,4) = (((((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__)) + ( 0.1254 *  cos__q_RH_HAA__));
    (*this)(1,5) = (((((( 0.1254 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( 0.1254 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(2,0) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = (((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + ((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(2,4) = (((((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__)) + ( 0.1254 *  sin__q_RH_HAA__));
    (*this)(2,5) = ((((((- 0.1254 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.1254 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(3,3) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(3,4) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(4,3) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(4,4) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,3) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,4) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_LF_foot_X_fr_LF_lowerleg::Type_LF_foot_X_fr_LF_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_LF_foot_X_fr_LF_lowerleg& iit::HyQ::ForceTransforms::Type_LF_foot_X_fr_LF_lowerleg::update(const state_t& q) {
    
    
    (*this)(0,5) = - lengths_values -> foot_x;
    (*this)(1,4) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::ForceTransforms::Type_RF_foot_X_fr_RF_lowerleg::Type_RF_foot_X_fr_RF_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_RF_foot_X_fr_RF_lowerleg& iit::HyQ::ForceTransforms::Type_RF_foot_X_fr_RF_lowerleg::update(const state_t& q) {
    
    
    (*this)(0,5) = - lengths_values -> foot_x;
    (*this)(1,4) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::ForceTransforms::Type_LH_foot_X_fr_LH_lowerleg::Type_LH_foot_X_fr_LH_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_LH_foot_X_fr_LH_lowerleg& iit::HyQ::ForceTransforms::Type_LH_foot_X_fr_LH_lowerleg::update(const state_t& q) {
    
    
    (*this)(0,5) = - lengths_values -> foot_x;
    (*this)(1,4) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::ForceTransforms::Type_RH_foot_X_fr_RH_lowerleg::Type_RH_foot_X_fr_RH_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_RH_foot_X_fr_RH_lowerleg& iit::HyQ::ForceTransforms::Type_RH_foot_X_fr_RH_lowerleg::update(const state_t& q) {
    
    
    (*this)(0,5) = - lengths_values -> foot_x;
    (*this)(1,4) = - lengths_values -> foot_x;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_foot::Type_fr_trunk_X_LF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_foot& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_foot::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(0,2) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,3) = ((((- 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,4) = ((((((- lengths_values -> foot_x *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + (( lengths_values -> foot_x *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + ( 0.35 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(0,5) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(1,0) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) =  cos__q_LF_HAA__;
    (*this)(1,2) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(1,3) = (((((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__)) - ( lengths_values -> foot_x *  cos__q_LF_HAA__));
    (*this)(1,4) = ((((((- lengths_values -> foot_x *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(1,5) = (((((( 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + ((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(2,0) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) = - sin__q_LF_HAA__;
    (*this)(2,2) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(2,3) = (((((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + ((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  cos__q_LF_HFE__)) + ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__)) + ( lengths_values -> foot_x *  sin__q_LF_HAA__));
    (*this)(2,4) = ((((((- lengths_values -> foot_x *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(2,5) = ((((((- 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + ((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  cos__q_LF_HFE__)) + ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + ((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(3,3) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(3,5) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(4,3) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,4) =  cos__q_LF_HAA__;
    (*this)(4,5) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(5,3) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,4) = - sin__q_LF_HAA__;
    (*this)(5,5) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_foot::Type_fr_trunk_X_RF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_foot& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_foot::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(0,2) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,4) = ((((((- lengths_values -> foot_x *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + (( lengths_values -> foot_x *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + ( 0.35 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(0,5) = (((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,0) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,3) = (((((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + ((((( 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__)) - ( lengths_values -> foot_x *  cos__q_RF_HAA__));
    (*this)(1,4) = (((((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(1,5) = (((((( 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(2,0) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(2,2) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(2,3) = ((((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + ((((( 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) - ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__)) - ( lengths_values -> foot_x *  sin__q_RF_HAA__));
    (*this)(2,4) = ((((((- lengths_values -> foot_x *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(2,5) = (((((( 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) - ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(3,3) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(3,5) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(4,3) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(4,5) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,3) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,4) =  sin__q_RF_HAA__;
    (*this)(5,5) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_foot::Type_fr_trunk_X_LH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_foot& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_foot::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(0,2) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,3) = ((((- 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,4) = ((((((- lengths_values -> foot_x *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + (( lengths_values -> foot_x *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + ( 0.35 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(0,5) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(1,0) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) =  cos__q_LH_HAA__;
    (*this)(1,2) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(1,3) = (((((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__)) - ( lengths_values -> foot_x *  cos__q_LH_HAA__));
    (*this)(1,4) = ((((((- lengths_values -> foot_x *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(1,5) = ((((((- 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(2,0) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) = - sin__q_LH_HAA__;
    (*this)(2,2) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(2,3) = (((((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + ((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  cos__q_LH_HFE__)) + ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__)) + ( lengths_values -> foot_x *  sin__q_LH_HAA__));
    (*this)(2,4) = ((((((- lengths_values -> foot_x *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(2,5) = (((((( 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + ((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  cos__q_LH_HFE__)) + ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + ((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(3,3) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(3,5) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(4,3) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,4) =  cos__q_LH_HAA__;
    (*this)(4,5) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(5,3) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,4) = - sin__q_LH_HAA__;
    (*this)(5,5) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_foot::Type_fr_trunk_X_RH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_foot& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_foot::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(0,2) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,4) = ((((((- lengths_values -> foot_x *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + (( lengths_values -> foot_x *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + ( 0.35 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(0,5) = (((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,0) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,3) = (((((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + (((((- 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__)) - ( lengths_values -> foot_x *  cos__q_RH_HAA__));
    (*this)(1,4) = (((((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(1,5) = ((((((- 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(2,0) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(2,2) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(2,3) = ((((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + (((((- 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) - ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__)) - ( lengths_values -> foot_x *  sin__q_RH_HAA__));
    (*this)(2,4) = ((((((- lengths_values -> foot_x *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(2,5) = ((((((- 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) - ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(3,3) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(3,5) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(4,3) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(4,5) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,3) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,4) =  sin__q_RH_HAA__;
    (*this)(5,5) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LF_lowerleg_X_fr_trunk::Type_fr_LF_lowerleg_X_fr_trunk()
{
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LF_lowerleg_X_fr_trunk& iit::HyQ::ForceTransforms::Type_fr_LF_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,1) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,2) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,4) = ((((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(0,5) = (((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(1,0) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(1,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,2) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,3) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,4) = (((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__));
    (*this)(1,5) = (((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__));
    (*this)(2,1) =  cos__q_LF_HAA__;
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = ((( 0.35 *  cos__q_LF_HFE__) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(2,4) = (( 0.3735 *  sin__q_LF_HAA__) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(2,5) = (( 0.3735 *  cos__q_LF_HAA__) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(3,3) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(3,4) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,5) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,3) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(4,4) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,5) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,4) =  cos__q_LF_HAA__;
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RF_lowerleg_X_fr_trunk::Type_fr_RF_lowerleg_X_fr_trunk()
{
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RF_lowerleg_X_fr_trunk& iit::HyQ::ForceTransforms::Type_fr_RF_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,1) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(0,2) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(0,4) = ((((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(0,5) = ((((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(1,0) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(1,1) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,2) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,3) = ((((- 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,4) = (((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__));
    (*this)(1,5) = ((((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__));
    (*this)(2,1) =  cos__q_RF_HAA__;
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = ((( 0.35 *  cos__q_RF_HFE__) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(2,4) = ((( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(2,5) = (( 0.3735 *  cos__q_RF_HAA__) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__));
    (*this)(3,3) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(3,4) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(3,5) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,3) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(4,4) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,5) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,4) =  cos__q_RF_HAA__;
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LH_lowerleg_X_fr_trunk::Type_fr_LH_lowerleg_X_fr_trunk()
{
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LH_lowerleg_X_fr_trunk& iit::HyQ::ForceTransforms::Type_fr_LH_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,1) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,2) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,4) = (((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(0,5) = ((((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(1,0) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(1,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,2) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,3) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,4) = ((((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__));
    (*this)(1,5) = (((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__));
    (*this)(2,1) =  cos__q_LH_HAA__;
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = ((( 0.35 *  cos__q_LH_HFE__) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(2,4) = (((- 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(2,5) = (((- 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(3,3) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(3,4) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,5) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,3) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(4,4) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,5) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,4) =  cos__q_LH_HAA__;
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RH_lowerleg_X_fr_trunk::Type_fr_RH_lowerleg_X_fr_trunk()
{
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RH_lowerleg_X_fr_trunk& iit::HyQ::ForceTransforms::Type_fr_RH_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,1) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(0,2) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(0,4) = (((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(0,5) = (((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + ((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(1,0) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(1,1) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,2) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,3) = ((((- 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,4) = ((((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__));
    (*this)(1,5) = ((((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__));
    (*this)(2,1) =  cos__q_RH_HAA__;
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = ((( 0.35 *  cos__q_RH_HFE__) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(2,4) = ((( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(2,5) = (((- 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(3,3) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(3,4) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(3,5) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,3) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(4,4) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,5) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,4) =  cos__q_RH_HAA__;
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_LF_foot_X_fr_trunk::Type_LF_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(1,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_LF_foot_X_fr_trunk& iit::HyQ::ForceTransforms::Type_LF_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(0,1) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,2) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,3) = ((((- 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,4) = (((((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__)) - ( lengths_values -> foot_x *  cos__q_LF_HAA__));
    (*this)(0,5) = (((((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + ((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  cos__q_LF_HFE__)) + ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__)) + ( lengths_values -> foot_x *  sin__q_LF_HAA__));
    (*this)(1,1) =  cos__q_LF_HAA__;
    (*this)(1,2) = - sin__q_LF_HAA__;
    (*this)(1,3) = ((((((- lengths_values -> foot_x *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + (( lengths_values -> foot_x *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + ( 0.35 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(1,4) = ((((((- lengths_values -> foot_x *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(1,5) = ((((((- lengths_values -> foot_x *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(2,0) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(2,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(2,2) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(2,3) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(2,4) = (((((( 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + ((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(2,5) = ((((((- 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + ((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  cos__q_LF_HFE__)) + ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + ((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(3,3) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(3,4) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(3,5) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,4) =  cos__q_LF_HAA__;
    (*this)(4,5) = - sin__q_LF_HAA__;
    (*this)(5,3) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(5,4) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(5,5) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    return *this;
}
iit::HyQ::ForceTransforms::Type_RF_foot_X_fr_trunk::Type_RF_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(1,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_RF_foot_X_fr_trunk& iit::HyQ::ForceTransforms::Type_RF_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(0,1) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,2) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,4) = (((((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + ((((( 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__)) - ( lengths_values -> foot_x *  cos__q_RF_HAA__));
    (*this)(0,5) = ((((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + ((((( 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) - ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__)) - ( lengths_values -> foot_x *  sin__q_RF_HAA__));
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) =  sin__q_RF_HAA__;
    (*this)(1,3) = ((((((- lengths_values -> foot_x *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + (( lengths_values -> foot_x *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + ( 0.35 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(1,4) = (((((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(1,5) = ((((((- lengths_values -> foot_x *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(2,0) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(2,1) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,2) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(2,3) = (((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,4) = (((((( 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(2,5) = (((((( 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) - ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(3,3) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(3,4) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(3,5) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(4,5) =  sin__q_RF_HAA__;
    (*this)(5,3) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(5,4) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,5) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    return *this;
}
iit::HyQ::ForceTransforms::Type_LH_foot_X_fr_trunk::Type_LH_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(1,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_LH_foot_X_fr_trunk& iit::HyQ::ForceTransforms::Type_LH_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(0,1) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,2) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,3) = ((((- 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,4) = (((((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__)) - ( lengths_values -> foot_x *  cos__q_LH_HAA__));
    (*this)(0,5) = (((((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + ((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  cos__q_LH_HFE__)) + ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__)) + ( lengths_values -> foot_x *  sin__q_LH_HAA__));
    (*this)(1,1) =  cos__q_LH_HAA__;
    (*this)(1,2) = - sin__q_LH_HAA__;
    (*this)(1,3) = ((((((- lengths_values -> foot_x *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + (( lengths_values -> foot_x *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + ( 0.35 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(1,4) = ((((((- lengths_values -> foot_x *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(1,5) = ((((((- lengths_values -> foot_x *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(2,0) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(2,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(2,2) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(2,3) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(2,4) = ((((((- 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(2,5) = (((((( 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + ((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  cos__q_LH_HFE__)) + ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + ((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(3,3) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(3,4) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(3,5) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,4) =  cos__q_LH_HAA__;
    (*this)(4,5) = - sin__q_LH_HAA__;
    (*this)(5,3) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(5,4) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(5,5) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    return *this;
}
iit::HyQ::ForceTransforms::Type_RH_foot_X_fr_trunk::Type_RH_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(1,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_RH_foot_X_fr_trunk& iit::HyQ::ForceTransforms::Type_RH_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(0,1) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,2) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,4) = (((((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + (((((- 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__)) - ( lengths_values -> foot_x *  cos__q_RH_HAA__));
    (*this)(0,5) = ((((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + (((((- 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) - ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__)) - ( lengths_values -> foot_x *  sin__q_RH_HAA__));
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) =  sin__q_RH_HAA__;
    (*this)(1,3) = ((((((- lengths_values -> foot_x *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + (( lengths_values -> foot_x *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + ( 0.35 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(1,4) = (((((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(1,5) = ((((((- lengths_values -> foot_x *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(2,0) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(2,1) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,2) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(2,3) = (((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,4) = ((((((- 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(2,5) = ((((((- 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) - ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(3,3) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(3,4) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(3,5) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(4,5) =  sin__q_RH_HAA__;
    (*this)(5,3) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(5,4) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,5) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_shin::Type_fr_trunk_X_LF_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_shin& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LF_shin::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,1) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,4) = (((( 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + ((( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,5) = ((((((- lengths_values -> LF_shin *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) + (( lengths_values -> LF_shin *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) + ( 0.35 *  cos__q_LF_HFE__)) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(1,0) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = ((((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(1,4) = ((((((- 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__)) *  cos__q_LF_KFE__)) + ( lengths_values -> LF_shin *  cos__q_LF_HAA__));
    (*this)(1,5) = ((((((- lengths_values -> LF_shin *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> LF_shin *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  sin__q_LF_HAA__));
    (*this)(2,0) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = (((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  sin__q_LF_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(2,4) = ((((((( 0.08 *  sin__q_LF_HAA__) -  0.207) *  sin__q_LF_HFE__) + (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__)) *  cos__q_LF_KFE__)) - ( lengths_values -> LF_shin *  sin__q_LF_HAA__));
    (*this)(2,5) = ((((((- lengths_values -> LF_shin *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__)) + ( 0.3735 *  cos__q_LF_HAA__));
    (*this)(3,3) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(3,4) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(4,3) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,4) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,3) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,4) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_shin::Type_fr_trunk_X_RF_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_shin& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RF_shin::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,1) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(0,4) = ((((- 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,5) = ((((((- lengths_values -> RF_shin *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + (( lengths_values -> RF_shin *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + ( 0.35 *  cos__q_RF_HFE__)) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(1,0) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(1,1) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = ((((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  sin__q_RF_KFE__) + (((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(1,4) = ((((((- 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__)) *  cos__q_RF_KFE__)) + ( lengths_values -> RF_shin *  cos__q_RF_HAA__));
    (*this)(1,5) = (((((( lengths_values -> RF_shin *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> RF_shin *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__)) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(2,0) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = ((((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  sin__q_RF_KFE__) + ((((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(2,4) = (((((( 0.207 - ( 0.08 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) - (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__)) *  cos__q_RF_KFE__)) + ( lengths_values -> RF_shin *  sin__q_RF_HAA__));
    (*this)(2,5) = ((((((- lengths_values -> RF_shin *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> RF_shin *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__)) + ( 0.3735 *  cos__q_RF_HAA__));
    (*this)(3,3) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(3,4) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(4,3) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(4,4) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,3) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,4) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_shin::Type_fr_trunk_X_LH_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_shin& iit::HyQ::ForceTransforms::Type_fr_trunk_X_LH_shin::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,1) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,4) = (((( 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + ((( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,5) = ((((((- lengths_values -> LH_shin *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) + (( lengths_values -> LH_shin *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) + ( 0.35 *  cos__q_LH_HFE__)) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(1,0) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = (((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(1,4) = (((((( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - (( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__)) *  cos__q_LH_KFE__)) + ( lengths_values -> LH_shin *  cos__q_LH_HAA__));
    (*this)(1,5) = ((((((- lengths_values -> LH_shin *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> LH_shin *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(2,0) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = ((((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  sin__q_LH_KFE__) + (((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(2,4) = ((((((( 0.08 *  sin__q_LH_HAA__) -  0.207) *  sin__q_LH_HFE__) - (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__)) *  cos__q_LH_KFE__)) - ( lengths_values -> LH_shin *  sin__q_LH_HAA__));
    (*this)(2,5) = ((((((- lengths_values -> LH_shin *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__)) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(3,3) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(3,4) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(4,3) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,4) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,3) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,4) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_shin::Type_fr_trunk_X_RH_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_shin& iit::HyQ::ForceTransforms::Type_fr_trunk_X_RH_shin::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,1) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,3) = (((( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(0,4) = ((((- 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,5) = ((((((- lengths_values -> RH_shin *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + (( lengths_values -> RH_shin *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + ( 0.35 *  cos__q_RH_HFE__)) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(1,0) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(1,1) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = (((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  sin__q_RH_KFE__) + (((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(1,4) = (((((( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - (( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__)) *  cos__q_RH_KFE__)) + ( lengths_values -> RH_shin *  cos__q_RH_HAA__));
    (*this)(1,5) = (((((( lengths_values -> RH_shin *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> RH_shin *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__)) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(2,0) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = (((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  sin__q_RH_KFE__) + ((((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(2,4) = (((((( 0.207 - ( 0.08 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) + (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__)) *  cos__q_RH_KFE__)) + ( lengths_values -> RH_shin *  sin__q_RH_HAA__));
    (*this)(2,5) = ((((((- lengths_values -> RH_shin *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> RH_shin *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__)) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(3,3) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(3,4) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(4,3) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(4,4) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,3) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,4) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_HAA::Type_fr_trunk_X_fr_LF_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = - 0.207;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = - 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0.3735;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = - 0.3735;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = - 1.0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_HAA& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_HFE::Type_fr_trunk_X_fr_LF_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,4) = 0;
    (*this)(1,1) = 0;
    (*this)(2,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1.0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,4) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_HFE& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_HFE::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,3) = (- 0.207 *  cos__q_LF_HAA__);
    (*this)(0,5) = ( 0.08 - ( 0.207 *  sin__q_LF_HAA__));
    (*this)(1,0) = - sin__q_LF_HAA__;
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = ( 0.3735 *  cos__q_LF_HAA__);
    (*this)(1,4) = ( 0.08 *  cos__q_LF_HAA__);
    (*this)(1,5) = ( 0.3735 *  sin__q_LF_HAA__);
    (*this)(2,0) = - cos__q_LF_HAA__;
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(2,4) = ( 0.207 - ( 0.08 *  sin__q_LF_HAA__));
    (*this)(2,5) = ( 0.3735 *  cos__q_LF_HAA__);
    (*this)(4,3) = - sin__q_LF_HAA__;
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,3) = - cos__q_LF_HAA__;
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_KFE::Type_fr_trunk_X_fr_LF_KFE()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_KFE& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_KFE::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = - sin__q_LF_HFE__;
    (*this)(0,1) = - cos__q_LF_HFE__;
    (*this)(0,3) = ((- 0.207 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__);
    (*this)(0,4) = (( 0.207 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__);
    (*this)(0,5) = ((( 0.35 *  cos__q_LF_HFE__) - ( 0.207 *  sin__q_LF_HAA__)) +  0.08);
    (*this)(1,0) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(1,1) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = ((( 0.08 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.3735 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__));
    (*this)(1,4) = ((((- 0.3735 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.08 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) + ( 0.35 *  cos__q_LF_HAA__));
    (*this)(1,5) = (( 0.3735 *  sin__q_LF_HAA__) - (( 0.35 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(2,0) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(2,1) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = ((( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) - (( 0.3735 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__));
    (*this)(2,4) = (((( 0.3735 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) - ( 0.35 *  sin__q_LF_HAA__));
    (*this)(2,5) = (( 0.3735 *  cos__q_LF_HAA__) - (( 0.35 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__));
    (*this)(3,3) = - sin__q_LF_HFE__;
    (*this)(3,4) = - cos__q_LF_HFE__;
    (*this)(4,3) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(4,4) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(4,5) =  cos__q_LF_HAA__;
    (*this)(5,3) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(5,4) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(5,5) = - sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_HAA::Type_fr_trunk_X_fr_RF_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0.207;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0.3735;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0.3735;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 1.0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_HAA& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_HFE::Type_fr_trunk_X_fr_RF_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,4) = 0;
    (*this)(1,1) = 0;
    (*this)(2,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1.0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,4) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_HFE& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_HFE::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,3) = ( 0.207 *  cos__q_RF_HAA__);
    (*this)(0,5) = ( 0.08 - ( 0.207 *  sin__q_RF_HAA__));
    (*this)(1,0) =  sin__q_RF_HAA__;
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(1,4) = ( 0.08 *  cos__q_RF_HAA__);
    (*this)(1,5) = (- 0.3735 *  sin__q_RF_HAA__);
    (*this)(2,0) = - cos__q_RF_HAA__;
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = ( 0.3735 *  sin__q_RF_HAA__);
    (*this)(2,4) = (( 0.08 *  sin__q_RF_HAA__) -  0.207);
    (*this)(2,5) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(4,3) =  sin__q_RF_HAA__;
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,3) = - cos__q_RF_HAA__;
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_KFE::Type_fr_trunk_X_fr_RF_KFE()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_KFE& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_KFE::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = - sin__q_RF_HFE__;
    (*this)(0,1) = - cos__q_RF_HFE__;
    (*this)(0,3) = (( 0.207 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__);
    (*this)(0,4) = ((- 0.207 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__);
    (*this)(0,5) = ((( 0.35 *  cos__q_RF_HFE__) - ( 0.207 *  sin__q_RF_HAA__)) +  0.08);
    (*this)(1,0) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(1,1) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = ((( 0.08 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.3735 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__));
    (*this)(1,4) = ((((- 0.3735 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.08 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.35 *  cos__q_RF_HAA__));
    (*this)(1,5) = ((( 0.35 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) - ( 0.3735 *  sin__q_RF_HAA__));
    (*this)(2,0) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(2,1) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = (((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  sin__q_RF_HFE__) + (( 0.3735 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__));
    (*this)(2,4) = ((((- 0.3735 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + ((( 0.08 *  sin__q_RF_HAA__) -  0.207) *  cos__q_RF_HFE__)) + ( 0.35 *  sin__q_RF_HAA__));
    (*this)(2,5) = (( 0.3735 *  cos__q_RF_HAA__) - (( 0.35 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__));
    (*this)(3,3) = - sin__q_RF_HFE__;
    (*this)(3,4) = - cos__q_RF_HFE__;
    (*this)(4,3) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(4,4) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(4,5) =  cos__q_RF_HAA__;
    (*this)(5,3) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(5,4) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(5,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_HAA::Type_fr_trunk_X_fr_LH_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = - 0.207;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = - 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = - 0.3735;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0.3735;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = - 1.0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_HAA& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_HFE::Type_fr_trunk_X_fr_LH_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,4) = 0;
    (*this)(1,1) = 0;
    (*this)(2,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1.0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,4) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_HFE& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_HFE::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,3) = (- 0.207 *  cos__q_LH_HAA__);
    (*this)(0,5) = ( 0.08 - ( 0.207 *  sin__q_LH_HAA__));
    (*this)(1,0) = - sin__q_LH_HAA__;
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = (- 0.3735 *  cos__q_LH_HAA__);
    (*this)(1,4) = ( 0.08 *  cos__q_LH_HAA__);
    (*this)(1,5) = (- 0.3735 *  sin__q_LH_HAA__);
    (*this)(2,0) = - cos__q_LH_HAA__;
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(2,4) = ( 0.207 - ( 0.08 *  sin__q_LH_HAA__));
    (*this)(2,5) = (- 0.3735 *  cos__q_LH_HAA__);
    (*this)(4,3) = - sin__q_LH_HAA__;
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,3) = - cos__q_LH_HAA__;
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_KFE::Type_fr_trunk_X_fr_LH_KFE()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_KFE& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_KFE::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = - sin__q_LH_HFE__;
    (*this)(0,1) = - cos__q_LH_HFE__;
    (*this)(0,3) = ((- 0.207 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__);
    (*this)(0,4) = (( 0.207 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__);
    (*this)(0,5) = ((( 0.35 *  cos__q_LH_HFE__) - ( 0.207 *  sin__q_LH_HAA__)) +  0.08);
    (*this)(1,0) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(1,1) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = ((( 0.08 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.3735 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__));
    (*this)(1,4) = (((( 0.3735 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.08 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) + ( 0.35 *  cos__q_LH_HAA__));
    (*this)(1,5) = (((- 0.35 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) - ( 0.3735 *  sin__q_LH_HAA__));
    (*this)(2,0) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(2,1) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = ((( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) + (( 0.3735 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__));
    (*this)(2,4) = ((((- 0.3735 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) + (( 0.207 - ( 0.08 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) - ( 0.35 *  sin__q_LH_HAA__));
    (*this)(2,5) = (((- 0.35 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - ( 0.3735 *  cos__q_LH_HAA__));
    (*this)(3,3) = - sin__q_LH_HFE__;
    (*this)(3,4) = - cos__q_LH_HFE__;
    (*this)(4,3) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(4,4) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(4,5) =  cos__q_LH_HAA__;
    (*this)(5,3) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(5,4) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(5,5) = - sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_HAA::Type_fr_trunk_X_fr_RH_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0.207;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = - 0.3735;
    (*this)(1,4) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = - 0.3735;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 1.0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_HAA& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_HFE::Type_fr_trunk_X_fr_RH_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,4) = 0;
    (*this)(1,1) = 0;
    (*this)(2,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = - 1.0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,4) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_HFE& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_HFE::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,3) = ( 0.207 *  cos__q_RH_HAA__);
    (*this)(0,5) = ( 0.08 - ( 0.207 *  sin__q_RH_HAA__));
    (*this)(1,0) =  sin__q_RH_HAA__;
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(1,4) = ( 0.08 *  cos__q_RH_HAA__);
    (*this)(1,5) = ( 0.3735 *  sin__q_RH_HAA__);
    (*this)(2,0) = - cos__q_RH_HAA__;
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = (- 0.3735 *  sin__q_RH_HAA__);
    (*this)(2,4) = (( 0.08 *  sin__q_RH_HAA__) -  0.207);
    (*this)(2,5) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(4,3) =  sin__q_RH_HAA__;
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,3) = - cos__q_RH_HAA__;
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_KFE::Type_fr_trunk_X_fr_RH_KFE()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_KFE& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_KFE::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = - sin__q_RH_HFE__;
    (*this)(0,1) = - cos__q_RH_HFE__;
    (*this)(0,3) = (( 0.207 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__);
    (*this)(0,4) = ((- 0.207 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__);
    (*this)(0,5) = ((( 0.35 *  cos__q_RH_HFE__) - ( 0.207 *  sin__q_RH_HAA__)) +  0.08);
    (*this)(1,0) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(1,1) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = ((( 0.08 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.3735 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__));
    (*this)(1,4) = (((( 0.3735 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.08 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.35 *  cos__q_RH_HAA__));
    (*this)(1,5) = ((( 0.35 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ( 0.3735 *  sin__q_RH_HAA__));
    (*this)(2,0) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(2,1) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = (((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  sin__q_RH_HFE__) - (( 0.3735 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__));
    (*this)(2,4) = (((( 0.3735 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + ((( 0.08 *  sin__q_RH_HAA__) -  0.207) *  cos__q_RH_HFE__)) + ( 0.35 *  sin__q_RH_HAA__));
    (*this)(2,5) = (((- 0.35 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - ( 0.3735 *  cos__q_RH_HAA__));
    (*this)(3,3) = - sin__q_RH_HFE__;
    (*this)(3,4) = - cos__q_RH_HFE__;
    (*this)(4,3) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(4,4) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(4,5) =  cos__q_RH_HAA__;
    (*this)(5,3) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(5,4) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(5,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LF_hipassembly_X_fr_trunk::Type_fr_LF_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LF_hipassembly_X_fr_trunk& iit::HyQ::ForceTransforms::Type_fr_LF_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,1) = - sin__q_LF_HAA__;
    (*this)(0,2) = - cos__q_LF_HAA__;
    (*this)(0,3) = (- 0.207 *  cos__q_LF_HAA__);
    (*this)(0,4) = ( 0.3735 *  cos__q_LF_HAA__);
    (*this)(0,5) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(1,1) = - cos__q_LF_HAA__;
    (*this)(1,2) =  sin__q_LF_HAA__;
    (*this)(1,3) = ( 0.207 *  sin__q_LF_HAA__);
    (*this)(1,4) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(1,5) = (- 0.3735 *  cos__q_LF_HAA__);
    (*this)(3,4) = - sin__q_LF_HAA__;
    (*this)(3,5) = - cos__q_LF_HAA__;
    (*this)(4,4) = - cos__q_LF_HAA__;
    (*this)(4,5) =  sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_hipassembly::Type_fr_trunk_X_fr_LF_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_hipassembly& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LF_hipassembly::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,3) = (- 0.207 *  cos__q_LF_HAA__);
    (*this)(0,4) = ( 0.207 *  sin__q_LF_HAA__);
    (*this)(1,0) = - sin__q_LF_HAA__;
    (*this)(1,1) = - cos__q_LF_HAA__;
    (*this)(1,3) = ( 0.3735 *  cos__q_LF_HAA__);
    (*this)(1,4) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(2,0) = - cos__q_LF_HAA__;
    (*this)(2,1) =  sin__q_LF_HAA__;
    (*this)(2,3) = (- 0.3735 *  sin__q_LF_HAA__);
    (*this)(2,4) = (- 0.3735 *  cos__q_LF_HAA__);
    (*this)(4,3) = - sin__q_LF_HAA__;
    (*this)(4,4) = - cos__q_LF_HAA__;
    (*this)(5,3) = - cos__q_LF_HAA__;
    (*this)(5,4) =  sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LF_upperleg_X_fr_LF_hipassembly::Type_fr_LF_upperleg_X_fr_LF_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = - 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = - 0.08;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = - 1.0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LF_upperleg_X_fr_LF_hipassembly& iit::HyQ::ForceTransforms::Type_fr_LF_upperleg_X_fr_LF_hipassembly::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double cos__q_LF_HFE__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    
    (*this)(0,0) =  cos__q_LF_HFE__;
    (*this)(0,2) =  sin__q_LF_HFE__;
    (*this)(0,4) = (- 0.08 *  sin__q_LF_HFE__);
    (*this)(1,0) = - sin__q_LF_HFE__;
    (*this)(1,2) =  cos__q_LF_HFE__;
    (*this)(1,4) = (- 0.08 *  cos__q_LF_HFE__);
    (*this)(3,3) =  cos__q_LF_HFE__;
    (*this)(3,5) =  sin__q_LF_HFE__;
    (*this)(4,3) = - sin__q_LF_HFE__;
    (*this)(4,5) =  cos__q_LF_HFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LF_hipassembly_X_fr_LF_upperleg::Type_fr_LF_hipassembly_X_fr_LF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = - 1.0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = - 0.08;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = - 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LF_hipassembly_X_fr_LF_upperleg& iit::HyQ::ForceTransforms::Type_fr_LF_hipassembly_X_fr_LF_upperleg::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double cos__q_LF_HFE__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    
    (*this)(0,0) =  cos__q_LF_HFE__;
    (*this)(0,1) = - sin__q_LF_HFE__;
    (*this)(1,3) = (- 0.08 *  sin__q_LF_HFE__);
    (*this)(1,4) = (- 0.08 *  cos__q_LF_HFE__);
    (*this)(2,0) =  sin__q_LF_HFE__;
    (*this)(2,1) =  cos__q_LF_HFE__;
    (*this)(3,3) =  cos__q_LF_HFE__;
    (*this)(3,4) = - sin__q_LF_HFE__;
    (*this)(5,3) =  sin__q_LF_HFE__;
    (*this)(5,4) =  cos__q_LF_HFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LF_lowerleg_X_fr_LF_upperleg::Type_fr_LF_lowerleg_X_fr_LF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(2,4) = - 0.35;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1.0;
}
const iit::HyQ::ForceTransforms::Type_fr_LF_lowerleg_X_fr_LF_upperleg& iit::HyQ::ForceTransforms::Type_fr_LF_lowerleg_X_fr_LF_upperleg::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double cos__q_LF_KFE__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    
    (*this)(0,0) =  cos__q_LF_KFE__;
    (*this)(0,1) =  sin__q_LF_KFE__;
    (*this)(0,5) = ( 0.35 *  sin__q_LF_KFE__);
    (*this)(1,0) = - sin__q_LF_KFE__;
    (*this)(1,1) =  cos__q_LF_KFE__;
    (*this)(1,5) = ( 0.35 *  cos__q_LF_KFE__);
    (*this)(3,3) =  cos__q_LF_KFE__;
    (*this)(3,4) =  sin__q_LF_KFE__;
    (*this)(4,3) = - sin__q_LF_KFE__;
    (*this)(4,4) =  cos__q_LF_KFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LF_upperleg_X_fr_LF_lowerleg::Type_fr_LF_upperleg_X_fr_LF_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = - 0.35;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1.0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1;
}
const iit::HyQ::ForceTransforms::Type_fr_LF_upperleg_X_fr_LF_lowerleg& iit::HyQ::ForceTransforms::Type_fr_LF_upperleg_X_fr_LF_lowerleg::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double cos__q_LF_KFE__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    
    (*this)(0,0) =  cos__q_LF_KFE__;
    (*this)(0,1) = - sin__q_LF_KFE__;
    (*this)(1,0) =  sin__q_LF_KFE__;
    (*this)(1,1) =  cos__q_LF_KFE__;
    (*this)(2,3) = ( 0.35 *  sin__q_LF_KFE__);
    (*this)(2,4) = ( 0.35 *  cos__q_LF_KFE__);
    (*this)(3,3) =  cos__q_LF_KFE__;
    (*this)(3,4) = - sin__q_LF_KFE__;
    (*this)(4,3) =  sin__q_LF_KFE__;
    (*this)(4,4) =  cos__q_LF_KFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RF_hipassembly_X_fr_trunk::Type_fr_RF_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RF_hipassembly_X_fr_trunk& iit::HyQ::ForceTransforms::Type_fr_RF_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,1) =  sin__q_RF_HAA__;
    (*this)(0,2) = - cos__q_RF_HAA__;
    (*this)(0,3) = ( 0.207 *  cos__q_RF_HAA__);
    (*this)(0,4) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(0,5) = ( 0.3735 *  sin__q_RF_HAA__);
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) =  sin__q_RF_HAA__;
    (*this)(1,3) = (- 0.207 *  sin__q_RF_HAA__);
    (*this)(1,4) = (- 0.3735 *  sin__q_RF_HAA__);
    (*this)(1,5) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(3,4) =  sin__q_RF_HAA__;
    (*this)(3,5) = - cos__q_RF_HAA__;
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(4,5) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_hipassembly::Type_fr_trunk_X_fr_RF_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_hipassembly& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RF_hipassembly::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,3) = ( 0.207 *  cos__q_RF_HAA__);
    (*this)(0,4) = (- 0.207 *  sin__q_RF_HAA__);
    (*this)(1,0) =  sin__q_RF_HAA__;
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,3) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(1,4) = (- 0.3735 *  sin__q_RF_HAA__);
    (*this)(2,0) = - cos__q_RF_HAA__;
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(2,3) = ( 0.3735 *  sin__q_RF_HAA__);
    (*this)(2,4) = ( 0.3735 *  cos__q_RF_HAA__);
    (*this)(4,3) =  sin__q_RF_HAA__;
    (*this)(4,4) =  cos__q_RF_HAA__;
    (*this)(5,3) = - cos__q_RF_HAA__;
    (*this)(5,4) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RF_upperleg_X_fr_RF_hipassembly::Type_fr_RF_upperleg_X_fr_RF_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0.08;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1.0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RF_upperleg_X_fr_RF_hipassembly& iit::HyQ::ForceTransforms::Type_fr_RF_upperleg_X_fr_RF_hipassembly::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double cos__q_RF_HFE__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    
    (*this)(0,0) =  cos__q_RF_HFE__;
    (*this)(0,2) = - sin__q_RF_HFE__;
    (*this)(0,4) = ( 0.08 *  sin__q_RF_HFE__);
    (*this)(1,0) = - sin__q_RF_HFE__;
    (*this)(1,2) = - cos__q_RF_HFE__;
    (*this)(1,4) = ( 0.08 *  cos__q_RF_HFE__);
    (*this)(3,3) =  cos__q_RF_HFE__;
    (*this)(3,5) = - sin__q_RF_HFE__;
    (*this)(4,3) = - sin__q_RF_HFE__;
    (*this)(4,5) = - cos__q_RF_HFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RF_hipassembly_X_fr_RF_upperleg::Type_fr_RF_hipassembly_X_fr_RF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1.0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0.08;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RF_hipassembly_X_fr_RF_upperleg& iit::HyQ::ForceTransforms::Type_fr_RF_hipassembly_X_fr_RF_upperleg::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double cos__q_RF_HFE__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    
    (*this)(0,0) =  cos__q_RF_HFE__;
    (*this)(0,1) = - sin__q_RF_HFE__;
    (*this)(1,3) = ( 0.08 *  sin__q_RF_HFE__);
    (*this)(1,4) = ( 0.08 *  cos__q_RF_HFE__);
    (*this)(2,0) = - sin__q_RF_HFE__;
    (*this)(2,1) = - cos__q_RF_HFE__;
    (*this)(3,3) =  cos__q_RF_HFE__;
    (*this)(3,4) = - sin__q_RF_HFE__;
    (*this)(5,3) = - sin__q_RF_HFE__;
    (*this)(5,4) = - cos__q_RF_HFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RF_lowerleg_X_fr_RF_upperleg::Type_fr_RF_lowerleg_X_fr_RF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(2,4) = - 0.35;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1.0;
}
const iit::HyQ::ForceTransforms::Type_fr_RF_lowerleg_X_fr_RF_upperleg& iit::HyQ::ForceTransforms::Type_fr_RF_lowerleg_X_fr_RF_upperleg::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double cos__q_RF_KFE__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    
    (*this)(0,0) =  cos__q_RF_KFE__;
    (*this)(0,1) =  sin__q_RF_KFE__;
    (*this)(0,5) = ( 0.35 *  sin__q_RF_KFE__);
    (*this)(1,0) = - sin__q_RF_KFE__;
    (*this)(1,1) =  cos__q_RF_KFE__;
    (*this)(1,5) = ( 0.35 *  cos__q_RF_KFE__);
    (*this)(3,3) =  cos__q_RF_KFE__;
    (*this)(3,4) =  sin__q_RF_KFE__;
    (*this)(4,3) = - sin__q_RF_KFE__;
    (*this)(4,4) =  cos__q_RF_KFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RF_upperleg_X_fr_RF_lowerleg::Type_fr_RF_upperleg_X_fr_RF_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = - 0.35;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1.0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1;
}
const iit::HyQ::ForceTransforms::Type_fr_RF_upperleg_X_fr_RF_lowerleg& iit::HyQ::ForceTransforms::Type_fr_RF_upperleg_X_fr_RF_lowerleg::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double cos__q_RF_KFE__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    
    (*this)(0,0) =  cos__q_RF_KFE__;
    (*this)(0,1) = - sin__q_RF_KFE__;
    (*this)(1,0) =  sin__q_RF_KFE__;
    (*this)(1,1) =  cos__q_RF_KFE__;
    (*this)(2,3) = ( 0.35 *  sin__q_RF_KFE__);
    (*this)(2,4) = ( 0.35 *  cos__q_RF_KFE__);
    (*this)(3,3) =  cos__q_RF_KFE__;
    (*this)(3,4) = - sin__q_RF_KFE__;
    (*this)(4,3) =  sin__q_RF_KFE__;
    (*this)(4,4) =  cos__q_RF_KFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LH_hipassembly_X_fr_trunk::Type_fr_LH_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = - 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LH_hipassembly_X_fr_trunk& iit::HyQ::ForceTransforms::Type_fr_LH_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,1) = - sin__q_LH_HAA__;
    (*this)(0,2) = - cos__q_LH_HAA__;
    (*this)(0,3) = (- 0.207 *  cos__q_LH_HAA__);
    (*this)(0,4) = (- 0.3735 *  cos__q_LH_HAA__);
    (*this)(0,5) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(1,1) = - cos__q_LH_HAA__;
    (*this)(1,2) =  sin__q_LH_HAA__;
    (*this)(1,3) = ( 0.207 *  sin__q_LH_HAA__);
    (*this)(1,4) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(1,5) = ( 0.3735 *  cos__q_LH_HAA__);
    (*this)(3,4) = - sin__q_LH_HAA__;
    (*this)(3,5) = - cos__q_LH_HAA__;
    (*this)(4,4) = - cos__q_LH_HAA__;
    (*this)(4,5) =  sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_hipassembly::Type_fr_trunk_X_fr_LH_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = - 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_hipassembly& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_LH_hipassembly::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,3) = (- 0.207 *  cos__q_LH_HAA__);
    (*this)(0,4) = ( 0.207 *  sin__q_LH_HAA__);
    (*this)(1,0) = - sin__q_LH_HAA__;
    (*this)(1,1) = - cos__q_LH_HAA__;
    (*this)(1,3) = (- 0.3735 *  cos__q_LH_HAA__);
    (*this)(1,4) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(2,0) = - cos__q_LH_HAA__;
    (*this)(2,1) =  sin__q_LH_HAA__;
    (*this)(2,3) = ( 0.3735 *  sin__q_LH_HAA__);
    (*this)(2,4) = ( 0.3735 *  cos__q_LH_HAA__);
    (*this)(4,3) = - sin__q_LH_HAA__;
    (*this)(4,4) = - cos__q_LH_HAA__;
    (*this)(5,3) = - cos__q_LH_HAA__;
    (*this)(5,4) =  sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LH_upperleg_X_fr_LH_hipassembly::Type_fr_LH_upperleg_X_fr_LH_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = - 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = - 0.08;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = - 1.0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LH_upperleg_X_fr_LH_hipassembly& iit::HyQ::ForceTransforms::Type_fr_LH_upperleg_X_fr_LH_hipassembly::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double cos__q_LH_HFE__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    
    (*this)(0,0) =  cos__q_LH_HFE__;
    (*this)(0,2) =  sin__q_LH_HFE__;
    (*this)(0,4) = (- 0.08 *  sin__q_LH_HFE__);
    (*this)(1,0) = - sin__q_LH_HFE__;
    (*this)(1,2) =  cos__q_LH_HFE__;
    (*this)(1,4) = (- 0.08 *  cos__q_LH_HFE__);
    (*this)(3,3) =  cos__q_LH_HFE__;
    (*this)(3,5) =  sin__q_LH_HFE__;
    (*this)(4,3) = - sin__q_LH_HFE__;
    (*this)(4,5) =  cos__q_LH_HFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LH_hipassembly_X_fr_LH_upperleg::Type_fr_LH_hipassembly_X_fr_LH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = - 1.0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = - 0.08;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = - 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_LH_hipassembly_X_fr_LH_upperleg& iit::HyQ::ForceTransforms::Type_fr_LH_hipassembly_X_fr_LH_upperleg::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double cos__q_LH_HFE__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    
    (*this)(0,0) =  cos__q_LH_HFE__;
    (*this)(0,1) = - sin__q_LH_HFE__;
    (*this)(1,3) = (- 0.08 *  sin__q_LH_HFE__);
    (*this)(1,4) = (- 0.08 *  cos__q_LH_HFE__);
    (*this)(2,0) =  sin__q_LH_HFE__;
    (*this)(2,1) =  cos__q_LH_HFE__;
    (*this)(3,3) =  cos__q_LH_HFE__;
    (*this)(3,4) = - sin__q_LH_HFE__;
    (*this)(5,3) =  sin__q_LH_HFE__;
    (*this)(5,4) =  cos__q_LH_HFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LH_lowerleg_X_fr_LH_upperleg::Type_fr_LH_lowerleg_X_fr_LH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(2,4) = - 0.35;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1.0;
}
const iit::HyQ::ForceTransforms::Type_fr_LH_lowerleg_X_fr_LH_upperleg& iit::HyQ::ForceTransforms::Type_fr_LH_lowerleg_X_fr_LH_upperleg::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double cos__q_LH_KFE__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    
    (*this)(0,0) =  cos__q_LH_KFE__;
    (*this)(0,1) =  sin__q_LH_KFE__;
    (*this)(0,5) = ( 0.35 *  sin__q_LH_KFE__);
    (*this)(1,0) = - sin__q_LH_KFE__;
    (*this)(1,1) =  cos__q_LH_KFE__;
    (*this)(1,5) = ( 0.35 *  cos__q_LH_KFE__);
    (*this)(3,3) =  cos__q_LH_KFE__;
    (*this)(3,4) =  sin__q_LH_KFE__;
    (*this)(4,3) = - sin__q_LH_KFE__;
    (*this)(4,4) =  cos__q_LH_KFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_LH_upperleg_X_fr_LH_lowerleg::Type_fr_LH_upperleg_X_fr_LH_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = - 0.35;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1.0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1;
}
const iit::HyQ::ForceTransforms::Type_fr_LH_upperleg_X_fr_LH_lowerleg& iit::HyQ::ForceTransforms::Type_fr_LH_upperleg_X_fr_LH_lowerleg::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double cos__q_LH_KFE__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    
    (*this)(0,0) =  cos__q_LH_KFE__;
    (*this)(0,1) = - sin__q_LH_KFE__;
    (*this)(1,0) =  sin__q_LH_KFE__;
    (*this)(1,1) =  cos__q_LH_KFE__;
    (*this)(2,3) = ( 0.35 *  sin__q_LH_KFE__);
    (*this)(2,4) = ( 0.35 *  cos__q_LH_KFE__);
    (*this)(3,3) =  cos__q_LH_KFE__;
    (*this)(3,4) = - sin__q_LH_KFE__;
    (*this)(4,3) =  sin__q_LH_KFE__;
    (*this)(4,4) =  cos__q_LH_KFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RH_hipassembly_X_fr_trunk::Type_fr_RH_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 1.0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RH_hipassembly_X_fr_trunk& iit::HyQ::ForceTransforms::Type_fr_RH_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,1) =  sin__q_RH_HAA__;
    (*this)(0,2) = - cos__q_RH_HAA__;
    (*this)(0,3) = ( 0.207 *  cos__q_RH_HAA__);
    (*this)(0,4) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(0,5) = (- 0.3735 *  sin__q_RH_HAA__);
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) =  sin__q_RH_HAA__;
    (*this)(1,3) = (- 0.207 *  sin__q_RH_HAA__);
    (*this)(1,4) = ( 0.3735 *  sin__q_RH_HAA__);
    (*this)(1,5) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(3,4) =  sin__q_RH_HAA__;
    (*this)(3,5) = - cos__q_RH_HAA__;
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(4,5) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_hipassembly::Type_fr_trunk_X_fr_RH_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,5) = 0.207;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 0;
    (*this)(3,4) = 0;
    (*this)(3,5) = 1.0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_hipassembly& iit::HyQ::ForceTransforms::Type_fr_trunk_X_fr_RH_hipassembly::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,3) = ( 0.207 *  cos__q_RH_HAA__);
    (*this)(0,4) = (- 0.207 *  sin__q_RH_HAA__);
    (*this)(1,0) =  sin__q_RH_HAA__;
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,3) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(1,4) = ( 0.3735 *  sin__q_RH_HAA__);
    (*this)(2,0) = - cos__q_RH_HAA__;
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(2,3) = (- 0.3735 *  sin__q_RH_HAA__);
    (*this)(2,4) = (- 0.3735 *  cos__q_RH_HAA__);
    (*this)(4,3) =  sin__q_RH_HAA__;
    (*this)(4,4) =  cos__q_RH_HAA__;
    (*this)(5,3) = - cos__q_RH_HAA__;
    (*this)(5,4) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RH_upperleg_X_fr_RH_hipassembly::Type_fr_RH_upperleg_X_fr_RH_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(0,3) = 0;
    (*this)(0,5) = 0;
    (*this)(1,1) = 0;
    (*this)(1,3) = 0;
    (*this)(1,5) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0.08;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,4) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,4) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 1.0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RH_upperleg_X_fr_RH_hipassembly& iit::HyQ::ForceTransforms::Type_fr_RH_upperleg_X_fr_RH_hipassembly::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double cos__q_RH_HFE__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    
    (*this)(0,0) =  cos__q_RH_HFE__;
    (*this)(0,2) = - sin__q_RH_HFE__;
    (*this)(0,4) = ( 0.08 *  sin__q_RH_HFE__);
    (*this)(1,0) = - sin__q_RH_HFE__;
    (*this)(1,2) = - cos__q_RH_HFE__;
    (*this)(1,4) = ( 0.08 *  cos__q_RH_HFE__);
    (*this)(3,3) =  cos__q_RH_HFE__;
    (*this)(3,5) = - sin__q_RH_HFE__;
    (*this)(4,3) = - sin__q_RH_HFE__;
    (*this)(4,5) = - cos__q_RH_HFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RH_hipassembly_X_fr_RH_upperleg::Type_fr_RH_hipassembly_X_fr_RH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1.0;
    (*this)(1,5) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(2,4) = 0;
    (*this)(2,5) = 0.08;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,3) = 0;
    (*this)(4,4) = 0;
    (*this)(4,5) = 1;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,5) = 0;
}
const iit::HyQ::ForceTransforms::Type_fr_RH_hipassembly_X_fr_RH_upperleg& iit::HyQ::ForceTransforms::Type_fr_RH_hipassembly_X_fr_RH_upperleg::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double cos__q_RH_HFE__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    
    (*this)(0,0) =  cos__q_RH_HFE__;
    (*this)(0,1) = - sin__q_RH_HFE__;
    (*this)(1,3) = ( 0.08 *  sin__q_RH_HFE__);
    (*this)(1,4) = ( 0.08 *  cos__q_RH_HFE__);
    (*this)(2,0) = - sin__q_RH_HFE__;
    (*this)(2,1) = - cos__q_RH_HFE__;
    (*this)(3,3) =  cos__q_RH_HFE__;
    (*this)(3,4) = - sin__q_RH_HFE__;
    (*this)(5,3) = - sin__q_RH_HFE__;
    (*this)(5,4) = - cos__q_RH_HFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RH_lowerleg_X_fr_RH_upperleg::Type_fr_RH_lowerleg_X_fr_RH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(2,4) = - 0.35;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1.0;
}
const iit::HyQ::ForceTransforms::Type_fr_RH_lowerleg_X_fr_RH_upperleg& iit::HyQ::ForceTransforms::Type_fr_RH_lowerleg_X_fr_RH_upperleg::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double cos__q_RH_KFE__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    
    (*this)(0,0) =  cos__q_RH_KFE__;
    (*this)(0,1) =  sin__q_RH_KFE__;
    (*this)(0,5) = ( 0.35 *  sin__q_RH_KFE__);
    (*this)(1,0) = - sin__q_RH_KFE__;
    (*this)(1,1) =  cos__q_RH_KFE__;
    (*this)(1,5) = ( 0.35 *  cos__q_RH_KFE__);
    (*this)(3,3) =  cos__q_RH_KFE__;
    (*this)(3,4) =  sin__q_RH_KFE__;
    (*this)(4,3) = - sin__q_RH_KFE__;
    (*this)(4,4) =  cos__q_RH_KFE__;
    return *this;
}
iit::HyQ::ForceTransforms::Type_fr_RH_upperleg_X_fr_RH_lowerleg::Type_fr_RH_upperleg_X_fr_RH_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(0,4) = 0;
    (*this)(0,5) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(1,4) = 0;
    (*this)(1,5) = - 0.35;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1.0;
    (*this)(2,5) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,5) = 0;
    (*this)(4,0) = 0;
    (*this)(4,1) = 0;
    (*this)(4,2) = 0;
    (*this)(4,5) = 0;
    (*this)(5,0) = 0;
    (*this)(5,1) = 0;
    (*this)(5,2) = 0;
    (*this)(5,3) = 0;
    (*this)(5,4) = 0;
    (*this)(5,5) = 1;
}
const iit::HyQ::ForceTransforms::Type_fr_RH_upperleg_X_fr_RH_lowerleg& iit::HyQ::ForceTransforms::Type_fr_RH_upperleg_X_fr_RH_lowerleg::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double cos__q_RH_KFE__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    
    (*this)(0,0) =  cos__q_RH_KFE__;
    (*this)(0,1) = - sin__q_RH_KFE__;
    (*this)(1,0) =  sin__q_RH_KFE__;
    (*this)(1,1) =  cos__q_RH_KFE__;
    (*this)(2,3) = ( 0.35 *  sin__q_RH_KFE__);
    (*this)(2,4) = ( 0.35 *  cos__q_RH_KFE__);
    (*this)(3,3) =  cos__q_RH_KFE__;
    (*this)(3,4) = - sin__q_RH_KFE__;
    (*this)(4,3) =  sin__q_RH_KFE__;
    (*this)(4,4) =  cos__q_RH_KFE__;
    return *this;
}

iit::HyQ::HomogeneousTransforms::Type_fr_LF_lowerleg_X_LF_foot::Type_fr_LF_lowerleg_X_LF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LF_lowerleg_X_LF_foot& iit::HyQ::HomogeneousTransforms::Type_fr_LF_lowerleg_X_LF_foot::update(const state_t& q) {
    
    
    (*this)(0,3) =  lengths_values -> foot_x;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RF_lowerleg_X_RF_foot::Type_fr_RF_lowerleg_X_RF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RF_lowerleg_X_RF_foot& iit::HyQ::HomogeneousTransforms::Type_fr_RF_lowerleg_X_RF_foot::update(const state_t& q) {
    
    
    (*this)(0,3) =  lengths_values -> foot_x;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LH_lowerleg_X_LH_foot::Type_fr_LH_lowerleg_X_LH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LH_lowerleg_X_LH_foot& iit::HyQ::HomogeneousTransforms::Type_fr_LH_lowerleg_X_LH_foot::update(const state_t& q) {
    
    
    (*this)(0,3) =  lengths_values -> foot_x;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RH_lowerleg_X_RH_foot::Type_fr_RH_lowerleg_X_RH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1;
    (*this)(1,0) = - 1;
    (*this)(1,1) = 0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RH_lowerleg_X_RH_foot& iit::HyQ::HomogeneousTransforms::Type_fr_RH_lowerleg_X_RH_foot::update(const state_t& q) {
    
    
    (*this)(0,3) =  lengths_values -> foot_x;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_hipassemblyCOM::Type_fr_trunk_X_LF_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = 0.2041;
    (*this)(1,2) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_hipassemblyCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(1,0) = - sin__q_LF_HAA__;
    (*this)(1,1) = - cos__q_LF_HAA__;
    (*this)(1,3) = ( 0.207 - ( 0.04263 *  sin__q_LF_HAA__));
    (*this)(2,0) = - cos__q_LF_HAA__;
    (*this)(2,1) =  sin__q_LF_HAA__;
    (*this)(2,3) = (- 0.04263 *  cos__q_LF_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_hipassemblyCOM::Type_fr_trunk_X_RF_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0.2041;
    (*this)(1,2) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_hipassemblyCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(1,0) =  sin__q_RF_HAA__;
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,3) = (( 0.04263 *  sin__q_RF_HAA__) -  0.207);
    (*this)(2,0) = - cos__q_RF_HAA__;
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(2,3) = (- 0.04263 *  cos__q_RF_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_hipassemblyCOM::Type_fr_trunk_X_LH_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = - 0.2041;
    (*this)(1,2) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_hipassemblyCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(1,0) = - sin__q_LH_HAA__;
    (*this)(1,1) = - cos__q_LH_HAA__;
    (*this)(1,3) = ( 0.207 - ( 0.04263 *  sin__q_LH_HAA__));
    (*this)(2,0) = - cos__q_LH_HAA__;
    (*this)(2,1) =  sin__q_LH_HAA__;
    (*this)(2,3) = (- 0.04263 *  cos__q_LH_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_hipassemblyCOM::Type_fr_trunk_X_RH_hipassemblyCOM()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = - 0.2041;
    (*this)(1,2) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_hipassemblyCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_hipassemblyCOM::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(1,0) =  sin__q_RH_HAA__;
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,3) = (( 0.04263 *  sin__q_RH_HAA__) -  0.207);
    (*this)(2,0) = - cos__q_RH_HAA__;
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(2,3) = (- 0.04263 *  cos__q_RH_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_upperlegCOM::Type_fr_trunk_X_LF_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_upperlegCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_upperlegCOM::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = - sin__q_LF_HFE__;
    (*this)(0,1) = - cos__q_LF_HFE__;
    (*this)(0,3) = (((- 0.1507 *  sin__q_LF_HFE__) + ( 0.02625 *  cos__q_LF_HFE__)) +  0.3735);
    (*this)(1,0) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(1,1) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = (((((- 0.02625 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.1507 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  sin__q_LF_HAA__)) +  0.207);
    (*this)(2,0) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(2,1) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = ((((- 0.02625 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) - (( 0.1507 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  cos__q_LF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_upperlegCOM::Type_fr_trunk_X_RF_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_upperlegCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_upperlegCOM::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = - sin__q_RF_HFE__;
    (*this)(0,1) = - cos__q_RF_HFE__;
    (*this)(0,3) = (((- 0.1507 *  sin__q_RF_HFE__) + ( 0.02625 *  cos__q_RF_HFE__)) +  0.3735);
    (*this)(1,0) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(1,1) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = ((((( 0.02625 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) + (( 0.1507 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.08 *  sin__q_RF_HAA__)) -  0.207);
    (*this)(2,0) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(2,1) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = ((((- 0.02625 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) - (( 0.1507 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.08 *  cos__q_RF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_upperlegCOM::Type_fr_trunk_X_LH_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_upperlegCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_upperlegCOM::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = - sin__q_LH_HFE__;
    (*this)(0,1) = - cos__q_LH_HFE__;
    (*this)(0,3) = (((- 0.1507 *  sin__q_LH_HFE__) - ( 0.02625 *  cos__q_LH_HFE__)) -  0.3735);
    (*this)(1,0) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(1,1) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = ((((( 0.02625 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.1507 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  sin__q_LH_HAA__)) +  0.207);
    (*this)(2,0) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(2,1) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = (((( 0.02625 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) - (( 0.1507 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  cos__q_LH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_upperlegCOM::Type_fr_trunk_X_RH_upperlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_upperlegCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_upperlegCOM::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = - sin__q_RH_HFE__;
    (*this)(0,1) = - cos__q_RH_HFE__;
    (*this)(0,3) = (((- 0.1507 *  sin__q_RH_HFE__) - ( 0.02625 *  cos__q_RH_HFE__)) -  0.3735);
    (*this)(1,0) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(1,1) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = (((((- 0.02625 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) + (( 0.1507 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.08 *  sin__q_RH_HAA__)) -  0.207);
    (*this)(2,0) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(2,1) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = (((( 0.02625 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) - (( 0.1507 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.08 *  cos__q_RH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_lowerlegCOM::Type_fr_trunk_X_LF_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_lowerlegCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_lowerlegCOM::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,1) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,3) = (((((- 0.1254 *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( 0.1254 *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - ( 0.35 *  sin__q_LF_HFE__)) +  0.3735);
    (*this)(1,0) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = ((((((( 0.1254 *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.1254 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  sin__q_LF_HAA__)) +  0.207);
    (*this)(2,0) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = (((((( 0.1254 *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( 0.1254 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  cos__q_LF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_lowerlegCOM::Type_fr_trunk_X_RF_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_lowerlegCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_lowerlegCOM::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,1) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,3) = (((((- 0.1254 *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( 0.1254 *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - ( 0.35 *  sin__q_RF_HFE__)) +  0.3735);
    (*this)(1,0) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(1,1) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = (((((((- 0.1254 *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( 0.1254 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.08 *  sin__q_RF_HAA__)) -  0.207);
    (*this)(2,0) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = (((((( 0.1254 *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( 0.1254 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.08 *  cos__q_RF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_lowerlegCOM::Type_fr_trunk_X_LH_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_lowerlegCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_lowerlegCOM::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,1) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,3) = (((((- 0.1254 *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( 0.1254 *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - ( 0.35 *  sin__q_LH_HFE__)) -  0.3735);
    (*this)(1,0) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = ((((((( 0.1254 *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.1254 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  sin__q_LH_HAA__)) +  0.207);
    (*this)(2,0) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = (((((( 0.1254 *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( 0.1254 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  cos__q_LH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_lowerlegCOM::Type_fr_trunk_X_RH_lowerlegCOM()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_lowerlegCOM& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_lowerlegCOM::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,1) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,3) = (((((- 0.1254 *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( 0.1254 *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - ( 0.35 *  sin__q_RH_HFE__)) -  0.3735);
    (*this)(1,0) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(1,1) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = (((((((- 0.1254 *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( 0.1254 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.08 *  sin__q_RH_HAA__)) -  0.207);
    (*this)(2,0) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = (((((( 0.1254 *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( 0.1254 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.08 *  cos__q_RH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_LF_foot_X_fr_LF_lowerleg::Type_LF_foot_X_fr_LF_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_LF_foot_X_fr_LF_lowerleg& iit::HyQ::HomogeneousTransforms::Type_LF_foot_X_fr_LF_lowerleg::update(const state_t& q) {
    
    
    (*this)(2,3) =  lengths_values -> foot_x;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_RF_foot_X_fr_RF_lowerleg::Type_RF_foot_X_fr_RF_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_RF_foot_X_fr_RF_lowerleg& iit::HyQ::HomogeneousTransforms::Type_RF_foot_X_fr_RF_lowerleg::update(const state_t& q) {
    
    
    (*this)(2,3) =  lengths_values -> foot_x;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_LH_foot_X_fr_LH_lowerleg::Type_LH_foot_X_fr_LH_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_LH_foot_X_fr_LH_lowerleg& iit::HyQ::HomogeneousTransforms::Type_LH_foot_X_fr_LH_lowerleg::update(const state_t& q) {
    
    
    (*this)(2,3) =  lengths_values -> foot_x;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_RH_foot_X_fr_RH_lowerleg::Type_RH_foot_X_fr_RH_lowerleg(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(2,0) = - 1;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_RH_foot_X_fr_RH_lowerleg& iit::HyQ::HomogeneousTransforms::Type_RH_foot_X_fr_RH_lowerleg::update(const state_t& q) {
    
    
    (*this)(2,3) =  lengths_values -> foot_x;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_foot::Type_fr_trunk_X_LF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_foot& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_foot::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(0,2) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,3) = (((((- lengths_values -> foot_x *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( lengths_values -> foot_x *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - ( 0.35 *  sin__q_LF_HFE__)) +  0.3735);
    (*this)(1,0) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) =  cos__q_LF_HAA__;
    (*this)(1,2) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(1,3) = ((((((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  sin__q_LF_HAA__)) +  0.207);
    (*this)(2,0) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) = - sin__q_LF_HAA__;
    (*this)(2,2) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(2,3) = (((((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> foot_x *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  cos__q_LF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_foot::Type_fr_trunk_X_RF_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_foot& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_foot::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(0,2) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,3) = (((((- lengths_values -> foot_x *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( lengths_values -> foot_x *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - ( 0.35 *  sin__q_RF_HFE__)) +  0.3735);
    (*this)(1,0) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,3) = (((((((- lengths_values -> foot_x *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> foot_x *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.08 *  sin__q_RF_HAA__)) -  0.207);
    (*this)(2,0) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) =  sin__q_RF_HAA__;
    (*this)(2,2) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(2,3) = (((((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> foot_x *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.08 *  cos__q_RF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_foot::Type_fr_trunk_X_LH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_foot& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_foot::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(0,2) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,3) = (((((- lengths_values -> foot_x *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( lengths_values -> foot_x *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - ( 0.35 *  sin__q_LH_HFE__)) -  0.3735);
    (*this)(1,0) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) =  cos__q_LH_HAA__;
    (*this)(1,2) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(1,3) = ((((((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  sin__q_LH_HAA__)) +  0.207);
    (*this)(2,0) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) = - sin__q_LH_HAA__;
    (*this)(2,2) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(2,3) = (((((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> foot_x *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  cos__q_LH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_foot::Type_fr_trunk_X_RH_foot(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_foot& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_foot::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(0,2) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,3) = (((((- lengths_values -> foot_x *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( lengths_values -> foot_x *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - ( 0.35 *  sin__q_RH_HFE__)) -  0.3735);
    (*this)(1,0) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,3) = (((((((- lengths_values -> foot_x *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> foot_x *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.08 *  sin__q_RH_HAA__)) -  0.207);
    (*this)(2,0) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) =  sin__q_RH_HAA__;
    (*this)(2,2) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(2,3) = (((((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> foot_x *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.08 *  cos__q_RH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LF_lowerleg_X_fr_trunk::Type_fr_LF_lowerleg_X_fr_trunk()
{
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LF_lowerleg_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_fr_LF_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,1) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,2) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,3) = ((((( 0.08 - ( 0.207 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) + ( 0.3735 *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + (((( 0.3735 *  sin__q_LF_HFE__) + ((( 0.207 *  sin__q_LF_HAA__) -  0.08) *  cos__q_LF_HFE__)) -  0.35) *  cos__q_LF_KFE__));
    (*this)(1,0) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(1,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,2) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,3) = (((((- 0.3735 *  sin__q_LF_HFE__) + (( 0.08 - ( 0.207 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) +  0.35) *  sin__q_LF_KFE__) + (((( 0.08 - ( 0.207 *  sin__q_LF_HAA__)) *  sin__q_LF_HFE__) + ( 0.3735 *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(2,1) =  cos__q_LF_HAA__;
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = (- 0.207 *  cos__q_LF_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RF_lowerleg_X_fr_trunk::Type_fr_RF_lowerleg_X_fr_trunk()
{
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RF_lowerleg_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_fr_RF_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,1) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(0,2) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,3) = ((((( 0.08 - ( 0.207 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) + ( 0.3735 *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + (((( 0.3735 *  sin__q_RF_HFE__) + ((( 0.207 *  sin__q_RF_HAA__) -  0.08) *  cos__q_RF_HFE__)) -  0.35) *  cos__q_RF_KFE__));
    (*this)(1,0) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(1,1) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,2) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,3) = (((((- 0.3735 *  sin__q_RF_HFE__) + (( 0.08 - ( 0.207 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) +  0.35) *  sin__q_RF_KFE__) + (((( 0.08 - ( 0.207 *  sin__q_RF_HAA__)) *  sin__q_RF_HFE__) + ( 0.3735 *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(2,1) =  cos__q_RF_HAA__;
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = ( 0.207 *  cos__q_RF_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LH_lowerleg_X_fr_trunk::Type_fr_LH_lowerleg_X_fr_trunk()
{
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LH_lowerleg_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_fr_LH_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,1) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,2) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,3) = ((((( 0.08 - ( 0.207 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) - ( 0.3735 *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + ((((- 0.3735 *  sin__q_LH_HFE__) + ((( 0.207 *  sin__q_LH_HAA__) -  0.08) *  cos__q_LH_HFE__)) -  0.35) *  cos__q_LH_KFE__));
    (*this)(1,0) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(1,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,2) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,3) = ((((( 0.3735 *  sin__q_LH_HFE__) + (( 0.08 - ( 0.207 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) +  0.35) *  sin__q_LH_KFE__) + (((( 0.08 - ( 0.207 *  sin__q_LH_HAA__)) *  sin__q_LH_HFE__) - ( 0.3735 *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(2,1) =  cos__q_LH_HAA__;
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = (- 0.207 *  cos__q_LH_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RH_lowerleg_X_fr_trunk::Type_fr_RH_lowerleg_X_fr_trunk()
{
    (*this)(2,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RH_lowerleg_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_fr_RH_lowerleg_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,1) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(0,2) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,3) = ((((( 0.08 - ( 0.207 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) - ( 0.3735 *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + ((((- 0.3735 *  sin__q_RH_HFE__) + ((( 0.207 *  sin__q_RH_HAA__) -  0.08) *  cos__q_RH_HFE__)) -  0.35) *  cos__q_RH_KFE__));
    (*this)(1,0) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(1,1) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,2) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,3) = ((((( 0.3735 *  sin__q_RH_HFE__) + (( 0.08 - ( 0.207 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) +  0.35) *  sin__q_RH_KFE__) + (((( 0.08 - ( 0.207 *  sin__q_RH_HAA__)) *  sin__q_RH_HFE__) - ( 0.3735 *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(2,1) =  cos__q_RH_HAA__;
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = ( 0.207 *  cos__q_RH_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_LF_foot_X_fr_trunk::Type_LF_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(1,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_LF_foot_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_LF_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = (( cos__q_LF_HFE__ *  cos__q_LF_KFE__) - ( sin__q_LF_HFE__ *  sin__q_LF_KFE__));
    (*this)(0,1) = (((- sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,2) = (((- cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(0,3) = ((((( 0.3735 *  sin__q_LF_HFE__) + ((( 0.207 *  sin__q_LF_HAA__) -  0.08) *  cos__q_LF_HFE__)) -  0.35) *  sin__q_LF_KFE__) + ((((( 0.207 *  sin__q_LF_HAA__) -  0.08) *  sin__q_LF_HFE__) - ( 0.3735 *  cos__q_LF_HFE__)) *  cos__q_LF_KFE__));
    (*this)(1,1) =  cos__q_LF_HAA__;
    (*this)(1,2) = - sin__q_LF_HAA__;
    (*this)(1,3) = (- 0.207 *  cos__q_LF_HAA__);
    (*this)(2,0) = (( cos__q_LF_HFE__ *  sin__q_LF_KFE__) + ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(2,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(2,2) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__) - (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__));
    (*this)(2,3) = ((((((( 0.207 *  sin__q_LF_HAA__) -  0.08) *  sin__q_LF_HFE__) - ( 0.3735 *  cos__q_LF_HFE__)) *  sin__q_LF_KFE__) + ((((- 0.3735 *  sin__q_LF_HFE__) + (( 0.08 - ( 0.207 *  sin__q_LF_HAA__)) *  cos__q_LF_HFE__)) +  0.35) *  cos__q_LF_KFE__)) +  lengths_values -> foot_x);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_RF_foot_X_fr_trunk::Type_RF_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(1,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_RF_foot_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_RF_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = (( cos__q_RF_HFE__ *  cos__q_RF_KFE__) - ( sin__q_RF_HFE__ *  sin__q_RF_KFE__));
    (*this)(0,1) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,2) = (((- cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(0,3) = ((((( 0.3735 *  sin__q_RF_HFE__) + ((( 0.207 *  sin__q_RF_HAA__) -  0.08) *  cos__q_RF_HFE__)) -  0.35) *  sin__q_RF_KFE__) + ((((( 0.207 *  sin__q_RF_HAA__) -  0.08) *  sin__q_RF_HFE__) - ( 0.3735 *  cos__q_RF_HFE__)) *  cos__q_RF_KFE__));
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) =  sin__q_RF_HAA__;
    (*this)(1,3) = ( 0.207 *  cos__q_RF_HAA__);
    (*this)(2,0) = (( cos__q_RF_HFE__ *  sin__q_RF_KFE__) + ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(2,1) = ((( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,2) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(2,3) = ((((((( 0.207 *  sin__q_RF_HAA__) -  0.08) *  sin__q_RF_HFE__) - ( 0.3735 *  cos__q_RF_HFE__)) *  sin__q_RF_KFE__) + ((((- 0.3735 *  sin__q_RF_HFE__) + (( 0.08 - ( 0.207 *  sin__q_RF_HAA__)) *  cos__q_RF_HFE__)) +  0.35) *  cos__q_RF_KFE__)) +  lengths_values -> foot_x);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_LH_foot_X_fr_trunk::Type_LH_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(1,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_LH_foot_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_LH_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = (( cos__q_LH_HFE__ *  cos__q_LH_KFE__) - ( sin__q_LH_HFE__ *  sin__q_LH_KFE__));
    (*this)(0,1) = (((- sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,2) = (((- cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(0,3) = (((((- 0.3735 *  sin__q_LH_HFE__) + ((( 0.207 *  sin__q_LH_HAA__) -  0.08) *  cos__q_LH_HFE__)) -  0.35) *  sin__q_LH_KFE__) + ((((( 0.207 *  sin__q_LH_HAA__) -  0.08) *  sin__q_LH_HFE__) + ( 0.3735 *  cos__q_LH_HFE__)) *  cos__q_LH_KFE__));
    (*this)(1,1) =  cos__q_LH_HAA__;
    (*this)(1,2) = - sin__q_LH_HAA__;
    (*this)(1,3) = (- 0.207 *  cos__q_LH_HAA__);
    (*this)(2,0) = (( cos__q_LH_HFE__ *  sin__q_LH_KFE__) + ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(2,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(2,2) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__) - (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__));
    (*this)(2,3) = ((((((( 0.207 *  sin__q_LH_HAA__) -  0.08) *  sin__q_LH_HFE__) + ( 0.3735 *  cos__q_LH_HFE__)) *  sin__q_LH_KFE__) + (((( 0.3735 *  sin__q_LH_HFE__) + (( 0.08 - ( 0.207 *  sin__q_LH_HAA__)) *  cos__q_LH_HFE__)) +  0.35) *  cos__q_LH_KFE__)) +  lengths_values -> foot_x);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_RH_foot_X_fr_trunk::Type_RH_foot_X_fr_trunk(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(1,0) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_RH_foot_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_RH_foot_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = (( cos__q_RH_HFE__ *  cos__q_RH_KFE__) - ( sin__q_RH_HFE__ *  sin__q_RH_KFE__));
    (*this)(0,1) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,2) = (((- cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(0,3) = (((((- 0.3735 *  sin__q_RH_HFE__) + ((( 0.207 *  sin__q_RH_HAA__) -  0.08) *  cos__q_RH_HFE__)) -  0.35) *  sin__q_RH_KFE__) + ((((( 0.207 *  sin__q_RH_HAA__) -  0.08) *  sin__q_RH_HFE__) + ( 0.3735 *  cos__q_RH_HFE__)) *  cos__q_RH_KFE__));
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) =  sin__q_RH_HAA__;
    (*this)(1,3) = ( 0.207 *  cos__q_RH_HAA__);
    (*this)(2,0) = (( cos__q_RH_HFE__ *  sin__q_RH_KFE__) + ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(2,1) = ((( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,2) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(2,3) = ((((((( 0.207 *  sin__q_RH_HAA__) -  0.08) *  sin__q_RH_HFE__) + ( 0.3735 *  cos__q_RH_HFE__)) *  sin__q_RH_KFE__) + (((( 0.3735 *  sin__q_RH_HFE__) + (( 0.08 - ( 0.207 *  sin__q_RH_HAA__)) *  cos__q_RH_HFE__)) +  0.35) *  cos__q_RH_KFE__)) +  lengths_values -> foot_x);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_shin::Type_fr_trunk_X_LF_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_shin& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LF_shin::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_KFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = ((- cos__q_LF_HFE__ *  sin__q_LF_KFE__) - ( sin__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,1) = (( sin__q_LF_HFE__ *  sin__q_LF_KFE__) - ( cos__q_LF_HFE__ *  cos__q_LF_KFE__));
    (*this)(0,3) = (((((- lengths_values -> LF_shin *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) - (( lengths_values -> LF_shin *  sin__q_LF_HFE__) *  cos__q_LF_KFE__)) - ( 0.35 *  sin__q_LF_HFE__)) +  0.3735);
    (*this)(1,0) = ((( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,1) = ((( sin__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( sin__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = ((((((( lengths_values -> LF_shin *  sin__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> LF_shin *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  sin__q_LF_HAA__)) +  0.207);
    (*this)(2,0) = ((( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - (( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,1) = ((( cos__q_LF_HAA__ *  cos__q_LF_HFE__) *  sin__q_LF_KFE__) + (( cos__q_LF_HAA__ *  sin__q_LF_HFE__) *  cos__q_LF_KFE__));
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = (((((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  sin__q_LF_HFE__) *  sin__q_LF_KFE__) - ((( lengths_values -> LF_shin *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) *  cos__q_LF_KFE__)) - (( 0.35 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__)) - ( 0.08 *  cos__q_LF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_shin::Type_fr_trunk_X_RF_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_shin& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RF_shin::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_KFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = ((- cos__q_RF_HFE__ *  sin__q_RF_KFE__) - ( sin__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,1) = (( sin__q_RF_HFE__ *  sin__q_RF_KFE__) - ( cos__q_RF_HFE__ *  cos__q_RF_KFE__));
    (*this)(0,3) = (((((- lengths_values -> RF_shin *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( lengths_values -> RF_shin *  sin__q_RF_HFE__) *  cos__q_RF_KFE__)) - ( 0.35 *  sin__q_RF_HFE__)) +  0.3735);
    (*this)(1,0) = ((( sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__));
    (*this)(1,1) = (((- sin__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) - (( sin__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = (((((((- lengths_values -> RF_shin *  sin__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) + ((( lengths_values -> RF_shin *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) + (( 0.35 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__)) + ( 0.08 *  sin__q_RF_HAA__)) -  0.207);
    (*this)(2,0) = ((( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - (( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,1) = ((( cos__q_RF_HAA__ *  cos__q_RF_HFE__) *  sin__q_RF_KFE__) + (( cos__q_RF_HAA__ *  sin__q_RF_HFE__) *  cos__q_RF_KFE__));
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = (((((( lengths_values -> RF_shin *  cos__q_RF_HAA__) *  sin__q_RF_HFE__) *  sin__q_RF_KFE__) - ((( lengths_values -> RF_shin *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) *  cos__q_RF_KFE__)) - (( 0.35 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__)) - ( 0.08 *  cos__q_RF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_shin::Type_fr_trunk_X_LH_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_shin& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_LH_shin::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_KFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = ((- cos__q_LH_HFE__ *  sin__q_LH_KFE__) - ( sin__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,1) = (( sin__q_LH_HFE__ *  sin__q_LH_KFE__) - ( cos__q_LH_HFE__ *  cos__q_LH_KFE__));
    (*this)(0,3) = (((((- lengths_values -> LH_shin *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) - (( lengths_values -> LH_shin *  sin__q_LH_HFE__) *  cos__q_LH_KFE__)) - ( 0.35 *  sin__q_LH_HFE__)) -  0.3735);
    (*this)(1,0) = ((( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,1) = ((( sin__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( sin__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = ((((((( lengths_values -> LH_shin *  sin__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> LH_shin *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  sin__q_LH_HAA__)) +  0.207);
    (*this)(2,0) = ((( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - (( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,1) = ((( cos__q_LH_HAA__ *  cos__q_LH_HFE__) *  sin__q_LH_KFE__) + (( cos__q_LH_HAA__ *  sin__q_LH_HFE__) *  cos__q_LH_KFE__));
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = (((((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  sin__q_LH_HFE__) *  sin__q_LH_KFE__) - ((( lengths_values -> LH_shin *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) *  cos__q_LH_KFE__)) - (( 0.35 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__)) - ( 0.08 *  cos__q_LH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_shin::Type_fr_trunk_X_RH_shin(const Params_lengths& _lengths_values)
    : lengths_values(& _lengths_values)
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_shin& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_RH_shin::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_KFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = ((- cos__q_RH_HFE__ *  sin__q_RH_KFE__) - ( sin__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,1) = (( sin__q_RH_HFE__ *  sin__q_RH_KFE__) - ( cos__q_RH_HFE__ *  cos__q_RH_KFE__));
    (*this)(0,3) = (((((- lengths_values -> RH_shin *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( lengths_values -> RH_shin *  sin__q_RH_HFE__) *  cos__q_RH_KFE__)) - ( 0.35 *  sin__q_RH_HFE__)) -  0.3735);
    (*this)(1,0) = ((( sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__));
    (*this)(1,1) = (((- sin__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) - (( sin__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = (((((((- lengths_values -> RH_shin *  sin__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) + ((( lengths_values -> RH_shin *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) + (( 0.35 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__)) + ( 0.08 *  sin__q_RH_HAA__)) -  0.207);
    (*this)(2,0) = ((( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - (( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,1) = ((( cos__q_RH_HAA__ *  cos__q_RH_HFE__) *  sin__q_RH_KFE__) + (( cos__q_RH_HAA__ *  sin__q_RH_HFE__) *  cos__q_RH_KFE__));
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = (((((( lengths_values -> RH_shin *  cos__q_RH_HAA__) *  sin__q_RH_HFE__) *  sin__q_RH_KFE__) - ((( lengths_values -> RH_shin *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) *  cos__q_RH_KFE__)) - (( 0.35 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__)) - ( 0.08 *  cos__q_RH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_HAA::Type_fr_trunk_X_fr_LF_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = 0.3735;
    (*this)(1,0) = 0;
    (*this)(1,1) = - 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0.207;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_HAA& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_HFE::Type_fr_trunk_X_fr_LF_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.3735;
    (*this)(1,1) = 0;
    (*this)(2,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_HFE& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_HFE::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(1,0) = - sin__q_LF_HAA__;
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = ( 0.207 - ( 0.08 *  sin__q_LF_HAA__));
    (*this)(2,0) = - cos__q_LF_HAA__;
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = (- 0.08 *  cos__q_LF_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_KFE::Type_fr_trunk_X_fr_LF_KFE()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_KFE& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_KFE::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HFE__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,0) = - sin__q_LF_HFE__;
    (*this)(0,1) = - cos__q_LF_HFE__;
    (*this)(0,3) = ( 0.3735 - ( 0.35 *  sin__q_LF_HFE__));
    (*this)(1,0) = (- sin__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(1,1) = ( sin__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(1,2) =  cos__q_LF_HAA__;
    (*this)(1,3) = ((((- 0.35 *  sin__q_LF_HAA__) *  cos__q_LF_HFE__) - ( 0.08 *  sin__q_LF_HAA__)) +  0.207);
    (*this)(2,0) = (- cos__q_LF_HAA__ *  cos__q_LF_HFE__);
    (*this)(2,1) = ( cos__q_LF_HAA__ *  sin__q_LF_HFE__);
    (*this)(2,2) = - sin__q_LF_HAA__;
    (*this)(2,3) = (((- 0.35 *  cos__q_LF_HAA__) *  cos__q_LF_HFE__) - ( 0.08 *  cos__q_LF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_HAA::Type_fr_trunk_X_fr_RF_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0.3735;
    (*this)(1,0) = 0;
    (*this)(1,1) = 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = - 0.207;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_HAA& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_HFE::Type_fr_trunk_X_fr_RF_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.3735;
    (*this)(1,1) = 0;
    (*this)(2,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_HFE& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_HFE::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(1,0) =  sin__q_RF_HAA__;
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = (( 0.08 *  sin__q_RF_HAA__) -  0.207);
    (*this)(2,0) = - cos__q_RF_HAA__;
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = (- 0.08 *  cos__q_RF_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_KFE::Type_fr_trunk_X_fr_RF_KFE()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_KFE& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_KFE::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HFE__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,0) = - sin__q_RF_HFE__;
    (*this)(0,1) = - cos__q_RF_HFE__;
    (*this)(0,3) = ( 0.3735 - ( 0.35 *  sin__q_RF_HFE__));
    (*this)(1,0) = ( sin__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(1,1) = (- sin__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(1,2) =  cos__q_RF_HAA__;
    (*this)(1,3) = (((( 0.35 *  sin__q_RF_HAA__) *  cos__q_RF_HFE__) + ( 0.08 *  sin__q_RF_HAA__)) -  0.207);
    (*this)(2,0) = (- cos__q_RF_HAA__ *  cos__q_RF_HFE__);
    (*this)(2,1) = ( cos__q_RF_HAA__ *  sin__q_RF_HFE__);
    (*this)(2,2) =  sin__q_RF_HAA__;
    (*this)(2,3) = (((- 0.35 *  cos__q_RF_HAA__) *  cos__q_RF_HFE__) - ( 0.08 *  cos__q_RF_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_HAA::Type_fr_trunk_X_fr_LH_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = - 0.3735;
    (*this)(1,0) = 0;
    (*this)(1,1) = - 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0.207;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_HAA& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_HFE::Type_fr_trunk_X_fr_LH_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,3) = - 0.3735;
    (*this)(1,1) = 0;
    (*this)(2,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_HFE& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_HFE::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(1,0) = - sin__q_LH_HAA__;
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = ( 0.207 - ( 0.08 *  sin__q_LH_HAA__));
    (*this)(2,0) = - cos__q_LH_HAA__;
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = (- 0.08 *  cos__q_LH_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_KFE::Type_fr_trunk_X_fr_LH_KFE()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_KFE& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_KFE::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HFE__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,0) = - sin__q_LH_HFE__;
    (*this)(0,1) = - cos__q_LH_HFE__;
    (*this)(0,3) = ((- 0.35 *  sin__q_LH_HFE__) -  0.3735);
    (*this)(1,0) = (- sin__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(1,1) = ( sin__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(1,2) =  cos__q_LH_HAA__;
    (*this)(1,3) = ((((- 0.35 *  sin__q_LH_HAA__) *  cos__q_LH_HFE__) - ( 0.08 *  sin__q_LH_HAA__)) +  0.207);
    (*this)(2,0) = (- cos__q_LH_HAA__ *  cos__q_LH_HFE__);
    (*this)(2,1) = ( cos__q_LH_HAA__ *  sin__q_LH_HFE__);
    (*this)(2,2) = - sin__q_LH_HAA__;
    (*this)(2,3) = (((- 0.35 *  cos__q_LH_HAA__) *  cos__q_LH_HFE__) - ( 0.08 *  cos__q_LH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_HAA::Type_fr_trunk_X_fr_RH_HAA()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = - 0.3735;
    (*this)(1,0) = 0;
    (*this)(1,1) = 1.0;
    (*this)(1,2) = 0;
    (*this)(1,3) = - 0.207;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_HAA& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_HAA::update(const state_t& q) {
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_HFE::Type_fr_trunk_X_fr_RH_HFE()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = - 1.0;
    (*this)(0,2) = 0;
    (*this)(0,3) = - 0.3735;
    (*this)(1,1) = 0;
    (*this)(2,1) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_HFE& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_HFE::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(1,0) =  sin__q_RH_HAA__;
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = (( 0.08 *  sin__q_RH_HAA__) -  0.207);
    (*this)(2,0) = - cos__q_RH_HAA__;
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = (- 0.08 *  cos__q_RH_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_KFE::Type_fr_trunk_X_fr_RH_KFE()
{
    (*this)(0,2) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_KFE& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_KFE::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HFE__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,0) = - sin__q_RH_HFE__;
    (*this)(0,1) = - cos__q_RH_HFE__;
    (*this)(0,3) = ((- 0.35 *  sin__q_RH_HFE__) -  0.3735);
    (*this)(1,0) = ( sin__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(1,1) = (- sin__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(1,2) =  cos__q_RH_HAA__;
    (*this)(1,3) = (((( 0.35 *  sin__q_RH_HAA__) *  cos__q_RH_HFE__) + ( 0.08 *  sin__q_RH_HAA__)) -  0.207);
    (*this)(2,0) = (- cos__q_RH_HAA__ *  cos__q_RH_HFE__);
    (*this)(2,1) = ( cos__q_RH_HAA__ *  sin__q_RH_HFE__);
    (*this)(2,2) =  sin__q_RH_HAA__;
    (*this)(2,3) = (((- 0.35 *  cos__q_RH_HAA__) *  cos__q_RH_HFE__) - ( 0.08 *  cos__q_RH_HAA__));
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LF_hipassembly_X_fr_trunk::Type_fr_LF_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0.3735;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LF_hipassembly_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_fr_LF_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(0,1) = - sin__q_LF_HAA__;
    (*this)(0,2) = - cos__q_LF_HAA__;
    (*this)(0,3) = ( 0.207 *  sin__q_LF_HAA__);
    (*this)(1,1) = - cos__q_LF_HAA__;
    (*this)(1,2) =  sin__q_LF_HAA__;
    (*this)(1,3) = ( 0.207 *  cos__q_LF_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_hipassembly::Type_fr_trunk_X_fr_LF_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = 0.3735;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0.207;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_hipassembly& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_hipassembly::update(const state_t& q) {
    static double sin__q_LF_HAA__;
    static double cos__q_LF_HAA__;
    
    sin__q_LF_HAA__ = std::sin( q(LF_HAA));
    cos__q_LF_HAA__ = std::cos( q(LF_HAA));
    
    (*this)(1,0) = - sin__q_LF_HAA__;
    (*this)(1,1) = - cos__q_LF_HAA__;
    (*this)(2,0) = - cos__q_LF_HAA__;
    (*this)(2,1) =  sin__q_LF_HAA__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LF_upperleg_X_fr_LF_hipassembly::Type_fr_LF_upperleg_X_fr_LF_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(1,1) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = - 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LF_upperleg_X_fr_LF_hipassembly& iit::HyQ::HomogeneousTransforms::Type_fr_LF_upperleg_X_fr_LF_hipassembly::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double cos__q_LF_HFE__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    
    (*this)(0,0) =  cos__q_LF_HFE__;
    (*this)(0,2) =  sin__q_LF_HFE__;
    (*this)(0,3) = (- 0.08 *  cos__q_LF_HFE__);
    (*this)(1,0) = - sin__q_LF_HFE__;
    (*this)(1,2) =  cos__q_LF_HFE__;
    (*this)(1,3) = ( 0.08 *  sin__q_LF_HFE__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LF_hipassembly_X_fr_LF_upperleg::Type_fr_LF_hipassembly_X_fr_LF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.08;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = - 1;
    (*this)(1,3) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LF_hipassembly_X_fr_LF_upperleg& iit::HyQ::HomogeneousTransforms::Type_fr_LF_hipassembly_X_fr_LF_upperleg::update(const state_t& q) {
    static double sin__q_LF_HFE__;
    static double cos__q_LF_HFE__;
    
    sin__q_LF_HFE__ = std::sin( q(LF_HFE));
    cos__q_LF_HFE__ = std::cos( q(LF_HFE));
    
    (*this)(0,0) =  cos__q_LF_HFE__;
    (*this)(0,1) = - sin__q_LF_HFE__;
    (*this)(2,0) =  sin__q_LF_HFE__;
    (*this)(2,1) =  cos__q_LF_HFE__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LF_lowerleg_X_fr_LF_upperleg::Type_fr_LF_lowerleg_X_fr_LF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(1,2) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LF_lowerleg_X_fr_LF_upperleg& iit::HyQ::HomogeneousTransforms::Type_fr_LF_lowerleg_X_fr_LF_upperleg::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double cos__q_LF_KFE__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    
    (*this)(0,0) =  cos__q_LF_KFE__;
    (*this)(0,1) =  sin__q_LF_KFE__;
    (*this)(0,3) = (- 0.35 *  cos__q_LF_KFE__);
    (*this)(1,0) = - sin__q_LF_KFE__;
    (*this)(1,1) =  cos__q_LF_KFE__;
    (*this)(1,3) = ( 0.35 *  sin__q_LF_KFE__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LF_upperleg_X_fr_LF_lowerleg::Type_fr_LF_upperleg_X_fr_LF_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.35;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LF_upperleg_X_fr_LF_lowerleg& iit::HyQ::HomogeneousTransforms::Type_fr_LF_upperleg_X_fr_LF_lowerleg::update(const state_t& q) {
    static double sin__q_LF_KFE__;
    static double cos__q_LF_KFE__;
    
    sin__q_LF_KFE__ = std::sin( q(LF_KFE));
    cos__q_LF_KFE__ = std::cos( q(LF_KFE));
    
    (*this)(0,0) =  cos__q_LF_KFE__;
    (*this)(0,1) = - sin__q_LF_KFE__;
    (*this)(1,0) =  sin__q_LF_KFE__;
    (*this)(1,1) =  cos__q_LF_KFE__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RF_hipassembly_X_fr_trunk::Type_fr_RF_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = - 0.3735;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RF_hipassembly_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_fr_RF_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(0,1) =  sin__q_RF_HAA__;
    (*this)(0,2) = - cos__q_RF_HAA__;
    (*this)(0,3) = ( 0.207 *  sin__q_RF_HAA__);
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(1,2) =  sin__q_RF_HAA__;
    (*this)(1,3) = ( 0.207 *  cos__q_RF_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_hipassembly::Type_fr_trunk_X_fr_RF_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = 0.3735;
    (*this)(1,2) = 0;
    (*this)(1,3) = - 0.207;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_hipassembly& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_hipassembly::update(const state_t& q) {
    static double sin__q_RF_HAA__;
    static double cos__q_RF_HAA__;
    
    sin__q_RF_HAA__ = std::sin( q(RF_HAA));
    cos__q_RF_HAA__ = std::cos( q(RF_HAA));
    
    (*this)(1,0) =  sin__q_RF_HAA__;
    (*this)(1,1) =  cos__q_RF_HAA__;
    (*this)(2,0) = - cos__q_RF_HAA__;
    (*this)(2,1) =  sin__q_RF_HAA__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RF_upperleg_X_fr_RF_hipassembly::Type_fr_RF_upperleg_X_fr_RF_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(1,1) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RF_upperleg_X_fr_RF_hipassembly& iit::HyQ::HomogeneousTransforms::Type_fr_RF_upperleg_X_fr_RF_hipassembly::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double cos__q_RF_HFE__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    
    (*this)(0,0) =  cos__q_RF_HFE__;
    (*this)(0,2) = - sin__q_RF_HFE__;
    (*this)(0,3) = (- 0.08 *  cos__q_RF_HFE__);
    (*this)(1,0) = - sin__q_RF_HFE__;
    (*this)(1,2) = - cos__q_RF_HFE__;
    (*this)(1,3) = ( 0.08 *  sin__q_RF_HFE__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RF_hipassembly_X_fr_RF_upperleg::Type_fr_RF_hipassembly_X_fr_RF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.08;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RF_hipassembly_X_fr_RF_upperleg& iit::HyQ::HomogeneousTransforms::Type_fr_RF_hipassembly_X_fr_RF_upperleg::update(const state_t& q) {
    static double sin__q_RF_HFE__;
    static double cos__q_RF_HFE__;
    
    sin__q_RF_HFE__ = std::sin( q(RF_HFE));
    cos__q_RF_HFE__ = std::cos( q(RF_HFE));
    
    (*this)(0,0) =  cos__q_RF_HFE__;
    (*this)(0,1) = - sin__q_RF_HFE__;
    (*this)(2,0) = - sin__q_RF_HFE__;
    (*this)(2,1) = - cos__q_RF_HFE__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RF_lowerleg_X_fr_RF_upperleg::Type_fr_RF_lowerleg_X_fr_RF_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(1,2) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RF_lowerleg_X_fr_RF_upperleg& iit::HyQ::HomogeneousTransforms::Type_fr_RF_lowerleg_X_fr_RF_upperleg::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double cos__q_RF_KFE__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    
    (*this)(0,0) =  cos__q_RF_KFE__;
    (*this)(0,1) =  sin__q_RF_KFE__;
    (*this)(0,3) = (- 0.35 *  cos__q_RF_KFE__);
    (*this)(1,0) = - sin__q_RF_KFE__;
    (*this)(1,1) =  cos__q_RF_KFE__;
    (*this)(1,3) = ( 0.35 *  sin__q_RF_KFE__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RF_upperleg_X_fr_RF_lowerleg::Type_fr_RF_upperleg_X_fr_RF_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.35;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RF_upperleg_X_fr_RF_lowerleg& iit::HyQ::HomogeneousTransforms::Type_fr_RF_upperleg_X_fr_RF_lowerleg::update(const state_t& q) {
    static double sin__q_RF_KFE__;
    static double cos__q_RF_KFE__;
    
    sin__q_RF_KFE__ = std::sin( q(RF_KFE));
    cos__q_RF_KFE__ = std::cos( q(RF_KFE));
    
    (*this)(0,0) =  cos__q_RF_KFE__;
    (*this)(0,1) = - sin__q_RF_KFE__;
    (*this)(1,0) =  sin__q_RF_KFE__;
    (*this)(1,1) =  cos__q_RF_KFE__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LH_hipassembly_X_fr_trunk::Type_fr_LH_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = - 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = - 0.3735;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LH_hipassembly_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_fr_LH_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(0,1) = - sin__q_LH_HAA__;
    (*this)(0,2) = - cos__q_LH_HAA__;
    (*this)(0,3) = ( 0.207 *  sin__q_LH_HAA__);
    (*this)(1,1) = - cos__q_LH_HAA__;
    (*this)(1,2) =  sin__q_LH_HAA__;
    (*this)(1,3) = ( 0.207 *  cos__q_LH_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_hipassembly::Type_fr_trunk_X_fr_LH_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = - 1.0;
    (*this)(0,3) = - 0.3735;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0.207;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_hipassembly& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_hipassembly::update(const state_t& q) {
    static double sin__q_LH_HAA__;
    static double cos__q_LH_HAA__;
    
    sin__q_LH_HAA__ = std::sin( q(LH_HAA));
    cos__q_LH_HAA__ = std::cos( q(LH_HAA));
    
    (*this)(1,0) = - sin__q_LH_HAA__;
    (*this)(1,1) = - cos__q_LH_HAA__;
    (*this)(2,0) = - cos__q_LH_HAA__;
    (*this)(2,1) =  sin__q_LH_HAA__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LH_upperleg_X_fr_LH_hipassembly::Type_fr_LH_upperleg_X_fr_LH_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(1,1) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = - 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LH_upperleg_X_fr_LH_hipassembly& iit::HyQ::HomogeneousTransforms::Type_fr_LH_upperleg_X_fr_LH_hipassembly::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double cos__q_LH_HFE__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    
    (*this)(0,0) =  cos__q_LH_HFE__;
    (*this)(0,2) =  sin__q_LH_HFE__;
    (*this)(0,3) = (- 0.08 *  cos__q_LH_HFE__);
    (*this)(1,0) = - sin__q_LH_HFE__;
    (*this)(1,2) =  cos__q_LH_HFE__;
    (*this)(1,3) = ( 0.08 *  sin__q_LH_HFE__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LH_hipassembly_X_fr_LH_upperleg::Type_fr_LH_hipassembly_X_fr_LH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.08;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = - 1;
    (*this)(1,3) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LH_hipassembly_X_fr_LH_upperleg& iit::HyQ::HomogeneousTransforms::Type_fr_LH_hipassembly_X_fr_LH_upperleg::update(const state_t& q) {
    static double sin__q_LH_HFE__;
    static double cos__q_LH_HFE__;
    
    sin__q_LH_HFE__ = std::sin( q(LH_HFE));
    cos__q_LH_HFE__ = std::cos( q(LH_HFE));
    
    (*this)(0,0) =  cos__q_LH_HFE__;
    (*this)(0,1) = - sin__q_LH_HFE__;
    (*this)(2,0) =  sin__q_LH_HFE__;
    (*this)(2,1) =  cos__q_LH_HFE__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LH_lowerleg_X_fr_LH_upperleg::Type_fr_LH_lowerleg_X_fr_LH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(1,2) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LH_lowerleg_X_fr_LH_upperleg& iit::HyQ::HomogeneousTransforms::Type_fr_LH_lowerleg_X_fr_LH_upperleg::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double cos__q_LH_KFE__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    
    (*this)(0,0) =  cos__q_LH_KFE__;
    (*this)(0,1) =  sin__q_LH_KFE__;
    (*this)(0,3) = (- 0.35 *  cos__q_LH_KFE__);
    (*this)(1,0) = - sin__q_LH_KFE__;
    (*this)(1,1) =  cos__q_LH_KFE__;
    (*this)(1,3) = ( 0.35 *  sin__q_LH_KFE__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_LH_upperleg_X_fr_LH_lowerleg::Type_fr_LH_upperleg_X_fr_LH_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.35;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_LH_upperleg_X_fr_LH_lowerleg& iit::HyQ::HomogeneousTransforms::Type_fr_LH_upperleg_X_fr_LH_lowerleg::update(const state_t& q) {
    static double sin__q_LH_KFE__;
    static double cos__q_LH_KFE__;
    
    sin__q_LH_KFE__ = std::sin( q(LH_KFE));
    cos__q_LH_KFE__ = std::cos( q(LH_KFE));
    
    (*this)(0,0) =  cos__q_LH_KFE__;
    (*this)(0,1) = - sin__q_LH_KFE__;
    (*this)(1,0) =  sin__q_LH_KFE__;
    (*this)(1,1) =  cos__q_LH_KFE__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RH_hipassembly_X_fr_trunk::Type_fr_RH_hipassembly_X_fr_trunk()
{
    (*this)(0,0) = 0;
    (*this)(1,0) = 0;
    (*this)(2,0) = 1.0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0.3735;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RH_hipassembly_X_fr_trunk& iit::HyQ::HomogeneousTransforms::Type_fr_RH_hipassembly_X_fr_trunk::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(0,1) =  sin__q_RH_HAA__;
    (*this)(0,2) = - cos__q_RH_HAA__;
    (*this)(0,3) = ( 0.207 *  sin__q_RH_HAA__);
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(1,2) =  sin__q_RH_HAA__;
    (*this)(1,3) = ( 0.207 *  cos__q_RH_HAA__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_hipassembly::Type_fr_trunk_X_fr_RH_hipassembly()
{
    (*this)(0,0) = 0;
    (*this)(0,1) = 0;
    (*this)(0,2) = 1.0;
    (*this)(0,3) = - 0.3735;
    (*this)(1,2) = 0;
    (*this)(1,3) = - 0.207;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_hipassembly& iit::HyQ::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_hipassembly::update(const state_t& q) {
    static double sin__q_RH_HAA__;
    static double cos__q_RH_HAA__;
    
    sin__q_RH_HAA__ = std::sin( q(RH_HAA));
    cos__q_RH_HAA__ = std::cos( q(RH_HAA));
    
    (*this)(1,0) =  sin__q_RH_HAA__;
    (*this)(1,1) =  cos__q_RH_HAA__;
    (*this)(2,0) = - cos__q_RH_HAA__;
    (*this)(2,1) =  sin__q_RH_HAA__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RH_upperleg_X_fr_RH_hipassembly::Type_fr_RH_upperleg_X_fr_RH_hipassembly()
{
    (*this)(0,1) = 0;
    (*this)(1,1) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 1;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RH_upperleg_X_fr_RH_hipassembly& iit::HyQ::HomogeneousTransforms::Type_fr_RH_upperleg_X_fr_RH_hipassembly::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double cos__q_RH_HFE__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    
    (*this)(0,0) =  cos__q_RH_HFE__;
    (*this)(0,2) = - sin__q_RH_HFE__;
    (*this)(0,3) = (- 0.08 *  cos__q_RH_HFE__);
    (*this)(1,0) = - sin__q_RH_HFE__;
    (*this)(1,2) = - cos__q_RH_HFE__;
    (*this)(1,3) = ( 0.08 *  sin__q_RH_HFE__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RH_hipassembly_X_fr_RH_upperleg::Type_fr_RH_hipassembly_X_fr_RH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.08;
    (*this)(1,0) = 0;
    (*this)(1,1) = 0;
    (*this)(1,2) = 1;
    (*this)(1,3) = 0;
    (*this)(2,2) = 0;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RH_hipassembly_X_fr_RH_upperleg& iit::HyQ::HomogeneousTransforms::Type_fr_RH_hipassembly_X_fr_RH_upperleg::update(const state_t& q) {
    static double sin__q_RH_HFE__;
    static double cos__q_RH_HFE__;
    
    sin__q_RH_HFE__ = std::sin( q(RH_HFE));
    cos__q_RH_HFE__ = std::cos( q(RH_HFE));
    
    (*this)(0,0) =  cos__q_RH_HFE__;
    (*this)(0,1) = - sin__q_RH_HFE__;
    (*this)(2,0) = - sin__q_RH_HFE__;
    (*this)(2,1) = - cos__q_RH_HFE__;
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RH_lowerleg_X_fr_RH_upperleg::Type_fr_RH_lowerleg_X_fr_RH_upperleg()
{
    (*this)(0,2) = 0;
    (*this)(1,2) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1.0;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RH_lowerleg_X_fr_RH_upperleg& iit::HyQ::HomogeneousTransforms::Type_fr_RH_lowerleg_X_fr_RH_upperleg::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double cos__q_RH_KFE__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    
    (*this)(0,0) =  cos__q_RH_KFE__;
    (*this)(0,1) =  sin__q_RH_KFE__;
    (*this)(0,3) = (- 0.35 *  cos__q_RH_KFE__);
    (*this)(1,0) = - sin__q_RH_KFE__;
    (*this)(1,1) =  cos__q_RH_KFE__;
    (*this)(1,3) = ( 0.35 *  sin__q_RH_KFE__);
    return *this;
}
iit::HyQ::HomogeneousTransforms::Type_fr_RH_upperleg_X_fr_RH_lowerleg::Type_fr_RH_upperleg_X_fr_RH_lowerleg()
{
    (*this)(0,2) = 0;
    (*this)(0,3) = 0.35;
    (*this)(1,2) = 0;
    (*this)(1,3) = 0;
    (*this)(2,0) = 0;
    (*this)(2,1) = 0;
    (*this)(2,2) = 1;
    (*this)(2,3) = 0;
    (*this)(3,0) = 0;
    (*this)(3,1) = 0;
    (*this)(3,2) = 0;
    (*this)(3,3) = 1;
}
const iit::HyQ::HomogeneousTransforms::Type_fr_RH_upperleg_X_fr_RH_lowerleg& iit::HyQ::HomogeneousTransforms::Type_fr_RH_upperleg_X_fr_RH_lowerleg::update(const state_t& q) {
    static double sin__q_RH_KFE__;
    static double cos__q_RH_KFE__;
    
    sin__q_RH_KFE__ = std::sin( q(RH_KFE));
    cos__q_RH_KFE__ = std::cos( q(RH_KFE));
    
    (*this)(0,0) =  cos__q_RH_KFE__;
    (*this)(0,1) = - sin__q_RH_KFE__;
    (*this)(1,0) =  sin__q_RH_KFE__;
    (*this)(1,1) =  cos__q_RH_KFE__;
    return *this;
}

