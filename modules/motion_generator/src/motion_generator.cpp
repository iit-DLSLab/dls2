
#include "dls2/motion_generator/motion_generator.hpp"


using namespace dls;

MotionGenerator::MotionGenerator
(
	const std::string &ID,
	const std::shared_ptr<robotlib::RobotBase>& pRobot_
) 
	: PeriodicApp(ID)
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

void MotionGenerator::setHomeConfiguration()
{
	const std::string robot_name = pRobot->getName();
	YAML::Node config = YAML::LoadFile("/usr/include/"+robot_name+"_description/default_postures/goHome.yml");
	for(auto joint : pRobot->getJoints()){
		this->home_configuration[joint->id] = config[joint->getName()].as<double>();
	}
}

void MotionGenerator::setFoldConfiguration()
{
	const std::string robot_name = pRobot->getName();
	YAML::Node config = YAML::LoadFile("/usr/include/"+robot_name+"_description/default_postures/goFold.yml");
	for(auto joint : pRobot->getJoints()){
		this->fold_configuration[joint->id] = config[joint->getName()].as<double>();
	}
}

void MotionGenerator::goHome()
{
	if (!go_fold_)
	{
		go_home_ = true;
	}
	else
	{
		std::cout << "Cannot run Go Home procedure: another procedure is already running" << std::endl;
	}
}

void MotionGenerator::goFold()
{
	if (!go_home_)
	{
		go_fold_ = true;
	}
	else
	{
		std::cout << "Cannot run Go Fold procedure: another procedure is already running" << std::endl;
	}
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