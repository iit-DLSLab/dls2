
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
	}
}

#endif /* end of include guard: DOMAINS_CPP */
