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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef APP_LAYER_COMPONENT_HPP_B4KVSXQY
#define APP_LAYER_COMPONENT_HPP_B4KVSXQY

#include <mutex>
#include "todo.h"

namespace dls
{
class AppLayerComponent
{
public:
	using ID_t = std::string;
	AppLayerComponent(const ID_t&);
	virtual ~AppLayerComponent() = default;

	enum class Status
	{
		UNCONSTRUCTED,
		RUNNING,
		FATAL_ERROR,
		E_STOP,
		SUCCESS,
		FAIL,
		STOPPED,
		BREAKING_REALTIME
	};

	// TODO perhaps add stop, pause functions?
	virtual Status run() = 0;
	//virtual Status shutdown() = 0;
	virtual Status eStop() = 0;
	virtual Status stop() = 0;
	//virtual Status pause() = 0;

	Status getStatus();

	/// Returns the ID of this component
	///
	/// @ret the ID
	ID_t getID();

protected:
	void setStatus(Status);

private:
	// BEGIN critical section
		Status status;
		std::mutex status_mutex;
	// END critical section
	const ID_t ID;								///< The ID of this gait generator
};
} // end namespace dls

#endif /* end of include guard: APP_LAYER_COMPONENT_HPP_B4KVSXQY */
