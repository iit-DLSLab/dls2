"""
Script implementing basic fastdds reader
"""
import fastdds
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

class ReaderListener(fastdds.DataReaderListener):
    def __init__(self, data):
        super().__init__()
        self.data = data
        self.sample_count = 0

    def on_data_available(self, reader):
        info = fastdds.SampleInfo()
        reader.take_next_sample(self.data, info)
        self.sample_count += 1
        try:
            seq = self.data.sequence_id() if hasattr(self.data, "sequence_id") else "n/a"
            npos = len(self.data.joints_position()) if hasattr(self.data, "joints_position") else "n/a"
            # print(f"[dls2_py.Reader] on_data_available sample={self.sample_count} seq={seq} positions={npos}")
        except Exception as exc:
            print(f"[dls2_py.Reader] on_data_available sample={self.sample_count} debug failed: {exc}")

    def on_subscription_matched(self, datareader, info) :
        yield

class Reader():
  def __init__(self, domain, topic_data_type, data, topic_name):
    factory = fastdds.DomainParticipantFactory.get_instance()
    self.participant_qos = fastdds.DomainParticipantQos()
    factory.get_default_participant_qos(self.participant_qos)
    self.participant = None
    self.subscriber = None
    self.reader = None

    # self.participant = factory.create_participant(domain, self.participant_qos)
    profile_path = _resolve_profile_path()
    if not os.path.exists(profile_path):
        raise FileNotFoundError(
            f"Missing DDS participant profile at {profile_path}. "
            "Install dls2_py runtime config or set up the package correctly."
        )
    factory.load_XML_profiles_file(profile_path)
    factory.get_participant_qos_from_profile(profile_path, self.participant_qos)
    profile_name = "disc_server_client_domain_" + str(domain)
    self.participant = factory.create_participant_with_profile(profile_name)
    if self.participant is None:
        raise RuntimeError(
            f"Failed to create Fast DDS participant using profile '{profile_name}' "
            f"from {profile_path}."
        )

    self.topic_data_type = topic_data_type
    self.topic_data_type.set_name(topic_data_type.get_name())
    self.type_support = fastdds.TypeSupport(self.topic_data_type)
    
    self.participant.register_type(self.type_support)

    self.topic_qos = fastdds.TopicQos()
    self.participant.get_default_topic_qos(self.topic_qos)
    self.topic = self.participant.create_topic(topic_name, self.topic_data_type.get_name(), self.topic_qos)

    self.subscriber_qos = fastdds.SubscriberQos()
    self.participant.get_default_subscriber_qos(self.subscriber_qos)
    self.subscriber = self.participant.create_subscriber(self.subscriber_qos)

    self.listener = ReaderListener(data)
    self.reader_qos = fastdds.DataReaderQos()
    self.subscriber.get_default_datareader_qos(self.reader_qos)
    self.reader = self.subscriber.create_datareader(self.topic, self.reader_qos, self.listener)
    
  def __del__(self):
    factory = fastdds.DomainParticipantFactory.get_instance()
    if self.participant is not None:
        self.participant.delete_contained_entities()
        factory.delete_participant(self.participant)

  def getData(self):
    return self.listener.data
