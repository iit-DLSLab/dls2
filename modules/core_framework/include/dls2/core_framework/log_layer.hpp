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
#ifndef LOG_LAYER_HPP_IAHZ5BZG
#define LOG_LAYER_HPP_IAHZ5BZG

#include "dls2/application/layer.hpp"
#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/topics/topics.hpp"
#include "foxglove/foxserver.hpp"

namespace dls
{
	// TODO this class was built with a lot of copy-pasting. Make it more elegant

	/// Class responsible for handling the logging of the framework
	///
	class LogLayer : public Layer
	{
	public:
		LogLayer(std::string ID);

		AppStatus run() override;
		AppStatus stop() override;

		std::string where() override {return "not yet implemented"; }

	private:
		static std::string get_current_time();

		DDSParticipant ddsLogLink;

		dls::FoxServer foxserver;
	};
} // end namespace dls

#endif /* end of include guard: LOG_LAYER_HPP_IAHZ5BZG */
