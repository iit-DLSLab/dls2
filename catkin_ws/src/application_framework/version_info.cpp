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
// =============================================================================
// Includes
// =============================================================================
#include <string>
#include "dls2/application_framework/version_info.phpp"

using namespace dls;

#ifndef NDEBUG
static std::string version_string = "0.0.0-Debug";
#else
static std::string version_string = "0.0.0";
#endif
static int version_major = 0;
static int version_minor = 0;
static int version_patch = 0;

std::string VersionInfo::getVersionString()
{
	return version_string;
}

int VersionInfo::getVersionMajor()
{
	return version_major;
}

int VersionInfo::getVersionMinor()
{
	return version_minor;
}

int VersionInfo::getVersionPatch()
{
	return version_patch;
}
