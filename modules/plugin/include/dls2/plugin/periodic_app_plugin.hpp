#ifndef PERIODIC_APP_PLUGIN_HPP
#define PERIODIC_APP_PLUGIN_HPP

#include "dls2/application/periodic_app.hpp"
#include "dls2/plugin/plugin.hpp"
#include "dls2/plugin/topic_frequency_monitor.hpp"
#include <mutex>
namespace dls
{
	/*!
	 * @class PeriodicAppPlugin
	 * @brief This is a base class for creating periodic components.
	 * @details
	 * Any software module that wants to run periodically while interacting with the DLS2 network has to create a plugin inheriting from this class
	 */
	class PeriodicAppPlugin : public PeriodicApp, public Plugin
	{
	public:
		/*!
		 * @brief Constructor.
		 * @param[in] ID name of plugin
		 * @param[in] domain domain the plugin belongs to
		 */
	PeriodicAppPlugin(const std::string &ID, const domainType &domain = domains::signals, const std::string &periods_file = "/usr/config/periods.yaml");

		/*!
		 * @brief Destructor.
		 */
		virtual ~PeriodicAppPlugin();

		/*!
		 * @brief Type definintion identifying the function that calls the constructor of the plugin.
		 * @details
		 * This function is called when loading at run-time this plugin, through dlopen.
		 * @param[in] ID name of plugin
		 * @param[in] robot_name name of the robot
		 */
		typedef PeriodicAppPlugin *create_t(const std::string& ID, const std::string& robot_name);

		/*!
		 * @brief Type definintion identifying the function that calls the destructor of the plugin.
		 * @details
		 * This function is called to destroy the plugin.
		 * @param[in] p pointer to the plugin to destroy
		 */
		typedef void destroy_t(PeriodicAppPlugin *p);

		virtual bool checkActivation() override;

		/*!
		 * @brief Basic checks when activating a plugin
		 * @return true if the basic checks succeded
		 * 
		 */
		bool basicActivationChecks();

	protected:
		void childMonitor() override;

	private:
		std::map<std::string, double> getInputsFrequency(bool &are_inputs_sync);
		std::map<std::string, double> getInputsDesiredFrequency();

		std::unique_ptr<TopicFrequencyMonitor> topic_frequency_monitor_{ nullptr };
};
} // end namespace dls

#endif /* PERIODIC_APP_PLUGIN_HPP */
