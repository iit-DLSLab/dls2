#ifndef APP_LAYER_COMPONENT_HPP_B4KVSXQY
#define APP_LAYER_COMPONENT_HPP_B4KVSXQY

#include <mutex>

class AppLayerComponent
{
public:
	virtual ~AppLayerComponent();

	enum class Status
	{
		RUNNING,
		FATAL_ERROR,
		E_STOP,
		SUCCESS,
		STOPPED
	};

	virtual Status run() = 0;
	virtual Status shutdown() = 0;
	virtual Status eStop() = 0;

	Status getStatus();

protected:
	void setStatus(Status);

	// BEGIN critical section
		Status status;
		std::mutex status_mutex;
	// END critical section
};

#endif /* end of include guard: APP_LAYER_COMPONENT_HPP_B4KVSXQY */
