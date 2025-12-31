#ifndef LAYER_HPP_H7JRIVPM
#define LAYER_HPP_H7JRIVPM

#include "dls2/application/app.hpp"
#include "dls2/application/app_data.hpp"

namespace dls
{
	/// An application layer
	///
	/// A layer can be launched and managed by the main application
	class Layer : public App
	{
	public:
		using pComponent_t = std::shared_ptr<App>;

		typedef Layer *create_t(const std::string& layer_name, const std::string& type, const std::string& name);
		typedef void destroy_t(Layer*);

		/// Constructor
		/// @param ID the name of this layer
		/// @param sleep_time_ms milliseconds to sleep before monitoring again
		Layer(const std::string &ID, int sleep_time_ms=1000);

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

		//! Run the layer - used in state machine
		virtual AppStatus run() override;

		//! Monitor the layer
		virtual void monitor();

		template<class Map>
		void checkAppData(const Map& app_data);

		// BEGIN critical section
			mutable std::mutex components_mutex;
			std::map<std::string, pComponent_t> components;
		// END critical section

		std::chrono::milliseconds sleep_time;

	protected:
		/// DDS communication link
        ///
		std::shared_ptr<dls::DDSParticipant> ddspart_layer;

	};
} // end namespace dls

#include "dls2/application/layer.tpp"

#endif /* end of include guard: LAYER_HPP_H7JRIVPM */
