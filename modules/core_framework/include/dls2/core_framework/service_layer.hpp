
#ifndef SERVICE_LAYER_HPP
#define SERVICE_LAYER_HPP

#include "dls2/application/layer.hpp"
#include "dls2/application/app_data.hpp"
#include "actions/action_client_stock.hpp"

namespace dls
{
	/// Class responsible for handling different services
	///
	class ServiceLayer : public Layer
	{
	public:
		ServiceLayer(std::string ID, const std::string& robot_name);
		~ServiceLayer();

		AppStatus run() override;
		AppStatus stop() override;

		std::string where() override {return "not yet implemented"; }

		bool loadService(const std::string&);
		bool unloadService(const std::string);

		bool loadAction(const std::string& ID);
		bool unloadAction(const std::string& ID);

		int numOfServices();

	private:
		// BEGIN critical section
		    std::map<std::string, std::shared_ptr<AppData>> services;
			std::map<std::string, std::shared_ptr<AppData>> actions;
		    std::mutex services_mutex;
	    // END critical section

		std::shared_ptr<dls::DDSWriter> ddsMonitor;

		ActionClientStock action_client_stock;

		//! Name of the robot
		const std::string robot_name;
	};
} // end namespace dls

#endif /* end of include guard: SERVICE_LAYER_HPP */
