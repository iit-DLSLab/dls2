
#include <dls_controller/support/ConsoleUtility.h>

namespace newline {


boost::shared_ptr<std::vector<std::string> > options_ptr;

char** consoleAutoComplete(const char * text , int start,  int end)
{
    // This prevents appending space to the end of the matching word
    rl_completion_append_character = '\0'; 
    //No spaces on the command names, otherwise it breaks

    char **matches = (char **)NULL;
 
    if (start == 0)
        matches = rl_completion_matches ((char*)text, &consoleAutoGenerator);
    else
        rl_bind_key('\t',rl_abort);
 
    return (matches);
 
}

char *consoleAutoGenerator(const char *text, int state)
{
    static unsigned int list_index, len;
    const char *name;

    if (!state)
    {
        list_index = 0;
        len = strlen (text);
    }

    while(list_index < options_ptr->size())
    {
    	name = (char *) options_ptr->at(list_index).c_str();
    	list_index++;
        if (strncmp (name, text, len) == 0) return strdup (name);
    }

    // If no names matched, then return NULL.
    return ((char *) NULL);
}

void getDouble(std::string comment, double defaultvalue, double &value)
{
    std::string input = "";

    while(true){
        std::cout << comment << "[" << std::setprecision (3) <<defaultvalue << "]:";
        getline(std::cin, input);
        if(input == ""){ //If user doesnt give input, use current
            value = defaultvalue;
            break;
        }
        // This code converts from string to number safely.
        std::stringstream myStream(input);
        if (myStream >> value)
            break;
        std::cout << "Invalid number, please try again" << std::endl;
    }
}

void getInt(std::string comment, int defaultvalue, int &value)
{
    std::string input = "";

    while(true){
        std::cout << comment << "[" << defaultvalue << "]:";
        getline(std::cin, input);
        if(input == ""){ //If user doesnt give input, use current
            value = defaultvalue;
            break;
        }
        // This code converts from string to number safely.
        std::stringstream myStream(input);
        if (myStream >> value)
            break;
        std::cout << "Invalid number, please try again" << std::endl;
    }
}

void getBool(std::string comment, bool defaultvalue, bool &value)
{
    std::string input = "";

    while(true){
        std::cout << comment << "[" << defaultvalue << "]:";
        getline(std::cin, input);
        if(input == ""){ //If user doesnt give input, use current
            value = defaultvalue;
            break;
        }
        // This code converts from string to number safely.
        std::stringstream myStream(input);
        if (myStream >> value)
            break;
        std::cout << "Invalid number, please try again" << std::endl;
    }
}

void getString(std::string comment, std::string defaultvalue, std::string &value)
{
    std::string input = "";

    std::cout << comment << "[" << defaultvalue << "]:";
    getline(std::cin, input);
    if(input == ""){ //If user doesnt give input, use current
        value = defaultvalue;
    } else{
        value = input;
    }
}

void consoleCleanUp(void)
{
    rl_free_line_state();
    rl_clear_signals();
    rl_cleanup_after_signal();
}

}
