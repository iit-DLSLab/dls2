
#ifndef SERVICE_LAYER_HPP
#define SERVICE_LAYER_HPP

#include "dls2/application/layer.hpp"
#include "dls2/application/app_data.hpp"
#include "dls2/data_visualizer/data_visualizer_base.hpp"

namespace dls
{
	/// Class responsible for handling different services
	///
	class ServiceLayer : public Layer
	{
	public:
		ServiceLayer(std::string ID, const std::string& robot_name);
		~ServiceLayer();

		void close() override;

		std::string where() override {return "not yet implemented"; }

		bool loadService(const std::string&);
		bool unloadService(const std::string);

		bool loadDataVisualizer(const std::string&);
		bool unloadDataVisualizer(const std::string);

		bool loadTask(const std::string& ID);
		bool unloadTask(const std::string& ID);

		int numOfServices();
		int numOfDataVisualizers();

		void monitor() override;

	private:
		// BEGIN critical section
		    std::map<std::string, std::shared_ptr<AppData>> services;
			std::map<std::string, std::shared_ptr<AppData>> data_visualizers_;
			std::map<std::string, std::shared_ptr<AppData>> tasks;
		    std::mutex services_mutex;
			std::mutex data_visualizers_mutex_;
	    // END critical section

		std::shared_ptr<dls::DDSWriter> ddsMonitor;

		//! Name of the robot
		const std::string robot_name;
	};
} // end namespace dls

#endif /* end of include guard: SERVICE_LAYER_HPP */
