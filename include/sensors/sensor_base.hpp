#ifndef SENSOR_BASE_HPP_XOP20IWP
#define SENSOR_BASE_HPP_XOP20IWP

#include <mutex>

class SensorBase
{
public:
	enum class Status
	{
		UNINITIALISED,
		NORMAL_OPERATION,
		FATAL_ERROR
	};

	enum class Criticality
	{
		REQUIRED,
		STARTUP_ONLY,
		SUPPORT
	};

	SensorBase(Criticality);
	virtual ~SensorBase() = 0;

	Status getStatus() const;
	Criticality getCriticality() const;

	virtual void update() = 0;

protected:
	void setStatus(Status);

private:
	const Criticality criticality;

	// BEGIN critical section
		Status status;
		mutable std::mutex status_mutex;
	// END critical section
};

#endif /* end of include guard: SENSOR_BASE_HPP_XOP20IWP */
