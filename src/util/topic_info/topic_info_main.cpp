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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
// TODO this is a tool that needs to be made into a separate project and made
// more sophisticated
#include "util/topic_info/topic_info.hpp"
#include <signal.h>

using namespace dls;
int main(int argc, char **argv)
{
	std::string name;
	if(argc > 1)
	{
		name = argv[1];
	}
	else
	{
		name = "dummy_controller";
	}
	// listen to topics and print
	TopicInfo info(name);

	// wait until killed by user
	sigset_t sigset;
	sigaddset(&sigset, SIGTERM);
	int signum;
	sigwait(&sigset, &signum);

	return 0;
}
