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
// TODO this is a tool that needs to be made more sophisticated
#include "dls2/util/topic_info/topic_info.hpp"
#include <thread>

using namespace dls;

std::string argv0;
void print_usage();
int main(int argc, char **argv)
{

	argv0 = argv[0];

	TopicInfo t;
	if(argc < 2)
	{
		print_usage();
		return EXIT_FAILURE;
	}

	// Give TopicInfo a chance to register all topics
	std::this_thread::sleep_for(std::chrono::seconds(1));

	if(strcmp(argv[1],"list") == 0)
	{
		std::cout << t << std::endl;

		return EXIT_SUCCESS;
	}
	else if(strcmp(argv[1], "type") == 0)
	{
		if(argc != 3)
		{
			print_usage();
			return EXIT_FAILURE;
		}
		std::cout << t.getTopicType(argv[2]) << std::endl;
	}
	else if(strcmp(argv[1], "echo") == 0)
	{
		if(argc != 3)
		{
			print_usage();
			return EXIT_FAILURE;
		}
		while(true)
		{
			t.echo(argv[2]);
		}
	}
	else if(strcmp(argv[1], "hz") == 0)
	{
		if(argc != 3)
		{
			print_usage();
			return EXIT_FAILURE;
		}
		t.hz(argv[2]);
		while(true);
	}
	else
	{
		std::cerr << "Usage: " << argv[0] << " list | hz <topic>" << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}

void print_usage()
{
	std::cerr << "Usage: " << argv0 <<
		" list | type <topic> | echo <topic>"
		<< std::endl;
}
