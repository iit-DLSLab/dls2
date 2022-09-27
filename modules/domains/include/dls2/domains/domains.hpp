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
#ifndef DOMAINS_HPP
#define DOMAINS_HPP

namespace dls
{
	typedef unsigned int domainType;
	
	namespace domains
	{
		extern domainType develop;
		extern domainType layers;
		extern domainType command;
		extern domainType control;
		extern domainType controllers;
        extern domainType hardwares;
		extern domainType console;
		extern domainType logging;
		extern domainType service;
	}
}

#endif /* end of include guard: DOMAINS_HPP */
