#ifndef PROFILE_HPP_S1TMXWDT
#define PROFILE_HPP_S1TMXWDT

#include <string>
#include <vector>
#include <map>
#include <set>

namespace dls
{
	struct ProfileAction
	{
		enum class Action
		{
			/// Change the current profile to a specified profile
			///
			CHANGE_PROFILE,
		};

		std::vector<std::pair<Action, std::string>> actions;
	};

	class Profile
	{
	public:
		/// Builds the Profile from a YAML string. The constructor will through
		/// a std::runtime_error if the YAML string is malformated
		/// @param yaml_config a YAML string
		Profile(const std::string &yaml_config);

		/// Query what action must be taken when a component crashes
		///
		/// This function does not actually take this action. It is up to the
		/// layer to implement the correct action
		///
		/// @param component the name of the component that crashed. This
		/// includes controllers, gait generators and state estimators
		/// @return an Action
		ProfileAction onCrash(const std::string &component);

		/// The gait generators that is required to run this profile
		///
		std::string required_gait_generator;

		/// The list of controllers that are required to run this profile
		///
		std::set<std::string> required_controllers;

		/// The list of estimators that are required to run this profile
		///
		std::set<std::string> required_estimators;

		/// List of the required states of the robot in order to run this
		/// profile
		std::set<std::string> required_states;

		/// List of actions to take when a component crashes
		///
		/// maps component-name -> Action
		std::map<std::string, ProfileAction> actions;
	};
} // end namespace dls

#endif /* end of include guard: PROFILE_HPP_S1TMXWDT */
