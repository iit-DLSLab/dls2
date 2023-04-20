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
#ifndef APP_STATUS_HPP
#define APP_STATUS_HPP

namespace dls
{
	enum class Status
	{
		UNCONSTRUCTED,    ///< App has not been built
		INITIALISING, 	  ///< App is initialising
		RUNNING,          ///< App is running normally
		FATAL_ERROR,      ///< App has had a fatal error
		E_STOP,           ///< App has performed an emergency stop
		SUCCESS,          ///< App finshed succesfully
		FAIL,             ///< App failed
		STOPPED,          ///< App stopped
		BREAKING_REALTIME ///< App is breaking realtime
	};
} // end namespace dls

#endif /* end of include guard: APP_STATUS */
