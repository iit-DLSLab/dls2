#include "dls2/core_framework/log_layer.hpp"
#include "dls2/core_framework/hardware_layer.hpp"
#include "dls2/core_framework/control_layer.hpp"
#include "dls2/core_framework/console_layer.hpp"
#include "dls2/core_framework/estimation_layer.hpp"
#include "dls2/core_framework/service_layer.hpp"
#include "dls2/core_framework/foxglove_layer.hpp"


extern "C" Layer *create(const std::string& type, const std::string& ID_)
{
	Layer* p = nullptr;
	std::cout << "Loading " << type << " layer" << std::endl;
	
	if (type == "log")
	{
		p = new LogLayer(ID_);
	}	
	else if (type == "hardware")	
	{
		p = new HardwareLayer(ID_);
	}
	else if (type == "control")
	{
		p = new ControlLayer(ID_);
	}
	else if (type == "console")	
	{
		p = new ConsoleLayer(ID_);
	}
	else if (type == "estimation")	
	{
		p = new EstimationLayer(ID_);
	}
	else if (type == "service")	
	{
		p = new ServiceLayer(ID_);
	}
	else if (type == "foxglove")	
	{
		p = new FoxgloveLayer(ID_);
	}


	return p;
}

extern "C" void destroy(Layer *p)
{
	delete p;
}