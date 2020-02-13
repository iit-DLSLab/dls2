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
#ifndef APP_LAYER_HPP_H7JRIVPM
#define APP_LAYER_HPP_H7JRIVPM

#include <functional>
#include <memory>
#include <vector>
#include <mutex>

#include "dls2/application_framework/components/app_layer_component.hpp"

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
	AppLayer(const std::initializer_list<pComponent_t>&);

	AppLayer();
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

protected:
	// TODO ("remove this")
	// BEGIN critical section
		mutable std::mutex components_mutex;
		std::vector<pComponent_t> components;
	// END critical section

private:
	// BEGIN critical section
		mutable std::mutex status_mutex;
		Status status;
	// END critical section

protected:
	// TODO remove this
	std::function<Status(void)> main;
};
} // end namespace dls

#endif /* end of include guard: APP_LAYER_HPP_H7JRIVPM */
