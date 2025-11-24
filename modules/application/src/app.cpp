#include "dls2/application/app.hpp"

using namespace dls;

App::App(const std::string &ID) 
    : should_quit(false)
    , command_manager(ID)
	, scout_sys(ID)
	, scout_warn(ID)
	, scout_err(ID)
	, event_notifier(ID)
    , ID_(ID)
	, sm(this)
	, activation_message("")
	, status_mutex()
	, status(AppStatus::INITIALISING)
{
	command_manager.addCommand<>
	(
		"shutdown",
		"Shutdown the " + ID_ + " app",
        std::function<bool()>([&]()->bool
        {
			this->stop();
            return true;
		}),
		{},
		true
	);

	command_manager.addCommand<>
	(
		"where",
		"Prints the state of " + this->ID_,
		std::function<bool()>([&]()->bool
        {
			std::cout << where() << std::endl;
            return true;
		}),
		{},
		true
	);

	this->command_manager.addCommand<>
	(
		"activate",
		"Activate " + this->getID(),
		std::function<bool()>([&]()->bool
        {
			sm.raiseEvent(sm.activation_request);
            return true;
		}),
		{{0,1}},
		false
	);

	this->command_manager.addCommand<>
	(
		"stopActivation",
		"Stop activation of " + this->getID(),
		std::function<bool()>([&]()->bool
        {
			sm.raiseEvent(sm.stop_activation);
            return true;
		}),
		{{1,0}},
		false
	);

	this->command_manager.addCommand<>
	(
		"deactivate",
		"Deactivate " + this->getID(),
		std::function<bool()>([&]()->bool
        {
			sm.raiseEvent(sm.deactivation_request);
            return true;
		}),
		{{CommandBase::ALL_STATES_EXCEPT_ZERO, 0}},
		false);

	// Launching app status monitor thread
	monitor_future_ = std::async(std::launch::async, &App::monitorApp, this);
}

App::~App(){}

std::string App::getID()
{
	return this->ID_;
}

AppStatus App::getStatus() const
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void App::setStatus(AppStatus s)
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	this->status = s;
}

bool App::shouldQuit()
{
	return should_quit;
}

AppStatus App::eStop()
{
	this->stop();
	return getStatus();
}

std::string App::get_current_time()
{
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[256];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%y-%m-%d_%H.%M.%S", &tstruct);

	return buf;
}

void App::execute(){
	sm.nextState(sm.initialized);
	sm.start();
}

void App::idle()
{
	setDefaultSchedulerPolicy();
	command_manager.getCommand("activate")->setEnabled();
	command_manager.triggerLevelWatcher();

	sm.waitAsynchEvent({sm.activation_request, sm.quit_request});
	if(sm.isRaised(sm.activation_request))
	{
	    sm.nextState(sm.activation_request);
	}
	else if(sm.isRaised(sm.quit_request))
	{
	    sm.nextState(sm.quit_request);
	}
}

void App::activation()
{
	command_manager.getCommand("stopActivation")->setEnabled();
	command_manager.triggerLevelWatcher();

	// Check if the app can be activated until
	// -- either it can be activated
	// -- or a quit request is received
	// while(enlapsed_time <= timeout && !sm.isRaised(sm.quit_request)){
	bool activate = false;
	std::string old_message = "";
	while(!activate && !sm.isRaised(sm.stop_activation) && !sm.isRaised(sm.quit_request)){
	    activate = checkActivation();

		if(activation_message.str()!="" && old_message!=activation_message.str())
			scout_warn << activation_message.str() << std::endl;
		old_message = activation_message.str();
		activation_message.str("");

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if(activate){
		command_manager.getCommand("activate")->setDisabled();
		command_manager.getCommand("stopActivation")->setDisabled();
		command_manager.getCommand("deactivate")->setEnabled();
		command_manager.triggerLevelWatcher();
	    sm.nextState(sm.activated);
	}
	else if (sm.isRaised(sm.stop_activation)){
		command_manager.getCommand("stopActivation")->setDisabled();
		command_manager.triggerLevelWatcher();
	    sm.nextState(sm.stop_activation);
	}
	else{ // quit request
	    sm.nextState(sm.quit_request);
	}
}

void App::deactivation()
{
	bool deactivation = deactivating();
	// a quit request might be raised during the deactivation
	if (sm.isRaised(sm.quit_request))
	{
	    sm.nextState(sm.quit_request);
	}
	else if (deactivation){
		command_manager.getCommand("deactivate")->setDisabled();
		command_manager.triggerLevelWatcher();
	    sm.nextState(sm.deactivated);
	}
	else{
		scout_err << "Deactivation failed. Quitting the application..." << std::endl;
		sm.nextState(sm.quit_request);
	}
}

void App::fail()
{
	sm.waitAsynchEvent({sm.deactivation_request, sm.quit_request});
	if(sm.isRaised(sm.deactivation_request))
	{
	    sm.nextState(sm.deactivation_request);
	}
	else if(sm.isRaised(sm.quit_request))
	{
	    sm.nextState(sm.quit_request);
	}
}

void App::quit()
{
	setDefaultSchedulerPolicy();
	close();
	sm.stop();
}

void App::close(){}

void App::stop()
{
	sm.raiseEvent(sm.quit_request);
}

std::string App::where(){
	std::stringstream ss;
	ss  << "App " << ID_ << " is in state " << sm.state->name << std::endl;
	return ss.str();
}

void App::setDefaultSchedulerPolicy()
{
	struct sched_attr scheduler_attributes;
	memset(&scheduler_attributes, 0, sizeof(struct sched_attr));
	scheduler_attributes.size = sizeof(struct sched_attr);
	scheduler_attributes.sched_policy = SCHED_OTHER;

	unsigned int flags = 0;
    int ret = sched_setattr(0, &scheduler_attributes, flags);
    if (ret < 0) {
        perror("sched_setattr");
        exit(-1);
    }
}

bool App::checkActivation(){
	return true;
}

bool App::deactivating(){
	return true;
}

void App::monitorApp()
{
	while (!should_quit)
	{
		// Fill in relevant fields
		status_msg.component_name() = getID();
		status_msg.current_state() = sm.state->name;
		status_msg.desired_state() = "run"; // TODO: check
		status_msg.cpu_usage() = 0.0;		// TODO: calculate

		const bool anomaliesDetected = status_msg.current_state() != status_msg.desired_state();

		// Notify anomalies if any at this stage
		if (anomaliesDetected)
		{
			dls2_interface::msg::EventLog event_msg;
			event_notifier.notify(EventID::CPU_THROTTLING, // TODO: method taking care of anomalies-event type association
								  EventSeverity::ERROR,	   // TODO: method taking care of anomalies-event type association
								  "Ops...");			   // TODO
		}

		childMonitor();

		std::this_thread::sleep_for(std::chrono::milliseconds(monitor_period_ms));
	}
	return;
}