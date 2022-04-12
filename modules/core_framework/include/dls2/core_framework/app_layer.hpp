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

#include "dls2/components/app_layer_component.hpp"
#include "dls2/command/command_manager.hpp"

namespace dls
{
/// An application layer
///
/// A layer can be launched and managed by the main application
class AppLayer
{
public:
	using pComponent_t = std::shared_ptr<AppLayerComponent>;

	/// The status of this layer
	///
	enum class Status
	{
		INITIALISING, ///< Layer is initialising
		RUNNING,      ///< Layer is running normally
		FATAL_ERROR,  ///< Layer has encountered a fatal error
		E_STOP,       ///< Layer has performed an emergency stop
		SUCCESS,      ///< Layer has executed succesfully
		STOP          ///< Layer has been stopped
	};

	// TODO remove this
	// AppLayer(const std::initializer_list<pComponent_t>&);

	AppLayer(const std::string &ID);
	virtual ~AppLayer() = default;

	/// Emergency stop
	///
	/// If a layer does not override this function, it defaults to the layer's
	/// shutdown function
	virtual Status eStop();


	/// Get the status of the layer
	///
	/// @ret the layer's status
	Status getStatus() const;

	// TODO Make protected
	/// Set the status of the layer
	///
	/// @param status the status
	void setStatus(Status status);

	// TODO These should probably only be accessible from HyQApp
	/// Runs the layer
	///
	virtual Status run() = 0;

	/// Kills the layer
	///
	virtual Status shutdown() = 0;

	/// Prints the state of the layer
	///
	virtual std::string where() = 0;


protected:
	/// Adds a component to this layer
	///
	/// @param name the name of the component
	/// @param component a pointer to the component
	void addComponent(const std::string &name, pComponent_t component);

	/// Reads a component from the install path of this layer and loads it
	///
	/// @param name the name of the component
	void loadComponentFromDisk(const std::string &name);

	/// Remove a component from this layer
	///
	/// @param name the name of the component
	void removeComponent(const std::string &name);

	/// Get the status of a component inside of this layer
	///
	/// @return the status of the component, or UNCONSTRUCTED if it does not
	/// exist
	AppLayerComponent::Status getComponentStatus(const std::string &name);

	/// Get the ID of the layer
	///
	/// @return the ID of the layer
	std::string getID();

	// BEGIN critical section
		mutable std::mutex components_mutex;
		std::map<std::string, pComponent_t> components;
	// END critical section

	/// Stores commands registered in the layer
	///
	CommandManager command_manager;

private:
	/// The ID of this layer
	///
	const std::string ID;

	// BEGIN critical section
		mutable std::mutex status_mutex;
		Status status;
	// END critical section

	//logging::coutstream scout;

};
} // end namespace dls

#endif /* end of include guard: APP_LAYER_HPP_H7JRIVPM */
