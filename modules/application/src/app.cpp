#include "dls2/application/app.hpp"

using namespace dls;

App::App(const std::string &ID) 
    : should_quit(false)
    , command_manager(ID)
	, scout_sys(ID)
	, scout_warn(ID)
	, scout_err(ID)
    , ID_(ID)
	, status_mutex()
	, status(AppStatus::INITIALISING)
	, state_machine(this)	
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
	return this->stop();
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
	setDefaultSchedulerPolicy();
	state_machine.nextState(state_machine.initialized);
	state_machine.start();
}

void App::idle()
{
}

void App::activation()
{
}

void App::deactivation()
{
}

void App::fail()
{
}

void App::quit()
{
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