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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef CLASS_LOADER_TPP_TDR8ZWLX
#define CLASS_LOADER_TPP_TDR8ZWLX

#include <iostream>
#include <dlfcn.h>
#include <sstream>
#include "util/class_loader.hpp"

namespace dls
{
template <class T>
std::shared_ptr<T> ClassLoader::loadClass(const std::string &name)
{
	// std::cout << name << std::endl;
	//void *T_lib = dlopen(name.c_str(), RTLD_NOW);
	 void *T_lib = dlopen(name.c_str(), RTLD_LAZY);
	if(!T_lib)
	{
		std::stringstream ss;
		ss << "Error: could not load object " << name << ": " << dlerror();
		std::cout << ss.str() << std::endl;
		throw std::runtime_error(ss.str());
	}

	// clear errors
	dlerror();

	typename T::create_t *create_T =
		(typename T::create_t*)dlsym(T_lib, "create");

	if(!create_T)
	{
		std::stringstream ss;
		ss	<< "Error: could not find instantiation code in " << name
			<< ". Did the module export the class?" << dlerror();
		std::cout << ss.str() << std::endl;		
		throw std::runtime_error(ss.str());
	}

	// clear errors
	dlerror();

	typename T::destroy_t *destroy_T =
		(typename T::destroy_t*)dlsym(T_lib, "destroy");

	if(!destroy_T)
	{
		std::stringstream ss;
		ss	<< "Error: could not find destruction code in " << name
			<< ". Did the module export the class?" << dlerror();
		std::cout << ss.str() << std::endl;		
		throw std::runtime_error(ss.str());
	}

	// clear errors
	dlerror();

	std::shared_ptr<T> pT
		(
			create_T(),		// create a pointer to be managed by the shared_ptr
			destroy_T		// use this as the deleter of the shared_ptr
		);

	return pT;
}
} // end namespace dls

#endif /* end of include guard: CLASS_LOADER_TPP_TDR8ZWLX */
