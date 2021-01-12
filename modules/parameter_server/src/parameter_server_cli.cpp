// ================================= Includes ==================================
#include "dls/parameter_server_client.hpp"
#include "parameter_server_topics.hpp"

#include <string>
#include <iostream>
#include <memory>

// ============================== Helper Structs ===============================
static struct Opts
{
	int  argc;
	char **argv;
} opts;

struct Command
{
	virtual void execute(dls::ParameterServerClient &c) = 0;
};

struct SetCommand : public Command
{
	std::string type;
	std::string key;
	std::string value;

	virtual void execute(dls::ParameterServerClient &c) override;
};

struct GetCommand : public Command
{
	std::string type;
	std::string key;

	virtual void execute(dls::ParameterServerClient &c) override;
};

// ================================ Prototypes =================================
void        exitError();
std::string getHelpString();
Command     *parseCommand();
Command     *parseSetCommand();
Command     *parseGetCommand();


// =================================== Main ====================================
int main(int argc, char **argv)
{
	opts.argc = argc;
	opts.argv = argv;

	if(argc != 4 && argc != 5)
	{
		exitError();
	}

	dls::ParameterServerClient client;


	Command *command = parseCommand();
	command->execute(client);
	return EXIT_SUCCESS;
}

// =========================== Supporting Functions ============================
std::string getHelpString()
{
	std::string name = opts.argv[0];
	std::string help_string =
		name + "set <type> <key> <value>\n" +
		name + "get <type> <key>\n" +
		"------------------------" +
		"Where <type> is one of [double]";

	return help_string;
}

Command *parseCommand()
{
	if(opts.argv[1] == std::string("set"))
	{
		return parseSetCommand();
	}
	else if(opts.argv[1] == std::string("get"))
	{
		return parseGetCommand();
	}
	else
	{
		exitError();
	}
}

Command *parseSetCommand()
{
	SetCommand *c = new SetCommand();
	c->type       = opts.argv[2];
	c->key        = opts.argv[3];
	c->value      = opts.argv[4];

	return c;
}

Command *parseGetCommand()
{
	GetCommand *c = new GetCommand();
	c->type       = opts.argv[2];
	c->key        = opts.argv[3];
	return c;
}

void exitError()
{
	std::cerr << getHelpString() << std::endl;
	exit(EXIT_FAILURE);
}

// ============================== Helper Structs ===============================
// -------------------------------- set command --------------------------------
void SetCommand::execute(dls::ParameterServerClient &c)
{
	if(type == std::string("double"))
	{

		c.setDouble(this->key, std::stod(this->value));
	}
	else
	{
		exitError();
	}
}

// -------------------------------- get command --------------------------------
void GetCommand::execute(dls::ParameterServerClient &c)
{
	if(type == std::string("double"))
	{
		double d = c.getDouble(this->key);
		std::cout << d << std::endl;
	}
	else
	{
		exitError();
	}
}
