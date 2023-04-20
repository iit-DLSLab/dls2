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
#ifndef LAYER_HPP_H7JRIVPM
#define LAYER_HPP_H7JRIVPM

#include "dls2/application/app.hpp"

namespace dls
{
	/// An application layer
	///
	/// A layer can be launched and managed by the main application
	class Layer : public App
	{
	public:
		using pComponent_t = std::shared_ptr<App>;

		typedef Layer *create_t(const std::string&);
		typedef void destroy_t(Layer*);

		/// Constructor
		/// @parm ID the name of this layer
		Layer(const std::string &ID);

		/// Destructor
		virtual ~Layer();

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
		AppStatus getComponentStatus(const std::string &name);

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

#endif /* end of include guard: LAYER_HPP_H7JRIVPM */
