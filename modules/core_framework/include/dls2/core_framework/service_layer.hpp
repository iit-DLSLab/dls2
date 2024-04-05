
#ifndef SERVICE_LAYER_HPP
#define SERVICE_LAYER_HPP

#include "dls2/application/layer.hpp"
#include "dls2/application/app_data.hpp"

namespace dls
{
	/// Class responsible for handling different services
	///
	class ServiceLayer : public Layer
	{
	public:
		ServiceLayer(std::string ID);
		~ServiceLayer();

		void close() override;

		std::string where() override {return "not yet implemented"; }

		bool loadService(const std::string&);
		bool unloadService(const std::string);

		int numOfServices();

		void monitor() override;

	private:
		// BEGIN critical section
		    std::map<std::string, std::shared_ptr<AppData>> services;
		    std::mutex services_mutex;
	    // END critical section

		std::shared_ptr<dls::DDSWriter> ddsMonitor;
	};
} // end namespace dls

#endif /* end of include guard: SERVICE_LAYER_HPP */
