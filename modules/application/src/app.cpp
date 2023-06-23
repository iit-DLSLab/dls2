/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#include "dls2/application/app.hpp"

using namespace dls;

App::App(const std::string &ID_) 
    : should_quit(false)
    , command_manager(ID_)
	, scout_sys(ID_)
	, scout_warn(ID_)
	, scout_err(ID_)
    , ID(ID_)
	, status_mutex()
	, status(AppStatus::INITIALISING)
	
{
	command_manager.addCommand<>
	(
		"shutdown",
		"Shutdown the " + ID + " app",
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
		"Prints the state of " + this->ID,
		std::function<bool()>([&]()->bool
        {
			std::cout << where() << std::endl;
            return true;
		}),
		{},
		true
	);
}

App::~App()
{ }

std::string App::getID()
{
	return this->ID;
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