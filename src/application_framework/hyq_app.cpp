#include "application_framework/hyq_app.hpp"
TODO("remove iostream include")
#include <iostream>

// =============================================================================
// Constructors
// =============================================================================
HyQApp::HyQApp() :
	layers(),
	layers_mutex(),
	status(Status::INITIALISING),
	status_mutex()
{
	init_libraries();
}

HyQApp::HyQApp(const std::initializer_list<pLayer_t> &_layers) :
	layers(_layers),
	layers_mutex(),
	status(Status::INITIALISING),
	status_mutex()
{
	init_libraries();
}

HyQApp::~HyQApp()
{
	close_libraries();
}

// =============================================================================
// Member Functions
// =============================================================================
void HyQApp::panic()
{
	std::cout << "Panic" << std::endl;
	TODO("Here set safety")

	std::lock_guard<std::mutex> lock(this->layers_mutex);
	for(const auto &pLayer : layers)
	{
		pLayer->eStop();
	}
}

HyQApp::Status HyQApp::getStatus()
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void HyQApp::setStatus(Status s)
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	this->status = s;
}

void HyQApp::run()
{
	TODO("start each layer in a different thread controlled by thread pool")
	{
		std::lock_guard<std::mutex> lock(this->layers_mutex);
		for(const auto &pLayer : this->layers)
		{
			pLayer->run();
		}
	}
}
