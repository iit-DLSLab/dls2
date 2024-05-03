#ifndef PERIODIC_PLUGIN_BASE_HPP
#define PERIODIC_PLUGIN_BASE_HPP

#include "dls2/application/periodic_app.hpp"
#include "dls2/msg_wrappers/wrapper_base.hpp"
#include "dls2/msg_wrappers/signal_writer.hpp"
#include "dls2/msg_wrappers/signal_reader.hpp"
#include "dls2/plugin_base/plugin_base.hpp"
#include <mutex>
namespace dls
{
	/*!
	 * @class PeriodicPluginBase
	 * @brief This is a base class for creating periodic components.
	 * @details
	 * Any software module that wants to run periodically while interacting with the DLS2 network has to create a plugin inheriting from this class
	 */
	class PeriodicPluginBase : public PeriodicApp, public PluginBase
	{
	public:
		/*!
		 * @brief Constructor.
		 * @param[in] ID name of plugin
		 * @param[in] domain domain the plugin belongs to
		 */
		PeriodicPluginBase(const std::string &ID, const domainType &domain = domains::signals);

		/*!
		 * @brief Destructor.
		 */
		virtual ~PeriodicPluginBase();

		/*!
		 * @brief Type definintion identifying the function that calls the constructor of the plugin.
		 * @details
		 * This function is called when loading at run-time this plugin, through dlopen.
		 * @param[in] ID name of plugin
		 * @param[in] robot_name name of the robot
		 */
		typedef PeriodicPluginBase *create_t(const std::string& ID, const std::string& robot_name);

		/*!
		 * @brief Type definintion identifying the function that calls the destructor of the plugin.
		 * @details
		 * This function is called to destroy the plugin.
		 * @param[in] p pointer to the plugin to destroy
		 */
		typedef void destroy_t(PeriodicPluginBase *p);

		virtual bool checkActivation() override;
	};
} // end namespace dls

#endif /* PERIODIC_PLUGIN_BASE_HPP */
