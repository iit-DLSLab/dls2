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
#ifndef HYQ_APP_HPP_2AEPZMMN
#define HYQ_APP_HPP_2AEPZMMN

#include <vector>
#include <memory>
#include <initializer_list>
#include <thread>
#include <mutex>

#include "app_layer.hpp"
#include "dls2/log/log.hpp"

namespace dls
{
/// Class encapsulating the framework
///
/// This class can dynamically launch different layers
class HyQApp
{
	using pLayer_t = std::shared_ptr<AppLayer>;
public:
	// =========================================================================
	// Status
	// =========================================================================
	/// Enum encapsulating the overall state of the program
	enum class Status
	{
		INITIALISING,	///< The program is being initialised
		RUNNING,		///< The program is running normally
		LAYER_CRASH,	///< A layer has crashed in the program
		SUCCESS,		///< The program has finished execution successfully
		FATAL_ERROR		///< The program has encountered a fatal error
	};

	// =========================================================================
	// Constructors
	// =========================================================================
	HyQApp();
	HyQApp(const std::initializer_list<pLayer_t>&);
	~HyQApp();


	/// Emergency stop function. When this is called, all layers and layer
	/// components are brought to an emergency stop
	///
	/// This function is called when a segfault occurs somewhere in the code
	void panic();

	/// Returns the current status of the overall application
	Status getStatus();

	/// Inserts a layer at runtime if such a layer is not yet present
	///
	/// Returns `true` if layer inserted successfully.
	/// Returns `false` if layer could not be inserted.
	/// Only one instance of any given layer type may be inserted at a given
	/// time
	template <typename layer_t>
	bool addLayer(std::shared_ptr<layer_t>);

	// TODO ("make this return a status")

	/// Starts the app
	///
	/// Spawns a thread for each layer and calls that layer's run inside the
	/// thread
	void run();

	/// Stops the app
	///
	/// Asks each layer in the app to stop. Each layer is responsible for nicely
	/// stopping when this request is received
	void stop();

private:
	/// Performs any initialisation required by the libraries used in the
	/// project
	void init_libraries();

	/// Properly closes any libraries used in the project
	void close_libraries();

	/// Sets the internal status of the overall application
	void setStatus(Status);

	// BEGIN critical section
		std::vector<pLayer_t> layers;
		std::vector<std::thread> layer_threads;
		std::mutex layers_mutex;
	// END critical section
	// BEGIN critical section
		Status status;
		std::mutex status_mutex;
	// END critical section
	logging::coutstream outstream;
};
} // end namespace dls

#include "hyq_app.tpp"

#endif /* end of include guard: HYQ_APP_HPP_2AEPZMMN */
