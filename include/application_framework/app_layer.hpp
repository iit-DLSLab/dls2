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

#include "application_framework/components/app_layer_component.hpp"

namespace dls
{
class AppLayer
{
public:
	using pComponent_t = std::shared_ptr<AppLayerComponent>;
	enum class Status
	{
		INITIALISING,
		RUNNING,
		FATAL_ERROR,
		E_STOP,
		SUCCESS,
		STOP
	};

	AppLayer(const std::initializer_list<pComponent_t>&);
	AppLayer();
	virtual ~AppLayer() = default;

	Status eStop();
	Status getStatus() const;
	TODO("Make protected")
	void setStatus(Status);

	TODO("These should probably only be accessible from HyQApp")
	virtual Status run() = 0;
	virtual Status shutdown() = 0;

protected:
	TODO("remove this")
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
	TODO("remove this")
	std::function<Status(void)> main;
};
} // end namespace dls

#endif /* end of include guard: APP_LAYER_HPP_H7JRIVPM */
