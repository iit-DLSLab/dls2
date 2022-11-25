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
#ifndef SERVICE_LAYER_HPP
#define SERVICE_LAYER_HPP

#include "app_layer.hpp"
#include "dls2/components/app_data.hpp"

namespace dls
{
	/// Class responsible for handling different services
	///
	class ServiceLayer : public AppLayer
	{
	public:
		ServiceLayer(std::string ID);
		~ServiceLayer();

		Status run() override;
		Status shutdown() override;

		std::string where() override {return "not yet implemented"; }

		bool loadService(const std::string&);
		bool removeService(const std::string&);

		int numOfServices();

	private:
		// BEGIN critical section
		    std::map<std::string, std::shared_ptr<AppData>> services;
		    std::mutex services_mutex;
	    // END critical section

		dls::DDSWriter *ddsMonitor;	
	};
} // end namespace dls

#endif /* end of include guard: SERVICE_LAYER_HPP */
