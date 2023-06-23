#include "dls2/core_framework/log_layer.hpp"
#include "dls2/core_framework/hardware_layer.hpp"
#include "dls2/core_framework/control_layer.hpp"
#include "dls2/core_framework/console_layer.hpp"
#include "dls2/core_framework/estimation_layer.hpp"
#include "dls2/core_framework/service_layer.hpp"

extern "C" Layer *create(const std::string& type, const std::string& robot_name)
{
	Layer* p = nullptr;
	if (type == "log")
	{
		p = new LogLayer("LogLayer");
	}	
	else if (type == "hardware")	
	{
		p = new HardwareLayer("HardwareLayer");
	}
	else if (type == "control")
	{
		p = new ControlLayer("ControlLayer", robot_name);
	}
	else if (type == "console")	
	{
		p = new ConsoleLayer("ConsoleLayer");
	}
	else if (type == "estimation")	
	{
		p = new EstimationLayer("EstimationLayer");
	}
	else if (type == "service")	
	{
		p = new ServiceLayer("ServiceLayer");
	}

	return p;
}

extern "C" void destroy(Layer *p)
{
	delete p;
}