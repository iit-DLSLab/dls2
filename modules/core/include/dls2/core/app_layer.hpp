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
#ifndef APP_LAYER_HPP_H7JRIVPM
#define APP_LAYER_HPP_H7JRIVPM

#include <functional>
#include <memory>
#include <map>
#include <mutex>

#include "dls2/core/app.hpp"

#include "dls2/components/app_layer_component.hpp"
#include "dls2/command/command_manager.hpp"
#include "dls2/util/messaging/dds_participant.hpp"

namespace dls
{
	/// An application layer
	///
	/// A layer can be launched and managed by the main application
	class AppLayer : public App
	{
	public:
		using pComponent_t = std::shared_ptr<AppLayerComponent>;

		/// Constructor
		AppLayer(const std::string &ID);

		/// Destructor
		virtual ~AppLayer();

	protected:
		/// Adds a component to this layer
		///
		/// @param name the name of the component
		/// @param component a pointer to the component
		void addComponent(const std::string &name, pComponent_t component);

		/// Reads a component from the install path of this layer and loads it
		///
		/// @param name the name of the component
		bool loadComponentFromDisk(const std::string &name);

		/// Remove a component from this layer
		///
		/// @param name the name of the component
		void removeComponent(const std::string &name);

		/// Get the status of a component inside of this layer
		///
		/// @return the status of the component, or UNCONSTRUCTED if it does not
		/// exist
		AppLayerComponent::Status getComponentStatus(const std::string &name);

		/// Get the dds communication participant of the layer
		/// @return pointer to the participant
		std::shared_ptr<dls::DDSParticipant> getParticipant();

		// BEGIN critical section
			mutable std::mutex components_mutex;
			std::map<std::string, pComponent_t> components;
		// END critical section

	private:
		/// DDS communication link
        ///
		std::shared_ptr<dls::DDSParticipant> ddsLink;

	};
} // end namespace dls

#endif /* end of include guard: APP_LAYER_HPP_H7JRIVPM */
