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
#ifndef SIMULATOR_BASE_HPP
#define SIMULATOR_BASE_HPP

#include <iostream>

#include <dls2/components/app_layer_component.hpp>
#include <dls2/command/command_manager.hpp>


namespace dls
{
	/// A interface for simulators in the framework
	///
	class SimulatorBase : protected AppLayerComponent
	{
	public:
		/// Default Constructor
		///
		SimulatorBase(std::string ID);

		/// Default Destructor
		///
		~SimulatorBase() = default;

		/// Print the state of this layer
		///
		virtual std::string where() {return this->getID() + " simulator";}

        /// Launch Simulator
        ///
		virtual void launchSim() = 0;

        /// Exits Simulator
        ///
		virtual void exitSim() = 0;

        /// Emergency stop for this component
        ///
        Status eStop() override {return this->getStatus();};

        // /// Normal stop for this component
        // ///
        // Status stop() override {return this->getStatus();};

	};

} // end namespace dls

#endif /* end of include guard: SIMULATOR_BASE_HPP */
