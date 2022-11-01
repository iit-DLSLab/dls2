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
		domainType console 		= 6;
		domainType logging 		= 7;
		domainType service 		= 8;
		domainType develop 		= 9;
	}
}

#endif /* end of include guard: DOMAINS_CPP */
