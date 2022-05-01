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
		domainType develop 		= 2;
		domainType layers 		= 4;
		domainType command		= 5;
		domainType control 		= 0;
		domainType controllers 	= 1;
		domainType console 		= 7;
		domainType logging 		= 8;
		domainType service 		= 9;
	}
}

#endif /* end of include guard: DOMAINS_CPP */
