#ifndef APP_LAYER_HPP_H7JRIVPM
#define APP_LAYER_HPP_H7JRIVPM

#include <functional>
#include <memory>
#include <vector>
#include <mutex>

#include "application_framework/components/app_layer_component.hpp"

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
		SUCCESS
	};

	AppLayer(const std::initializer_list<pComponent_t>&);
	AppLayer();
	virtual ~AppLayer() = default;

	Status eStop();
	Status getStatus() const;
	void setStatus(Status);

	virtual Status run() = 0;
	virtual Status shutdown() = 0;

protected:
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
	std::function<Status(void)> main;
};

#endif /* end of include guard: APP_LAYER_HPP_H7JRIVPM */
