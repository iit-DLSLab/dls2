"""
Script implementing basic fastdds publisher
"""
import fastdds
from threading import Condition
import os
from pathlib import Path


def _resolve_profile_path() -> str:
    module_dir = Path(__file__).resolve().parent
    env_path = os.environ.get("DLS_DDS_PARTICIPANT_CONFIG")
    if env_path:
        return str(Path(env_path).resolve())

    candidates = (
        Path("/usr/include/dls2/util/messaging/dds_participant_config.xml"),
        module_dir.parent / "config" / "dds_participant_config.xml",
    )
    for candidate in candidates:
        if candidate.exists():
            return str(candidate)
    return str(candidates[0])

class WriterListener (fastdds.DataWriterListener) :
    def __init__(self, writer) :
        self._writer = writer
        super().__init__()

    def on_publication_matched(self, datawriter, info) :
        if (0 < info.current_count_change) :
            self._writer._cvDiscovery.acquire()
            self._writer._matched_reader += 1
            self._writer._cvDiscovery.notify()
            self._writer._cvDiscovery.release()
        else :
            self._writer._cvDiscovery.acquire()
            self._writer._matched_reader += 1
            self._writer._cvDiscovery.notify()
            self._writer._cvDiscovery.release()

class Writer:
  def __init__(self, domain, topic_data_type, data, topic_name):
    self._matched_reader = 0
    self._cvDiscovery = Condition()
    self.data = data

    factory = fastdds.DomainParticipantFactory.get_instance()
    self.participant_qos = fastdds.DomainParticipantQos()
    factory.get_default_participant_qos(self.participant_qos)    
    
    # self.participant = factory.create_participant(domain, self.participant_qos)
    profile_path = _resolve_profile_path()
    if not os.path.exists(profile_path):
        raise FileNotFoundError(
            f"Missing DDS participant profile at {profile_path}. "
            "Install dls2 development config or set DLS_DDS_PARTICIPANT_CONFIG."
        )
    factory.load_XML_profiles_file(profile_path)
    factory.get_participant_qos_from_profile(profile_path,self.participant_qos)
    self.participant = factory.create_participant_with_profile("disc_server_client_domain_"+str(domain))

    self.topic_data_type = topic_data_type
    self.topic_data_type.set_name(topic_data_type.get_name())
    # self.topic_data_type.auto_fill_type_information(False)
    # self.topic_data_type.auto_fill_type_object(True)
    self.type_support = fastdds.TypeSupport(self.topic_data_type)
    self.participant.register_type(self.type_support)

    self.topic_qos = fastdds.TopicQos()
    self.participant.get_default_topic_qos(self.topic_qos)
    self.topic = self.participant.create_topic(topic_name, self.topic_data_type.get_name(), self.topic_qos)

    self.publisher_qos = fastdds.PublisherQos()
    self.participant.get_default_publisher_qos(self.publisher_qos)
    self.publisher = self.participant.create_publisher(self.publisher_qos)

    self.listener = WriterListener(self)
    self.writer_qos = fastdds.DataWriterQos()
    self.publisher.get_default_datawriter_qos(self.writer_qos)
    self.writer = self.publisher.create_datawriter(self.topic, self.writer_qos, self.listener)

    self.index = 0

  def write(self):
    self.writer.write(self.data)

  def __del__(self):
    factory = fastdds.DomainParticipantFactory.get_instance()
    self.participant.delete_contained_entities()
    factory.delete_participant(self.participant)

  # def wait_discovery(self) :
  #   self._cvDiscovery.acquire()
  #   print ("Writer is waiting discovery...")
  #   self._cvDiscovery.wait_for(lambda : self._matched_reader != 0)
  #   self._cvDiscovery.release()
  #   print("Writer discovery finished...")
