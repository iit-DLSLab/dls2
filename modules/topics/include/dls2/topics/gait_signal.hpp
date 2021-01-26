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
#ifndef GAIT_SIGNAL_HPP_DQFN1IAQ
#define GAIT_SIGNAL_HPP_DQFN1IAQ


namespace dls
{
	namespace topics
	{
		/// The topic where gait signals are published
		constexpr auto gait_signal = "GaitSignal";
		constexpr auto gait_layer  = "GaitSignalLayer";
	}
}

#endif /* end of include guard: GAIT_SIGNAL_HPP_DQFN1IAQ */
