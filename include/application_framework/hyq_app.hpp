#ifndef HYQ_APP_HPP_2AEPZMMN
#define HYQ_APP_HPP_2AEPZMMN

#include <vector>
#include <memory>
#include <initializer_list>
#include <mutex>

#include "application_framework/app_layer.hpp"
#include "todo.h"

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

	TODO("make this return a status")
	void run();

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
		std::mutex layers_mutex;
	// END critical section
	// BEGIN critical section
		Status status;
		std::mutex status_mutex;
	// END critical section
};

#include "application_framework/hyq_app.tpp"

#endif /* end of include guard: HYQ_APP_HPP_2AEPZMMN */
