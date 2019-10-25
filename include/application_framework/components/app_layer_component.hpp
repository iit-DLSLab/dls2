#ifndef APP_LAYER_COMPONENT_HPP_B4KVSXQY
#define APP_LAYER_COMPONENT_HPP_B4KVSXQY

#include <mutex>

class AppLayerComponent
{
public:
	AppLayerComponent();
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

	// TODO perhaps add stop, pause?
	virtual Status run() = 0;
	//virtual Status shutdown() = 0;
	virtual Status eStop() = 0;
	virtual Status stop() = 0;
	//virtual Status pause() = 0;

	Status getStatus();

protected:
	void setStatus(Status);

private:
	// BEGIN critical section
		Status status;
		std::mutex status_mutex;
	// END critical section
};

#endif /* end of include guard: APP_LAYER_COMPONENT_HPP_B4KVSXQY */
