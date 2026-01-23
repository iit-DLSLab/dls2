
#include "dls2/log/log.hpp"
#include "magic_enum/magic_enum.hpp"

#include <chrono>
#include <thread>
#include <climits>

using namespace dls::logging;

// =============================================================================
// ColorStreambuf
// =============================================================================
ColorStreambuf::ColorStreambuf(std::ostream& out,
								std::string prefix,
								const char* color_code,
								std::size_t /*buffer_size*/)
	: out_(out),
	prefix_(std::move(prefix)),
	color_(color_code)
{}

ColorStreambuf::~ColorStreambuf() {
	flush_line_if_needed(true);
}

// Called when a character is inserted
int ColorStreambuf::overflow(int ch) {
	if (ch == traits_type::eof()) {
		return traits_type::not_eof(ch);
	}

	char c = static_cast<char>(ch);
	if (c == '\n') {
		flush_line_if_needed(false); // flush current line
		return ch;
	}

	buffer_.push_back(c);
	return ch;
}

// Called on std::flush / std::endl and some other cases
int ColorStreambuf::sync() {
	flush_line_if_needed(false);
	return 0;
}

void ColorStreambuf::flush_line_if_needed(bool flush_even_if_empty) {
	if (buffer_.empty() && !flush_even_if_empty) return;

	// Print: color + prefix + buffered text + reset + newline (if we flushed because of '\n', newline already implied)
	out_ << color_ << prefix_ << buffer_ << RESET << '\n';
	out_.flush();
	buffer_.clear();
}
// =============================================================================
// cdbgstream
// =============================================================================
cdbgstream::cdbgstream(const std::string& prefix, std::size_t buffer_size)
	: std::ostream(nullptr),
	buf_(std::cout, prefix, YELLOW, buffer_size)
{
	rdbuf(&buf_);
}

cdbgstream::~cdbgstream() {
	// Ensure any partial line is flushed and color reset happens in buf_
	flush();
}

void cdbgstream::print(const std::string& s) { 
	auto time = Time::convertTimeToDate(std::chrono::duration_cast<std::chrono::nanoseconds>( 
					std::chrono::system_clock::now().time_since_epoch()).count()
				);
	(*this) << time << " [DEBUG] " << s << '\n'; 
}

// =============================================================================
// clogstream
// =============================================================================
clogstream::clogstream(const std::string& prefix, std::size_t buffer_size)
	: std::ostream(nullptr),
	buf_(std::cout, prefix, RESET, buffer_size)
{
	rdbuf(&buf_);
}

clogstream::~clogstream() {
	// Ensure any partial line is flushed and color reset happens in buf_
	flush();
}

void clogstream::print(const std::string& s) { 
	auto time = Time::convertTimeToDate(std::chrono::duration_cast<std::chrono::nanoseconds>( 
					std::chrono::system_clock::now().time_since_epoch()).count()
				);
	(*this) << time << " [INFO] " << s << '\n'; 
}

// =============================================================================
// warnstream
// =============================================================================
warnstream::warnstream(const std::string& prefix, std::size_t buffer_size)
	: std::ostream(nullptr),
	buf_(std::cout, prefix, YELLOW, buffer_size)
{
	rdbuf(&buf_);
}

warnstream::~warnstream() {
	// Ensure any partial line is flushed and color reset happens in buf_
	flush();
}

void warnstream::print(const std::string& s) { 
	auto time = Time::convertTimeToDate(std::chrono::duration_cast<std::chrono::nanoseconds>( 
					std::chrono::system_clock::now().time_since_epoch()).count()
				);
	(*this) << time << " [WARNING] " << s << '\n'; 
}

// =============================================================================
// cerrstream
// =============================================================================
cerrstream::cerrstream(const std::string& prefix, std::size_t buffer_size)
	: std::ostream(nullptr),
	buf_(std::cerr, prefix, RED, buffer_size)
{
	rdbuf(&buf_);
}

cerrstream::~cerrstream() {
	// Ensure any partial line is flushed and color reset happens in buf_
	flush();
}

void cerrstream::print(const std::string& s) { 
	auto time = Time::convertTimeToDate(std::chrono::duration_cast<std::chrono::nanoseconds>( 
					std::chrono::system_clock::now().time_since_epoch()).count()
				);
	(*this) << time << " [ERROR] " << s << '\n'; 
}

// =============================================================================
// cfatalstream
// =============================================================================
cfatalstream::cfatalstream(const std::string& prefix, std::size_t buffer_size)
	: std::ostream(nullptr),
	buf_(std::cerr, prefix, RED, buffer_size)
{
	rdbuf(&buf_);
}

