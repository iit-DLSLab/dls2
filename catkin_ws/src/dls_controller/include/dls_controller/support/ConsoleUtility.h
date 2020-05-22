#ifndef CONSOLE_UTILITY_H_
#define CONSOLE_UTILITY_H_


#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <boost/shared_ptr.hpp>

namespace newline {

extern boost::shared_ptr<std::vector<std::string> > options_ptr;


char** consoleAutoComplete(const char*, int ,int);
char* consoleAutoGenerator(const char*,int);


void getDouble(std::string comment, double defaultvalue, double& value);

void getInt(std::string comment, int defaultvalue, int& value);

void getBool(std::string comment, bool defaultvalue, bool &value);

void getString(std::string comment, std::string defaultvalue, std::string& value);

void consoleCleanUp();

}

#endif /* UTILS_H_ */
