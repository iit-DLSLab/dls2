#include "safety_plugin.hpp"
#include "dls2/topics/topics.hpp"

SafetyPlugin::SafetyPlugin(const std::string& ID)
	: dls::PeriodicAppPlugin(ID){}

SafetyPlugin::~SafetyPlugin(){}

void SafetyPlugin::run(const std::chrono::system_clock::time_point &time){	
	read();

	(void)time;

	// Triggering the following events for testing the safety layer (with cooldown time):

	// PROCESS_DIED
    // WRONG_PROCESS_STATE
    // WRONG_PROCESS_FREQUENCY
    // MISSING_INPUT
    // CPU_USAGE_TOO_HIGH
    // MEM_USAGE_TOO_HIGH
    // INPUTS_NOT_SYNCHRONIZED
    // WRONG_INPUT_FREQUENCY
    // WRONG_SEQUENCE_ID

	this->robust_event_notifier.notify(
		EventID::PROCESS_DIED,
		EventSeverity::ERROR,
		this->getID() + ": node is not running"
	);
	std::cout << "PROCESS_DIED triggered\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms)); // cooldown to avoid anti-spamming filter kicking in

	robust_event_notifier.notify(
		EventID::WRONG_PROCESS_STATE,
		EventSeverity::WARNING,
		this->getID() + " app state is idle (not run)..."
	);
	std::cout << "WRONG_PROCESS_STATE triggered\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms));
	
	this->robust_event_notifier.notify(	
		EventID::WRONG_PROCESS_FREQUENCY,
		EventSeverity::WARNING,
		"Des freq: 250.0 Hz, Curr freq: 186.73 Hz"
	);
	std::cout << "WRONG_PROCESS_FREQUENCY triggered\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms));

	this->robust_event_notifier.notify(
		EventID::MISSING_INPUT,
		EventSeverity::WARNING,
		this->getID() + " app detected missing input topic /blind_state\n"
	);
	std::cout << "MISSING_INPUT triggered\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms));

	this->robust_event_notifier.notify(	
		EventID::CPU_USAGE_TOO_HIGH,
		EventSeverity::WARNING,
		"CPU usage is 234.7 (threshold is: 150.0)"
	);
	std::cout << "CPU_USAGE_TOO_HIGH triggered\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms));

	this->robust_event_notifier.notify(	
		EventID::MEM_USAGE_TOO_HIGH,
		EventSeverity::WARNING,
		"MEM usage is 87.32 (threshold is: 70.0)"
	);
	std::cout << "MEM_USAGE_TOO_HIGH triggered\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms));

	this->robust_event_notifier.notify(
		EventID::INPUTS_NOT_SYNCHRONIZED,
		EventSeverity::WARNING,
		this->getID() + " app detected not synchronized input topics\n"
	);
	std::cout << "INPUTS_NOT_SYNCHRONIZED triggered\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms));

	this->robust_event_notifier.notify(
		EventID::WRONG_INPUT_FREQUENCY,
		EventSeverity::WARNING,
		this->getID() + " app detected unexpected input frequency (110.98 Hz vs 50.0 Hz) on topic /blind_state\n"
	);
	std::cout << "WRONG_INPUT_FREQUENCY triggered\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms));

	this->robust_event_notifier.notify(
		EventID::WRONG_SEQUENCE_ID,
		EventSeverity::WARNING,
		this->getID() + " app detected wrong sequence id from topic /blind_state (missed 4)\n"
	);
	std::cout << "WRONG_SEQUENCE_ID triggered\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_ms));

	std::cout << "--- \n";

	write();
}

// Function for run-time dynamic loading through DLS2 console
extern "C" PeriodicAppPlugin *create(const std::string& ID, const std::string& name){
	// avoid compilation warnings
	(void)name;
	return new SafetyPlugin(ID);
}
extern "C" void destroy(PeriodicAppPlugin *p){
		delete p;
}