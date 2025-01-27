#ifndef TOPICS_UTILS_HPP
#define TOPICS_UTILS_HPP

#include <fastdds/dds/topic/TypeSupport.hpp>

#include <string>

namespace dls
{
	typedef std::pair<std::string, eprosima::fastdds::dds::TypeSupport> topicType;
}

#endif /* end of include guard: TOPICS_UTILS_HPP */