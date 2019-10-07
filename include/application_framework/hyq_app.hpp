#ifndef HYQ_APP_HPP_2AEPZMMN
#define HYQ_APP_HPP_2AEPZMMN

#include <vector>
#include <memory>
#include <initializer_list>
#include <mutex>

#include "application_framework/app_layer.hpp"

class HyQApp
{
	using pLayer_t = std::shared_ptr<AppLayer>;
public:
	enum class Status
	{
		INITIALISING,
		RUNNING,
		LAYER_CRASH,
		SUCCESS,
		FATAL_ERROR
	};

	HyQApp();
	HyQApp(const std::initializer_list<pLayer_t>&);
	void panic();

	Status getStatus();

	/// Inserts a layer at runtime if such a layer is not yet present
	///
	/// Returns `true` if layer inserted successfully.
	/// Returns `false` if layer could not be inserted.
	/// Only one instance of any given layer type may be inserted at a given
	/// time
	template <typename layer_t>
	bool addLayer(std::shared_ptr<layer_t>);

private:
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
