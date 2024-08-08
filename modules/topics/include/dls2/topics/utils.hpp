#ifndef TOPICS_UTILS_HPP
#define TOPICS_UTILS_HPP

#include <fastdds/dds/topic/TypeSupport.hpp>

#include <string>

namespace dls
{
	typedef std::pair<std::string, eprosima::fastdds::dds::TypeSupport> topicType;
	eprosima::fastdds::dds::TypeSupport createDynamicTypeSupport(const std::string& xml_file_path, const std::string& type_name);
	eprosima::fastrtps::types::DynamicData_ptr createDynamicMessage(const eprosima::fastdds::dds::TypeSupport type_support);
}


#endif /* end of include guard: TOPICS_UTILS_HPP */