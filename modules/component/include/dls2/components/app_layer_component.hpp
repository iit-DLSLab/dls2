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
#ifndef APP_LAYER_COMPONENT_HPP_B4KVSXQY
#define APP_LAYER_COMPONENT_HPP_B4KVSXQY

#include <mutex>

#include "dls2/command/command_manager.hpp"
#include "dls2/log/log.hpp"

namespace dls
{
/// A component that can be launched inside of an application layer
///
class AppLayerComponent
{
public:
	typedef AppLayerComponent *create_t(const std::string&);
	typedef void destroy_t(AppLayerComponent*);


	/// Constructor
	/// @parm ID the name of this component
	AppLayerComponent(const std::string &ID);

	virtual ~AppLayerComponent() = default;

	/// Status of this component
	///
	enum class Status
	{
		UNCONSTRUCTED,    ///< Component has not been built
		RUNNING,          ///< Component is running normally
		FATAL_ERROR,      ///< Component has had a fatal error
		E_STOP,           ///< Component has performed an emergency stop
		SUCCESS,          ///< Component finshed succesfully
		FAIL,             ///< Componnent failed
		STOPPED,          ///< Component stopped
		BREAKING_REALTIME ///< Component is breaking realtime
	};

	// TODO perhaps add stop, pause functions?

	/// Runs this compoent
	///
	virtual Status run() = 0;

	//virtual Status shutdown() = 0;

	/// Emergency stop for this component
	///
	virtual Status eStop() = 0;

	/// Normal stop for this component
	///
	virtual Status stop() = 0;
	//virtual Status pause() = 0;

	/// Return the status of this component
	///
	/// @ret the component's status
	Status getStatus();

	/// Returns the ID of this component
	///
	/// @ret the ID
	std::string getID();

	/// Prints the state of this component
	///
	virtual std::string where() = 0;

protected:

	/// Sets the status of this component
	///
	/// @param status the status to set
	void setStatus(Status status);

	/// Command manager for commands that all components must have
	///
	CommandManager command_manager;

	/// Print stream
	///
	logging::coutstream scout;

private:
	// BEGIN critical section
		Status status;           ///< The status of this  component
		std::mutex status_mutex; ///< Status mutex
	// END critical section
	const std::string ID; ///< The ID of this component
};
} // end namespace dls

#endif /* end of include guard: APP_LAYER_COMPONENT_HPP_B4KVSXQY */
