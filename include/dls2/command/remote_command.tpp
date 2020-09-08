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
#ifndef REMOTE_COMMAND_TPP_NEKXILQ4
#define REMOTE_COMMAND_TPP_NEKXILQ4

namespace dls
{
	// -------------------------------------------------------------------------
	// RemoteCommand
	// -------------------------------------------------------------------------
	template <typename U, typename... Ts>
	void RemoteCommand::pushArg(U u, Ts... ts) const
	{
		// push single argument
		//
		// this is implemented in the cpp file
		this->pushArg(u);

		// recurse over remaining arguments
		//
		// TODO make this `if constexpr` if migrating to a newer standard
		if(sizeof...(ts) >= 1)
		{
			this->pushArg(ts...);
		}
	}

	// -------------------------------------------------------------------------
	// RemoteCommandCallable
	// -------------------------------------------------------------------------
	template <typename... Ts>
	void RemoteCommandCallable::operator()(Ts... ts)
	{
		this->pRemote_command->call(ts...);
	}
} // end namespace dls

#endif /* end of include guard: REMOTE_COMMAND_TPP_NEKXILQ4 */
