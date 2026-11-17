#include "dls2/util/messaging/dds_participant.hpp"
#include "dls_messages/dds/NavigationReferencePubSubTypes.hpp"

#include <thread>
#include <any>

int main()
{
  dls::DDSParticipant participant("participant", 0);
  auto writer = participant.addWriter("dds_writer", dls::topicType("Sample",
      new dls2_interface::msg::NavigationReferencePubSubType()));
  dls2_interface::msg::NavigationReference msg;
  msg.sequence_id() = 0;
  msg.timestamp() = 0;
  msg.velocity()[0] = 0;
  msg.velocity()[1] = 0.2;
  msg.velocity()[2] = 0.4;
  msg.robot_height() = 0.8;
  msg.start() = false;
  int count = 0;
  while(true)
  {
    msg.sequence_id() = count;
    msg.timestamp()  = count * 0.5;
    msg.velocity()[0] = count;
    msg.velocity()[1] = count + 0.2;
    msg.velocity()[2] = count + 0.4;
    msg.robot_height() = count + 0.8;
    msg.start() = count % 2 > 0;
    count++;

    writer->write(&msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  return 0;
}
