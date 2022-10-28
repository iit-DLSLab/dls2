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

#include "app_layer.hpp"
#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/topics/topics.hpp"
#include "foxglove/websocket/server.hpp"

namespace dls
{
	// TODO this class was built with a lot of copy-pasting. Make it more elegant

	/// Class responsible for handling the logging of the framework
	///
	class LogLayer : public AppLayer
	{
	public:
		LogLayer(std::string ID);

		Status run() override;
		Status shutdown() override;

		std::string where() override {return "not yet implemented"; }

	private:
		static std::string get_current_time();

		dls::FoxServer server;

		bool startFoxServer();
		bool stopFoxServer();

		DDSParticipant ddslink;

	};
} // end namespace dls

#endif /* end of include guard: LOG_LAYER_HPP_IAHZ5BZG */
