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
#ifndef SIM_LAYER_HPP
#define SIM_LAYER_HPP

#include "app_layer.hpp"

namespace dls
{
	/// A ROS interface into the framework
	///
	class SimLayer : public AppLayer
	{
	public:
		/// Default Constructor
		///
		SimLayer(std::string ID);

		/// Default Destructor
		///
		~SimLayer() = default;

		/// Run the layer
		///
		virtual Status run() override;

		/// Stop the layer
		///
		virtual Status shutdown() override;

		/// Print the state of this layer
		///
		virtual std::string where() override{return "Base layer for simulators";}
	};
} // end namespace dls

#endif /* end of include guard: SIM_LAYER_HPP */
