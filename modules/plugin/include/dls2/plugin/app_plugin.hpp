#ifndef APP_PLUGIN_HPP
#define APP_PLUGIN_HPP

#include "dls2/application/app.hpp"
#include "dls2/plugin/plugin.hpp"
#include <mutex>

namespace dls
{
	/*!
	 * @class AppPlugin
	 * @brief This is a base class for creating app components.
	 * @details
	 * Any software module that wants to run a generic application while interacting with the DLS2 network has to create a plugin inheriting from this class
	 */
	class AppPlugin : public App, public Plugin
	{
	public:
		/*!
		 * @brief Constructor.
		 * @param[in] ID name of plugin
		 * @param[in] domain domain the plugin belongs to
		 */
		AppPlugin(const std::string &ID, const domainType &domain = domains::signals);

		/*!
		 * @brief Destructor.
		 */
		virtual ~AppPlugin();

		/*!
		 * @brief Type definintion identifying the function that calls the constructor of the plugin.
		 * @details
		 * This function is called when loading at run-time this plugin, through dlopen.
		 * @param[in] ID name of plugin
		 * @param[in] robot_name name of the robot
		 */
		typedef AppPlugin *create_t(const std::string& ID, const std::string& robot_name);

		/*!
		 * @brief Type definintion identifying the function that calls the destructor of the plugin.
		 * @details
		 * This function is called to destroy the plugin.
		 * @param[in] p pointer to the plugin to destroy
		 */
		typedef void destroy_t(AppPlugin *p);

		virtual bool checkActivation() override;
	};
} // end namespace dls

#endif /* APP_PLUGIN_HPP */
