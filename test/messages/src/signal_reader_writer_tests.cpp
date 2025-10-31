#include "dls_messages/dds/blind_state.hpp"
#include "dls2/signal/desired_torques.hpp"
#include "dls2/signal/signal_writer.hpp"
#include "dls2/signal/signal_reader.hpp"
#include "robotlib/robot_factory.hpp"

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <chrono>

using namespace dls;

bool run = true;

void shutdown(int){
	run = false;
}

void readInputs(){

}

void publishOutputs(){

}

int main(int argc, char** argv)
{
	static_cast<void> (argc);
	static_cast<void> (argv);

    // CTRL-C handler
	signal
	(
		SIGINT,
		shutdown
	);

	auto robot = robotlib::RobotFactory::openRobot("aliengo");
	std::shared_ptr<DDSParticipant> dds_participant (new DDSParticipant("dds_part", domains::signals));
	// Define signal reader/writer base
	std::shared_ptr<SignalReaderBase> signal_reader_base = std::make_shared<SignalReader<BlindState>>(
															dds_participant,
															topics::low_level_estimation::blind_state,
															std::make_shared<BlindState>(robot));
	std::shared_ptr<SignalWriterBase> signal_writer_base = std::make_shared<SignalWriter<DesiredTorques>>(
															dds_participant,
															topics::desired_torques,
															std::make_shared<DesiredTorques>(robot));
	BlindState blind_state(robot);
	DesiredTorques desired_torques(robot);
	// WrapperBase* wb_blind_state = &blind_state;
	// WrapperBase* wb_desired_torques = &desired_torques;
	double increment = 0.3;
	while(run)
	{
		// Test if message is received
		// wb_blind_state->setDataFromWrapperBase(signal_reader_base->getWrapperBasePtr());
		blind_state.joints_position.print();
		// // Test if message is published
		desired_torques.desired_torques["LF"]["LF_HAA"]=increment;
		if (blind_state.joints_position["LF"]["LF_HAA"]>=0.4)
		{
			increment=(-1)*abs(increment);
		}
		else if (blind_state.joints_position["LF"]["LF_HAA"]<=-0.4)
		{
			increment=abs(increment);
		}
		// signal_writer_base->setDataFromWrapperBase(wb_desired_torques);
		if(signal_writer_base->hasTimestamp())
		{
			signal_writer_base->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
		}
		signal_writer_base->publish();
		usleep(100000);//100ms
	}	

	return EXIT_SUCCESS;
}