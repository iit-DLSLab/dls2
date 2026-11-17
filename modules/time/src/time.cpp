
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

bool Time::checkFrequency(const double &tolerance_factor, 
						  const double &desired_frequency,
						  std::chrono::time_point<std::chrono::steady_clock> &loop_time_prec,
						  double &current_frequency)
{
	// Compute current time
	auto loop_time_curr = std::chrono::steady_clock::now();

	// compute current frequency
	auto elapsed_time_sec = toSec(loop_time_curr - loop_time_prec);
 
	current_frequency = 1.0 / (elapsed_time_sec); 
	
	loop_time_prec = loop_time_curr;

	// check if the process is running in real time.
	return std::abs(desired_frequency - current_frequency) < tolerance_factor * desired_frequency;
}


std::string Time::convertTimeToDate(long long epoch_ns)
{
	// Split into seconds + nanoseconds
	std::time_t seconds = static_cast<std::time_t>(epoch_ns / 1'000'000'000);
	long int nanoseconds = epoch_ns % 1'000'000'000;

	std::tm tm{};
#if defined(_WIN32)
	localtime_s(&tm, &seconds);
#else
	localtime_r(&seconds, &tm);
#endif

	char buf[64];
	std::snprintf(buf, sizeof(buf),
				"%04d-%02d-%02d %02d:%02d:%02d.%09ld",
				tm.tm_year + 1900,
				tm.tm_mon + 1,
				tm.tm_mday,
				tm.tm_hour,
				tm.tm_min,
				tm.tm_sec,
				nanoseconds);

	return std::string(buf);
}
