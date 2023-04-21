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
#ifndef FOXGLOVE_LAYER_HPP
#define FOXGLOVE_LAYER_HPP

#include "dls2/application/layer.hpp"
#include "foxglove/foxserver.hpp"

namespace dls
{
/// Foxglove layer

	class FoxgloveLayer : public Layer
	{
	public:
		FoxgloveLayer(std::string ID);
		~FoxgloveLayer();

		AppStatus run() override;
		AppStatus stop() override;

		std::string where() override {return "not yet implemented"; }

	private:
        dls::FoxServer foxserver;
	};
} // end namespace dls

#endif /* end of include guard: FOXGLOVE_LAYER_HPP */
