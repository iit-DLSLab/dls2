#include "dls2/application/app.hpp"

using namespace dls;

App::App(const std::string &ID) 
    : should_quit(false)
    , command_manager(ID)
	, scout_sys(ID)
	, scout_warn(ID)
	, scout_err(ID)
    , ID_(ID)
	, sm(this)
	, status_mutex()
	, status(AppStatus::INITIALISING)
	, activate_cmd_locked(false)
	, deactivate_cmd_locked(false)
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
			activate_cmd_locked = true;
			sm.raiseEvent(sm.activation_request);
			// wait for reply - TODO: handle commands with state machine
			// wait to go in ACTIVATION state
			while(activate_cmd_locked)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(4));
			}
			// in ACTIVATION state: wait for RUN or IDLE or QUIT state
			while(!sm.isRaised(sm.quit_request))
			{
				if(*sm.state == sm.RUN)
					return true;
				else if (*sm.state == sm.IDLE)
					return false;
				std::this_thread::sleep_for(std::chrono::milliseconds(4));
			}
            return true;
		}),
		{{0,1}},
		true
	);

	this->command_manager.addCommand<>
	(
		"deactivate",
		"Deactivate " + this->getID(),
		std::function<bool()>([&]()->bool
        {
			deactivate_cmd_locked = true;
			sm.raiseEvent(sm.deactivation_request);
			// wait for reply - TODO: handle commands with state machine
			// wait to go in DEACTIVATION state
			while(deactivate_cmd_locked)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(4));
			}
			// in DEACTIVATION state: wait for IDLE or QUIT state
			while(!sm.isRaised(sm.quit_request) && *sm.state != sm.IDLE)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(4));
			}
            return true;
		}),
		{{CommandBase::ALL_STATES_EXCEPT_ZERO, 0}},
		true
	);
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

void App::execute(state_machine::State& state){
	sm.initState(&state);
	sm.start();
}

void App::idle()
{
	setDefaultSchedulerPolicy();
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
	activate_cmd_locked = false;
	// Wait for timeout seconds the input readyness
	double timeout = 10.0; //seconds
	double enlapsed_time = 0.0;
	bool activate = false;

	auto start = std::chrono::high_resolution_clock::now();

	// Check if the app can be activated until
	// -- either it can be activated
	// -- or the timeout is expired
	// -- or a quit request is received
	while(!activate && enlapsed_time <= timeout && !sm.isRaised(sm.quit_request)){
	    activate = checkActivation();

	    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	    enlapsed_time = std::chrono::duration_cast<std::chrono::seconds>(
	                    std::chrono::high_resolution_clock::now() - start).count();
	}

	if(activate)
	    sm.nextState(sm.activated);
	else if (enlapsed_time>timeout)
	    sm.nextState(sm.failed_activation);
	else // quit request
	    sm.nextState(sm.quit_request);
}

void App::deactivation()
{
	deactivate_cmd_locked = false;
	if (sm.isRaised(sm.quit_request))
	{
	    sm.nextState(sm.quit_request);
	}
	else
		sm.nextState(sm.deactivated);
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