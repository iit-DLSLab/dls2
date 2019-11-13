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
#ifndef CLASS_LOADER_HPP_S3OYPT1Z
#define CLASS_LOADER_HPP_S3OYPT1Z

/// Utility class that simplifies loading shared objects into classes
class ClassLoader
{
public:
	/// Loads a class from a shared object.
	///
	/// The shared object must expose `create` and a `destroy` C functions. The
	/// shared_ptr will use `create` to instantiate an instance of the class,
	/// and will use `destroy` as its custom deleter.
	/// @param name The string name of the shared object containing the class
	/// @return a shared pointer to the class
	template <class T>
	static std::shared_ptr<T> loadClass(const std::string &name);
};

#include "util/class_loader.tpp"

#endif /* end of include guard: CLASS_LOADER_HPP_S3OYPT1Z */
