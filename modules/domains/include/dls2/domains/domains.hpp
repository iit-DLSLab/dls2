
#ifndef DOMAINS_HPP
#define DOMAINS_HPP

#include <map>
#include <string>
namespace dls
{
	typedef unsigned int domainType;
	
	namespace domains
	{
		extern domainType users;
		extern domainType layers;
		extern domainType command;
		extern domainType signals;
		extern domainType controllers;
        extern domainType hardwares;
		extern domainType consoles;
		extern domainType logging;
		extern domainType services;
		extern domainType estimators;
		extern domainType visualization;

		extern std::map<domainType,std::string> id_to_name_map;
		extern std::map<std::string,domainType> name_to_id_map;
	}
}

#endif /* end of include guard: DOMAINS_HPP */
