#include "application_framework/hyq_app.hpp"
#include "util/debug/debug.hpp"

// =============================================================================
// Constructors
// =============================================================================
TODO("Make these constructors DRY")
HyQApp::HyQApp() :
	layers(),
	layer_threads(),
	layers_mutex(),
	status(Status::INITIALISING),
	status_mutex()
{
	this->setStatus(Status::INITIALISING);
	init_libraries();
}

HyQApp::HyQApp(const std::initializer_list<pLayer_t> &_layers) :
	layers(_layers),
	layer_threads(),
	layers_mutex(),
	status(Status::INITIALISING),
	status_mutex()
{
	this->setStatus(Status::INITIALISING);
	init_libraries();
}

HyQApp::~HyQApp()
{
	{
		std::lock_guard<std::mutex> lock(this->layers_mutex);

		// Tell each layer that it needs to stop
		// for(auto &pLayer : this->layers)
		// {
		// 	pLayer->shutdown();
		// }

		// Join each layer's thread
		for(auto &thread : this->layer_threads)
		{
			thread.join();
		}
	}
	close_libraries();
}

// =============================================================================
// Member Functions
// =============================================================================
void HyQApp::panic()
{
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
	// Start each layer in a new thread
	// These threads are joined in the destructor
	{
		std::lock_guard<std::mutex> lock(this->layers_mutex);
		for(const auto &pLayer : this->layers)
		{
			TODO("Set this thread to realtime thread priority")
			this->layer_threads.emplace_back(&AppLayer::run, &(*pLayer));
		}
	}
	this->setStatus(Status::RUNNING);
}