cfatalstream::~cfatalstream() {
	// Ensure any partial line is flushed and color reset happens in buf_
	flush();
}

void cfatalstream::print(const std::string& s) {
	auto time = Time::convertTimeToDate(std::chrono::duration_cast<std::chrono::nanoseconds>( 
					std::chrono::system_clock::now().time_since_epoch()).count()
				);
	(*this) << time << " [FATAL] " << s << '\n'; 
}

// =============================================================================
// EventNotifier
// =============================================================================
EventNotifier::EventNotifier(const std::string &name)
	: name(name)
{
	msg.component_name() = name;
	msg.header().sequence_id() = 0;

	eprosima::fastdds::dds::DataWriterQos qos(eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);
	qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
	qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
	dds_writer = std::make_shared<dls::DDSWriter>(
				name,
				dls::domains::logging,
				dls::topics::log_events,
				qos
	);
}

void EventNotifier::notify(
	const EventID& event_id,
	const EventSeverity& severity,
	const std::string &message
)
{
	msg.event_id() = static_cast<uint8_t>(event_id);
	msg.severity() = static_cast<uint8_t>(severity);
	msg.msg() = message;
	msg.header().sequence_id() = (msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
	msg.header().timestamp() = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	dds_writer->sendMessage(&msg);
}

std::string EventNotifier::get_name() const
{
	return name;
}

dls2_interface::msg::EventLog EventNotifier::getMsg() const
{
	return msg;
}

bool EventNotifier::hasMatched()
{
	return dds_writer->hasMatched();
}

bool EventNotifier::waitForMatch()
{
	while(!dds_writer->hasMatched())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return true;
}


// =============================================================================
// RobustEventNotifier
// =============================================================================
RobustEventNotifier::RobustEventNotifier(const std::string &name, 
										 const double &spamming_threshold)
: EventNotifier(name)
{
	spamming_threshold_ms_ = spamming_threshold;
}

void RobustEventNotifier::notify(const EventID& event_id, 
							const EventSeverity& severity, 
							const std::string &message)
{
	auto now = std::chrono::steady_clock::now();

	const auto last_it = last_event_pub_time_.find(event_id);
	auto publish = (last_it == last_event_pub_time_.end());

	if(!publish){
		const auto spamming_event = toMs(now - last_it->second) < spamming_threshold_ms_;
		publish = !spamming_event;
	}

	if(publish){
		EventNotifier::notify(event_id, severity, message);
		last_event_pub_time_.insert_or_assign(event_id, now);
	}
}

void RobustEventNotifier::setSpammingThreshold(const double &spamming_threshold){
	spamming_threshold_ms_ = spamming_threshold;
}

// =============================================================================
// EventListener
// =============================================================================
EventListener::EventListener(const std::string &name)
	: event_buffer(1000000), name(name), unbounded_buffer_idx(-1) {
	eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
	qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
	qos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;

	dds_reader = std::make_shared<dls::DDSReader>(
				name,
				dls::domains::logging,
				dls::topics::log_events,
				std::function<void(void *)>
				{
					[&](void *tuple)
					{
						dls2_interface::msg::EventLog *msg = (dls2_interface::msg::EventLog*) tuple;
						// std::cout 	<< "###################"
						// 			<< "\ntimestamp: " << msg->header().timestamp()
						// 			<< "\nsequence_id: " << msg->header().sequence_id()
						// 			<< "\nevent: " <<magic_enum::enum_name(static_cast<EventID>(msg->event_id()))
						// 		  	<< "\nseverity: " << magic_enum::enum_name(static_cast<EventSeverity>(msg->severity()))
						// 		  	<< "\ncomponent: " << msg->component_name()
						// 		  	<< "\nmessage: " << msg->msg()
						// 		  	<< "\n###################" << std::endl;
						event_buffer.push_back(*msg);
						// }
						unbounded_buffer_idx++;
						if(unbounded_buffer_idx>LLONG_MAX)
						{
							throw std::runtime_error("Unbounded buffer index of EventLister exceeded ULLONG_MAX");
						}
					}
				},
				qos);
}

std::string EventListener::get_name() const
{
	return name;
}

int EventListener::getNumOfMatches() const
{
	return dds_reader->getNumOfMatches();
}

unsigned long long int EventListener::getUnboundedBufferIdx() const
{
	return unbounded_buffer_idx;
}

unsigned long int EventListener::getBufferMaxIdx() const
{
	return event_buffer.capacity()-1;
}
