/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef HARDWARE_LAYER_HPP_64INVL3T
#define HARDWARE_LAYER_HPP_64INVL3T

#include "app_layer.hpp"
namespace dls
{
    class HardwareLayer : public AppLayer
    {
    public:
        /// Default Constructor
		///
        HardwareLayer(std::string);

        /// Default Destructor
		///
        ~HardwareLayer();

        /// Run the layer
		///
		Status run() override;

		/// Stop the layer
		///
		Status shutdown() override;

		/// Print the state of this layer
		///
		std::string where() override{return "Base layer for hardwares";}

    };
} // end namespace dls

#endif /* end of include guard: HARDWARE_LAYER_HPP_64INVL3T */
