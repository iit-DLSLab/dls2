
#ifndef DOMAINS_CPP
#define DOMAINS_CPP

#include "dls2/domains/domains.hpp"

namespace dls
{
	namespace domains
	{
		domainType users 		= 0;
		domainType layers 		= 1;
		domainType command		= 2;
		domainType signals 		= 3;
		domainType controllers 	= 4;
        domainType hardwares 	= 5;
		domainType consoles		= 6;
		domainType logging 		= 7;
		domainType services		= 8;
		domainType estimators	= 9;
		domainType visualization = 10;

		// define id_to_name_map
		std::map<domainType,std::string> id_to_name_map{
			{users, "users"},
			{layers, "layers"},
			{command, "command"},
			{signals, "signals"},
			{controllers, "controllers"},
			{hardwares, "hardwares"},
			{consoles, "consoles"},
			{logging, "logging"},
			{services, "services"},
			{estimators, "estimators"},
			{visualization, "visualization"}
		};

		// define name_to_id_map
		std::map<std::string,domainType> name_to_id_map{
			{"users", users},
			{"layers", layers},
			{"command", command},
			{"signals", signals},
			{"controllers", controllers},
			{"hardwares", hardwares},
			{"consoles", consoles},
			{"logging", logging},
			{"services", services},
			{"estimators", estimators},
			{"visualization", visualization}
		};
	}
}

#endif /* end of include guard: DOMAINS_CPP */
