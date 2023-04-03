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
#include "dls2/motion_generator/motion_generator.hpp"


using namespace dls;

MotionGenerator::MotionGenerator
(
	const std::string &ID,
	const std::shared_ptr<robotlib::RobotBase>& pRobot_,
	const period_t &period_
) 
	: PeriodicAppLayerComponent(ID, period_)
	, pRobot(pRobot_)
	, home_configuration(pRobot->makeJointState(0.0))
	, fold_configuration(pRobot->makeJointState(0.0))
	, go_home_(false)
	, go_fold_(false)
{
	ddsLink = std::make_shared<dls::DDSParticipant>("MotionGen::" + this->getID(), dls::domains::signals);

	setConsoleFunctions();
}

MotionGenerator::~MotionGenerator()
{}

std::shared_ptr<dls::DDSParticipant> MotionGenerator::getParticipant()
{
	return this->ddsLink;
}

std::shared_ptr<const robotlib::RobotBase> MotionGenerator::getRobot()
{
	return this->pRobot;
}

void MotionGenerator::setHomeConfiguration(YAML::Node& config, const std::string& data_name)
{
	for(auto &leg_pair : this->home_configuration)
	{
		for(auto &joint_pair : *leg_pair.data_)
		{
			this->home_configuration[joint_pair.key_] = config[data_name][joint_pair.key_->getName()].as<double>();
		}
	}
}

void MotionGenerator::setFoldConfiguration(YAML::Node& config, const std::string& data_name)
{
	for(auto &leg_pair : this->fold_configuration)
	{
		for(auto &joint_pair : *leg_pair.data_)
		{
			this->fold_configuration[joint_pair.key_] = config[data_name][joint_pair.key_->getName()].as<double>();
		}
	}
}

void MotionGenerator::goHome()
{
	go_home_ = true;
}

void MotionGenerator::goFold()
{
	go_fold_ = true;
}

void MotionGenerator::runPostures()
{
	// Posture homing call
	if(this->go_home_)
	{
		runGoHome();
	}
	// Posture folding call
	else if(this->go_fold_)
	{
		runGoFold();
	}
}

void MotionGenerator::setConsoleFunctions()
{
	command_manager.addCommand<>
	(
		"goHome",
		"Go to home position",
		std::function<bool()>([&]()->bool
		{
			goHome();
			return true;
		}),
		{{1,1}, {2,2}, {3,3}},
		true
	);

	command_manager.addCommand<>
	(
		"goFold",
		"Go to fold position",
		std::function<bool()>([&]()->bool
		{
			goFold();
			return true;
		}),
		{{1,1}, {2,2}, {3,3}},
		true
	);

	command_manager.addCommand<>
    (
        "stopMotion",
        "Stop motion generation",
        std::function<bool()>([&]()->bool
        {
            this->stopMotion();
            return true;
        }),
        {{2,1}},
        true
    );
}