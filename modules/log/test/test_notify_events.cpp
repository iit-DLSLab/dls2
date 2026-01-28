#include "dls2/log/event_logger.hpp"
#include "magic_enum/magic_enum.hpp"

#include "dls2/util/messaging/dds_writer.hpp"

#include <nlohmann/json.hpp>

#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <future>
#include <fstream>
#include <algorithm>

// create an empty structure (null)
nlohmann::json json_data;
std::mutex pub_mutex;
std::atomic_bool stop_pub_thread(false);

bool pub_thread(double frequency, const std::shared_ptr<dls::logging::EventNotifier> event_notifier)
{   
    // create distribution for event IDs and severities sampling
    constexpr auto event_ids = magic_enum::enum_values<EventID>();
    constexpr auto event_severities = magic_enum::enum_values<EventSeverity>();
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist_ids(0, event_ids.size() - 1);
    std::uniform_int_distribution<std::size_t> dist_severities(0, event_severities.size() - 1);

    const std::string component_name = event_notifier->get_name();
    const int period = (1.0/frequency)*1000; // period in milliseconds
    int msg_count = 0;

    while(!stop_pub_thread)
    {
        EventID event_id = event_ids[dist_ids(gen)];
        EventSeverity event_severity = event_severities[dist_severities(gen)];
        std::string message = "Message text_"+ std::to_string(++msg_count);
        event_notifier->notify(
            event_id,
            event_severity,
            message
        );
        {
            std::lock_guard<std::mutex> lock(pub_mutex);
            dls2_interface::msg::EventLog event_log = event_notifier->getMsg();
            std::cout << "Event from component: " << event_log.component_name() << "\n"
                      << "Timestamp: " << event_log.header().timestamp() << "\n"
                      << "Sequence ID: " << event_log.header().sequence_id() << "\n"
                      << "Event ID: " << magic_enum::enum_name(static_cast<EventID>(event_log.event_id())) << "\n"
                      << "Severity: " << magic_enum::enum_name(static_cast<EventSeverity>(event_log.severity())) << "\n"
                      << "Message: " << event_log.msg() << std::endl;

            // json_data[component_name].push_back({
            //     {"timestamp", event_log.header().timestamp()},
            //     {"sequence_id", event_log.header().sequence_id()},
            //     {"event_id", magic_enum::enum_name(static_cast<EventID>(event_log.event_id()))},
            //     {"severity", magic_enum::enum_name(static_cast<EventSeverity>(event_log.severity()))},
            //     {"message", event_log.msg()}
            // });
            // light version
            json_data[event_log.component_name()]["timestamp"].push_back(event_log.header().timestamp());
            json_data[event_log.component_name()]["sequence_id"].push_back(event_log.header().sequence_id());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(period));
    }

    return true;
}

