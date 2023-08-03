#ifndef APP_DATA_CPP
#define APP_DATA_CPP

#include "dls2/application/app_data.hpp"

using namespace dls;

AppData::AppData(const std::string& ID) 
    : proc(nullptr)
    , ID_(ID)
{ }

AppData::~AppData(){}

std::string AppData::getID()
{
    return this->ID_;
}

#endif /* end of include guard: APP_DATA_CPP*/