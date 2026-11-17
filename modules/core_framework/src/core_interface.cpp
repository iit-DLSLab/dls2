#include "dls2/core_framework/log_layer.hpp"
#include "dls2/core_framework/hardware_layer.hpp"
#include "dls2/core_framework/control_layer.hpp"
#include "dls2/core_framework/console_layer.hpp"
#include "dls2/core_framework/estimation_layer.hpp"
// #include "dls2/core_framework/service_layer.hpp"
#include "dls2/supervisor/supervisor.hpp"

extern "C" Layer *create(const std::string& layer_name, const std::string& type, const std::string& robot_name)
{
	Layer* p = nullptr;
	if (type == "log")
	{
		p = new LogLayer(layer_name);
	}	
	else if (type == "hardware")	
	{
		p = new HardwareLayer(layer_name);
	}
	else if (type == "control")
	{
		p = new ControlLayer(layer_name, robot_name);
	}
	else if (type == "console")	
	{
		p = new ConsoleLayer(layer_name);
	}
	else if (type == "estimation")	
	{
		p = new EstimationLayer(layer_name, robot_name);
	}
	// else if (type == "service")	
	// {
	// 	p = new ServiceLayer(layer_name, robot_name);
	// }
	else if (type == "supervisor")	
	{
		p = new Supervisor(layer_name, robot_name);
	}

	return p;
}

extern "C" void destroy(Layer *p)
{
	delete p;
}