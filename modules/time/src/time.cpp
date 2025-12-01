
#include "dls2/util/time/time.hpp"
#include "dls2/topics/topics.hpp"

using namespace dls;

Time::Time()
	: time_factor(1)
	, timeLink("Time::timeLink", dls::domains::signals)
{
	timeLink.addReader(
		"Time::sinc",
		dls::topics::time_factor,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				dls2_interface::msg::Double msg = *((dls2_interface::msg::Double *)tuple);

				if(msg.value() > 1)
					Time::time_factor = msg.value();
			}
		}
	);

	timeLink.addWriter("time_writer", dls::topics::time_factor);
}

Time::~Time()
{ }

double Time::getRealTimeFactor()
{
	return this->time_factor;
}

void Time::setRealTimeFactor(double factor)
{
	if(factor < 1)
		return;

	this->time_factor = factor;

	dls2_interface::msg::Double msg;
	msg.value(factor);

	this->timeLink.sendMessage("time_writer", (void *)&msg);
}

bool Time::checkFrequency(const double &desired_frequency,
						  std::chrono::time_point<std::chrono::steady_clock> &loop_time_prec,
						  double &current_frequency)
{
	// Compute current time
	auto loop_time_curr = std::chrono::steady_clock::now();

	// compute current frequency
	auto elapsed_time = loop_time_curr - loop_time_prec;
	current_frequency = 1.0 / (std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_time).count() / 1e9);
	loop_time_prec = loop_time_curr;

	// check if the process is running in real time.
	return (current_frequency - desired_frequency) < 10; // 1% tolerance
}