void read_events(
    dls::logging::EventListener &event_listener,
    nlohmann::json &json_recorded_data)
{
    std::cout << "Unbounded buffer index: " << event_listener.getUnboundedBufferIdx() << std::endl;

    auto events = event_listener.readEvents();
	for(const auto& event_log : events){
        json_recorded_data[event_log.component_name()]["timestamp"].push_back(event_log.header().timestamp());
        json_recorded_data[event_log.component_name()]["sequence_id"].push_back(event_log.header().sequence_id());
    }
}
void event_listener(int num_pub, int test_time){
    dls::logging::EventListener event_listener("event_listener");
    std::cout << "Event listener created. Waiting for publishers to match..." << std::endl;
    // wait for all publishers to match
    while(event_listener.getNumOfMatches() < num_pub)
    {
        std::cout << "Matched publishers: " << event_listener.getNumOfMatches() << "/" << num_pub << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // loop until timeout
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point end_time = start_time + std::chrono::seconds(test_time);

    nlohmann::json json_recorded_data;
    while(std::chrono::high_resolution_clock::now() < end_time || event_listener.getNumOfMatches() > 0)
    {
        read_events(event_listener, json_recorded_data);

        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count();
        std::cout << "Event listener running... Elapsed time: " << elapsed << "/" << test_time << " seconds\r" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    // do another round of event read to avoid missing last events due to the fact that the unmatches happen while reading in the last while loop.
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    read_events(event_listener, json_recorded_data);

    // //get events
    // boost::circular_buffer<EventLog> events = event_listener.get_event_buffer_();

    // // save recorded events to json
    // nlohmann::json json_recorded_data;
    // for(const auto &event : events)
    // {
    //     // json_recorded_data[event.component_name()].push_back({
    //     //     {"timestamp", event.header().timestamp()},
    //     //     {"sequence_id", event.header().sequence_id()},
    //     //     {"event_id", magic_enum::enum_name(static_cast<EventID>(event.event_id()))},
    //     //     {"severity", magic_enum::enum_name(static_cast<EventSeverity>(event.severity()))},
    //     //     {"message", event.msg()}
    //     // });
    //     // light version
    //     json_recorded_data[event.component_name()]["timestamp"].push_back(event.header().timestamp());
    //     json_recorded_data[event.component_name()]["sequence_id"].push_back(event.header().sequence_id());
    // }

    std::ofstream output_file("sub_msgs.json");
    output_file << json_recorded_data << std::endl;
}

void event_spawner(int num_pub, int test_time){
    std::vector<std::future<bool>> futures;

    std::vector<double> frequencies;    
    for(int i=0; i<num_pub;++i)
    {
        if(i<2)
            frequencies.push_back(1);
        else if(i<4)
            frequencies.push_back(50);
        else if(i<10)
            frequencies.push_back(100);
        else if(i<15)
            frequencies.push_back(250);
        else if(i<20)
            frequencies.push_back(500);
        else
            frequencies.push_back(1000);
        
        std::shared_ptr<dls::logging::EventNotifier> event_notifier = std::make_shared<dls::logging::EventNotifier>("pub_" + std::to_string(i));
        std::cout << "Publisher " << i << " created with frequency: " << frequencies[i] << " Hz." << " Waiting for match..." << std::endl;
        event_notifier->waitForMatch();
        std::cout << "Publisher " << i << " has matched." << std::endl;
        futures.push_back(std::async(std::launch::async, pub_thread, frequencies[i], event_notifier));
    }

    // loop until timeout
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point end_time = start_time + std::chrono::seconds(test_time);
    while(std::chrono::high_resolution_clock::now() < end_time)
    {
        // print enlapsed time over test time
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count();
        std::cout << "Event spawner running... Elapsed time: " << elapsed << "/" << test_time << " seconds\r" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    // stop all publishers
    stop_pub_thread = true;

    bool stop = false;
    std::cout << "Waiting for all publishers to finish..." << std::endl;
    while(!stop)
	{
        stop = true;
        for(auto &future : futures)
        {
            if(!future.get())
            {
                stop = false;
                break;
            }
        }
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
    
    std::ofstream output_file("pub_msgs.json");
    output_file << json_data << std::endl;
}

void print_res(const nlohmann::json &json_pub_msgs, const nlohmann::json &json_res)
{

    int total_missing_events = 0;
    if(json_res.find("missing_components") != json_res.end() && !json_res["missing_components"].empty())
    {
        std::cout << "Missing components: " << std::endl;
        for(const auto &component : json_res["missing_components"]) {
            std::cout << "- " << component << std::endl;
            total_missing_events+= json_pub_msgs[component]["sequence_id"].size();
        }
    }
    else
    {
        std::cout << "All components are present." << std::endl;
    }

    for (const auto &item : json_res.items()) {
        if(item.key() != "missing_components") {
            std::cout << "---"<<item.key() << "---\n" << std::endl;
            int num_missing_events = item.value()["num_missing_events"].template get<int>();
            std::cout << "num_missing_events/total_component_events: " << num_missing_events << "/" << json_pub_msgs[item.key()]["sequence_id"].size()<<"\n";
            std::cout << "missing sequence_ids: " <<  item.value()["missing_sequence_ids"] << "\n";
            total_missing_events += num_missing_events;
        }
    }

    // compute total events
    int total_events = 0;
    for (const auto &item : json_pub_msgs.items()) {
        total_events += item.value()["sequence_id"].size();
    }
    if(total_missing_events > 0)
    {
        std::cout << "\nTOT missing events/total_events: " << total_missing_events <<"/" << total_events<<std::endl;
    }
    else
    {
        std::cout << "No missing events." << std::endl;
    }
}
// void compute_metrics(const nlohmann::json &json_pub_msgs, const nlohmann::json &json_sub_msgs)
// {
//     nlohmann::json json_res;
//     std::cout << "Metrics:" << std::endl;
//     for (const auto &item : json_pub_msgs.items()) {
//         const std::string component_name = item.key();
//         const auto events = item.value();
//         std::cout << "Checking component: " << component_name << std::endl;
//         // Check for missing components. If a component is not present in json_sub_msgs, add it to the missing_component field
//         if (json_sub_msgs.find(component_name) == json_sub_msgs.end()) {
//             json_res["missing_components"].push_back(component_name);
//         }
//         else {
//             // Check all messages from each component are received. If not, save component name with associated number of missing events and event ids.
//             std::cout << "Component " << component_name << " has " << events.size() << " events." << std::endl;
//             int num_missing_events = 0;
//             for(const auto &event : events) {
//                 const int sequence_id = event["sequence_id"];
//                 std::cout << "Checking sequence_id: " << sequence_id << std::endl;

//                 // Check if the event is present in the subscriber messages
//                 bool found=false;
//                 for(const auto &sub_item : json_sub_msgs[component_name]) {
//                     if(sub_item["sequence_id"] == sequence_id) {
//                         // Event is present in subscriber messages
//                         found = true;
//                         break;
//                     }
//                 }
//                 if(!found) {
//                     num_missing_events++;
//                     json_res[component_name]["missing_sequence_ids"].push_back(sequence_id);
//                 }
//             }
//             if(num_missing_events > 0) {
//                 json_res[component_name]["num_missing_events"] = num_missing_events;
//             }
//         }
//     }

//     print_res(json_pub_msgs, json_res);

//     std::ofstream output_file("test_notify_events_res.json");
//     output_file << json_res << std::endl;
// }

void compute_metrics(const nlohmann::json &json_pub_msgs, const nlohmann::json &json_sub_msgs)
{
    nlohmann::json json_res;
    std::cout << "Metrics:" << std::endl;
    for (const auto &item : json_pub_msgs.items()) {
        const std::string component_name = item.key();
        std::vector<int> sequence_ids = json_pub_msgs[component_name]["sequence_id"];
        std::vector<int> sub_sequence_ids = json_sub_msgs[component_name]["sequence_id"];
        // sort arrays for later binary search
        std::sort (sub_sequence_ids.begin(), sub_sequence_ids.end());

        std::cout << "Checking component: " << component_name << std::endl;
        // Check for missing components. If a component is not present in json_sub_msgs, add it to the missing_component field
        if (json_sub_msgs.find(component_name) == json_sub_msgs.end()) {
            json_res["missing_components"].push_back(component_name);
        }
        else {
            // Check all messages from each component are received. If not, save component name with associated number of missing events and event ids.
            std::cout << "Component " << component_name << " has " << sequence_ids.size() << " events." << std::endl;
            int num_missing_events = 0;
            for(const int seq_id : sequence_ids) {
                // Check if the event is present in the subscriber messages
                bool found=false;
                if(std::binary_search(sub_sequence_ids.begin(), sub_sequence_ids.end(), seq_id)){
                    found = true;
                }

                if(!found) {
                    num_missing_events++;
                    json_res[component_name]["missing_sequence_ids"].push_back(seq_id);
                }
            }
            if(num_missing_events > 0) {
                json_res[component_name]["num_missing_events"] = num_missing_events;
            }
        }
    }

    print_res(json_pub_msgs, json_res);

    std::ofstream output_file("test_notify_events_res.json");
    output_file << json_res << std::endl;
}

void print(const nlohmann::json &json_pub_msgs, const nlohmann::json &json_sub_msgs)
{
    // print pub and sub msgs
    std::cout << "***********Publisher Messages:************" << std::endl;
    for (const auto& item : json_pub_msgs.items()) {
        std::cout << "---"<<item.key() << "---\n" << std::endl;
        for (const auto& sub_item : item.value()) {
            std::cout << "timestamp: " << sub_item["timestamp"] << "\n" <<
                        "sequence_id: " << sub_item["sequence_id"] << "\n" <<
                        "event_id: " << sub_item["event_id"] << "\n" <<
                        "severity: " << sub_item["severity"] << "\n" <<
                        "message: " << sub_item["message"] << std::endl;
        }
    }
    std::cout << "***********Subscriber Messages:************" << std::endl;
    for (const auto& item : json_sub_msgs.items()) {
        std::cout << "---"<<item.key() << "---\n" << std::endl;
        for (const auto& sub_item : item.value()) {
            std::cout << "timestamp: " << sub_item["timestamp"] << "\n" <<
                        "sequence_id: " << sub_item["sequence_id"] << "\n" <<
                        "event_id: " << sub_item["event_id"] << "\n" <<
                        "severity: " << sub_item["severity"] << "\n" <<
                        "message: " << sub_item["message"] << std::endl;
        }
    }
}

void checker(){
    // read pub_msgs.json
    std::ifstream pub_msgs_file("pub_msgs.json");
    nlohmann::json json_pub_msgs = nlohmann::json::parse(pub_msgs_file);
    // read sub_msgs.json
    std::ifstream sub_msgs_file("sub_msgs.json");
    nlohmann::json json_sub_msgs = nlohmann::json::parse(sub_msgs_file);

    // print(json_pub_msgs, json_sub_msgs);

    compute_metrics(json_pub_msgs, json_sub_msgs);
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <mode>" << std::endl;
        std::cerr << "Modes: spawnenr, listener" << std::endl;
        return 1;
    }
    std::string mode = argv[1];
    if(mode == "spawner")
    {
        if(argc==4){
            int num_pub = std::stoi(argv[2]);
            int test_time = std::stoi(argv[3]);
            event_spawner(num_pub,test_time);
        }
        else{
            std::cerr << "Usage: " << argv[0] << " spawner <num_publishers> <test_time>" << std::endl;
            return 1;
        }
    }
    else if(mode == "listener")
    {
        if(argc==4){
            int num_pub = std::stoi(argv[2]);
            int test_time = std::stoi(argv[3]);
            event_listener(num_pub, test_time);
        }
        else{
            std::cerr << "Usage: " << argv[0] << " listener <num_publishers> <test_time>" << std::endl;
            return 1;
        }
    }
    else if(mode =="checker"){
        checker();
    }
    else
    {
        std::cerr << "Unknown mode: " << mode << std::endl;
        return 1;
    }
}