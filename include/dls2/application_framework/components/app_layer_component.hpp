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

#include "dls2/command/command.hpp"
#include "dls2/util/log/log.hpp"

namespace dls
{
/// A component that can be launched inside of an application layer
///
class AppLayerComponent
{
public:
	using ID_t = std::string;
	typedef AppLayerComponent *create_t(const ID_t&);
	typedef void destroy_t(AppLayerComponent*);


	/// Constructor
	/// @parm ID the name of this component
	AppLayerComponent(const ID_t &ID);

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
	ID_t getID();

	/// Prints the state of this component
	///
	virtual std::string where() = 0;

protected:

	/// Sets the status of this component
	///
	/// @param status the status to set
	void setStatus(Status status);

private:
	// BEGIN critical section
		Status status;           ///< The status of this  component
		std::mutex status_mutex; ///< Status mutex
	// END critical section
	const ID_t ID; ///< The ID of this component

	/// Command manager for commands that all components must have
	///
	// CommandManager command_manager;

	/// Print stream
	///
	// logging::coutstream scout;
};
} // end namespace dls

#endif /* end of include guard: APP_LAYER_COMPONENT_HPP_B4KVSXQY */
